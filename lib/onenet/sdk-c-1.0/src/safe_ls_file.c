#include "safe_ls_file.h"
#include "regina/allocator.h"
#include "regina/error.h"

/* call free() to free the return value if no use. */
static char *safe_ls_file_get_tmppath(const char *path)
{
    char *tmppath;
    size_t len = regina_strlen(path) + 5;

    tmppath = regina_malloc(len);
    if (!tmppath)
        return NULL;

    regina_snprintf(tmppath, len, "%s.bak", path);
    return tmppath;
}

int safe_save_file(const char *path,
                   int (*write)(void *context, regina_file_t *file),
                   void *context)
{
    char *tmppath;
    int suc = 0;

    tmppath = safe_ls_file_get_tmppath(path);

    if (tmppath) {
        regina_file_t *file = regina_file_open(tmppath, "w");
        if (file) {
            suc = write(context, file);

            if (suc > 0){
                /* do not worry about failed, for we can detect it and
                   defend the bad data. */
                (void)regina_file_rename(tmppath, path);
            }

            regina_file_close(file);
        }

       regina_free(tmppath);
    }

    return suc;
}

#define safe_do_load_file(Path, Read, Context, Suc) do {   \
        regina_file_t *file = regina_file_open(Path, "r");  \
        if (file) {                                         \
            suc = Read(Context, file);                      \
            regina_file_close(file);                        \
        }                                                   \
        else { Suc = REGINA_ERR_NOT_FOUND; }                \
    } while (0)


int safe_load_file(const char *path,
                   int (*read)(void *context, regina_file_t *file),
                   void *context)
{
    char *tmppath;
    int suc = 0;

    tmppath = safe_ls_file_get_tmppath(path);
    if (tmppath) {
        safe_do_load_file(tmppath, read, context, suc);

        if (suc > 0)
            (void)regina_file_rename(tmppath, path);
        else if (0 == suc)
            (void)regina_file_remove(tmppath);

        regina_free(tmppath);
    }

    if (suc <= 0)
        safe_do_load_file(path, read, context, suc);

    return suc;
}
