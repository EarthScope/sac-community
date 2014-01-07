
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "config.h"
#include "string_utils.h"

#if ( defined __DEBUG__ || __SAC_DEVELOPER__ )
#define DEBUG(fmt, args...) debug("%s:%d "fmt, __FUNCTION__, __LINE__, ##args)
#else 
#define DEBUG(fmt, args...)
#endif

#define UNUSED(x) (void) x

#define FREE(x) do { \
    if(x) {          \
      free(x);       \
      x = NULL;      \
    }                \
  } while(0);


#endif /* __DEBUG_H__ */

