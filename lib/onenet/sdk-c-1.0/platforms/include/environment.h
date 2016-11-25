#ifndef __REGINA_PLATFORM_ENVIRONMENT_H__
#define __REGINA_PLATFORM_ENVIRONMENT_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Initialize some environment about current platform.
 */
int initialize_environment( void );

/**
 * Cleanup some environment abount current platform.
 */
void cleanup_environment( void );

#ifdef __cplusplus
} /* extern "C"{ */
#endif

#endif /* __REGINA_PLATFORM_ENVIRONMENT_H__ */
