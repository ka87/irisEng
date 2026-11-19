using System;
using Avalonia;

namespace IrisEngSettings
{
    internal static class Program
    {
        /* STAThread for Windows' sake: the Direct3D backend is COM and
           wants an apartment, and the rest does not mind. */
        [STAThread]
        public static void Main(string[] args)
        {
            BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);
        }

        /* Named and public because the Avalonia tooling looks for it. */
        public static AppBuilder BuildAvaloniaApp()
        {
            return AppBuilder.Configure<App>()
                             .UsePlatformDetect()
                             .LogToTrace();
        }
    }
}
