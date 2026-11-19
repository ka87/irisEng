using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;

namespace IrisEngSettings
{
    /// The screens and their sizes, off Windows.
    ///
    /// SDL first, because the engine opens its window through SDL and
    /// so SDL's answer is the one that will be acted on -- and because
    /// SDL is the only one of these that speaks both X11 and Wayland.
    /// Wayland has no protocol for a client to ask what modes a
    /// monitor has; the compositor tells the toolkit, and here SDL is
    /// the toolkit.
    ///
    /// The kernel is the fallback, for a machine where libSDL3 is not
    /// installed as a shared library -- which is most of them, because
    /// the engine links SDL statically and so does not put one there.
    /// /sys/class/drm is the driver's own mode list, needs nothing
    /// installed and is readable without a display server at all.
    ///
    /// Only the size is written either way: off Windows the engine
    /// reads width, height and windowed and nothing else, so no index
    /// here has to line up with anything.
    internal sealed class LinuxBackend : IDisplayBackend
    {
        private IntPtr _sdl;
        private string _name = "the kernel's mode list";

        public string Name { get { return _name; } }

        /* The OpenGL renderer reads neither, so there is nothing to
           offer and the window hides the controls rather than
           offering a choice nothing will read. */
        public bool HasDepthFormats { get { return false; } }
        public bool HasModeIndex { get { return false; } }

        public List<Adapter> EnumerateAdapters()
        {
            List<Adapter> sdl = TrySdl();
            if (sdl != null && sdl.Count > 0)
                return sdl;
            return FromDrm();
        }

        // ---- SDL -------------------------------------------------

        private const uint SDL_INIT_VIDEO = 0x20;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private delegate bool InitFn(uint flags);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void QuitSubSystemFn(uint flags);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr GetDisplaysFn(out int count);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr GetDisplayNameFn(uint displayId);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr GetModesFn(uint displayId, out int count);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void FreeFn(IntPtr mem);

        /// SDL_DisplayMode, field for field as SDL3 declares it. The
        /// trailing pointer is SDL's own and is not read here.
        [StructLayout(LayoutKind.Sequential)]
        private struct SdlDisplayMode
        {
            public uint DisplayId;
            public uint Format;
            public int Width;
            public int Height;
            public float PixelDensity;
            public float RefreshRate;
            public int RefreshNumerator;
            public int RefreshDenominator;
            public IntPtr Internal;
        }

        private static readonly string[] SdlNames =
        {
            "libSDL3.so.0", "libSDL3.so", "SDL3",
        };

        private T Fn<T>(string name) where T : class
        {
            IntPtr p;
            if (!NativeLibrary.TryGetExport(_sdl, name, out p))
                return null;
            return (T)(object)Marshal.GetDelegateForFunctionPointer(p, typeof(T));
        }

        private List<Adapter> TrySdl()
        {
            foreach (string lib in SdlNames)
                if (NativeLibrary.TryLoad(lib, out _sdl))
                    break;
            if (_sdl == IntPtr.Zero)
                return null;

            try
            {
                var init = Fn<InitFn>("SDL_Init");
                var getDisplays = Fn<GetDisplaysFn>("SDL_GetDisplays");
                var getName = Fn<GetDisplayNameFn>("SDL_GetDisplayName");
                var getModes = Fn<GetModesFn>("SDL_GetFullscreenDisplayModes");
                var free = Fn<FreeFn>("SDL_free");
                if (init == null || getDisplays == null || getModes == null || free == null)
                    return null;

                /* Video only, and on the thread this is called from,
                   which is the one SDL wants. */
                if (!init(SDL_INIT_VIDEO))
                    return null;

                int displayCount;
                IntPtr displays = getDisplays(out displayCount);
                if (displays == IntPtr.Zero || displayCount <= 0)
                    return null;

                var result = new List<Adapter>();
                for (int d = 0; d < displayCount; d++)
                {
                    uint id = (uint)Marshal.ReadInt32(displays, d * sizeof(uint));
                    var adapter = new Adapter
                    {
                        Ordinal = d,
                        Description = NameOf(getName, id, d),
                    };

                    int modeCount;
                    IntPtr modes = getModes(id, out modeCount);
                    if (modes != IntPtr.Zero)
                    {
                        /* An array of pointers to modes, not an array
                           of modes. */
                        for (int m = 0; m < modeCount; m++)
                        {
                            IntPtr one = Marshal.ReadIntPtr(modes, m * IntPtr.Size);
                            if (one == IntPtr.Zero)
                                continue;
                            var dm = (SdlDisplayMode)Marshal.PtrToStructure(
                                one, typeof(SdlDisplayMode));
                            adapter.Modes.Add(new DisplayMode
                            {
                                Index = adapter.Modes.Count,
                                Width = dm.Width,
                                Height = dm.Height,
                                RefreshRate = (int)Math.Round(dm.RefreshRate),
                            });
                        }
                        free(modes);
                    }

                    if (adapter.Modes.Count > 0)
                        result.Add(adapter);
                }
                free(displays);

                if (result.Count > 0)
                    _name = "SDL, the same list the game will see";
                return result;
            }
            catch (Exception)
            {
                /* Any version of SDL that does not answer the way this
                   expects is not worth arguing with: the kernel's list
                   is there and needs nothing. */
                return null;
            }
        }

        private static string NameOf(GetDisplayNameFn getName, uint id, int ordinal)
        {
            if (getName != null)
            {
                IntPtr s = getName(id);
                if (s != IntPtr.Zero)
                {
                    string text = Marshal.PtrToStringUTF8(s);
                    if (!string.IsNullOrEmpty(text))
                        return text;
                }
            }
            return "Display " + (ordinal + 1);
        }

        // ---- the kernel ------------------------------------------

        /// Each connected output under /sys/class/drm has a modes file
        /// listing what the driver will set, widest first, one
        /// "<width>x<height>" per line. No refresh rate is there, and
        /// none is needed: the engine does not store one.
        internal static List<Adapter> FromDrm() { return FromDrm("/sys/class/drm"); }

        internal static List<Adapter> FromDrm(string root)
        {
            var result = new List<Adapter>();
            if (!Directory.Exists(root))
                return result;

            var cards = new List<string>(Directory.GetDirectories(root));
            cards.Sort(StringComparer.Ordinal);

            foreach (string card in cards)
            {
                string status = Path.Combine(card, "status");
                string modes = Path.Combine(card, "modes");
                if (!File.Exists(status) || !File.Exists(modes))
                    continue;
                if (ReadAll(status).Trim() != "connected")
                    continue;

                var adapter = new Adapter
                {
                    Ordinal = result.Count,
                    Description = Path.GetFileName(card),
                };
                var seen = new HashSet<string>();
                foreach (string line in ReadAll(modes).Split('\n'))
                {
                    string t = line.Trim();
                    int x = t.IndexOf('x');
                    if (x <= 0 || x == t.Length - 1 || !seen.Add(t))
                        continue;
                    int w, h;
                    if (!int.TryParse(t.Substring(0, x), NumberStyles.None,
                                      CultureInfo.InvariantCulture, out w)
                        || !int.TryParse(t.Substring(x + 1), NumberStyles.None,
                                         CultureInfo.InvariantCulture, out h))
                        continue;
                    adapter.Modes.Add(new DisplayMode
                    {
                        Index = adapter.Modes.Count,
                        Width = w,
                        Height = h,
                        RefreshRate = 0,
                    });
                }
                if (adapter.Modes.Count > 0)
                    result.Add(adapter);
            }
            return result;
        }

        private static string ReadAll(string path)
        {
            try { return File.ReadAllText(path); }
            catch (IOException) { return string.Empty; }
            catch (UnauthorizedAccessException) { return string.Empty; }
        }

        public void Dispose()
        {
            if (_sdl != IntPtr.Zero)
            {
                var quit = Fn<QuitSubSystemFn>("SDL_QuitSubSystem");
                if (quit != null)
                    quit(SDL_INIT_VIDEO);
                NativeLibrary.Free(_sdl);
                _sdl = IntPtr.Zero;
            }
        }
    }
}
