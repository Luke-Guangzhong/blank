#pragma once

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} log_level_t;

int  log_init(void);
void log_shutdown(void);

void log_log(log_level_t level, const char* category, const char* format, ...);

#define log_debug(category, ...) log_log(LOG_LEVEL_DEBUG, category, __VA_ARGS__)
#define log_info(category, ...)  log_log(LOG_LEVEL_INFO, category, __VA_ARGS__)
#define log_warn(category, ...)  log_log(LOG_LEVEL_WARN, category, __VA_ARGS__)
#define log_error(category, ...) log_log(LOG_LEVEL_ERROR, category, __VA_ARGS__)
#define log_fatal(category, ...) log_log(LOG_LEVEL_FATAL, category, __VA_ARGS__)

void* log_get_category(const char* category_name);
