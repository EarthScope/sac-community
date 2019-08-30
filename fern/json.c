/**
 * @file
 * @brief json parsing
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// https://github.com/DaveGamble/cJSON 3c89356
#include "cJSON.h"
#include "strip.h"

/**
 * @brief json structure empty
 * @private
 */
struct json {};

/**
 * @defgroup json json
 * @brief json data parsing
 *
 * This uses the cJSON library found at https://github.com/DaveGamble/cJSON
 *
 * To use the function above first parse the json data into a cJSON structure
 *
 * @code{.c}
 *   char *data = read_in_json_data_magically();
 *   // Parse json data contained in data
 *   cJSON *json = cJSON_Parse( data );
 *
 * @endcode
 *
 * then you can find items in the json structure
 *
 * @code{.c}
 *
 *   // Find nodes at properties/products/origin
 *   const cJSON *org = cjson_path(json, "properties", "products", "origin", NULL);
 *
 *   // Find the latitude at properties/latitude
 *   double evla = 0.0;
 *   cjson_double(org, &evla, "properties", "latitude", NULL);
 *
 * @endcode
 */

/**
 * @brief      find nodes by path in a json structure
 *
 * @details    find nodes by path in a json structure
 * @private
 * @memberof json
 * @ingroup json
 *
 * @param      root   json root node
 * @param      ap     argument list of path, NULL terminated
 *
 * @return     json node or NULL on error
 */
const cJSON *
cjson_path_internal(const cJSON *root, va_list ap) {
    char *s = NULL;
    const cJSON *cur = NULL;
    cur = root;
    while(cur && (s = va_arg(ap, char *))) {
        if(!(cur = cJSON_GetObjectItemCaseSensitive(cur, s))) {
            break;
        }
    }
    return cur;
}
/**
 * @brief      find nodes by path in a json structure
 *
 * @details    find nodes by path in a json structure
 *
 * @memberof json
 * @ingroup json
 *
 * @param      root   json root node
 * @param      ...    argument names of path, NULL terminated
 *
 * @return     json node or NULL on error
 */
const cJSON *
cjson_path(const cJSON *root, ...) {
    const cJSON *cur = NULL;
    va_list ap;
    va_start(ap, root);
    cur = cjson_path_internal(root, ap);
    va_end(ap);
    return cur;
}

/**
 * @brief      find a character string in a json document 
 *
 * @details    find a character string in a json document and copy the out to v
 *
 * @memberof json
 * @ingroup json
 *
 * @param      root   json root node
 * @param      v      output character string
 * @param      n      length of v
 * @param      ...    argument names of path, NULL terminated
 *
 * @return     1 on success, 0 on failure
 */
int
cjson_string(const cJSON *root, char *v, size_t n, ...) {
    const cJSON *cur = NULL;
    va_list ap;
    va_start(ap, n);
    cur = cjson_path_internal(root, ap);
    va_end(ap);
    if(!cur) {
        return 0;
    }
    if(cJSON_IsString(cur)) {
        fern_strlcpy(v, cur->valuestring, n);
    } else {
        return 0;
    }
    return 1;
}

/**
 * @brief      find an integer in a json document 
 *
 * @details    find an integer in a json document and copy the out to v
 *
 * @memberof json
 * @ingroup json
 *
 * @param      root   json root node
 * @param      v      output integer
 * @param      ...    argument names of path, NULL terminated
 *
 * @return     1 on success, 0 on failure
 */
int
cjson_int(const cJSON *root, int *v, ...) {
    const cJSON *cur = NULL;
    va_list ap;
    va_start(ap, v);
    cur = cjson_path_internal(root, ap);
    va_end(ap);
    if(!cur) {
        return 0;
    }
    if(cJSON_IsNumber(cur)) {
        *v = cur->valueint;
    } else if(cJSON_IsString(cur)) {
        char *endptr = NULL;
        *v = (int) strtol(cur->valuestring, &endptr, 10);
        return (endptr == NULL || strlen(endptr) == 0);
    } else {

    }
    return 1;
}
/**
 * @brief      find an floating point number in a json document 
 *
 * @details    find an floating point number in a json document and copy the out to v
 *
 * @memberof json
 * @ingroup json
 *
 * @param      root   json root node
 * @param      v      output floating point number
 * @param      ...    argument names of path, NULL terminated
 *
 * @return     1 on success, 0 on failure
 */
int
cjson_double(const cJSON *root, double *v, ...) {
    const cJSON *cur = NULL;
    va_list ap;
    va_start(ap, v);
    cur = cjson_path_internal(root, ap);
    va_end(ap);
    if(!cur) {
        return 0;
    }
    if(cJSON_IsNumber(cur)) {
        *v = cur->valuedouble;
    } else if(cJSON_IsString(cur)) {
        char *endptr = NULL;
        *v = strtod(cur->valuestring, &endptr);
        return (endptr == NULL || strlen(endptr) == 0);
    } else {
        return 0;
    }
    return 1;
}


