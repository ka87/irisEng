using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

namespace IrisEngSettings
{
    /// The display settings the engine reads at start-up. They used to
    /// be registry values under Software\IRISEng, then the [display]
    /// section of engine.ini beside the game; they are now settings.ini
    /// in the player's own folder, beside the saves. The first four
    /// must be there or the renderer stops with a message box before
    /// opening a window; the other three are optional and the engine
    /// falls back to fullscreen at 640x480 without them.
    ///
    /// Beside the saves rather than beside the game because this is a
    /// file that gets written. The game can certainly write there,
    /// having already had to in order to save at all, where an install
    /// folder under Program Files is not somewhere either of us can
    /// count on -- and a player's choices then survive the install
    /// being replaced or copied instead of going with it.
    internal static class Settings
    {
        private const string FileName = "settings.ini";

        /// The folder under Documents, which is the engine's own spelling
        /// of its name: one folder for the saves, the screenshots and
        /// this.
        private const string GameFolder = "irisEng";
        private const string DisplaySection = "display";

        /// The longest of the seven key names, which is what a setting
        /// line written from scratch pads its key out to so the values
        /// line up the way the shipped file's do.
        private const int KeyColumn = 10;

        // The seven [display] keys. The engine matches them without
        // regard to case, and so does the reader below.
        public const string VideoAdapter = "adapter";
        public const string Mode = "mode";
        public const string ZBufferDepth = "zbuffer";
        public const string MipMapping = "mipmapping";
        public const string Windowed = "windowed";
        public const string Width = "width";
        public const string Height = "height";

        /// In the order the shipped file lists them, which is the order
        /// any of them that are missing get added in.
        private static readonly string[] DisplayKeys =
        {
            VideoAdapter, Mode, ZBufferDepth, MipMapping, Windowed, Width, Height
        };

        /// The player's own folder, which is where the engine looks:
        /// Documents\irisEng\settings.ini. Not the folder this
        /// application is in -- that is the install, and the install is
        /// not what these settings are.
        ///
        /// The engine asks SDL for the documents folder and puts its own
        /// name under it; Environment is the same answer, and the one
        /// place the two have to agree is spelled in GameFolder above.
        public static string FolderPath
        {
            get
            {
                return Path.Combine(
                    Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments),
                    GameFolder);
            }
        }

        public static string FilePath
        {
            get { return Path.Combine(FolderPath, FileName); }
        }

        /// What the ini says about one [display] setting, or null when
        /// it says nothing usable about it. A key that is absent, empty
        /// or not a number reads as no value rather than as a zero --
        /// which is what it is to the engine as well, so the caller
        /// keeps whatever default it already had.
        public static int? ReadDisplayValue(string key)
        {
            string value;

            if (!ReadDisplaySection().TryGetValue(key, out value))
                return null;
            return ParseNumber(value);
        }

        /// Reads the [display] section the way the engine's own reader
        /// does: section and key names without regard to case,
        /// whitespace trimmed off both, a line beginning with ; or # a
        /// comment, a value optionally in double quotes, and the last
        /// value given for a repeated key.
        private static Dictionary<string, string> ReadDisplaySection()
        {
            var values = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            string[] lines;
            bool inDisplay = false;

            try
            {
                lines = File.ReadAllLines(FilePath);
            }
            catch (Exception)
            {
                // No file, or one that cannot be read, is no settings at
                // all -- the same answer the engine arrives at, and not
                // an error here either: the form falls back to its
                // defaults and offers to save them.
                return values;
            }

            foreach (string raw in lines)
            {
                string line = raw.Trim();
                string section;
                int eq;

                if (line.Length == 0 || line[0] == ';' || line[0] == '#')
                    continue;

                if (TrySection(line, out section))
                {
                    inDisplay = string.Equals(section, DisplaySection,
                                              StringComparison.OrdinalIgnoreCase);
                    continue;
                }
                if (!inDisplay)
                    continue;

                eq = line.IndexOf('=');
                if (eq < 0)
                    continue;
                values[line.Substring(0, eq).Trim()] =
                    Unquote(line.Substring(eq + 1).Trim());
            }
            return values;
        }

        /// A section header, [like this]. Returns false for anything
        /// else, including an unclosed bracket, which the engine also
        /// declines to read as a section.
        private static bool TrySection(string line, out string name)
        {
            int close;

            name = null;
            if (line.Length == 0 || line[0] != '[')
                return false;
            close = line.IndexOf(']');
            if (close < 0)
                return false;
            name = line.Substring(1, close - 1).Trim();
            return true;
        }

        /// A value may be wrapped in double quotes, which is how the ini
        /// gives a name with spaces in it. A quote on only one end is
        /// part of the value.
        private static string Unquote(string value)
        {
            if (value.Length >= 2 && value[0] == '"' && value[value.Length - 1] == '"')
                return value.Substring(1, value.Length - 2);
            return value;
        }

        /// Decimal, hex with a 0x prefix, or octal with a leading zero,
        /// which is the set the engine's reader takes. Anything else is
        /// no value: in particular the engine does not strip a comment
        /// from the end of a value, so a line that carries one has no
        /// number on it as far as either of us is concerned.
        private static int? ParseNumber(string value)
        {
            string digits;
            bool negative = false;
            int numberBase = 10;
            long parsed;

            if (string.IsNullOrEmpty(value))
                return null;

            digits = value;
            if (digits[0] == '+' || digits[0] == '-')
            {
                negative = digits[0] == '-';
                digits = digits.Substring(1);
            }
            if (digits.Length == 0)
                return null;

            if (digits.Length > 2 && digits[0] == '0' && (digits[1] == 'x' || digits[1] == 'X'))
            {
                numberBase = 16;
                digits = digits.Substring(2);
            }
            else if (digits.Length > 1 && digits[0] == '0')
            {
                numberBase = 8;
                digits = digits.Substring(1);
            }

            foreach (char c in digits)
            {
                if (!IsDigit(c, numberBase))
                    return null;
            }

            try
            {
                parsed = Convert.ToInt64(digits, numberBase);
            }
            catch (Exception)
            {
                return null;
            }
            if (negative)
                parsed = -parsed;

            // Anything that will not fit 32 bits is no value. What does
            // is kept as those bits, so a setting written in hex with
            // the top bit set reads as the negative int standing for the
            // same bits rather than being thrown out for being too big.
            if (parsed < int.MinValue || parsed > uint.MaxValue)
                return null;
            return unchecked((int)parsed);
        }

        private static bool IsDigit(char c, int numberBase)
        {
            int v;

            if (c >= '0' && c <= '9')
                v = c - '0';
            else if (c >= 'a' && c <= 'f')
                v = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F')
                v = c - 'A' + 10;
            else
                return false;
            return v < numberBase;
        }

        /// Writes the seven [display] settings and leaves the rest of
        /// the file exactly as it was.
        ///
        /// settings.ini may be hand-edited, and anything in it that is
        /// not one of the seven keys this application knows nothing
        /// about -- its comments above all -- so it is edited line by
        /// line rather than written out afresh: each of the seven keys
        /// already in [display] has its value replaced where it stands,
        /// any that are missing are added at the end of that section,
        /// and a file with no [display] section at all gets one
        /// appended. A file that is not there at all is created, folder
        /// and all.
        public static void Write(int adapter, int mode, uint zBufferDepth, bool mipMapping,
                                 bool windowed, int width, int height)
        {
            var values = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase)
            {
                { VideoAdapter, Text(adapter) },
                { Mode, Text(mode) },
                { ZBufferDepth, Text(unchecked((int)zBufferDepth)) },
                { MipMapping, Text(mipMapping ? 1 : 0) },
                { Windowed, Text(windowed ? 1 : 0) },
                { Width, Text(width) },
                { Height, Text(height) },
            };
            string path = FilePath;
            string existing = null;

            try
            {
                if (File.Exists(path))
                    existing = File.ReadAllText(path);
            }
            catch (Exception ex)
            {
                // Rewriting a file whose contents could not be read
                // would throw away the parts of it that are not ours,
                // so this is as fatal as not being able to write.
                throw new InvalidOperationException("Could not read " + path + ".", ex);
            }

            string newline = Environment.NewLine;
            bool endsWithNewline = true;
            var lines = new List<string>();

            if (existing != null)
            {
                // The file's own line ending is kept. It is hand-edited,
                // and a tool that quietly re-ended every line of it
                // would show up as a change to the whole file.
                if (existing.IndexOf("\r\n", StringComparison.Ordinal) >= 0)
                    newline = "\r\n";
                else if (existing.IndexOf('\n') >= 0)
                    newline = "\n";

                endsWithNewline = existing.Length == 0
                                  || existing.EndsWith("\n", StringComparison.Ordinal);
                lines.AddRange(existing.Split(new[] { "\r\n", "\n" }, StringSplitOptions.None));
                if (endsWithNewline && lines.Count > 0 && lines[lines.Count - 1].Length == 0)
                    lines.RemoveAt(lines.Count - 1);
            }

            var written = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            var missing = new List<string>();
            bool inDisplay = false;
            int sectionEnd = -1;

            for (int i = 0; i < lines.Count; i++)
            {
                string trimmed = lines[i].Trim();
                string section;
                string key;
                string value;
                int eq;

                if (trimmed.Length == 0 || trimmed[0] == ';' || trimmed[0] == '#')
                    continue;

                if (TrySection(trimmed, out section))
                {
                    inDisplay = string.Equals(section, DisplaySection,
                                              StringComparison.OrdinalIgnoreCase);
                    if (inDisplay)
                        sectionEnd = i;     // an empty section grows from its header
                    continue;
                }
                if (!inDisplay)
                    continue;

                sectionEnd = i;
                eq = lines[i].IndexOf('=');
                if (eq < 0)
                    continue;
                key = lines[i].Substring(0, eq).Trim();
                if (!values.TryGetValue(key, out value))
                    continue;
                // Every occurrence is replaced, not just the first: the
                // engine takes the last value given for a repeated key,
                // so leaving an earlier one standing would be harmless
                // but leaving a later one would not.
                lines[i] = ReplaceValue(lines[i], eq, value);
                written.Add(key);
            }

            foreach (string key in DisplayKeys)
            {
                if (!written.Contains(key))
                    missing.Add(SettingLine(key, values[key]));
            }

            if (sectionEnd < 0)
            {
                if (lines.Count > 0 && lines[lines.Count - 1].Trim().Length != 0)
                    lines.Add(string.Empty);
                lines.Add("[" + DisplaySection + "]");
                lines.AddRange(missing);
            }
            else
            {
                lines.InsertRange(sectionEnd + 1, missing);
            }

            string text = string.Join(newline, lines);

            if (endsWithNewline)
                text += newline;

            try
            {
                // The folder may not be there yet: this application can
                // be run before the game ever has been, and then nothing
                // has made it. The engine makes it too, on its way to
                // the same file, so whichever runs first does it.
                Directory.CreateDirectory(Path.GetDirectoryName(path));
                // No byte order mark: the engine reads the file a line at
                // a time and a mark would be part of the first one.
                File.WriteAllText(path, text, new UTF8Encoding(false));
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException("Could not write " + path + ".", ex);
            }
        }

        /// Puts a new value into a setting line that is already there,
        /// keeping the line's own spelling of the key, the spacing
        /// around the sign, and any comment on the end of it.
        private static string ReplaceValue(string line, int eq, string value)
        {
            string rest = line.Substring(eq + 1);
            string body;
            string tail = string.Empty;
            int lead = 0;
            int comment;
            int end;

            while (lead < rest.Length && (rest[lead] == ' ' || rest[lead] == '\t'))
                lead++;
            body = rest.Substring(lead);

            comment = body.IndexOfAny(new[] { ';', '#' });
            if (comment >= 0)
            {
                tail = body.Substring(comment);
                body = body.Substring(0, comment);
            }

            // Whatever separated the old value from its comment belongs
            // with the comment rather than with the value.
            end = body.Length;
            while (end > 0 && (body[end - 1] == ' ' || body[end - 1] == '\t'))
                end--;
            tail = body.Substring(end) + tail;

            return line.Substring(0, eq + 1) + rest.Substring(0, lead) + value + tail;
        }

        private static string SettingLine(string key, string value)
        {
            return key.PadRight(KeyColumn) + " = " + value;
        }

        private static string Text(int value)
        {
            return value.ToString(CultureInfo.InvariantCulture);
        }
    }
}
