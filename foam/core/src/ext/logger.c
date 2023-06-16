#include <foam/core/ext/logger.h>

static struct 
{
	void *udata;
	FE_LOG_LOCK_FUNC lock;
	FILE *fp;
	u32 level;
	bool quiet;
} L;

static const char 
*level_names[] = 
{
	"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef FE_LOG_USE_COLOR
static const char 
*level_colors[] = 
{
    "\x1b[36m", "\x1b[32m", "\x1b[94m", "\x1b[33m", "\x1b[31m", "\x1b[31m"
};
#endif


static void 
lock(void)   
{
	if (L.lock) L.lock(L.udata, true);
}

static void 
unlock(void) 
{
	if (L.lock) L.lock(L.udata, false);
}

void 
fe_log_set_udata(void *udata) 
{
	L.udata = udata;
}

void 
fe_log_set_lock(FE_LOG_LOCK_FUNC fn) 
{
	L.lock = fn;
}

void 
fe_log_set_fp(FILE *fp) 
{
	L.fp = fp;
}

void 
fe_log_set_level(u32 level) 
{
	L.level = level;
}

void 
fe_log_set_quiet(bool enable) 
{
	L.quiet = enable ? true : false;
}

void 
fe_log_write(u32 level, const char *file, int line, const char *fmt, ...) 
{

	if (level < L.level) return;

	lock();

	time_t t = time(NULL);
	struct tm *lt = localtime(&t);

	if (!L.quiet) 
	{
		va_list args;
		char buf[16];
		buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
#ifdef FE_LOG_USE_COLOR
		fprintf(
				stderr, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
				buf, level_colors[level], level_names[level], file, line);
#else
		fprintf(stderr, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
#endif
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
		fprintf(stderr, "\n");
	}

	if (L.fp) 
	{
		va_list args;
		char buf[32];
		buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
		fprintf(L.fp, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
		va_start(args, fmt);
		vfprintf(L.fp, fmt, args);
		va_end(args);
		fprintf(L.fp, "\n");
	}

	unlock();

}

