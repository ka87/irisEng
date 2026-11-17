#pragma once

/* Writes a 24-bit PNG from 32-bit pixels in the order Windows and
 * Direct3D keep them -- blue, green, red, then a byte that is ignored.
 *
 * `pitch` is the distance between rows in bytes, which is not always
 * the width: a locked surface and a read-back frame both come with
 * their own padding. `bottomUp` says the first row in memory is the
 * bottom of the picture, which is how OpenGL hands a frame back and
 * the opposite of how Direct3D does.
 *
 * Returns non-zero when the file was written.
 *
 * The file is named apart from libpng's own pngwrite.c, which this is
 * built on: Visual Studio puts every object in one directory and two
 * sources of the same name collide there.
 */
int PNG_i_WriteBgra(const char* path, const unsigned char* pixels,
                    int pitch, int width, int height, int bottomUp);
