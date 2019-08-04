
#ifndef _STRIP_H_
#define _STRIP_H_


char * fern_rstrip(char *s);
char * fern_lstrip(char *s);

size_t fern_strlcpy(char *dst, const char *src, size_t size);
size_t fern_strlcat(char *dst, const char *src, size_t size);

#endif /* _STRIP_H_ */
