
#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>

#define UNUSED(x) (void) x

#define FREE(x) do { \
    if(x) {          \
      free(x);       \
      x = NULL;      \
    }                \
  } while(0);

#define FALSE 0
#define TRUE  1

#endif /* _DEFS_H_ */
