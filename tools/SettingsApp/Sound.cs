using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace IrisEngSettings
{
    internal sealed class SoundDevice
    {
        public Guid Id;
        public string Name;

        public override string ToString() { return Name; }
    }

    /// Lists the DirectSound output devices. The all-zero id is the
    /// system default and is always offered first.
    internal static class Sound
    {
        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private delegate bool EnumCallback(IntPtr guid, IntPtr description,
                                           IntPtr module, IntPtr context);

        [DllImport("dsound.dll", EntryPoint = "DirectSoundEnumerateA")]
        private static extern int DirectSoundEnumerateA(EnumCallback callback, IntPtr context);

        public static List<SoundDevice> Enumerate()
        {
            var devices = new List<SoundDevice>
            {
                new SoundDevice { Id = Guid.Empty, Name = "Default device" }
            };

            try
            {
                EnumCallback cb = (guid, description, module, context) =>
                {
                    // A null id is the primary device, already listed.
                    if (guid != IntPtr.Zero)
                    {
                        var bytes = new byte[16];
                        Marshal.Copy(guid, bytes, 0, 16);
                        devices.Add(new SoundDevice
                        {
                            Id = new Guid(bytes),
                            Name = Marshal.PtrToStringAnsi(description) ?? "Unnamed device",
                        });
                    }
                    return true;
                };
                DirectSoundEnumerateA(cb, IntPtr.Zero);
                GC.KeepAlive(cb);
            }
            catch (DllNotFoundException)
            {
                // No DirectSound: the default entry still lets the user save.
            }

            return devices;
        }
    }
}
