#ifndef __REGINA_PLATFROM_THREAD_H__
#define __REGINA_PLATFROM_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Suspends the execution of the current thread until the time-out
 * interval elapses.
 *
 * @return REGINA_ERR_NONE if succeeds.
 */
int regina_sleep(int milliseconds);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_PLATFROM_THREAD_H__ */
