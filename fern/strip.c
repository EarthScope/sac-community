
#include <string.h>
#include <ctype.h>

#include "strip.h"


char *
fern_rstrip(char *s) {
    char *back;
    if (*s == 0) {
        return s;
    }
    back = s + strlen(s) - 1;
    while (back >= s && isspace(*back)) {
        --back;
    }
    *(back + 1) = 0;
    return s;
}

char *
fern_lstrip(char *s) {
    if (*s == 0) {
        return s;
    }
    while (isspace(*s)) {
        s++;
    }
    return s;
}

char *
fern_strip(char *s) {
    char *new = NULL;
    char *p = NULL;
    if(!s) {
        return NULL;
    }
    new = strdup(s);
    p = fern_lstrip(fern_rstrip(new));
    if(p != new) {
        memmove(new, p, strlen(p)+1);
    }
    return new;
}

size_t
fern_strlcpy(char *dst, const char *src, size_t size) {
    size_t length, copy;

    length = strlen(src);
    if (size > 0) {
        copy = (length >= size) ? size - 1 : length;
        memcpy(dst, src, copy);
        dst[copy] = '\0';
    }
    return length;
}
size_t
fern_strlcat(char *dst, const char *src, size_t size) {
  size_t    srclen;         /* Length of source string */
  size_t    dstlen;         /* Length of destination string */

 /* Figure out how much room is left...  */
  dstlen = strlen(dst);
  size   -= dstlen + 1;

  if (!size) {
    return (dstlen);        /* No room, return immediately... */
  }

 /* Figure out how much room is needed...  */
  srclen = strlen(src);

 /* Copy the appropriate amount...  */
  if (srclen > size) {
    srclen = size;
  }
  memcpy(dst + dstlen, src, srclen);
  dst[dstlen + srclen] = '\0';

  return (dstlen + srclen);
}


