#include "datafiles.h"
#include "dbg.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* As many videos as any one build is likely to name. An entry past
   this is reported and dropped rather than growing the table. */
#define DAT_MAXVIDEOS 32

/* A line longer than this is read in pieces and the remainder
   discarded, which keeps a stray binary file from being mistaken for
   a very long setting. */
#define DAT_LINEMAX   512

enum { DAT_SECTION_NONE, DAT_SECTION_DATA, DAT_SECTION_VIDEO };

typedef struct sDatVideo
{
    unsigned id;
    char file[DAT_NAMEMAX];
} tDatVideo;

static char      dat_bigFile[DAT_NAMEMAX];
static int       dat_binarized;
static tDatVideo dat_videos[DAT_MAXVIDEOS];
static int       dat_videoNum;

static int DAT_l_b_Space(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* Trims both ends in place and returns the start of what is left. */
static char* DAT_l_pc_Trim(char* s)
{
    char* end;

    while (*s && DAT_l_b_Space(*s))
        s++;
    end = s + strlen(s);
    while (end > s && DAT_l_b_Space(end[-1]))
        end--;
    *end = '\0';
    return s;
}

/* A value may be quoted, which is the only way to give a name with a
   trailing space and the clearest way to give one with spaces in the
   middle. */
static char* DAT_l_pc_Unquote(char* s)
{
    size_t len = strlen(s);

    if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
    {
        s[len - 1] = '\0';
        return s + 1;
    }
    return s;
}

static int DAT_l_b_SameWord(const char* a, const char* b)
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
static int DAT_l_b_Flag(const char* v)
{
    if (DAT_l_b_SameWord(v, "yes") || DAT_l_b_SameWord(v, "true")
        || DAT_l_b_SameWord(v, "on"))
        return 1;
    return atoi(v) != 0;
}

static void DAT_l_CopyName(char* dest, const char* src)
{
    size_t n = strlen(src);

    if (n >= DAT_NAMEMAX)
        n = DAT_NAMEMAX - 1;
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
static int DAT_l_b_Id(const char* s, unsigned* out)
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

static void DAT_l_AddVideo(unsigned id, const char* file)
{
    int i;

    for (i = 0; i < dat_videoNum; i++)
    {
        if (dat_videos[i].id == id)
        {
            /* a repeated id: the last one wins, which is what an ini
               reader is normally taken to do */
            DAT_l_CopyName(dat_videos[i].file, file);
            return;
        }
    }
    if (dat_videoNum >= DAT_MAXVIDEOS)
    {
        DBG_OutputDebugString("%s: more than %d videos, id 0x%08X dropped\n",
                              DAT_INIFILE, DAT_MAXVIDEOS, id);
        return;
    }
    dat_videos[dat_videoNum].id = id;
    DAT_l_CopyName(dat_videos[dat_videoNum].file, file);
    dat_videoNum++;
}

static void DAT_l_Setting(int section, char* key, char* value)
{
    unsigned id;

    if (section == DAT_SECTION_DATA)
    {
        if (DAT_l_b_SameWord(key, "bigfile"))
            DAT_l_CopyName(dat_bigFile, value);
        else if (DAT_l_b_SameWord(key, "binarized"))
            dat_binarized = DAT_l_b_Flag(value);
        else
            DBG_OutputDebugString("%s: [data] has no setting \"%s\"\n",
                                  DAT_INIFILE, key);
        return;
    }
    if (section == DAT_SECTION_VIDEO)
    {
        if (DAT_l_b_Id(key, &id))
            DAT_l_AddVideo(id, value);
        else
            DBG_OutputDebugString("%s: [video] key \"%s\" is not an id\n",
                                  DAT_INIFILE, key);
        return;
    }
    DBG_OutputDebugString("%s: \"%s\" is outside any section\n",
                          DAT_INIFILE, key);
}

static void DAT_l_Read(const char* path)
{
    FILE* f;
    char line[DAT_LINEMAX];
    int section = DAT_SECTION_NONE;

    dat_bigFile[0] = '\0';
    dat_binarized = 0;
    dat_videoNum = 0;

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

        s = DAT_l_pc_Trim(line);
        if (!*s || *s == ';' || *s == '#')
            continue;

        if (*s == '[')
        {
            char* close = strchr(s, ']');

            if (!close)
            {
                DBG_OutputDebugString("%s: unclosed section \"%s\"\n",
                                      DAT_INIFILE, s);
                continue;
            }
            *close = '\0';
            s = DAT_l_pc_Trim(s + 1);
            if (DAT_l_b_SameWord(s, "data"))
                section = DAT_SECTION_DATA;
            else if (DAT_l_b_SameWord(s, "video"))
                section = DAT_SECTION_VIDEO;
            else
            {
                section = DAT_SECTION_NONE;
                DBG_OutputDebugString("%s: unknown section [%s]\n",
                                      DAT_INIFILE, s);
            }
            continue;
        }

        eq = strchr(s, '=');
        if (!eq)
        {
            DBG_OutputDebugString("%s: \"%s\" is not a setting\n",
                                  DAT_INIFILE, s);
            continue;
        }
        *eq = '\0';
        DAT_l_Setting(section, DAT_l_pc_Trim(s),
                      DAT_l_pc_Unquote(DAT_l_pc_Trim(eq + 1)));
    }

    fclose(f);
}

void DAT_InitModule(void)
{
    DAT_l_Read(DAT_INIFILE);
}

const char* DAT_pc_BigFile(void)
{
    return dat_bigFile[0] ? dat_bigFile : NULL;
}

int DAT_b_Binarized(void)
{
    return dat_binarized;
}

const char* DAT_pc_Video(int id)
{
    int i;

    /* compared as the 32 bits that were pushed, so a key written in
       the file as hex and one written as a decimal match the same
       call whichever way round they are spelled */
    for (i = 0; i < dat_videoNum; i++)
    {
        if (dat_videos[i].id == (unsigned)id)
            return dat_videos[i].file[0] ? dat_videos[i].file : NULL;
    }
    return NULL;
}
