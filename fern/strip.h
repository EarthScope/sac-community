
#ifndef _STRIP_H_
#define _STRIP_H_

#include <stdarg.h>

char * fern_rstrip(char *s);
char * fern_lstrip(char *s);
char * fern_strip(char *s);

size_t fern_strlcpy(char *dst, const char *src, size_t size);
size_t fern_strlcat(char *dst, const char *src, size_t size);

int fern_asprintf(char **ret, const char *format, ...);
char * fern_strcasestr(const char *s, const char *find);

#endif /* _STRIP_H_ */
