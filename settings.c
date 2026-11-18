#include "settings.h"
#include "datapath.h"
#include "dbg.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* As many videos as any one build is likely to name. An entry past
   this is reported and dropped rather than growing the table. */
#define SET_MAXVIDEOS 32

/* A line longer than this is read in pieces and the remainder
   discarded, which keeps a stray binary file from being mistaken for
   a very long setting. */
#define SET_LINEMAX   512

enum { SET_SECTION_NONE, SET_SECTION_DATA, SET_SECTION_VIDEO,
       SET_SECTION_RENDER, SET_SECTION_DISPLAY };

/* A display setting, and whether the ini said anything about it. The
   two are kept apart because nought is a real answer to most of these
   -- adapter nought is the first adapter, and windowed nought is
   fullscreen -- so "not set" cannot be a value. */
typedef struct sDatSetting
{
    int said;
    int value;
} tDatSetting;

typedef struct sDatVideo
{
    unsigned id;
    char file[SET_NAMEMAX];
} tDatVideo;

static char      set_bigFile[SET_NAMEMAX];
static char      set_specialArray[SET_NAMEMAX] = "jade.spe";
static tDatSetting set_adapter;
static tDatSetting set_mode;
static tDatSetting set_zbuffer;
static tDatSetting set_mipmapping;
static tDatSetting set_windowed;
static tDatSetting set_maxFps;
static tDatSetting set_width;
static tDatSetting set_height;
static char      set_backend[SET_NAMEMAX];
static int       set_binarized;
static tDatVideo set_videos[SET_MAXVIDEOS];
static int       set_videoNum;

static int SET_l_b_Space(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* Trims both ends in place and returns the start of what is left. */
static char* SET_l_pc_Trim(char* s)
{
    char* end;

    while (*s && SET_l_b_Space(*s))
        s++;
    end = s + strlen(s);
    while (end > s && SET_l_b_Space(end[-1]))
        end--;
    *end = '\0';
    return s;
}

/* A value may be quoted, which is the only way to give a name with a
   trailing space and the clearest way to give one with spaces in the
   middle. */
static char* SET_l_pc_Unquote(char* s)
{
    size_t len = strlen(s);

    if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
    {
        s[len - 1] = '\0';
        return s + 1;
    }
    return s;
}

static int SET_l_b_SameWord(const char* a, const char* b)
{
    while (*a && *b)
    {
        char ca = *a;
        char cb = *b;

        if (ca >= 'A' && ca <= 'Z')
            ca = (char)(ca - 'A' + 'a');
        if (cb >= 'A' && cb <= 'Z')
            cb = (char)(cb - 'A' + 'a');
        if (ca != cb)
            return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

/* 1, yes, true and on all mean on, and so does any other number that
   is not zero. Anything else is off. */
static int SET_l_b_Flag(const char* v)
{
    if (SET_l_b_SameWord(v, "yes") || SET_l_b_SameWord(v, "true")
        || SET_l_b_SameWord(v, "on"))
        return 1;
    return atoi(v) != 0;
}

static void SET_l_CopyName(char* dest, const char* src)
{
    size_t n = strlen(src);

    if (n >= SET_NAMEMAX)
        n = SET_NAMEMAX - 1;
    memcpy(dest, src, n);
    dest[n] = '\0';
}

/* The key under [video] is the id the script passes, and the whole
   32-bit range of it is in use -- what gets pushed is a resource key,
   not a small index, so 0x3001978F is the natural way to write one
   and a key with the top bit set does not fit a positive int. So this
   takes hex with a 0x prefix as well as decimal, reads it as unsigned
   32-bit, and insists the whole key is a number and nothing else: a
   mistyped key is worth saying so about rather than silently becoming
   id 0, which atoi alone would do. */
static int SET_l_b_Id(const char* s, unsigned* out)
{
    const char* p = s;
    char* end;
    unsigned long v;
    int base = 10;
    int negate = 0;

    if (*p == '-')
    {
        negate = 1;
        p++;
    }
    else if (*p == '+')
    {
        p++;
    }
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
    {
        base = 16;
        p += 2;
        if (negate)
            return 0;      /* -0x... is not a spelling anyone means */
    }
    if (!*p)
        return 0;

    errno = 0;
    v = strtoul(p, &end, base);
    if (*end || errno == ERANGE)
        return 0;

    /* The range is the key's 32 bits, whichever base it was written
       in. strtoul is 64-bit wide here, so without this a hex id with
       nine digits would quietly keep only the low eight. */
    if (v > (negate ? 0x80000000ul : 0xFFFFFFFFul))
        return 0;

    *out = negate ? (unsigned)(0u - (unsigned)v) : (unsigned)v;
    return 1;
}

static void SET_l_AddVideo(unsigned id, const char* file)
{
    int i;

    for (i = 0; i < set_videoNum; i++)
    {
        if (set_videos[i].id == id)
        {
            /* a repeated id: the last one wins, which is what an ini
               reader is normally taken to do */
            SET_l_CopyName(set_videos[i].file, file);
            return;
        }
    }
    if (set_videoNum >= SET_MAXVIDEOS)
    {
        DBG_OutputDebugString("%s: more than %d videos, id 0x%08X dropped\n",
                              SET_INIFILE, SET_MAXVIDEOS, id);
        return;
    }
    set_videos[set_videoNum].id = id;
    SET_l_CopyName(set_videos[set_videoNum].file, file);
    set_videoNum++;
}

static void SET_l_Setting(int section, char* key, char* value)
{
    unsigned id;

    if (section == SET_SECTION_DATA)
    {
        if (SET_l_b_SameWord(key, "bigfile"))
            SET_l_CopyName(set_bigFile, value);
        else if (SET_l_b_SameWord(key, "specialarray"))
            SET_l_CopyName(set_specialArray, value);
        else if (SET_l_b_SameWord(key, "binarized"))
            set_binarized = SET_l_b_Flag(value);
        else
            DBG_OutputDebugString("%s: [data] has no setting \"%s\"\n",
                                  SET_INIFILE, key);
        return;
    }
    if (section == SET_SECTION_RENDER)
    {
        if (SET_l_b_SameWord(key, "backend"))
            SET_l_CopyName(set_backend, value);
        else
            DBG_OutputDebugString("%s: [render] has no setting \"%s\"\n",
                                  SET_INIFILE, key);
        return;
    }
    if (section == SET_SECTION_DISPLAY)
    {
        static const struct { const char* name; tDatSetting* where; } known[] = {
            { "adapter",    &set_adapter },
            { "mode",       &set_mode },
            { "zbuffer",    &set_zbuffer },
            { "mipmapping", &set_mipmapping },
            { "windowed",   &set_windowed },
            { "maxfps",     &set_maxFps },
            { "width",      &set_width },
            { "height",     &set_height }
        };
        int i;

        for (i = 0; i < (int)(sizeof(known) / sizeof(known[0])); ++i)
        {
            if (SET_l_b_SameWord(key, known[i].name))
            {
                char* end = 0;
                long  v = strtol(value, &end, 0);

                if (end == value || (end && *end))
                    DBG_OutputDebugString("%s: [display] %s is not a number: \"%s\"\n",
                                          SET_INIFILE, key, value);
                else
                {
                    known[i].where->value = (int)v;
                    known[i].where->said = 1;
                }
                return;
            }
        }
        DBG_OutputDebugString("%s: [display] has no setting \"%s\"\n",
                              SET_INIFILE, key);
        return;
    }
    if (section == SET_SECTION_VIDEO)
    {
        if (SET_l_b_Id(key, &id))
            SET_l_AddVideo(id, value);
        else
            DBG_OutputDebugString("%s: [video] key \"%s\" is not an id\n",
                                  SET_INIFILE, key);
        return;
    }
    DBG_OutputDebugString("%s: \"%s\" is outside any section\n",
                          SET_INIFILE, key);
}

static void SET_l_Read(const char* path)
{
    FILE* f;
    char line[SET_LINEMAX];
    int section = SET_SECTION_NONE;

    set_bigFile[0] = '\0';
    set_backend[0] = '\0';
    set_binarized = 0;
    set_videoNum = 0;

    f = fopen(path, "r");
    if (!f)
        return;        /* no ini is not a problem; the game has its command line */

    while (fgets(line, sizeof(line), f))
    {
        char* s;
        char* eq;

        /* a line too long for the buffer: take what fits and drop the
           rest, so the remainder is not read as a setting of its own */
        if (!strchr(line, '\n') && !feof(f))
        {
            int c;

            while ((c = fgetc(f)) != EOF && c != '\n')
                ;
        }

        s = SET_l_pc_Trim(line);
        if (!*s || *s == ';' || *s == '#')
            continue;

        if (*s == '[')
        {
            char* close = strchr(s, ']');

            if (!close)
            {
                DBG_OutputDebugString("%s: unclosed section \"%s\"\n",
                                      SET_INIFILE, s);
                continue;
            }
            *close = '\0';
            s = SET_l_pc_Trim(s + 1);
            if (SET_l_b_SameWord(s, "data"))
                section = SET_SECTION_DATA;
            else if (SET_l_b_SameWord(s, "video"))
                section = SET_SECTION_VIDEO;
            else if (SET_l_b_SameWord(s, "render"))
                section = SET_SECTION_RENDER;
            else if (SET_l_b_SameWord(s, "display"))
                section = SET_SECTION_DISPLAY;
            else
            {
                section = SET_SECTION_NONE;
                DBG_OutputDebugString("%s: unknown section [%s]\n",
                                      SET_INIFILE, s);
            }
            continue;
        }

        eq = strchr(s, '=');
        if (!eq)
        {
            DBG_OutputDebugString("%s: \"%s\" is not a setting\n",
                                  SET_INIFILE, s);
            continue;
        }
        *eq = '\0';
        SET_l_Setting(section, SET_l_pc_Trim(s),
                      SET_l_pc_Unquote(SET_l_pc_Trim(eq + 1)));
    }

    fclose(f);
}

void SET_InitModule(void)
{
    char path[512];

    /* Beside the executable, not in the working directory: a game
       started from a shortcut has whatever working directory the
       shortcut says, and the settings are part of the install. */
    if (DPT_b_ExePath(SET_INIFILE, path, sizeof(path)))
        SET_l_Read(path);
    else
        SET_l_Read(SET_INIFILE);
}

const char* SET_pc_SpecialArray(void)
{
    return set_specialArray;
}

/* One shape for all eight: say whether the ini mentioned it, and
   write the value only if it did. */
#define SET_DISPLAY_READER(fn, var)        \
    int fn(int* out)                       \
    {                                      \
        if (!var.said)                     \
            return 0;                      \
        if (out)                           \
            *out = var.value;              \
        return 1;                          \
    }

SET_DISPLAY_READER(SET_b_DisplayAdapter,    set_adapter)
SET_DISPLAY_READER(SET_b_DisplayMode,       set_mode)
SET_DISPLAY_READER(SET_b_DisplayZBuffer,    set_zbuffer)
SET_DISPLAY_READER(SET_b_DisplayMipMapping, set_mipmapping)
SET_DISPLAY_READER(SET_b_DisplayWindowed,   set_windowed)
SET_DISPLAY_READER(SET_b_DisplayMaxFps,     set_maxFps)
SET_DISPLAY_READER(SET_b_DisplayWidth,      set_width)
SET_DISPLAY_READER(SET_b_DisplayHeight,     set_height)

const char* SET_pc_BigFile(void)
{
    return set_bigFile[0] ? set_bigFile : NULL;
}

const char* SET_pc_Backend(void)
{
    return set_backend[0] ? set_backend : NULL;
}

int SET_b_Binarized(void)
{
    return set_binarized;
}

const char* SET_pc_Video(int id)
{
    int i;

    /* compared as the 32 bits that were pushed, so a key written in
       the file as hex and one written as a decimal match the same
       call whichever way round they are spelled */
    for (i = 0; i < set_videoNum; i++)
    {
        if (set_videos[i].id == (unsigned)id)
            return set_videos[i].file[0] ? set_videos[i].file : NULL;
    }
    return NULL;
}
