using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace IrisEngSettings
{
    internal sealed class MainForm : Form
    {
        private readonly ComboBox _adapter = new ComboBox();
        private readonly ComboBox _mode = new ComboBox();
        private readonly ComboBox _depth = new ComboBox();
        private readonly ComboBox _sound = new ComboBox();
        private readonly CheckBox _mipMapping = new CheckBox();
        private readonly CheckBox _soundEffects = new CheckBox();
        private readonly Label _status = new Label();

        private List<Adapter> _adapters = new List<Adapter>();
        private bool _loading;

        public MainForm()
        {
            Text = "irisEng settings";
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            StartPosition = FormStartPosition.CenterScreen;
            ClientSize = new Size(470, 290);
            Font = SystemFonts.MessageBoxFont;

            foreach (var c in new ComboBox[] { _adapter, _mode, _depth, _sound })
                c.DropDownStyle = ComboBoxStyle.DropDownList;

            int y = 16;
            AddRow("Graphics adapter", _adapter, ref y);
            AddRow("Fullscreen resolution", _mode, ref y);
            AddRow("Depth buffer", _depth, ref y);
            AddRow("Sound device", _sound, ref y);

            _mipMapping.Text = "Mip mapping";
            _mipMapping.SetBounds(150, y, 200, 22);
            Controls.Add(_mipMapping);
            y += 26;

            _soundEffects.Text = "Sound effects (EAX)";
            _soundEffects.SetBounds(150, y, 200, 22);
            Controls.Add(_soundEffects);
            y += 34;

            _status.SetBounds(16, y, 440, 34);
            _status.ForeColor = SystemColors.GrayText;
            Controls.Add(_status);

            var save = new Button { Text = "Save" };
            save.SetBounds(250, 248, 95, 28);
            save.Click += (s, e) => Save();
            Controls.Add(save);

            var close = new Button { Text = "Close" };
            close.SetBounds(355, 248, 95, 28);
            close.Click += (s, e) => Close();
            Controls.Add(close);

            var defaults = new Button { Text = "Defaults" };
            defaults.SetBounds(16, 248, 95, 28);
            defaults.Click += (s, e) => ApplyDefaults();
            Controls.Add(defaults);

            KeyPreview = true;
            KeyDown += (s, e) => { if (e.KeyCode == Keys.Escape) Close(); };

            _adapter.SelectedIndexChanged += (s, e) => AdapterChanged();

            Load += (s, e) => Populate();
        }

        private void AddRow(string label, Control field, ref int y)
        {
            var l = new Label { Text = label, AutoSize = false };
            l.SetBounds(16, y + 3, 130, 20);
            Controls.Add(l);
            field.SetBounds(150, y, 300, 22);
            Controls.Add(field);
            y += 30;
        }

        private void Populate()
        {
            try
            {
                using (var d3d = new Direct3D())
                    _adapters = d3d.EnumerateAdapters();
            }
            catch (Exception ex)
            {
                MessageBox.Show(this,
                    "Could not enumerate Direct3D adapters:\n\n" + ex.Message,
                    "irisEng settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Close();
                return;
            }

            if (_adapters.Count == 0)
            {
                MessageBox.Show(this, "No Direct3D adapter reported any display mode.",
                    "irisEng settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Close();
                return;
            }

            _loading = true;
            _adapter.Items.AddRange(_adapters.Cast<object>().ToArray());
            _sound.Items.AddRange(Sound.Enumerate().Cast<object>().ToArray());
            _loading = false;

            LoadExisting();
        }

        private Adapter Current
        {
            get { return _adapter.SelectedItem as Adapter; }
        }

        private void AdapterChanged()
        {
            if (_loading) return;
            var a = Current;
            if (a == null) return;

            _mode.Items.Clear();
            _mode.Items.AddRange(a.Modes.Cast<object>().ToArray());
            _depth.Items.Clear();
            _depth.Items.AddRange(a.DepthFormats.Cast<object>().ToArray());

            SelectBestMode(a);
            _depth.SelectedIndex = 0;
        }

        /// Prefers the current desktop size, then the largest mode, and
        /// among equal sizes the highest refresh rate at or below 60 Hz.
        private void SelectBestMode(Adapter a)
        {
            if (a.Modes.Count == 0) return;

            Size desktop = Screen.PrimaryScreen.Bounds.Size;
            var atDesktop = a.Modes.Where(m => m.Width == desktop.Width &&
                                               m.Height == desktop.Height).ToList();
            var pool = atDesktop.Count > 0
                ? atDesktop
                : a.Modes.Where(m => m.Width == a.Modes.Max(x => x.Width)).ToList();

            DisplayMode best = pool.Where(m => m.RefreshRate <= 60)
                                   .OrderByDescending(m => m.RefreshRate)
                                   .FirstOrDefault()
                               ?? pool.OrderBy(m => m.RefreshRate).First();

            _mode.SelectedItem = best;
            if (_mode.SelectedItem == null) _mode.SelectedIndex = 0;
        }

        private void LoadExisting()
        {
            int? adapter = Settings.ReadVideoDword(Settings.VideoAdapter);
            int? mode = Settings.ReadVideoDword(Settings.Mode);
            int? depth = Settings.ReadVideoDword(Settings.ZBufferDepth);
            int? mip = Settings.ReadVideoDword(Settings.MipMapping);
            int? fx = Settings.ReadSoundDword(Settings.SoundFx);
            Guid? dev = Settings.ReadAudioGuid();

            bool complete = adapter.HasValue && mode.HasValue && depth.HasValue && mip.HasValue;

            _loading = true;
            var chosen = _adapters.FirstOrDefault(x => x.Ordinal == (adapter ?? 0)) ?? _adapters[0];
            _adapter.SelectedItem = chosen;
            _loading = false;
            AdapterChanged();

            if (mode.HasValue)
            {
                var m = chosen.Modes.FirstOrDefault(x => x.Index == mode.Value);
                if (m != null) _mode.SelectedItem = m;
            }
            if (depth.HasValue)
            {
                var d = chosen.DepthFormats.FirstOrDefault(
                    x => x.Value == unchecked((uint)depth.Value));
                if (d != null) _depth.SelectedItem = d;
            }

            _mipMapping.Checked = mip.HasValue ? mip.Value != 0 : true;
            // The engine turns effects off when the value reads exactly 1.
            _soundEffects.Checked = fx.HasValue && fx.Value != 1;

            var devices = _sound.Items.Cast<SoundDevice>().ToList();
            var match = dev.HasValue ? devices.FirstOrDefault(x => x.Id == dev.Value) : null;
            _sound.SelectedItem = match ?? devices[0];

            _status.Text = complete
                ? "Existing settings loaded."
                : "No usable settings found; defaults selected. Press Save to write them.";
        }

        private void ApplyDefaults()
        {
            _loading = true;
            _adapter.SelectedItem = _adapters[0];
            _loading = false;
            AdapterChanged();
            _mipMapping.Checked = true;
            _soundEffects.Checked = false;
            if (_sound.Items.Count > 0) _sound.SelectedIndex = 0;
            _status.Text = "Defaults selected. Press Save to write them.";
        }

        private void Save()
        {
            var a = Current;
            var m = _mode.SelectedItem as DisplayMode;
            var d = _depth.SelectedItem as DepthFormat;
            var s = _sound.SelectedItem as SoundDevice;
            if (a == null || m == null || d == null || s == null)
            {
                MessageBox.Show(this, "Pick a value in every box first.",
                    "irisEng settings", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            try
            {
                Settings.Write(a.Ordinal, m.Index, d.Value, _mipMapping.Checked,
                               _soundEffects.Checked, s.Id);
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "Could not write the settings:\n\n" + ex.Message,
                    "irisEng settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            _status.Text = "Saved.";
        }
    }
}
