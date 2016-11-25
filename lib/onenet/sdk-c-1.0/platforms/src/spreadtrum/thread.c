#include "regina/error.h"
#include "regina/allocator.h"
#include "std_wrapper.h"
#include "thread.h"

typedef struct {
    regina_thread_fn fun;
    void *context;
} thread_param_t;

struct regina_thread_t {
    BLOCK_ID id;
    char *name;
};

struct regina_mutex_t {
   SCI_MUTEX_PTR mutex;
   char *name;
};

void thread_entry(uint32 argc,void *argv)
{
    thread_param_t ctx = {0};

    memcpy(&ctx,(thread_param_t*)argv,sizeof(thread_param_t));

    regina_free(argv);

    ctx.fun(ctx.context);
}

int regina_thread_create(regina_thread_t **thread, regina_thread_fn fun,
                         void *context, const char *name)
{
    #define ONENET_COR_THREAD_STACK_SIZE (1024*40)

    thread_param_t *arg;
    char queue_name[128] = {0};

    SCI_TRACE_LOW("regina: regina_thread_create");

    *thread = (regina_thread_t*)regina_malloc(sizeof(regina_thread_t));
    if (!*thread)
        return REGINA_ERR_NOMEM;

    (*thread)->name = regina_strdup(name);

    arg = (thread_param_t*)regina_malloc(sizeof(thread_param_t));
    if (!arg) {
        regina_free(*thread);
        *thread = NULL;
        return REGINA_ERR_NOMEM;
    }

    arg->fun = fun;
    arg->context = context;

    strcpy(queue_name,(*thread)->name);
    strcat(queue_name,"_queue");

	(*thread)->id = SCI_CreateThread((*thread)->name,
				(char*)queue_name,
				thread_entry,
				1,
				(void *)arg,
				ONENET_COR_THREAD_STACK_SIZE,//(0x0F << 10),//4096,
				4,
				TX_BASE_NORMALE + 5,//TX_BASE_LOW - 3,
				SCI_PREEMPT,
				SCI_AUTO_START//SCI_DONT_START
			   );

    return REGINA_ERR_NONE;
}

void regina_thread_destroy(regina_thread_t *thread)
{

    SCI_TRACE_LOW("regina: regina_thread_destroy");

    if(thread && thread->id && (SCI_SUCCESS == SCI_IsThreadExist(thread->id)))
    {
        SCI_TerminateThread(thread->id);
        SCI_DeleteThread(thread->id);   //销毁线程
        thread->id = 0;

        SCI_TRACE_LOW("regina: regina_thread_destroy ok");
   }

    regina_free(thread->name);
    thread->name = NULL;
    regina_free(thread);
    thread = NULL;
}

int regina_sleep(int milliseconds)
{
    SCI_Sleep(milliseconds);  //以毫米为单位
    return REGINA_ERR_NONE;
}


/*-------------------------- regina mutex -----------------------*/
int regina_mutex_create(regina_mutex_t **mutex, const char *name)
{

    *mutex = (regina_mutex_t*)regina_malloc(sizeof(regina_mutex_t));
    if (!*mutex)
        return REGINA_ERR_NOMEM;

    /* mutex->name could be null for no use.*/
      (*mutex)->name = regina_strdup(name);

    (*mutex)->mutex = SCI_CreateMutex((*mutex)->name, SCI_NO_INHERIT);

    if(SCI_NULL == (*mutex)->mutex)
    {
        SCI_TRACE_LOW("regina: regina_mutex_create %s.. fail",(*mutex)->name);
        regina_free((*mutex)->name);
        (*mutex)->name = NULL;
        regina_free(*mutex);
        *mutex = NULL;
        return REGINA_ERR_INTERNAL;
    }

    SCI_TRACE_LOW("regina: regina_mutex_create %s.. ok",(*mutex)->name);

    return REGINA_ERR_NONE;
}

int regina_mutex_lock(regina_mutex_t *mutex)
{
    if(mutex)
    {
       if(SCI_SUCCESS != SCI_GetMutex(mutex->mutex,SCI_WAIT_FOREVER))  //互斥锁，一直等待
       {
            SCI_TRACE_LOW("regina: regina_mutex_lock %s  fail",mutex->name);
            return REGINA_ERR_INTERNAL;
       }

       SCI_TRACE_LOW("regina: regina_mutex_lock %s ok",mutex->name);
    }
    return REGINA_ERR_NONE;
}

int regina_mutex_unlock(regina_mutex_t *mutex)
{
    if(mutex)
    {
       if (SCI_SUCCESS != SCI_PutMutex(mutex->mutex))
       {
            char *errStr = NULL;
            int errCode = SCI_GetLastError(&errStr);

            SCI_TRACE_LOW("regina: regina_mutex_unlock %s fail , %d, %s",mutex->name,errCode, errStr);
            //return REGINA_ERR_INTERNAL;
       }
       else
        SCI_TRACE_LOW("regina: regina_mutex_unlock %s ok",mutex->name);
    }

    return REGINA_ERR_NONE;
}

void regina_mutex_destroy(regina_mutex_t *mutex)
{
    if(mutex)
    {
        SCI_DeleteMutex(mutex->mutex); //删除锁
        regina_free(mutex->name);
        mutex->name = NULL;
        regina_free(mutex);
        mutex = NULL;
    }
}
