
#ifndef _JSON_H_
#define _JSON_H_

#include "cJSON.h"
const cJSON * cjson_path_internal(const cJSON *root, va_list ap);
const cJSON * cjson_path(const cJSON *root, ...);
int cjson_string(const cJSON *root, char *v, size_t n, ...);
int cjson_int(const cJSON *root, int *v, ...);
int cjson_double(const cJSON *root, double *v, ...);

#endif /* _JSON_H_ */
