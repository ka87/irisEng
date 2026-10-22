#include "sideload.h"
#include "dbg.h"

/*
    The table is built once at startup by listing the directory, so a
    lookup during loading costs a walk over a handful of entries and
    never touches the disk. There is no sensible upper bound on what
    someone might drop in, but a development folder holds a few files
    at a time, so the array grows by doubling rather than being
    fixed.
*/

#define SIDE_C_InitialCapacity 16

typedef struct sSideEntry
{
    uint32_t key;
    char     path[MAX_PATH];   /* empty for an interpret.txt entry */
} tSideEntry;

int SIDE_gb_Enabled = 0;
int SIDE_gi_Count = 0;

static char        SIDE_gs_Dir[MAX_PATH];
static tSideEntry* SIDE_gp_Entries = NULL;
static int         SIDE_gi_Capacity = 0;


/* Eight hex digits, optionally 0x-prefixed, and nothing else before
   the extension. Returns FALSE for a name that is not a key. */
static BOOL SIDE_b_KeyFromName(const char* name, uint32_t* keyRet)
{
    uint32_t key = 0;
    int digits = 0;

    if (name[0] == '0' && (name[1] == 'x' || name[1] == 'X'))
        name += 2;

    while (*name && *name != '.')
    {
        char c = *name++;
        int v;

        if (c >= '0' && c <= '9')       v = c - '0';
        else if (c >= 'a' && c <= 'f')  v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')  v = c - 'A' + 10;
        else return FALSE;

        key = (key << 4) | (uint32_t)v;
        if (++digits > 8)
            return FALSE;
    }

    if (digits != 8)
        return FALSE;

    *keyRet = key;
    return TRUE;
}


static tSideEntry* SIDE_p_Find(uint32_t key)
{
    int i;

    for (i = 0; i < SIDE_gi_Count; i++)
    {
        if (SIDE_gp_Entries[i].key == key)
            return &SIDE_gp_Entries[i];
    }
    return NULL;
}


static tSideEntry* SIDE_p_Add(uint32_t key)
{
    tSideEntry* entry = SIDE_p_Find(key);

    if (entry)
        return entry;

    if (SIDE_gi_Count == SIDE_gi_Capacity)
    {
        int wanted = SIDE_gi_Capacity ? SIDE_gi_Capacity * 2
                                      : SIDE_C_InitialCapacity;
        tSideEntry* grown = (tSideEntry*)realloc(SIDE_gp_Entries,
                                                 wanted * sizeof(tSideEntry));
        if (!grown)
            return NULL;

        SIDE_gp_Entries = grown;
        SIDE_gi_Capacity = wanted;
    }

    entry = &SIDE_gp_Entries[SIDE_gi_Count++];
    entry->key = key;
    entry->path[0] = '\0';
    return entry;
}


/* interpret.txt: one key per line, # and // start a comment. */
static void SIDE_ReadInterpretList(void)
{
    char  path[MAX_PATH];
    char  line[256];
    FILE* fh;

    snprintf(path, sizeof(path), "%sinterpret.txt", SIDE_gs_Dir);
    fh = fopen(path, "r");
    if (!fh)
        return;

    while (fgets(line, sizeof(line), fh))
    {
        char* p = line;
        char* end;
        uint32_t key;

        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '#' || *p == '\0' || *p == '\n' || *p == '\r'
            || (p[0] == '/' && p[1] == '/'))
            continue;

        /* Trim whatever follows the token, so a trailing comment or a
           name written after the key is harmless. */
        end = p;
        while (*end && *end != ' ' && *end != '\t'
               && *end != '\n' && *end != '\r')
            end++;
        *end = '\0';

        if (SIDE_b_KeyFromName(p, &key))
        {
            if (SIDE_p_Add(key))
                DBG_OutputDebugString("SIDE: 0x%08x will be interpreted\n", key);
        }
        else
        {
            DBG_OutputDebugString("SIDE: interpret.txt: not a key: %s\n", p);
        }
    }

    fclose(fh);
}


void SIDE_InitModule(void)
{
    WIN32_FIND_DATAA find;
    HANDLE           handle;
    char             pattern[MAX_PATH];
    char*            cut;
    DWORD            attrs;

    SIDE_gb_Enabled = 0;
    SIDE_gi_Count = 0;

    if (!GetModuleFileNameA(NULL, SIDE_gs_Dir, MAX_PATH))
        return;

    cut = strrchr(SIDE_gs_Dir, '\\');
    if (!cut)
        cut = strrchr(SIDE_gs_Dir, '/');
    if (!cut)
        return;

    /* Keep the separator, so everything below can just append. An
       exe sitting deep enough that the folder name does not fit is
       not a case worth handling beyond declining to look. */
    if ((size_t)(cut + 1 - SIDE_gs_Dir) + sizeof("sideload\\") > MAX_PATH)
        return;
    strcpy(cut + 1, "sideload\\");

    attrs = GetFileAttributesA(SIDE_gs_Dir);
    if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY))
        return;   /* no folder, nothing to do, normal install */

    snprintf(pattern, sizeof(pattern), "%s*", SIDE_gs_Dir);
    handle = FindFirstFileA(pattern, &find);
    if (handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            uint32_t    key;
            tSideEntry* entry;

            if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                continue;
            if (!SIDE_b_KeyFromName(find.cFileName, &key))
                continue;   /* notes, interpret.txt, spare copies */

            entry = SIDE_p_Add(key);
            if (!entry)
                continue;

            if (snprintf(entry->path, sizeof(entry->path), "%s%s",
                         SIDE_gs_Dir, find.cFileName) >= sizeof(entry->path))
            {
                /* The path did not fit, so the entry would name a
                   file that cannot be opened. Drop it rather than
                   leave a truncated path behind. */
                DBG_OutputDebugString("SIDE: path too long: %s\n", find.cFileName);
                entry->path[0] = '\0';
                continue;
            }
            DBG_OutputDebugString("SIDE: 0x%08x <- %s\n", key, find.cFileName);
        }
        while (FindNextFileA(handle, &find));

        FindClose(handle);
    }

    SIDE_ReadInterpretList();

    SIDE_gb_Enabled = (SIDE_gi_Count > 0);
    if (SIDE_gb_Enabled)
        DBG_OutputDebugString("SIDE: %d key(s) overridden from %s\n",
                              SIDE_gi_Count, SIDE_gs_Dir);
}


void SIDE_CloseModule(void)
{
    free(SIDE_gp_Entries);
    SIDE_gp_Entries = NULL;
    SIDE_gi_Capacity = 0;
    SIDE_gi_Count = 0;
    SIDE_gb_Enabled = 0;
}


BOOL SIDE_b_Overrides(uint32_t key)
{
    if (!SIDE_gb_Enabled)
        return FALSE;
    return SIDE_p_Find(key) != NULL;
}


char* SIDE_pc_ReadByKey(uint32_t key, int* sizeRet)
{
    tSideEntry* entry;
    FILE*       fh;
    long        size;
    char*       buffer;

    if (sizeRet)
        *sizeRet = 0;

    if (!SIDE_gb_Enabled)
        return NULL;

    entry = SIDE_p_Find(key);
    if (!entry || !entry->path[0])
        return NULL;        /* listed for interpreting, but no file */

    fh = fopen(entry->path, "rb");
    if (!fh)
    {
        DBG_OutputDebugString("SIDE: cannot open %s\n", entry->path);
        return NULL;
    }

    fseek(fh, 0, SEEK_END);
    size = ftell(fh);
    fseek(fh, 0, SEEK_SET);

    if (size <= 0)
    {
        fclose(fh);
        DBG_OutputDebugString("SIDE: %s is empty\n", entry->path);
        return NULL;
    }

    /* One byte over, for the same reason BIG_pc_ReadFileTmp asks for
       len + 1: a reader that runs to the end of the block and looks
       once more stays inside the allocation. */
    buffer = (char*)malloc((size_t)size + 1);
    if (!buffer)
    {
        fclose(fh);
        return NULL;
    }

    if (fread(buffer, 1, (size_t)size, fh) != (size_t)size)
    {
        fclose(fh);
        free(buffer);
        DBG_OutputDebugString("SIDE: short read on %s\n", entry->path);
        return NULL;
    }
    fclose(fh);

    buffer[size] = '\0';
    if (sizeRet)
        *sizeRet = (int)size;

    DBG_OutputDebugString("SIDE: 0x%08x sideloaded, %ld bytes\n", key, size);
    return buffer;
}


void SIDE_Free(char* buffer)
{
    free(buffer);
}
