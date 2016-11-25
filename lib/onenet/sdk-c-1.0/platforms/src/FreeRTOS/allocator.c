#include <stdlib.h>
#include "std_wrapper.h"
#include "FreeRTOS.h"
REGINA_API void *regina_malloc(size_t size)
{
	void * addr=NULL;
	portENTER_CRITICAL();
	addr=(void *)malloc(size);
	vPortExitCritical();
	return addr;
}

REGINA_API void regina_free(void *ptr)
{
	portENTER_CRITICAL();
	free(ptr);
	vPortExitCritical(); 
}

REGINA_API void *regina_calloc(size_t nmemb, size_t size)
{
	void * addr=NULL;
	portENTER_CRITICAL();
	addr=(void *)malloc(size*nmemb); 
	memset(addr,0,size*nmemb);
	vPortExitCritical(); 
	return addr;
}

REGINA_API void *regina_realloc(void *ptr, size_t size)
{
	void * addr=NULL;
	portENTER_CRITICAL();
        
	addr= (void*)malloc(size);
        if(addr)
        {
          memset(addr,0,size);
          memcpy(addr,ptr,size);
          free(ptr);
        }
	vPortExitCritical(); 
	return addr;
}
