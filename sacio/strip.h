
#ifndef _STRIP_H_
#define _STRIP_H_


char * sacio_rstrip(char *s);
char * sacio_lstrip(char *s);

size_t sacio_strlcpy(char *dst, const char *src, size_t size);
size_t sacio_strlcat(char *dst, const char *src, size_t size);

#endif /* _STRIP_H_ */
