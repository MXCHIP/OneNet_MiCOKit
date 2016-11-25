#include <stdlib.h>
#include <string.h>
#include "regina/platform.h"

REGINA_API void *regina_malloc(size_t size)
{
  void * addr=NULL;
  addr=(void *)malloc(size);
  return addr;
}

REGINA_API void regina_free(void *ptr)
{
    if( ptr ) free(ptr);
}

REGINA_API void *regina_calloc(size_t nmemb, size_t size)
{
  void * addr=NULL;
  addr=(void *)malloc(size*nmemb); 
  if(addr)
  {
    memset((char *)addr,0,size*nmemb);
  }
  return addr;
}

REGINA_API void *regina_realloc(void *ptr, size_t size)
{
  void * addr=NULL;
  addr= (void*)realloc(ptr,size);
  return addr;
}
