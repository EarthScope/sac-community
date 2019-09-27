
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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


#define VASPRINTF_BUFSIZE 2

int
fern_vasprintf(char **strp, const char *fmt, va_list args) {
    va_list args_copy;
    int needed;
    int status;
    char *c;

    /* Work around for buggy Solaris vsnprintf(), 
       it requires a char *, a NULL and a length of 0 does not work */
    c = malloc(sizeof(char) * VASPRINTF_BUFSIZE);
    memset(c, 0, VASPRINTF_BUFSIZE);

    needed = 2;
    va_copy(args_copy, args);
    needed = vsnprintf(c, VASPRINTF_BUFSIZE, fmt, args_copy);
    va_end(args_copy);
    free(c);

    if (needed < 0) {
        *strp = NULL;
        return needed;
    }
    *strp = malloc(needed + 1);
    if (*strp == NULL) {
        return -1;
    }
    status = vsnprintf(*strp, needed + 1, fmt, args);
    if (status >= 0) {
        return status;
    }
    free(*strp);
    *strp = NULL;
    return status;
}

int
fern_asprintf(char **strp, const char *fmt, ...) {
    va_list args;
    int status;

    va_start(args, fmt);
    status = fern_vasprintf(strp, fmt, args);
    va_end(args);
    return status;
}

/* From openssh/openbsd-compat/strcasestr.c 
     License: BSD 3-Clause
*/
char *
fern_strcasestr(const char *s, const char *find) {
	char c, sc;
	size_t len;

	if ((c = *find++) != 0) {
		c = (char)tolower((unsigned char)c);
		len = strlen(find);
		do {
			do {
          if ((sc = *s++) == 0) {
              return (NULL);
          }
			} while ((char)tolower((unsigned char)sc) != c);
		} while (strncasecmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}
