#include "std_wrapper.h"
#include "regina/allocator.h"
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "mbedtls/base64.h"
#include "onenet_mico_platform.h"
#include "nv_dep.h"

#define MAX_FILE_LENGTH  512
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

char *regina_strdup(const char *str)
{
  char * strNew=NULL;
  if(str != NULL) {
    strNew = (char *)malloc(strlen(str)+1);
    strcpy(strNew,str);
  }
  return strNew;
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

/**********************fs io********************************************************/
void NvitemDelete(int id);
#define INVALID_FILE_ID  ((uint16_t)(~0U))

#define DEVICE_STATUS_FILE_ID        0
#define DEVICE_STATUS_BAK_FILE_ID    DEVICE_STATUS_FILE_ID
#define DEVICE_CONFIG_FILE_ID        1
#define DEVICE_CONFIG_BAK_FILE_ID    DEVICE_CONFIG_FILE_ID

typedef struct file_name_pair_t {
  const char *name;
  uint16_t id;
} file_name_pair_t;

typedef enum {
  FILE_MODE_READ  = 0x01,
  FILE_MODE_WRITE = 0x02
} file_mode_t;

typedef struct file_wrapper_t {
  uint16_t id;
  uint16_t length; /* the length of the file block*/
  uint16_t cursor;
  file_mode_t mode;
  char buf[MAX_FILE_LENGTH];
} file_wrapper_t;

static file_name_pair_t map[] = {
  {"regina_device",     DEVICE_STATUS_FILE_ID},
  {"regina_device.bak", DEVICE_STATUS_BAK_FILE_ID},
  {"regina_config",     DEVICE_CONFIG_FILE_ID},
  {"regina_config.bak", DEVICE_CONFIG_BAK_FILE_ID}
};

static uint16_t std_wrapper_get_file_id(const char *name)
{
  int i;
  for (i = 0; i < sizeof(map) / sizeof(map[0]); ++i) {
    if (regina_strcmp(name, map[i].name) == 0)
      return map[i].id;
  }
  return INVALID_FILE_ID;
}

regina_file_t *regina_file_open(const char *path, const char *mode)
{
  uint16_t id;
  file_wrapper_t *file;
  id = std_wrapper_get_file_id(path);
  if (INVALID_FILE_ID == id)
    return NULL;
  
  if (MAX_FILE_LENGTH != NVITEM_GetLength(id))
    return NULL;
  
  file = (file_wrapper_t*)regina_malloc(sizeof(file_wrapper_t));
  if (!file)
    return NULL;
  
  regina_memset(file, 0, sizeof(file_wrapper_t));
  file->id = id;
  
  if (regina_strcmp("r", mode) == 0) {
    char buf[MAX_FILE_LENGTH] = {0};
    
    file->mode |= FILE_MODE_READ;
    
    if (0 != NvitemRead(file->id, MAX_FILE_LENGTH, buf)) {
      regina_free(file);
      file = NULL;
    }
    else {
      size_t real_length;
      size_t length = strlen(buf);
      if (0 != mbedtls_base64_decode((unsigned char*)file->buf, MAX_FILE_LENGTH,
                                     &real_length, (unsigned char*)buf, length)) {
                                       printf("%s %d\n",__func__,__LINE__);
                                       regina_free(file);
                                       file = NULL;
                                     }
      else
      {
        file->length = (uint16_t)real_length;
      }
    }
  }
  else if (regina_strcmp("w", mode) == 0) {
    file->mode |= FILE_MODE_WRITE;
  }
  else { 
    regina_free(file);
    return NULL;
  }
  
  if(file && file->length ==0)
  {
    if(regina_strcmp("r", mode) == 0)
    {
      regina_free(file);
      return NULL;
    }
  }
  return file;
}

int regina_file_close(regina_file_t *file)
{      
  regina_free(file);
  return 0;
}

int regina_file_rename(const char *oldpath, const char *newpath)
{    
  return -1;
}

int regina_file_remove(const char *path)
{  
  uint16_t id;
  id = std_wrapper_get_file_id(path);
  if (INVALID_FILE_ID == id)
    return 0;
  NvitemDelete(id);
  return 0;
}

size_t regina_file_write(const void *ptr, size_t size, size_t nmemb, regina_file_t *stream)
{
  uint16_t length;
  int err;
  char buf[MAX_FILE_LENGTH];
  size_t bytes_encoded;
  
  file_wrapper_t *file = (file_wrapper_t*)stream;
  size_t total = size * nmemb;
  if (!(file->mode & FILE_MODE_WRITE) ||
      (total + file->cursor > MAX_FILE_LENGTH))
    return 0;
  
  regina_memcpy(file->buf + file->cursor, ptr, total);
  length = (uint16_t)total + file->cursor;
  if (length < file->length)
    length = file->length;
  
  memset(buf, 0, sizeof(buf));
  err =  mbedtls_base64_encode((unsigned char*)buf, MAX_FILE_LENGTH,
                               &bytes_encoded, (unsigned char*)file->buf, length);
  if ((0 != err) || (bytes_encoded == MAX_FILE_LENGTH)) {
    return 0;
  }
  buf[bytes_encoded] = '\0';
  
  NvitemDelete(file->id);
  
  if (0 == NvitemWrite(file->id, MAX_FILE_LENGTH, buf))
  {
    file->length = length;
    file->cursor += total;
    return nmemb;
  }
  
  return 0;
}

size_t regina_file_read(void *ptr, size_t size, size_t nmemb, regina_file_t *stream)
{
  file_wrapper_t *file = (file_wrapper_t*)stream;
  size_t total = size * nmemb;
  
  if (!(FILE_MODE_READ & file->mode) ||
      (total > (file->length - file->cursor)))
    return 0;
  
  regina_memcpy(ptr, file->buf + file->cursor, total);
  file->cursor += total;
  return nmemb;
}

int regina_file_seek(regina_file_t *stream, long offset, int whence)
{
  file_wrapper_t *file = (file_wrapper_t*)stream;
  
  if ((offset > (long)file->length) || (offset < 0))
    return -1;
  
  switch(whence) {
  case REGINA_FILE_SEEK_SET:
    file->cursor = offset;
    break;
    
  case REGINA_FILE_SEEK_END:
    file->cursor = file->length - offset;
    break;
    
  case REGINA_FILE_SEEK_CUR:  /* do not support for no use. */
  default:
    return -1;
  }
  
  return 0;
}

long regina_file_tell(regina_file_t *stream)
{
  file_wrapper_t *file = (file_wrapper_t*)stream;
  return (long)(file->cursor);
}

regina_time_t regina_time(regina_time_t *t)
{
  uint32_t cur_time = onenet_mico_platform_get_time();
  return (regina_time_t)(cur_time/1000);
}

int64_t regina_get_localtime()
{
  return 0;
}

int NVITEM_GetLength(int id)
{
  return MAX_FILE_LENGTH;
}
