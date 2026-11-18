#pragma once
#include <stddef.h>

/* Where things are.
 *
 * Two places, and the division is what the player would expect:
 *
 * - **The install**, beside the executable. Everything the game reads
 *   and never writes: the big file, the special array, the settings,
 *   and data/<folder>/ for the scripts, shaders and fonts. Not the
 *   working directory -- a game started from a shortcut has whatever
 *   working directory the shortcut says, and the data is part of the
 *   install.
 * - **The player's own**, which is where the game writes: saves and
 *   screenshots. Under Documents on Windows, because that is where a
 *   Windows player looks for them, and under the user's data
 *   directory elsewhere, because that is the convention there.
 *
 * One copy of each rule, because there are several callers and two
 * copies of a path rule drift in opposite directions.
 *
 * Each of these writes the whole path into out and answers whether it
 * fits. A truncated path is reported as a failure rather than handed
 * back short, where it would fail to open for a reason nobody can
 * see.
 */

/* <the executable's directory>/<name> */
int DPT_b_ExePath(const char* name, char* out, size_t n);

/* <the executable's directory>/data/<folder>/<name> */
int DPT_b_DataPath(const char* folder, const char* name, char* out, size_t n);

/* <the player's own directory>/<name>, the directory made if it is
   not there yet. This is the only one of the three the game writes
   to. */
int DPT_b_UserPath(const char* name, char* out, size_t n);

/* <the player's own directory>/screenshots/<the date and time>.png,
   both directories made if they are not there yet.
 *
 * Here rather than in a renderer because both renderers take
 * screenshots and the player wants them in one place whichever one is
 * running. The name is the local date and time down to the
 * millisecond, which is what keeps two in the same second apart.
 */
int DPT_b_ScreenshotPath(char* out, size_t n);
