#include "std_wrapper.h"
#include "regina/allocator.h"
#include <mbedtls/base64.h>

#define INVALID_FILE_ID  ((uint16)(~0U))

#define DEVICE_STATUS_FILE_ID        1006
#define DEVICE_STATUS_BAK_FILE_ID    DEVICE_STATUS_FILE_ID
#define DEVICE_CONFIG_FILE_ID        1007
#define DEVICE_CONFIG_BAK_FILE_ID    DEVICE_CONFIG_FILE_ID


#define MAX_FILE_LENGTH  512



typedef struct file_name_pair_t {
    const char *name;
    uint16 id;
} file_name_pair_t;

typedef enum {
    FILE_MODE_READ  = 0x01,
    FILE_MODE_WRITE = 0x02
} file_mode_t;

typedef struct file_wrapper_t {
    uint16 id;
    uint16 length; /* the length of the file block*/
    uint16 cursor;
    file_mode_t mode;
    char buf[MAX_FILE_LENGTH];
} file_wrapper_t;

static uint16 std_wrapper_get_file_id(const char *name)
{
    static const file_name_pair_t map[] = {
        {"regina_device",     DEVICE_STATUS_FILE_ID},
        {"regina_device.bak", DEVICE_STATUS_BAK_FILE_ID},
        {"regina_config",     DEVICE_CONFIG_FILE_ID},
        {"regina_config.bak", DEVICE_CONFIG_BAK_FILE_ID}
    };

    int i;

    for (i = 0; i < sizeof(map) / sizeof(map[0]); ++i) {
        if (regina_strcmp(name, map[i].name) == 0)
            return map[i].id;
    }

    return INVALID_FILE_ID;
}

void *regina_memset(void *s, int c, size_t n)
{
    if(s && (n > 0))
        return memset(s, c, n);
}

void *regina_memcpy(void *dest, const void *src, size_t n)
{
    if(dest && src && (n > 0))
        return memcpy(dest, src, n);
}

int regina_memcmp(const void *s1, const void *s2, size_t n)
{
    if(s1 && s2 && (n > 0))
        return memcmp(s1, s2, n);
    else
        return -1;
}

size_t regina_strlen(const char *s)
{
    if(s)
        return strlen(s);
    else
        return 0;
}

char *regina_strdup(const char *s)
{
    if(s)
    {
        int l = strlen(s) + 1;
        char *dst = malloc(l);
        if (dst)
            strcpy(dst, s);
        return dst;
    }
    else
        return NULL;
}

int regina_strcmp(const char *s1, const char *s2)
{
    if(s1 && s2)
        return strcmp(s1, s2);
    else
        return -1;
}

char *regina_strcpy(char *dest, const char *src)
{
    if(dest && src)
        return strcpy(dest, src);
    else
        return NULL;
}

// 使用 NVITEM_Read  NVITEM_Write 类似于FLASH 块操作，不能字节操作， 第二个参数必须是固定的值，以配置时的最大长度读或者写，

//比如参数最大长度512，那么只写10个字符，第二个参数也必须写512，以512字节写入，后面的为0，相当于FLASH 块操作。

//返回0，表示成功

regina_file_t *regina_file_open(const char *path, const char *mode)
{
    uint16 id;
    file_wrapper_t *file;

    id = std_wrapper_get_file_id(path);
    if (INVALID_FILE_ID == id)
        return NULL;

    //注意: NVITEM_GetLength 不表示具体长度，而是NV里这个参数所占的长度 --最大长度
    if (MAX_FILE_LENGTH != NVITEM_GetLength(id))
        return NULL;

    file = (file_wrapper_t*)regina_malloc(sizeof(file_wrapper_t));
    if (!file)
        return NULL;

    regina_memset(file, 0, sizeof(file_wrapper_t));
    file->id = id;

    if (regina_strcmp("r", mode) == 0) {
        char buf[MAX_FILE_LENGTH + 1] = {0};

        file->mode |= FILE_MODE_READ;

        if (0 != EFS_NvitemRead(file->id, MAX_FILE_LENGTH,buf)) {

            REGINA_LOG0(WARNING, "regina : EFS_NvitemRead fail.");
            regina_free(file);
            file = NULL;
        }
        else {
            size_t real_length;
            size_t length = strlen(buf);

            REGINA_LOG2(WARNING, "regina: EFS_NvitemRead size %d,%s.",length,buf);
            //buf[MAX_FILE_LENGTH - 1] = '\0';
            if (0 != mbedtls_base64_decode((unsigned char*)file->buf, MAX_FILE_LENGTH,
                                           &real_length, (unsigned char*)buf, length)) {
                regina_free(file);
                file = NULL;
            }
            else
            {
                file->length = (uint16)real_length;
            }
        }

        if(!file->length)
        {
            regina_free(file);
            file = NULL;
            return NULL;
        }

    }
    else if (regina_strcmp("w", mode) == 0) {
        file->mode |= FILE_MODE_WRITE;
    }
    else { /* not support for no use. */
        regina_free(file);
        file = NULL;
        return NULL;
    }

    return file;
}

int regina_file_close(regina_file_t *file)
{
    regina_free(file);
    file = NULL;
    return 0;
}

int regina_file_rename(const char *oldpath, const char *newpath)
{
    return -1;
}

int regina_file_remove(const char *path)  //不要随便用
{
    /* NV system can not remove the item. */
    uint16 id;

    id = std_wrapper_get_file_id(path);
    if (INVALID_FILE_ID == id)
        return 0;

    EFS_NvitemDelete(id, id);

    return 0;
}

size_t regina_file_write(const void *ptr, size_t size,
                         size_t nmemb, regina_file_t *stream)
{
    uint16 length;
    int err;
    char buf[MAX_FILE_LENGTH + 1];
    size_t bytes_encoded;

    file_wrapper_t *file = (file_wrapper_t*)stream;
    size_t total = size * nmemb;

    if(total == 0)
    {
        REGINA_LOG0(WARNING, "regina: regina_file_write size 0 is error");
        return 0;
    }

    if (!(file->mode & FILE_MODE_WRITE) ||
        (total + file->cursor > MAX_FILE_LENGTH))
        return 0;

    regina_memcpy(file->buf + file->cursor, ptr, total);
    length = (uint16)total + file->cursor;
    if (length < file->length)
        length = file->length;

    REGINA_LOG3(WARNING, "regina: regina_file_write file %d,%d,%s",total,length,file->buf);

    memset(buf, 0, sizeof(buf));
    err =  mbedtls_base64_encode((unsigned char*)buf, MAX_FILE_LENGTH,
                                 &bytes_encoded, (unsigned char*)file->buf, length);
    if ((0 != err) || (bytes_encoded == MAX_FILE_LENGTH)) {
        return 0;
    }
    buf[bytes_encoded] = '\0';

	EFS_NvitemDelete(file->id, file->id);

	if (0 == EFS_NvitemWrite(file->id, MAX_FILE_LENGTH, buf, FALSE))
	{
        file->length = length;
        file->cursor += total;
        return nmemb;
	}

    return 0;
}

size_t regina_file_read(void *ptr, size_t size,
                        size_t nmemb, regina_file_t *stream)
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
	uint32 time = 0;

    time = TM_GetTotalSeconds();

    if (t)
        *t = (regina_time_t)time;

    return (regina_time_t)time;
}

int64_t regina_get_localtime()
{
    return (regina_time_t)TM_GetTotalSeconds();
}

