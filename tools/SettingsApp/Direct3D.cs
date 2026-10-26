using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace IrisEngSettings
{
    /// Display mode as the engine will index it: the position in the
    /// adapter's X8R8G8B8 mode list is what gets stored, not the size.
    internal sealed class DisplayMode
    {
        public int Index;
        public int Width;
        public int Height;
        public int RefreshRate;

        public override string ToString()
        {
            return Width + " x " + Height + "  @ " + RefreshRate + " Hz";
        }
    }

    internal sealed class Adapter
    {
        public int Ordinal;
        public string Description;
        public List<DisplayMode> Modes = new List<DisplayMode>();
        public List<DepthFormat> DepthFormats = new List<DepthFormat>();

        public override string ToString()
        {
            return Ordinal + ": " + Description;
        }
    }

    internal sealed class DepthFormat
    {
        public uint Value;
        public string Name;

        public DepthFormat(uint value, string name) { Value = value; Name = name; }
        public override string ToString() { return Name; }
    }

    /// Minimal IDirect3D9 interop: enough to list adapters, their
    /// X8R8G8B8 display modes and the depth formats each supports.
    internal sealed class Direct3D : IDisposable
    {
        private const uint D3D_SDK_VERSION = 32;
        private const uint D3DFMT_X8R8G8B8 = 22;
        private const int D3DDEVTYPE_HAL = 1;
        private const uint D3DUSAGE_DEPTHSTENCIL = 0x2;
        private const int D3DRTYPE_SURFACE = 1;

        // IDirect3D9 vtable slots.
        private const int VtRelease = 2;
        private const int VtGetAdapterCount = 4;
        private const int VtGetAdapterIdentifier = 5;
        private const int VtGetAdapterModeCount = 6;
        private const int VtEnumAdapterModes = 7;
        private const int VtCheckDeviceFormat = 10;

        [DllImport("d3d9.dll")]
        private static extern IntPtr Direct3DCreate9(uint sdkVersion);

        [StructLayout(LayoutKind.Sequential)]
        private struct D3DDISPLAYMODE
        {
            public uint Width;
            public uint Height;
            public uint RefreshRate;
            public uint Format;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        private struct D3DADAPTER_IDENTIFIER9
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 512)] public string Driver;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 512)] public string Description;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)] public string DeviceName;
            public long DriverVersion;
            public uint VendorId;
            public uint DeviceId;
            public uint SubSysId;
            public uint Revision;
            public Guid DeviceIdentifier;
            public uint WHQLLevel;
        }

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate uint ReleaseFn(IntPtr self);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate uint GetAdapterCountFn(IntPtr self);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate int GetAdapterIdentifierFn(IntPtr self, uint adapter, uint flags,
                                                    out D3DADAPTER_IDENTIFIER9 id);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate uint GetAdapterModeCountFn(IntPtr self, uint adapter, uint format);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate int EnumAdapterModesFn(IntPtr self, uint adapter, uint format,
                                                uint mode, out D3DDISPLAYMODE m);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate int CheckDeviceFormatFn(IntPtr self, uint adapter, int deviceType,
                                                 uint adapterFormat, uint usage, int rtype,
                                                 uint checkFormat);

        private IntPtr _d3d;

        private static readonly DepthFormat[] CandidateDepthFormats =
        {
            new DepthFormat(75, "D24S8  - 24 bit depth, 8 bit stencil"),
            new DepthFormat(77, "D24X8  - 24 bit depth, no stencil"),
            new DepthFormat(80, "D16    - 16 bit depth"),
            new DepthFormat(71, "D32    - 32 bit depth"),
            new DepthFormat(79, "D24X4S4 - 24 bit depth, 4 bit stencil"),
            new DepthFormat(73, "D15S1  - 15 bit depth, 1 bit stencil"),
        };

        public Direct3D()
        {
            _d3d = Direct3DCreate9(D3D_SDK_VERSION);
            if (_d3d == IntPtr.Zero)
                throw new InvalidOperationException(
                    "Direct3DCreate9 failed. DirectX 9 does not appear to be available.");
        }

        private T Fn<T>(int slot) where T : class
        {
            IntPtr vtbl = Marshal.ReadIntPtr(_d3d);
            IntPtr fn = Marshal.ReadIntPtr(vtbl, slot * IntPtr.Size);
            return (T)(object)Marshal.GetDelegateForFunctionPointer(fn, typeof(T));
        }

        public List<Adapter> EnumerateAdapters()
        {
            var result = new List<Adapter>();
            uint count = Fn<GetAdapterCountFn>(VtGetAdapterCount)(_d3d);
            var getId = Fn<GetAdapterIdentifierFn>(VtGetAdapterIdentifier);
            var modeCount = Fn<GetAdapterModeCountFn>(VtGetAdapterModeCount);
            var enumModes = Fn<EnumAdapterModesFn>(VtEnumAdapterModes);
            var checkFormat = Fn<CheckDeviceFormatFn>(VtCheckDeviceFormat);

            for (uint a = 0; a < count; a++)
            {
                var adapter = new Adapter { Ordinal = (int)a, Description = "Adapter " + a };

                D3DADAPTER_IDENTIFIER9 id;
                if (getId(_d3d, a, 0, out id) >= 0 && !string.IsNullOrEmpty(id.Description))
                    adapter.Description = id.Description.Trim();

                uint n = modeCount(_d3d, a, D3DFMT_X8R8G8B8);
                for (uint m = 0; m < n; m++)
                {
                    D3DDISPLAYMODE dm;
                    if (enumModes(_d3d, a, D3DFMT_X8R8G8B8, m, out dm) < 0)
                        continue;
                    adapter.Modes.Add(new DisplayMode
                    {
                        Index = (int)m,
                        Width = (int)dm.Width,
                        Height = (int)dm.Height,
                        RefreshRate = (int)dm.RefreshRate,
                    });
                }

                foreach (var df in CandidateDepthFormats)
                {
                    int hr = checkFormat(_d3d, a, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8,
                                         D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, df.Value);
                    if (hr >= 0)
                        adapter.DepthFormats.Add(df);
                }
                if (adapter.DepthFormats.Count == 0)
                    adapter.DepthFormats.Add(CandidateDepthFormats[0]);

                result.Add(adapter);
            }
            return result;
        }

        public void Dispose()
        {
            if (_d3d != IntPtr.Zero)
            {
                Fn<ReleaseFn>(VtRelease)(_d3d);
                _d3d = IntPtr.Zero;
            }
        }
    }
}
