#include "std_wrapper.h"
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>



void *regina_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}

void *regina_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}

int regina_memcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}

size_t regina_strlen(const char *s)
{
    return strlen(s);
}

char *regina_strdup(const char *s)
{
    return strdup(s);
}

int regina_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

char *regina_strcpy(char *dest, const char *src)
{
    return strcpy(dest, src);
}

int regina_snprintf(char *str, size_t size, const char *format, ...)
{
    int ret;
    va_list args;
    va_start(args, format);
    ret = vsnprintf(str, size, format, args);
    va_end(args);
    return ret;
}

regina_file_t *regina_file_open(const char *path, const char *mode)
{
    return fopen(path, mode);
}

int regina_file_close(regina_file_t *file)
{
    return fclose((FILE*)file);
}

int regina_file_rename(const char *oldpath, const char *newpath)
{
    return rename(oldpath, newpath);
}

int regina_file_remove(const char *path)
{
    return remove(path);
}

size_t regina_file_write(const void *ptr, size_t size,
                                   size_t nmemb, regina_file_t *stream)
{
    return fwrite(ptr, size, nmemb, (FILE*)stream);
}

size_t regina_file_read(void *ptr, size_t size, size_t nmemb, regina_file_t *stream)
{
    return fread(ptr, size, nmemb, (FILE*)stream);
}

int regina_file_seek(regina_file_t *stream, long offset, int whence)
{
    return fseek((FILE*)stream, offset, whence);
}

long regina_file_tell(regina_file_t *stream)
{
    return ftell((FILE*)stream);
}

regina_time_t regina_time(regina_time_t *t)
{
    time_t sec;
    time(&sec);
    if (t)
        *t = (int64_t)sec;
    return (int64_t)sec;
}
