#include "common.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef LOG4C_RCPATH
#error "Need MACRO LOG4C_RCPATH"
#endif

int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    // Check and set LOG4C_RCPATH environment variable if not exists
    if (getenv("LOG4C_RCPATH") == NULL) {
        setenv("LOG4C_RCPATH", LOG4C_RCPATH, 1); // Set default config path
    }

    log_init();

    // 使用 category name 直接记录日志
    log_debug("main", "hello world!");
    log_info("network", "connecting to server");
    log_warn("database", "connection slow");
    log_error("ui", "failed to load resource");

    log_shutdown();

    return 0;
}
