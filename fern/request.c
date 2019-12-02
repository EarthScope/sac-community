/**
 * @file
 * @brief HTTP \ref request and \ref result
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <inttypes.h>

#include <curl/curl.h>

#include <sacio/timespec.h>

#include "request.h"
#include "cprint.h"

#include "chash.h"
#include "defs.h"
#include "strip.h"

/**
 * Structure to hold the remote transfer filename
 * @private
 * @ingroup request
 */
typedef struct {
    char        remote_fname[4096];
} dnld_params_t;

#ifndef CURL_VERSION_BITS
#define CURL_VERSION_BITS(x,y,z) ((x)<<16|(y)<<8|(z))
#endif

#ifndef CURL_AT_LEAST_VERSION
#define CURL_AT_LEAST_VERSION(x,y,z) \
  (LIBCURL_VERSION_NUM >= CURL_VERSION_BITS(x, y, z))
#endif

#if CURL_AT_LEAST_VERSION(7,61,0)

#define TIME_IN_US 1
#define TIMETYPE curl_off_t
#define TIMEOPT CURLINFO_TOTAL_TIME_T
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     1000000

#elif CURL_AT_LEAST_VERSION(7,4,1)

#define TIME_IN_US 0
#define TIMETYPE double
#define TIMEOPT CURLINFO_TOTAL_TIME
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     1e-6

#else

#error "libcurl is less than 7.4.1"

#endif


/**
 * Progress struct for data download
 * @private
 * @ingroup request
 */
struct myprogress {
    TIMETYPE lastruntime[3]; /* type depends on version, see above */ 
    CURL *curl;
    curl_off_t last_dlnow;
    curl_off_t prog[3];
};


void result_from_curl(result *r, int code, char *data, size_t n);
size_t dnld_header_parse(void *hdr, size_t size, size_t nmemb, void *userdata);
static size_t memory_callback(void *contents, size_t size, size_t nmemb, void *userp);
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
#if LIBCURL_VERSION_NUM < 0x072000
static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow);
#endif

typedef struct zarray zarray; /**< \private */
/**
 * \private 
 */
struct zarray {
    size_t alloc;
    size_t n;
    char *data;
};

void
zarray_init(zarray *a) {
    a->alloc = 16;
    a->n = 0;
    a->data = calloc(a->alloc, sizeof(char));
}

void
zarray_grow(zarray *a, size_t n) {
    char *tmp;
    if(a->alloc == 0) {
        a->alloc = 2;
    }
    if(n < a->alloc) {
        return;
    }
    while(a->alloc <= n) {
        a->alloc *= 2;
    }
    tmp = realloc(a->data, a->alloc);
    if(tmp) {
        a->data = tmp;
        memset(a->data + a->n, 0, a->alloc - a->n);
    } else {
        fprintf(stderr, "array: error while expanding\n");
    }
}


void
zarray_append(zarray *a, char *data, size_t n) {
    zarray_grow(a, a->n + n);
    memcpy(a->data + a->n, data, n);
    a->n = a->n + n;
}

/**
 * @defgroup request request
 * @brief HTTP request and HTTP result
 *
 */

/**
 *  @brief HTTP request
 *
 * @memberof request
 * @ingroup request
 *
 * @code
 *   result *res = NULL;
 *   request *r = request_new();
 *
 *   request_set_url(r, "https://www.example.com/query?");
 *   request_set_arg(r "key",    arg_double_new(3.13));
 *   request_set_arg(r "string", arg_string_new("hello"));
 *   request_set_arg(r, "thing"  arg_int_new(4));
 *
 *   res = request_get(r);
 *   if(result_is_ok(res)) {
 *       printf("%s\n", result_data(res));
 *   } else {
 *       printf("Error: %s\n", error_msg_str(res));
 *   }
 *
 * @endcode
 *
 */
struct request {
    char *url;   /**< \private  URL to send request to */
    dict *args;  /**< \private  Key-Values pairs */
    int verbose; /**< \private  Display details about the request */
    int progress; /**< \private Show progress bar during download */
};

/**
 * @brief Result from a \ref request
 * @ingroup request
 *
 */
struct result {
    CURLcode code;     /**< \private Curl result code */
    int http_code;     /**< \private HTTP Response code */
    const char *error; /**< \private Error string */
    char *data;        /**< \private Returned data */
    size_t n;          /**< \private Length of data */
    char *filename;    /**< \private Possible filename returned from request */
};

/**
 * @brief Argument for a \ref request
 * @ingroup request
 */
struct Arg {
    int type;     /**< \private  Arg data type */
    int i;        /**< \private  Integer value */
    double fp;    /**< \private  Double floating point value */
    char *str;    /**< \private  String value */
    timespec64 t; /**< \private  Time value with nanoseconds precision */
    void *data;   /**< \private  Raw Data - REMOVE */
    char* (*data_fmt)(void *p, char *dst, size_t n);
    /**< \private  Raw Data formatting function */
    void  (*data_free)(void *p);
    /**< \private  Raw Data free function */

};




/**
 * Create a new Request 
 *
 * @memberof request
 * @ingroup request
 *
 * Memory of this request must be freed with @ref request_free
 *
 */
request *
request_new() {
    request *r = calloc(1, sizeof(request));
    r->args = dict_new();
    r->verbose = 0;
    r->progress = 1;
    return r;
}

/**
 * Free memory associated with a request
 *
 * @memberof request
 * @ingroup request
 *
 * @param[in]  r   Request to free
 *
 */
void
request_free(request *r) {
    if(r) {
        FREE(r->url);
        dict_free(r->args, arg_free);
        FREE(r);
    }
}


/**
 * Set if request will be have verbose reporting
 *
 * @memberof request
 * @ingroup request
 *
 * @param r        Request to change verbosity
 * @param verbose  1 for verbose, 0 for quiet
 */
void
request_set_verbose(request *r, int verbose) {
    r->verbose = verbose;
}

/**
 * Set if request will be have a progress bar during download
 *
 * @memberof request
 * @ingroup request
 *
 * @param r        Request to change progress during download
 * @param verbose  1 for show progress bar, 0 for no progress bar
 */
void
request_set_progress(request *r, int progress) {
    r->progress = progress;
}

/**
 * Grow a character string if necessary
 *
 * @private
 *
 * @param s       Character string to resize if necessary
 * @param nalloc  current size of s, new size on output
 * @param n       currently used length of s
 * @param nadd    number of characters to add
 *
 * @return original character string, or realloced character string
 *
 * @note String always grows by a factor of 2
 *
 */
char *
str_grow(char *s, size_t *nalloc, size_t n, size_t nadd) {
    char *tmp = NULL;
    if(*nalloc == 0) {
        *nalloc = 16;
    }
    if(s == NULL) {
        s = calloc(*nalloc, sizeof(char));
    }
    if(n + nadd + 1 >= *nalloc) {
        while(n + nadd + 1 > *nalloc) {
            *nalloc *= 2;
        }
        if(!(tmp = realloc(s, *nalloc))) {
            printf("Error reallocating space for string\n");
            return NULL;
        }
        s = tmp;
    }
    return s;
}

/**
 * Convert a request to a URL
 *
 * @memberof request
 * @ingroup request
 *
 * @private
 *
 * @param r  request to convert to a URL
 *
 * @return URL character string
 *
 * @warning The output character string is owned by the user and it is the user's 
 *    responsibility to free the underlying memory with \ref free
 */
char *
request_to_url(request *r) {
    /// Data request would be from
    ///   - Reading a station file and eventid (time) + duration
    ///   - Taking a station search parameter and eventid (time + location)
    ///       - Make the stations request
    ///       - Make the availability request
    ///   - After avail request, then the data request
    ///   - How to restart a data download after stopping?
    ///     - Chunk up request and save
    char *out = NULL;
    size_t nalloc = 256;
    size_t n = 0;
    char tmp[128];

    if(!r->url) {
        return NULL;
    }
    out = str_grow(out, &nalloc, n, strlen(r->url));
    fern_strlcat(out, r->url, nalloc);
    n = strlen(out);
    char **keys = dict_keys(r->args);
    for(size_t i = 0; keys[i]; i++) {
        arg_to_string(dict_get(r->args, keys[i]), tmp, sizeof(tmp));
        out = str_grow(out, &nalloc, n, strlen(tmp) + strlen(keys[i] + 2) );
        snprintf(out+n, nalloc-n, "%s=%s&", keys[i], tmp);
        n = strlen(out);
    }
    dict_keys_free(keys);

    if(out[strlen(out)-1] == '&') {
        out[strlen(out)-1] = 0;
    }


    return out;
}


/**
 * Set the URL for the request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r        Request to modify
 * @param url      URL of the request
 *
 * @note If the request does not exist, nothing is done
 *
 */
void
request_set_url(request *r, char *url) {
    if(!r) {
        return;
    }
    FREE(r->url);
    r->url = strdup(url);
}
/**
 * Get the URL for the request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r        Request to get the URL from
 *
 * If the request or the URL does not exist, NULL is returned
 *
 * @warning The URL is owned by the request and should not be modified
 */
char *
request_get_url(request *r) {
    if(!r) {
        return NULL;
    }
    return r->url;
}

/**
 * Set an argument for the request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r     Request to add the argument to
 * @param key   Name of the argument
 * @param a     Value of the Argument
 *
 * @note Undefined requests, keys, or argument results in nothing 
 */
void
request_set_arg(request *r, char *key, Arg *a) {
    if(!r || !r->args || !key || !a) {
        return;
    }
    dict_put(r->args, key, a);
}

/**
 * Get an argument for the request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r     Request to get the argument from
 * @param key   Name of the argument
 *
 * @note Undefined requests, keys, or unknown keys results in nothing
 * @warning Arugment is owned by the request and must not be modified
 */
Arg *
request_get_arg(request *r, char *key) {
    if(!r || !r->args || !key) {
        return NULL;
    }
    return dict_get(r->args, key);
}

/**
 * Remove an argument for the request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r   Request to remove the argument from
 * @param key Name of the argument to remove
 *
 * @return 1 if successful, 0 if not successful
 */
int
request_del_arg(request *r, char *key) {
    if(!r || !r->args || !key) {
        return 0;
    }
    return dict_remove(r->args, key, arg_free);
}


/**
 * Make a request, with POST data if needed
 *
 * @memberof request
 * @ingroup request
 * @private
 *
 * @param url         URL to request data from
 * @param post_data   POST data to send
 *
 * @return result with data and return codes
 *
 * @note An NULL post data is a GET request
 *
 */
result *
request_url_post(char *url, char *post_data, int progress_bar) {
    result *r = result_new();
    struct myprogress prog;
    dnld_params_t dnld_params;
    CURL *curl;
    struct curl_slist *list = NULL;

    zarray data;
    zarray_init(&data);

    //curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    prog.lastruntime[0] = 0;
    prog.lastruntime[1] = 0;
    prog.lastruntime[2] = 0;
    prog.prog[0] = 0;
    prog.prog[1] = 0;
    prog.prog[2] = 0;
    prog.curl = curl;
    prog.last_dlnow = -1;
    memset(dnld_params.remote_fname, 0, sizeof(dnld_params.remote_fname));
    if(curl) {
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // Set User-Agent
        list = curl_slist_append(list, "User-Agent: sac/102.0");
        // Peer Verification
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        // Hostname Verificaiton
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        // Callback to collect data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)& data);

        // Callback to parse header data
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dnld_header_parse);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &dnld_params);

        /* enable all supported built-in compressions */
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

        if(progress_bar) {
            if(!isatty(fileno(stderr))) {
                progress_bar = 0;
            }
        }
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, ! progress_bar);
        if(progress_bar) {
            // Transfer Information
#if CURL_AT_LEAST_VERSION(7,32,0)
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
#else
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
#endif
        }

        // Setup POST if necessary
        if(post_data) {
            list = curl_slist_append(list, "Content-Type: text/plain");
            curl_easy_setopt(curl, CURLOPT_POST, 1); // Send a POST
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data); // Send post data
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        // Verbose
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Perform the request, res will get the return code */

        int code = curl_easy_perform(curl);

        // Create Error result
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &r->http_code);
        r->filename = strdup(dnld_params.remote_fname);
        result_from_curl(r, code, data.data, data.n);

        // CURLINFO_CONTENT_LENGTH_DOWNLOAD_T -- Content Body size
        // https://stackoverflow.com/a/25878250 - Server provided File

        // cleanup
        curl_slist_free_all(list); // free the list
        curl_easy_cleanup(curl);
    }
    //curl_global_cleanup();
    if(progress_bar) {
        clear_line();
    }

    return r;
}

/**
 * Make a POST request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r          request to make
 * @param post_data  POST data to send
 *
 * @return result with data and return codes
 *
 * @warning It is the user's responsibility to free the result, use result_free()
 *
 */
result *
request_post(request *r, char *post_data) {
    result *out = NULL;
    char *url = NULL;
    if(!(url = request_to_url(r))) {
        out = result_error(667, "Error constructing url");
        goto error;
    }
    if(r->verbose) {
        printf("%s\n", url);
        if(post_data) {
            printf("%s\n", post_data);
        }
    }
    out = request_url_post(url, post_data, r->progress);
 error:
    FREE(url);
    return out;
}
/**
 * Make a GET request
 *
 * @memberof request
 * @ingroup request
 *
 * @param r    request to GET
 *
 * @return result with data and return codes
 *
 * @warning It is the user's responsibility to free the result, use result_free()
 *
 */
result *
request_get(request *r) {
    return request_post(r, NULL);
}


/**
 * Make a GET request
 *
 * @memberof request
 * @ingroup request
 * @private
 *
 * @param url    URL to get
 *
 * @return result with data and return codes
 *
 * @warning It is the user's responsibility to free the result, use \ref result_free
 *
 */
result *
request_url_get(char *url, int progress) {
    return request_url_post(url, NULL, progress);
}


enum {
    ARG_NONE = 0,
    INTEGER = 1,
    DOUBLE = 2,
    STRING = 3,
    DATA = 4,
    TIME = 5,
};

/**
 * Format an argument
 *
 * @memberof Arg
 * @ingroup request
 *
 * @private
 * @param p   Argument to format
 * @param dst  Destination for formated Arg
 * @param n    Length of dst
 *
 * @return Destination string
 */
char *
arg_to_string(Arg *p, char *dst, size_t n) {
    if(!dst) {
        return dst;
    }
    memset(dst, 0, n);
    if(!p) {
        return dst;
    }

    switch(p->type) {
    case INTEGER:  snprintf(dst, n, "%d", p->i); break;
    case DOUBLE:   snprintf(dst, n, "%f", p->fp); break;
    case STRING:   snprintf(dst, n, "%s", p->str); break;
    case DATA:     p->data_fmt(p->data, dst, n); break;
    case TIME:     strftime64t(dst, n, "%FT%T", &p->t); break;
    }
    return dst;
}
/**
 * Free an Arg
 *
 * @memberof Arg
 * @ingroup request
 *
 * @param v  Arg to free
 *
 * @note This is typically called by the enclosing \ref request
 */
void
arg_free(void *v) {
    Arg *p = (void *) v;
    if(p) {
        switch(p->type) {
        case ARG_NONE: break;
        case INTEGER:  break;
        case DOUBLE:   break;
        case TIME:     break;
        case STRING:   FREE(p->str); break;
        case DATA:     p->data_free(p->data); break;
        }
        FREE(p);
    }
}
/**
 * Create a new Arg
 *
 * @memberof Arg
 * @ingroup request
 * @private
 * @return newly created Arg
 */
Arg *arg_new() {
    Arg *d = calloc(1, sizeof(Arg));
    d->type = ARG_NONE;
    d->fp = 0.0;
    d->t.tv_sec = 0;
    d->t.tv_nsec = 0;
    d->str = NULL;
    d->data = NULL;
    d->data_fmt = NULL;
    d->data_free = NULL;
    return d;
}

/**
 * Create a new "data" Arg
 *
 * @memberof Arg
 * @ingroup request
 * @private
 * @param data     pointer to data
 * @param data_fmt function to format the data
 * @param data_fmt function to free the data
 *
 * @return new Arg with internal data
 */
Arg * arg_data_new(void *data,
                   char * (*data_fmt)(void *p, char *dst, size_t n),
                   void (*data_free)(void *p)) {
    Arg *d = arg_new();
    d->type = DATA;
    d->data  = data;
    d->data_fmt = data_fmt;
    d->data_free = data_free;
    return d;
}
/**
 * Create a new floating point Arg
 *
 * @memberof Arg
 * @ingroup request
 *
 * @param v  floating point value to store in the Arg
 *
 * @return new Arg with floating point value
 */
Arg * arg_double_new(double v) {
    Arg *d = arg_new();
    d->type = DOUBLE;
    d->fp   = v;
    return d;
}
/**
 * Create a new character string Arg
 *
 * @memberof Arg
 * @ingroup request
 *
 * @param s  character string to store in the Arg
 *
 * @return new Arg with character string
 *
 * @note character string is cloned
 */
Arg * arg_string_new(char *s) {
    Arg *d = arg_new();
    d->type = STRING;
    d->str  = strdup(s);
    return d;
}

/**
 * Create a new integer Arg
 *
 * @memberof Arg
 * @ingroup request
 *
 * @param i  integer to store in the Arg
 *
 * @return new Arg with integer
 */
Arg * arg_int_new(int i) {
    Arg *d = arg_new();
    d->type = INTEGER;
    d->i    = i;
    return d;
}

/**
 * Create a new timevalue Arg
 *
 * @memberof Arg
 * @ingroup request
 *
 * @param t  \ref timespec64 to store in the Arg
 *
 * @return new Arg with timevalue
 */
Arg * arg_time_new(timespec64 t) {
    Arg *d = arg_new();
    d->type = TIME;
    d->t = t;
    return d;
}

/**
 * Get the timevalue from an Arg
 *
 * @memberof Arg
 * @ingroup request
 *
 * @param a  Argument to get the timevalue from
 * @param t  \ref timespec64 to store the timevalue in
 *
 * @return 1 on success, 0 on failure
 */
int
arg_get_time(Arg *a, timespec64 *t) {
    if(!a || a->type != TIME) {
        return 0;
    }
    *t = a->t;
    return 1;
}

/**
 * Get the data from an Arg
 *
 * @memberof Arg
 * @ingroup request
 * @private
 * @param a  Argument to get the data from
 * @param t  pointer to store the data in
 *
 * @return 1 on success, 0 on failure
 */
int arg_get_data(Arg *a, void **data) {
    if(!a || a->type != DATA || ! a->data) {
        return 0;
    }
    *data = a->data;
    return 1;
}




/**
 * Clear the current line
 *
 * @memberof request
 * @ingroup request
 *
 * Fill the current line with spaces and return to the beginning of the line
 *
 * - https://stackoverflow.com/a/1022961
 * - https://stackoverflow.com/a/7105918
 *
 */
void clear_line() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    fprintf(stderr, "\r%*s\r", w.ws_col, " ");
}

/**
 * Format a data size for readability with units
 *
 * @memberof result
 * @ingroup request
 *
 * @param bytes Size in bytes
 * @param out   Destination of formatted string
 * @param n     Length of out
 *
 * @return formatted bytes with units
 *
 */
char *
data_size(int64_t bytes, char *out, size_t n) {
    double kib = 1024.0;
    char *unit[] = { "bytes", "KiB", "MiB", "GiB", "TiB", "PiB" };
    double B[] = { 1, pow(kib,1), pow(kib,2), pow(kib,3), pow(kib,4), pow(kib,5) };
    if(bytes < 0) {
        bytes = 0;
    }
    for(int i = 5; i >= 0; i--) {
        if(bytes >= B[i]) {
            snprintf(out, n, "%6.2f %s", (double)bytes/B[i], unit[i]);
            return out;
        }
    }
    snprintf(out, n, "%" PRId64 " %s", bytes, unit[0]);
    return out;
}

/**
 * Callback to report transfer info
 * @private
 * @ingroup request
 *
 * @param p  Data structure
 * @param dltotal  Total expected downloaded in bytes
 * @param dlnow    Total downloaded in bytes
 * @param ultotal  Total expected uploaded in bytes
 * @param ulnow    Total uploaded in bytes
 *
 * @return 0 to continue, 1 to stop transfer
 */
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
    static int spin = 0;
    char spinny[4] = "-\\|/";
    struct myprogress *myp = (struct myprogress *)p;
    CURL *curl = myp->curl;
    TIMETYPE curtime = 0;
    char tmp[32];
    char tmp2[32];
    static float speed = 0.0;
    UNUSED(ultotal);
    UNUSED(dltotal);
    UNUSED(ulnow);


    curl_easy_getinfo(curl, TIMEOPT, &curtime);
    /* under certain circumstances it may be desirable for certain functionality
       to only run every N seconds, in order to do this the transaction time can
       be used */

    if((curtime - myp->lastruntime[2]) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
        myp->lastruntime[0] = myp->lastruntime[1];
        myp->lastruntime[1] = myp->lastruntime[2];
        myp->lastruntime[2] = curtime;
        myp->prog[0] = myp->prog[1];
        myp->prog[1] = myp->prog[2];
        myp->prog[2] = dlnow;
        float ddata = (float)(myp->prog[2] - myp->prog[0]);
        float dsec  = ((float)(myp->lastruntime[2] - myp->lastruntime[0]) / 1000000);
        if(dsec <= 0.0) {
            speed = 1e9;
        } else {
            speed = ddata / dsec;
        }
    }
    if(dlnow == 0.0) {
        clear_line();
        fprintf(stderr, "Requesting data ... %c", spinny[spin]);
        spin += 1;
        if(spin >= 4) {
            spin = 0;
        }
    }
    if(myp->last_dlnow == dlnow) {
        return 0;
    }
    myp->last_dlnow = dlnow;

#define UNICORN   "\U0001F984"
#define RAINBOW   "\U0001F308"
#define PANDA     "\U0001F43C"
#define SPARKLES  "\u2728"
#define GLOWSTAR  "\U0001F31F"
#define BUTTERFLY "\U0001F98B"

    if(dlnow > 0) {
        clear_line();
        fprintf(stderr, "Downloading data ... %s received %s/sec" ,
                data_size(dlnow,tmp,sizeof(tmp)),
                data_size((curl_off_t) speed,tmp2,sizeof(tmp2))
                );
        /*
        float fac = 1e5;
        if(speed > fac) {
            fprintf(stderr, "%s", UNICORN);
        } else if(speed > 0.8 * fac) {
            fprintf(stderr, "%s", RAINBOW);
        } else if(speed > 0.6 * fac) {
            fprintf(stderr, "%s", PANDA);
        } else if(speed > 0.4 * fac) {
            fprintf(stderr, "%s", BUTTERFLY);
        } else if(speed > 0.2 * fac) {
            fprintf(stderr, "%s", SPARKLES);
        } else if(speed > 0.1 * fac) {
            fprintf(stderr, "%s", GLOWSTAR);
        }
        */
    }
    return 0;
}

#if LIBCURL_VERSION_NUM < 0x072000
/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow)
{
  return xferinfo(p,
                  (curl_off_t)dltotal,
                  (curl_off_t)dlnow,
                  (curl_off_t)ultotal,
                  (curl_off_t)ulnow);
}
#endif

/**
 * Retrieve the filename from the Content Disposition
 *
 * @ingroup request
 * @private
 *
 * @param cd     Content Disposition
 * @param oname  Output filename
 *
 * @return 0 always
 *
 * - https://stackoverflow.com/a/25878250
 */
static
int get_oname_from_cd(char const*const cd, char *oname) {
    char    const*const cdtag   = "Content-disposition:";
    char    const*const key     = "filename=";
    int     ret                 = 0;
    char    *val                = NULL;

    /* Example Content-Disposition: filename=name1367; charset=funny; option=strange */

    char *p = oname;
    /* If filename is present */
    val = fern_strcasestr(cd, key);
    if (!val) {
        printf("No key-value for \"%s\" in \"%s\"", key, cdtag);
        goto bail;
    }

    /* Move to value */
    val += strlen(key);

    /* Copy value as oname */
    while (*val != '\0' && *val != ';') {
        //printf (".... %c\n", *val);
        *oname++ = *val++;
    }
    *oname = '\0';
    // Remove Trailing space
    fern_rstrip(p);

    // Remove possible enclosing quotes
    {
        size_t n = strlen(p);
        if((p[n-1] == '"' && p[0] == '"') ||
           (p[n-1] == '\'' && p[0] == '\'')) {
            p[n-1] = 0;          // Last Character
            memmove(p, p+1, n-1); // Copy all characters back one 
        }

    }

bail:
    return ret;
}

/**
 * Parse the Download Header
 *
 * @memberof request
 * @ingroup request
 * @private
 *
 * @param hdr      Header to parse
 * @param size     size of hdr
 * @param nmemb    items
 * @param userdata User data
 *
 * @param total number of bytes
 */
size_t
dnld_header_parse(void *hdr, size_t size, size_t nmemb, void *userdata) {
    const   size_t  cb      = size * nmemb;
    const   char    *hdr_str= hdr;
    dnld_params_t *dnld_params = (dnld_params_t*)userdata;
    char const*const cdtag = "Content-disposition:";

    /* Example:
     * ...
     * Content-Type: text/html
     * Content-Disposition: filename=name1367; charset=funny; option=strange
     */
    if (!strncasecmp(hdr_str, cdtag, strlen(cdtag))) {
        //printf ("Found c-d: %s\n", hdr_str);
        int ret = get_oname_from_cd(hdr_str+strlen(cdtag), dnld_params->remote_fname);
        if (ret) {
            printf("ERR: bad remote name");
        }
    }

    return cb;
}

/**
 * Read data for the requeset
 *
 * @private
 * @ingroup request
 *
 * @param contents  data
 * @param size      size of data
 * @param nmemb     number of items
 * @param userp     user data
 *
 * @return total size of data
 *
 */
static size_t
memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    zarray *a = (zarray *) userp;
    zarray_append(a, contents, realsize);
    return realsize;
}

/**
 * Get the filename given in the request
 *
 * @memberof result
 * @ingroup request
 *
 * @param r   result to get the filename from
 *
 * @return filename of the result
 *
 * @warning Filename is owned by the result and must not be modified
 *
 */
char *
result_filename(result *r) {
    return r->filename;
}
/**
 * Get the CURL return code
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to get the CURL code from
 *
 * @return CURL code
 *
 */
int
result_code(result *r) {
    return r->code;
}
/**
 * Get the HTTP Return code
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to get the HTTP code from
 *
 * @return HTTP code
 *
 */
int
result_http_code(result *r) {
    return r->http_code;
}

/**
 * Get the returned data
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to get the HTTP code from
 *
 * @return returned data
 */
char *
result_data(result *r) {
    return r->data;
}
/**
 * Get the returned data's length
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to get the data length of
 *
 * @return returned data length in bytes
 */
size_t
result_len(result *r) {
    return r->n;
}

/**
 * Test if the result is empty
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to check if empty
 *
 * @return 1 if empty, 0 if contains data
 *
 * @note This actually checks if the HTTP Return code is either 404 or 204.
 */
int
result_is_empty(result *r) {
    return (r->http_code == 404 || r->http_code == 204);
}

/**
 * Check if the result is "ok"
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to check is ok
 *
 * @return 1 if result is ok, 0 otherwise
 *
 * @note result is ok if 
 *   - result is not NULL
 *   - CURL code is CURLE_OK 
 *   - HTTP code < 400 and != 204
 *
 * - https://stackoverflow.com/a/20490361
 */
int
result_is_ok(result *r) {
    if(!r) {
        return 0;
    }
    return (r->code == CURLE_OK &&
            r->http_code < 400 &&
            r->http_code != 204);
}

/**
 * Crete a new result
 *
 * @memberof result
 * @ingroup request
 * @private
 *
 * @return new result
 *
 * @note This is not noramlly called by the user, instead a result should
 *   be gotten from a request
 *
 */
result *
result_new() {
    result *r = calloc(1, sizeof(result));
    result_init(r);
    return r;
}

/**
 * Return a results error message
 *
 * @memberof result
 * @ingroup request
 *
 * @return Error message
 *
 * @warning It is the user's responsibility to free the memory associated with
 *    the error message
 *
 */
char *
result_error_msg(result *r) {
    char *msg = NULL;
    if(r->code != CURLE_OK) {
        fern_asprintf(&msg, "Error %d: %s\n", r->code, r->error);
    } else {
        if(r->http_code == 404) {
            fern_asprintf(&msg, "Error %d (HTTP): %s\n", r->http_code, "No Content");
        } else {
            fern_asprintf(&msg, "Error %d (HTTP): %s\n", r->http_code, r->data);
        }
    }
    return msg;
}

/**
 * Create an error'ed result
 *
 * @memberof result
 * @ingroup request
 * @private
 *
 * @param code  Error code
 * @param msg  Error message, character string is cloned
 *
 * @return new result with an error
 */
result *
result_error(int code, char *msg) {
    result *r = result_new();
    r->code = CURLE_OK;
    r->http_code = code;
    r->data = strdup(msg);
    return r;
}

/**
 * Free a result and return the requested data
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to free
 *
 * @return requested data
 *
 * @warning Ownership of the data is transferred to the user and it is 
 *     the user's responsibility to free the returned data
 */
char *
result_free_move_data(result *r) {
    char *data = NULL;
    if(!r) {
        return NULL;
    }
    data = r->data;
    FREE(r->filename);
    FREE(r);
    return data;
}

/**
 * Free a result
 *
 * @memberof result
 * @ingroup request
 *
 * @param r result to free
 *
 */
void
result_free(result *r) {
    if(r) {
        FREE(r->data);
        FREE(r->filename);
        FREE(r);
    }
}

/**
 * Initialize a result
 *
 * @memberof result
 * @ingroup request
 * @private
 *
 * @param r  result to initialize
 *
 */
void
result_init(result *r) {
    r->error = NULL;
    r->data  = NULL;
    r->code  = CURLE_FAILED_INIT;
    r->n     = 0;
    r->filename = NULL;
}

/**
 * Initialize a result with data from curl
 *
 * @memberof result
 * @ingroup request
 * @private
 *
 * @param r     result to initialize
 * @param code  CURL code
 * @param data  requested data
 * @param n     length of data
 *
 */
void
result_from_curl(result *r, int code, char *data, size_t n) {
    /* Check for errors */
    r->code = (CURLcode) code;
    if(r->code != CURLE_OK) {
        r->error = curl_easy_strerror(r->code);
    } else {
        r->data = data;
        r->n    = n;
    }
}

/**
 * Find a unique filename by appending numbers to the end
 *
 * @private
 *
 * @param base    base filename
 * @param file    output filename
 * @param nfile   length of file
 *
 */
static void
find_unique_filename(char *base, char *file, size_t nfile) {
    int n = 0;
    fern_strlcpy(file, base, nfile);
    while(TRUE) {
        if( access( file, F_OK ) == -1 ) {
            break;
        }
        snprintf(file, nfile, "%s.%d", base, n);
        n += 1;
    }
}

/**
 * Write the requested data to a file
 *
 * @memberof result
 * @ingroup request
 *
 * @param r        result to requested data to a file
 * @param filename file to create with data
 *
 * @return output filename
 *
 * @note A unique filename is created if the file already exists
 *    by appending numbers to the end of the file
 *
 * @warning The returned filename is owned by the user and the user must free the 
 *   underlying memory
 *
 */
char *
result_write_to_file(result *r, char *filename) {
    FILE *fp = NULL;
    char file[4296] = { 0 };
    char base[4096] = { 0 };
    if(!filename && !r->filename) {
        printf("Error writing data to file: unknown filename\n");
        return 0;
    }
    fern_strlcpy(base, (filename) ? filename : r->filename , sizeof(base));
    find_unique_filename(base, file, sizeof(file));

    if(!(fp = fopen(file, "w"))) {
        printf("Error writing data: Could not open file: %s\n", file);
        return 0;
    }
    if(fwrite(r->data, r->n, 1, fp) != 1) {
        printf("Error writing data to file: Incomplete write\n");
        return 0;
    }
    fclose(fp);
    return strdup(file);
}
/**
 * Write the request data to a file and show the filename written
 *
 * @memberof result
 * @ingroup request
 *
 * @param r   result to write data from
 * @param file  filename to write data to
 *
 */
void
result_write_to_file_show(result *r, char *file) {
    char tmp[64] = {0};
    char *out = result_write_to_file(r, file);
    cprintf("green",
            "Writing data to %s [%s]\n", out,
            data_size((int64_t)result_len(r),tmp,sizeof(tmp)));
    FREE(out);
}






