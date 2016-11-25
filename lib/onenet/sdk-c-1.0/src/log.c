#include "regina/log.h"
#include <stdio.h>

static void regina_log_default_log(void *ctx, regina_log_level_t ll, const char *format, ...)
{
    (void)ctx;
    (void)ll;
    (void)format;
}

REGINA_API regina_logger_t regina_logger = {
    REGINA_LL_DEBUG,
    NULL,
    regina_log_default_log
};

REGINA_API void regina_log_set_logger(void *context, regina_log_func log_func)
{
    if (NULL == log_func)
        log_func = regina_log_default_log;

    regina_logger.context = context;
    regina_logger.log = log_func;
}

