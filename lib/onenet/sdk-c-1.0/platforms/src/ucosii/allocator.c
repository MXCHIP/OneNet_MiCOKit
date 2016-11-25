#include <stdlib.h>
#include "std_wrapper.h"
#include "malloc.h"
#include "os_cpu.h"

#ifndef  OS_MASTER_FILE
#include <ucos_ii.h>
#endif

REGINA_API void *regina_malloc(size_t size)
{
	void * addr=NULL;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	addr=(void *)myMalloc(size);
	OS_EXIT_CRITICAL();
	return addr;
}

REGINA_API void regina_free(void *ptr)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	myFree(ptr);
	OS_EXIT_CRITICAL(); 
}

REGINA_API void *regina_calloc(size_t nmemb, size_t size)
{
	void * addr=NULL;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	addr=(void *)myMalloc(size*nmemb); 

	if(addr)
	{
		memset((char *)addr,0,size*nmemb);
	}
	OS_EXIT_CRITICAL(); 
	return addr;
}

REGINA_API void *regina_realloc(void *ptr, size_t size)
{
	OS_CPU_SR cpu_sr;
	void * addr=NULL;
	OS_ENTER_CRITICAL();
	addr= (void*)myRealloc(ptr,size);
	OS_EXIT_CRITICAL(); 
	return addr;
}
