/**
 * @file common.h
 * @author luke_guangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include "log.h"
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t byte;

typedef struct binary_data b_data_t;

/******************************************************************************/
/*                                Custom Types                                */
/******************************************************************************/

struct binary_data {
    byte*  data;
    size_t size;
};

/******************************************************************************/
/*                            RESOURCE FREE MACRO                             */
/******************************************************************************/

#ifndef FREE
#define FREE(pointer)        \
    do {                     \
        if (pointer != NULL) \
            free(pointer);   \
        pointer = NULL;      \
    } while (0)
#endif

#ifndef FCLOSE
#define FCLOSE(fp)      \
    do {                \
        if (fp != NULL) \
            fclose(fp); \
        fp = NULL;      \
    } while (0)
#endif

#ifndef CLOSE
#define CLOSE(fd)      \
    do {               \
        if (fd != 0)   \
            close(fd); \
        fd = 0;        \
    } while (0)
#endif

#ifndef PCLOSE
#define PCLOSE(fp)      \
    do {                \
        if (fp != NULL) \
            pclose(fp); \
        fp = NULL;      \
    } while (0)
#endif

/******************************************************************************/
/*                                JUDGE MACRO                                 */
/******************************************************************************/

#ifndef IS_NULL
#define IS_NULL(ptr) (NULL == (ptr))
#endif /* IS_NULL */

#ifndef IS_NOT_NULL
#define IS_NOT_NULL(ptr) (NULL != (ptr))
#endif /* IS_NOT_NULL */