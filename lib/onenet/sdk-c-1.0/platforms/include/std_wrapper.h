#ifndef __REGINA_PLATFORMS_STD_WRAPPER_H__
#define __REGINA_PLATFORMS_STD_WRAPPER_H__

#include "regina/platform.h"

/* define errno used by avro */

#ifndef ENOMEM
#define	ENOMEM		12	/* Out of memory */
#endif

#ifndef EINVAL
#define EINVAL      22 /* Invalid argument */
#endif

#ifndef ENOSPC
#define ENOSPC      28   /* No space left on device */
#endif

#ifndef ERAGNE
#define	ERANGE		34	/* Math result not representable */
#endif


#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef INT_MIN
#define INT_MIN (-INT_MAX - 1)
#endif

#define REGINA_FILE_SEEK_SET   0     /* Seek from beginning of file.  */
#define REGINA_FILE_SEEK_CUR   1     /* Seek from current position.  */
#define REGINA_FILE_SEEK_END   2     /* Seek from end of file.  */

void *regina_memset(void *s, int c, size_t n);
void *regina_memcpy(void *dest, const void *src, size_t n);
int regina_memcmp(const void *s1, const void *s2, size_t n);

size_t regina_strlen(const char *s);
char *regina_strdup(const char *s);
int regina_strcmp(const char *s1, const char *s2);
char *regina_strcpy(char *dest, const char *src);

int regina_snprintf(char *str, size_t size, const char *format, ...);

typedef void regina_file_t;
regina_file_t *regina_file_open(const char *path, const char *mode);
int regina_file_close(regina_file_t *file);
int regina_file_rename(const char *oldpath, const char *newpath);
int regina_file_remove(const char *path);
size_t regina_file_write(const void *ptr, size_t size,
                         size_t nmemb, regina_file_t *stream);
size_t regina_file_read(void *ptr, size_t size, size_t nmemb, regina_file_t *stream);
int regina_file_seek(regina_file_t *stream, long offset, int whence);
long regina_file_tell(regina_file_t *stream);

typedef int64_t regina_time_t;
regina_time_t regina_time(regina_time_t *t);


#endif /* __REGINA_PLATFORMS_STD_WRAPPER_H__ */
