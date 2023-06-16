#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

#define FE_LOG_USE_COLOR 1

typedef void 
(*FE_LOG_LOCK_FUNC)(void *udata, bool lock);

enum 
{ 
	FE_LOG_TRACE, 
	FE_LOG_DEBUG, 
	FE_LOG_INFO, 
	FE_LOG_WARN, 
	FE_LOG_ERROR, 
	FE_LOG_FATAL 
};


#ifdef FE_LOG_TODO
#define FE_TODO(msg) fe_log_warn(msg)
#else
#define FE_TODO(msg)
#endif

#define fe_log(...) fe_log_write(FE_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_return_code(code, ...) fe_log_write(FE_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_trace(...) fe_log_write(FE_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_debug(...) fe_log_write(FE_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_info(...)  fe_log_write(FE_LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_warn(...)  fe_log_write(FE_LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_error(...) fe_log_write(FE_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define fe_log_fatal(...) fe_log_write(FE_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void fe_log_set_udata(void *udata);
void fe_log_set_lock(FE_LOG_LOCK_FUNC fn);
void fe_log_set_fp(FILE *fp);
void fe_log_set_level(u32 level);
void fe_log_set_quiet(bool enable);
void fe_log_write(u32 level, const char *file, int line, const char *fmt, ...);


