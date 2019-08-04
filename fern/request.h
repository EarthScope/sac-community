
#ifndef _REQUEST_H_
#define _REQUEST_H_

/**
 * Result from Request types
 */
typedef struct result result;
/**
 * Request type for making requests
 */
typedef struct request request;
/**
 * Arguments for the Request type
 */
typedef struct Arg Arg;

#include <sacio/timespec.h>

result * request_get(request *r);
result * request_post(request *r, char *post_data);

void     request_free(request *r);
char *   request_to_url(request *r);
request *request_new();
void     request_set_arg(request *r, char *key, Arg *a);
Arg     *request_get_arg(request *r, char *key);
int      request_del_arg(request *r, char *key);
void     request_set_url(request *r, char *url);
void     request_set_verbose(request *r, int verbose);
void     request_set_progress(request *r, int progress);
char    *request_get_url(request *r);

result *result_new();
void    result_free();
char *  result_free_move_data(result *r);
void    result_init(result *r);
char   *result_error_msg(result *r);
int     result_code(result *r);
int     result_http_code(result *r);
char   *result_data(result *r);
size_t  result_len(result *r);
int     result_is_ok(result *r);
result *result_error(int code, char *msg);
char   *result_filename(result *r);
char   *result_write_to_file(result *r, char *filename);
int     result_is_empty(result *r);
void    result_write_to_file_show(result *r, char *file);

Arg  * arg_string_new(char *s);
Arg  * arg_double_new(double v);
Arg  * arg_int_new(int i);
Arg  * arg_data_new(void *data,
                    char * (*data_fmt)(void *p, char *dst, size_t n),
                    void (*data_free)(void *p));
Arg *  arg_time_new(timespec64 t);
void   arg_free(void *v);
char * arg_to_string(Arg *p, char *tmp, size_t n);
int    arg_get_data(Arg *a, void **data);
int    arg_get_time(Arg *a, timespec64 *t);

char * data_size(int64_t bytes, char *out, size_t n);
void clear_line();

char * str_grow(char *s, size_t *nalloc, size_t n, size_t nadd);

/**
 * Free a result 
 * @memberof result
 * @param x result to free
 */
#define RESULT_FREE(x) do { \
        result_free(x); \
        x = NULL;        \
    } while(0);
/**
 * Free a request
 * @memberof request
 * @param x request to free
 */
#define REQUEST_FREE(x) do { \
        request_free(x); \
        x = NULL;        \
    } while(0);

#endif /* _REQUEST_H_ */
