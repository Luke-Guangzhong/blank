#include "log.h"
#include "log4c.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MAX_CATEGORIES 64

typedef enum {
    LOG_STATE_UNINITIALIZED,
    LOG_STATE_LOG4C,
    LOG_STATE_FALLBACK
} log_state_t;

static log_state_t g_log_state = LOG_STATE_UNINITIALIZED;

typedef struct {
    char  name[64];
    void* ctx;
} category_entry_t;

static category_entry_t g_categories[MAX_CATEGORIES];
static int              g_category_count = 0;

static void* get_category_ctx(const char* category_name) {
    if (category_name == NULL || category_name[0] == '\0') {
        return NULL;
    }

    for (int i = 0; i < g_category_count; i++) {
        if (strcmp(g_categories[i].name, category_name) == 0) {
            return g_categories[i].ctx;
        }
    }

    if (g_category_count >= MAX_CATEGORIES) {
        return NULL;
    }

    void* ctx = NULL;
    if (g_log_state == LOG_STATE_LOG4C) {
        ctx = log4c_category_get(category_name);
    }

    strncpy(g_categories[g_category_count].name, category_name, sizeof(g_categories[g_category_count].name) - 1);
    g_categories[g_category_count].name[sizeof(g_categories[g_category_count].name) - 1] = '\0';
    g_categories[g_category_count].ctx                                                   = ctx;
    g_category_count++;

    return ctx;
}

static void fallback_log(log_level_t level, const char* category, const char* format, va_list args) {
    FILE* out = (level == LOG_LEVEL_ERROR || level == LOG_LEVEL_FATAL) ? stderr : stdout;

    if (category) {
        fprintf(out, "[%s] ", category);
    }
    vfprintf(out, format, args);
    fprintf(out, "\n");

    fflush(out);
}

int log_init(void) {
    if (g_log_state != LOG_STATE_UNINITIALIZED) {
        return 0;
    }

    if (log4c_init() == 0) {
        g_log_state = LOG_STATE_LOG4C;
        return 0;
    }

    fprintf(stderr, "[WARN] log4c initialization failed, using fallback logging\n");
    g_log_state = LOG_STATE_FALLBACK;
    return 0;
}

void log_shutdown(void) {
    if (g_log_state == LOG_STATE_LOG4C) {
        log4c_fini();
    }
    g_log_state      = LOG_STATE_UNINITIALIZED;
    g_category_count = 0;
}

void* log_get_category(const char* category_name) { return get_category_ctx(category_name); }

void log_log(log_level_t level, const char* category, const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (g_log_state == LOG_STATE_LOG4C) {
        void* ctx = get_category_ctx(category);
        if (ctx) {
            char buf[1024];
            vsnprintf(buf, sizeof(buf), format, args);

            switch (level) {
            case LOG_LEVEL_DEBUG:
                log4c_category_debug((log4c_category_t*)ctx, "%s", buf);
                break;
            case LOG_LEVEL_INFO:
                log4c_category_info((log4c_category_t*)ctx, "%s", buf);
                break;
            case LOG_LEVEL_WARN:
                log4c_category_warn((log4c_category_t*)ctx, "%s", buf);
                break;
            case LOG_LEVEL_ERROR:
                log4c_category_error((log4c_category_t*)ctx, "%s", buf);
                break;
            case LOG_LEVEL_FATAL:
                log4c_category_fatal((log4c_category_t*)ctx, "%s", buf);
                break;
            }
        }
    } else {
        fallback_log(level, category, format, args);
    }

    va_end(args);
}
