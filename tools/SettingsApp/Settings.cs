using System;
using Microsoft.Win32;

namespace IrisEngSettings
{
    /// The registry values the engine reads at start-up. Every one of
    /// them must be present and of the right type or the engine stops
    /// with a message box; the video ones are fatal, the sound ones
    /// leave the game silent.
    internal static class Settings
    {
        private const string BaseKey = @"Software\IRISEng";
        private const string VideoKey = BaseKey + @"\Basic video";
        private const string SoundKey = BaseKey + @"\Sound";

        // Basic video, all REG_DWORD.
        public const string VideoAdapter = "Video Adapter";
        public const string ZBufferDepth = "Z Buffer depth";
        public const string Mode = "Mode";
        public const string MipMapping = "MipMapping";

        // Sound: a REG_DWORD and a 16 byte REG_BINARY.
        public const string SoundFx = "Sound FX";
        public const string AudioBoardGuid = "Audio board GUID";

        public static int? ReadVideoDword(string name)
        {
            return ReadDword(VideoKey, name);
        }

        public static int? ReadSoundDword(string name)
        {
            return ReadDword(SoundKey, name);
        }

        private static int? ReadDword(string path, string name)
        {
            using (RegistryKey k = Registry.CurrentUser.OpenSubKey(path))
            {
                if (k == null) return null;
                object v = k.GetValue(name, null);
                if (v == null) return null;
                if (k.GetValueKind(name) != RegistryValueKind.DWord) return null;
                return Convert.ToInt32(v);
            }
        }

        public static Guid? ReadAudioGuid()
        {
            using (RegistryKey k = Registry.CurrentUser.OpenSubKey(SoundKey))
            {
                if (k == null) return null;
                object v = k.GetValue(AudioBoardGuid, null);
                byte[] b = v as byte[];
                if (b == null || b.Length != 16) return null;
                return new Guid(b);
            }
        }

        /// Creates the key path if it is not there yet; CreateSubKey
        /// makes every missing level, so a clean machine needs no
        /// preparation.
        private static RegistryKey Create(string path)
        {
            RegistryKey k = Registry.CurrentUser.CreateSubKey(path);
            if (k == null)
                throw new InvalidOperationException(
                    @"Could not create HKEY_CURRENT_USER\" + path + ".");
            return k;
        }

        public static void Write(int adapter, int mode, uint zBufferDepth, bool mipMapping,
                                 bool soundEffects, Guid audioDevice)
        {
            using (RegistryKey k = Create(VideoKey))
            {
                k.SetValue(VideoAdapter, adapter, RegistryValueKind.DWord);
                k.SetValue(Mode, mode, RegistryValueKind.DWord);
                k.SetValue(ZBufferDepth, unchecked((int)zBufferDepth), RegistryValueKind.DWord);
                k.SetValue(MipMapping, mipMapping ? 1 : 0, RegistryValueKind.DWord);
            }
            using (RegistryKey k = Create(SoundKey))
            {
                // The engine disables effects when this reads exactly 1.
                k.SetValue(SoundFx, soundEffects ? 0 : 1, RegistryValueKind.DWord);
                k.SetValue(AudioBoardGuid, audioDevice.ToByteArray(), RegistryValueKind.Binary);
            }
        }
    }
}
