#ifndef __REGINA_SAFE_LS_FILE_H__
#define __REGINA_SAFE_LS_FILE_H__

#include "std_wrapper.h"


int safe_save_file(const char *path,
                   int (*write)(void *context, regina_file_t *file),
                   void *context);

/**
 * @param read load the whole data from file, return positive number
 * if success, 0 if the file is damaged, negative number if errors in
 * read function.
 * @return same as read function
 */
int safe_load_file(const char *path,
                   int (*read)(void *context, regina_file_t *file),
                   void *context);

#endif /* __REGINA_SAFE_RW_FILE_H__ */
