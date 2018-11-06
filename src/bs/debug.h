// LICENSE: MIT


#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "config.h"
#include "string_utils.h"

#if ( defined __DEBUG__ || __SAC_DEVELOPER__ )
#define DEBUG(fmt, ...) debug("%s:%d "fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__ )
#else
#define DEBUG(fmt, ...)
#endif

#define UNUSED(x) (void) x

#define FREE(x) do { \
    if(x) {          \
      free(x);       \
      x = NULL;      \
    }                \
  } while(0);

/* #define HAVE_PRINT_TRACE */
#ifdef HAVE_PRINT_TRACE
#include <execinfo.h>

#define BACKTRACE                               \
    do {                                        \
        void *array[10];                        \
        size_t size;                            \
        char **strings;                         \
        size_t i;                               \
                                                \
        size = backtrace(array, 10);                  \
        strings = backtrace_symbols(array, size);     \
                                                      \
        printf("Obtained %zd stack frames.\n", size); \
                                                      \
        for (i = 1; i < 6; i++) {                     \
            printf("%s\n", strings[i]);               \
        }                                             \
        free(strings);                                \
    } while(0);
#endif /* HAVE_PRINT_TRACE */


#endif /* __DEBUG_H__ */
