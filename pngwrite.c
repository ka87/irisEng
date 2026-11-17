#include "pngwrite.h"

#include <stdio.h>
#include <string.h>

/* A PNG writer with no compression in it: the pixels go into one
 * stored deflate stream, which is what lets this be a few hundred
 * lines with no library behind it. A screenshot is written once and
 * read by a person, so the file being larger than it needs to be
 * costs nothing worth having.
 *
 * It was the Direct3D renderer's own, which was its home only because
 * that renderer's screenshot was the only caller. Both renderers take
 * a screenshot now, so there is one copy of it here.
 */

static unsigned int PNG_l_CrcTable[256];
static int PNG_l_CrcReady;

static void PNG_l_CrcInit(void)
{
    unsigned int c;
    int i;
    int k;

    if (PNG_l_CrcReady)
        return;
    for (i = 0; i < 256; i++)
    {
        c = (unsigned int)i;
        for (k = 0; k < 8; k++)
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        PNG_l_CrcTable[i] = c;
    }
    PNG_l_CrcReady = 1;
}

typedef struct sPngWriter
{
    FILE* file;
    unsigned int crc;
    unsigned int adlerA;
    unsigned int adlerB;
    int blockLeft;
    int rawLeft;
} tPngWriter;

static void PNG_l_Raw(tPngWriter* w, int uint8_t)
{
    fputc(uint8_t & 0xFF, w->file);
}

static void PNG_l_Chk(tPngWriter* w, int uint8_t)
{
    unsigned int b;

    b = (unsigned int)(uint8_t & 0xFF);
    fputc((int)b, w->file);
    w->crc = PNG_l_CrcTable[(w->crc ^ b) & 0xFF] ^ (w->crc >> 8);
}

static void PNG_l_Data(tPngWriter* w, int uint8_t)
{
    int n;

    if (w->blockLeft == 0)
    {
        n = w->rawLeft < 65535 ? w->rawLeft : 65535;
        PNG_l_Chk(w, w->rawLeft <= 65535 ? 1 : 0);
        PNG_l_Chk(w, n);
        PNG_l_Chk(w, n >> 8);
        PNG_l_Chk(w, ~n);
        PNG_l_Chk(w, ~n >> 8);
        w->blockLeft = n;
    }
    PNG_l_Chk(w, uint8_t);
    w->adlerA = (w->adlerA + (unsigned int)(uint8_t & 0xFF)) % 65521u;
    w->adlerB = (w->adlerB + w->adlerA) % 65521u;
    w->blockLeft--;
    w->rawLeft--;
}

static void PNG_l_BeginChunk(tPngWriter* w, int length, const char* type)
{
    PNG_l_Raw(w, length >> 24);
    PNG_l_Raw(w, length >> 16);
    PNG_l_Raw(w, length >> 8);
    PNG_l_Raw(w, length);
    w->crc = 0xFFFFFFFFu;
    PNG_l_Chk(w, type[0]);
    PNG_l_Chk(w, type[1]);
    PNG_l_Chk(w, type[2]);
    PNG_l_Chk(w, type[3]);
}

static void PNG_l_EndChunk(tPngWriter* w)
{
    unsigned int c;

    c = ~w->crc;
    PNG_l_Raw(w, (int)(c >> 24));
    PNG_l_Raw(w, (int)(c >> 16));
    PNG_l_Raw(w, (int)(c >> 8));
    PNG_l_Raw(w, (int)c);
}

int PNG_i_WriteBgra(const char* path, const unsigned char* pixels,
                    int pitch, int width, int height, int bottomUp)
{
    static const unsigned char signature[8] =
        { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    tPngWriter w;
    const unsigned char* row;
    unsigned int adler;
    int blocks;
    int raw;
    int i;
    int x;
    int y;

    PNG_l_CrcInit();
    memset(&w, 0, sizeof(w));
    w.file = fopen(path, "wb");
    if (!w.file)
        return 0;
    w.adlerA = 1;
    w.adlerB = 0;

    for (i = 0; i < 8; i++)
        PNG_l_Raw(&w, signature[i]);

    PNG_l_BeginChunk(&w, 13, "IHDR");
    PNG_l_Chk(&w, width >> 24);
    PNG_l_Chk(&w, width >> 16);
    PNG_l_Chk(&w, width >> 8);
    PNG_l_Chk(&w, width);
    PNG_l_Chk(&w, height >> 24);
    PNG_l_Chk(&w, height >> 16);
    PNG_l_Chk(&w, height >> 8);
    PNG_l_Chk(&w, height);
    PNG_l_Chk(&w, 8);
    PNG_l_Chk(&w, 2);
    PNG_l_Chk(&w, 0);
    PNG_l_Chk(&w, 0);
    PNG_l_Chk(&w, 0);
    PNG_l_EndChunk(&w);

    raw = height * (1 + width * 3);
    blocks = (raw + 65534) / 65535;
    if (blocks < 1)
        blocks = 1;
    w.rawLeft = raw;
    w.blockLeft = 0;

    PNG_l_BeginChunk(&w, 2 + blocks * 5 + raw + 4, "IDAT");
    PNG_l_Chk(&w, 0x78);
    PNG_l_Chk(&w, 0x01);
    for (y = 0; y < height; y++)
    {
        /* Bottom-up means the first row in memory is the bottom of the
           picture, which is how OpenGL hands a frame back. The rows are
           walked backwards rather than copied into another buffer
           first. */
        int source = bottomUp ? height - 1 - y : y;

        row = pixels + (size_t)source * (size_t)pitch;
        PNG_l_Data(&w, 0);
        for (x = 0; x < width; x++)
        {
            PNG_l_Data(&w, row[x * 4 + 2]);
            PNG_l_Data(&w, row[x * 4 + 1]);
            PNG_l_Data(&w, row[x * 4 + 0]);
        }
    }
    adler = (w.adlerB << 16) | w.adlerA;
    PNG_l_Chk(&w, (int)(adler >> 24));
    PNG_l_Chk(&w, (int)(adler >> 16));
    PNG_l_Chk(&w, (int)(adler >> 8));
    PNG_l_Chk(&w, (int)adler);
    PNG_l_EndChunk(&w);

    PNG_l_BeginChunk(&w, 0, "IEND");
    PNG_l_EndChunk(&w);

    fclose(w.file);
    return 1;
}

