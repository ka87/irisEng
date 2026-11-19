using System;
using System.Collections.Generic;
using System.Linq;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace IrisEngSettings
{
    public partial class MainWindow : Window
    {
        private readonly ComboBox _adapter;
        private readonly ComboBox _mode;
        private readonly ComboBox _depth;
        private readonly TextBlock _depthLabel;
        private readonly CheckBox _mipMapping;
        private readonly CheckBox _windowed;
        private readonly TextBlock _status;
        private readonly Button _save;

        private List<Adapter> _adapters = new List<Adapter>();
        private bool _loading;

        /* What this platform's engine actually reads. Off Windows the
           OpenGL renderer takes the size and the windowed flag and
           nothing else, so the two Direct3D controls are hidden rather
           than offering a choice nothing will act on -- and the values
           already in the file are written back untouched, so a dual
           boot does not lose the Windows side's. */
        private readonly bool _hasDepth;
        private int _keptDepth;
        private int _keptMipMapping;

        public MainWindow()
        {
            InitializeComponent();

            _adapter = this.FindControl<ComboBox>("AdapterBox");
            _mode = this.FindControl<ComboBox>("ModeBox");
            _depth = this.FindControl<ComboBox>("DepthBox");
            _depthLabel = this.FindControl<TextBlock>("DepthLabel");
            _mipMapping = this.FindControl<CheckBox>("MipMappingBox");
            _windowed = this.FindControl<CheckBox>("WindowedBox");
            _status = this.FindControl<TextBlock>("StatusText");
            _save = this.FindControl<Button>("SaveButton");

            this.FindControl<Button>("CloseButton").Click += (s, e) => Close();
            this.FindControl<Button>("DefaultsButton").Click += (s, e) => ApplyDefaults();
            _save.Click += (s, e) => Save();
            _adapter.SelectionChanged += (s, e) => AdapterChanged();

            using (IDisplayBackend backend = MakeBackend())
            {
                _hasDepth = backend.HasDepthFormats;
                Populate(backend);
            }

            _depthLabel.IsVisible = _hasDepth;
            _depth.IsVisible = _hasDepth;
            _mipMapping.IsVisible = _hasDepth;
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }

        private static IDisplayBackend MakeBackend()
        {
            if (OperatingSystem.IsWindows())
                return new Direct3DBackend();
            return new LinuxBackend();
        }

        private void Populate(IDisplayBackend backend)
        {
            string where = backend.Name;
            try
            {
                _adapters = backend.EnumerateAdapters();
            }
            catch (Exception ex)
            {
                Fail("Could not ask " + where + " what the screen can do: " + ex.Message);
                return;
            }

            if (_adapters.Count == 0)
            {
                Fail("Nothing reported a display mode, so there is nothing to choose "
                     + "between. Asked: " + where + ".");
                return;
            }

            _loading = true;
            _adapter.ItemsSource = _adapters;
            _adapter.SelectedIndex = 0;
            _loading = false;
            AdapterChanged();

            LoadExisting();
        }

        private void Fail(string why)
        {
            _status.Text = why;
            _save.IsEnabled = false;
            _adapter.IsEnabled = false;
            _mode.IsEnabled = false;
            _depth.IsEnabled = false;
        }

        private Adapter Current
        {
            get { return _adapter.SelectedItem as Adapter; }
        }

        private void AdapterChanged()
        {
            if (_loading) return;
            Adapter a = Current;
            if (a == null) return;

            _mode.ItemsSource = a.Modes;
            _depth.ItemsSource = a.DepthFormats;

            SelectBestMode(a);
            if (a.DepthFormats.Count > 0)
                _depth.SelectedIndex = 0;
        }

        /// Prefers the size the screen is already running at, then the
        /// largest on offer, and among equal sizes the highest refresh
        /// rate at or below 60 Hz -- the engine's clock is happier
        /// there, and a mode list is usually sorted the other way.
        private void SelectBestMode(Adapter a)
        {
            if (a.Modes.Count == 0) return;

            int deskW = 0, deskH = 0;
            Screen primary = Screens != null ? Screens.Primary : null;
            if (primary != null)
            {
                deskW = primary.Bounds.Width;
                deskH = primary.Bounds.Height;
            }

            List<DisplayMode> pool = a.Modes
                .Where(m => m.Width == deskW && m.Height == deskH).ToList();
            if (pool.Count == 0)
            {
                int widest = a.Modes.Max(m => m.Width);
                pool = a.Modes.Where(m => m.Width == widest).ToList();
            }

            DisplayMode best = pool.Where(m => m.RefreshRate <= 60)
                                   .OrderByDescending(m => m.RefreshRate)
                                   .FirstOrDefault()
                               ?? pool.OrderBy(m => m.RefreshRate).First();

            _mode.SelectedItem = best;
            if (_mode.SelectedItem == null)
                _mode.SelectedIndex = 0;
        }

        private void LoadExisting()
        {
            int? adapter = Settings.ReadDisplayValue(Settings.VideoAdapter);
            int? mode = Settings.ReadDisplayValue(Settings.Mode);
            int? depth = Settings.ReadDisplayValue(Settings.ZBufferDepth);
            int? mip = Settings.ReadDisplayValue(Settings.MipMapping);
            int? win = Settings.ReadDisplayValue(Settings.Windowed);
            int? width = Settings.ReadDisplayValue(Settings.Width);
            int? height = Settings.ReadDisplayValue(Settings.Height);

            _keptDepth = depth ?? 0;
            _keptMipMapping = mip ?? 1;

            bool complete = _hasDepth
                ? adapter.HasValue && mode.HasValue && depth.HasValue && mip.HasValue
                : width.HasValue && height.HasValue;

            _loading = true;
            Adapter chosen = _adapters.FirstOrDefault(x => x.Ordinal == (adapter ?? 0))
                             ?? _adapters[0];
            _adapter.SelectedItem = chosen;
            _loading = false;
            AdapterChanged();

            /* Windows looks the mode up by the index it stored, because
               that index is what its renderer will use. Off Windows
               there is no index to trust -- the list is whatever SDL or
               the kernel reported this time -- so the size is matched
               instead. */
            if (_hasDepth && mode.HasValue)
            {
                DisplayMode m = chosen.Modes.FirstOrDefault(x => x.Index == mode.Value);
                if (m != null) _mode.SelectedItem = m;
            }
            else if (width.HasValue && height.HasValue)
            {
                DisplayMode m = chosen.Modes.FirstOrDefault(
                    x => x.Width == width.Value && x.Height == height.Value);
                if (m != null) _mode.SelectedItem = m;
            }

            if (_hasDepth && depth.HasValue)
            {
                DepthFormat d = chosen.DepthFormats.FirstOrDefault(
                    x => x.Value == unchecked((uint)depth.Value));
                if (d != null) _depth.SelectedItem = d;
            }

            _mipMapping.IsChecked = mip.HasValue ? mip.Value != 0 : true;
            _windowed.IsChecked = win.HasValue && win.Value != 0;

            _status.Text = complete
                ? "Existing settings loaded. Modes come from " + BackendNote() + "."
                : "No usable settings found; defaults selected. Press Save to write them.";
        }

        private string BackendNote()
        {
            return _hasDepth ? "Direct3D" : "the system";
        }

        private void ApplyDefaults()
        {
            _loading = true;
            _adapter.SelectedItem = _adapters.Count > 0 ? _adapters[0] : null;
            _loading = false;
            AdapterChanged();
            _mipMapping.IsChecked = true;
            _windowed.IsChecked = false;
            _status.Text = "Defaults selected. Press Save to write them.";
        }

        private void Save()
        {
            Adapter a = Current;
            DisplayMode m = _mode.SelectedItem as DisplayMode;
            if (a == null || m == null)
            {
                _status.Text = "Pick a screen and a resolution first.";
                return;
            }

            uint depthValue;
            bool mip;
            if (_hasDepth)
            {
                DepthFormat d = _depth.SelectedItem as DepthFormat;
                if (d == null)
                {
                    _status.Text = "Pick a depth buffer first.";
                    return;
                }
                depthValue = d.Value;
                mip = _mipMapping.IsChecked == true;
            }
            else
            {
                /* Not this renderer's business, and not this program's
                   to discard: whatever the file already said goes back
                   in unchanged. */
                depthValue = unchecked((uint)_keptDepth);
                mip = _keptMipMapping != 0;
            }

            try
            {
                Settings.Write(a.Ordinal, m.Index, depthValue, mip,
                               _windowed.IsChecked == true, m.Width, m.Height);
            }
            catch (Exception ex)
            {
                _status.Text = "Could not write the settings: " + ex.Message;
                return;
            }

            _status.Text = "Saved to " + Settings.FilePath + ".";
        }
    }
}
