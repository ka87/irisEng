using System;
using System.Collections.Generic;

namespace IrisEngSettings
{
    /// One entry in a display's mode list.
    ///
    /// Index is the position in the list the backend built, and on
    /// Windows that position is the value the engine stores and looks
    /// the mode up by again -- so it is a value, not a convenience.
    /// Off Windows nothing stores it: the engine's OpenGL path reads
    /// width, height and windowed and no index at all.
    internal sealed class DisplayMode
    {
        public int Index;
        public int Width;
        public int Height;
        public int RefreshRate;

        public override string ToString()
        {
            return RefreshRate > 0
                ? Width + " x " + Height + "  @ " + RefreshRate + " Hz"
                : Width + " x " + Height;
        }
    }

    /// A depth-buffer format. Direct3D's idea; the OpenGL path has no
    /// equivalent and its backend offers none.
    internal sealed class DepthFormat
    {
        public uint Value;
        public string Name;

        public DepthFormat(uint value, string name) { Value = value; Name = name; }
        public override string ToString() { return Name; }
    }

    /// A screen, or on Windows a Direct3D adapter.
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

    /// Where the list of screens and modes comes from.
    ///
    /// There are two, and they are not two implementations of one
    /// thing: they answer for two different renderers. Direct3D is
    /// asked on Windows because the engine's Direct3D renderer is what
    /// will read the answer, and the mode index it stores is an index
    /// into Direct3D's own list. Everywhere else the engine draws
    /// through OpenGL on an SDL window, which reads only the size and
    /// whether it is windowed -- so the backend there offers sizes and
    /// says it has nothing to say about the rest.
    internal interface IDisplayBackend : IDisposable
    {
        /// What to call this in the window, for the status line.
        string Name { get; }

        /// Whether the engine, on this platform, reads the depth
        /// format and the mip-mapping flag at all. Only the Direct3D
        /// renderer does; the controls are hidden when it does not,
        /// rather than offering a choice that nothing will read.
        bool HasDepthFormats { get; }

        /// Whether the stored mode index means anything here. Same
        /// story: Direct3D looks the mode up by it, OpenGL does not.
        bool HasModeIndex { get; }

        List<Adapter> EnumerateAdapters();
    }
}
