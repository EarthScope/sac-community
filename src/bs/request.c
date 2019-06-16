
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <curl/curl.h>
#include "octopus.h"

#define UNUSED(x) (void) x

// Externals
char *rstrip(char *s);

struct _result {
    CURLcode code;
    int http_code;
    const char *error;
    char *data;
    size_t n;
    char *filename;
};


typedef struct _zarray zarray;
struct _zarray {
    size_t alloc;
    size_t n;
    char *data;
};

void
zarray_init(zarray *a) {
    a->alloc = 16;
    a->n = 0;
    a->data = malloc(sizeof(char) * a->alloc);
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



#define TIME_IN_US 1
#define TIMETYPE curl_off_t
#define TIMEOPT CURLINFO_TOTAL_TIME_T
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     1000000
struct myprogress {
    TIMETYPE lastruntime[3]; /* type depends on version, see above */ 
    CURL *curl;
    curl_off_t last_dlnow;
    curl_off_t prog[3];
};

// https://stackoverflow.com/a/1022961
// https://stackoverflow.com/a/7105918
static
void clear_line() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    fprintf(stderr, "\r%*s\r", w.ws_col, "");
}

char *
data_size(int64_t bytes, char *out, size_t n) {
    float kib = 1024.0;
    char *unit[] = { "bytes", "KiB", "MiB", "GiB", "TiB", "PiB" };
    float B[] = { 1, pow(kib,1), pow(kib,2), pow(kib,3), pow(kib,4), pow(kib,5) };
    if(bytes < 0) {
        bytes = 0;
    }
    for(int i = 5; i >= 0; i--) {
        if(bytes >= B[i]) {
            snprintf(out, n, "%6.2f %s", (float)bytes/B[i], unit[i]);
            return out;
        }
    }
    snprintf(out, n, "%lld %s", bytes, unit[0]);
    return out;
}

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */ 
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

typedef struct {
    char        remote_fname[4096];
} dnld_params_t;

// https://stackoverflow.com/a/25878250
static
int get_oname_from_cd(char const*const cd, char *oname) {
    char    const*const cdtag   = "Content-disposition:";
    char    const*const key     = "filename=";
    int     ret                 = 0;
    char    *val                = NULL;

    /* Example Content-Disposition: filename=name1367; charset=funny; option=strange */

    char *p = oname;
    /* If filename is present */
    val = strcasestr(cd, key);
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
    rstrip(p);

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

static size_t
memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    zarray *a = (zarray *) userp;
    zarray_append(a, contents, realsize);
    return realsize;
}

char *
result_filename(result *r) {
    return r->filename;
}
int
result_code(result *r) {
    return r->code;
}
int
result_http_code(result *r) {
    return r->http_code;
}

char *
result_data(result *r) {
    return r->data;
}
size_t
result_len(result *r) {
    return r->n;
}

int
result_is_empty(result *r) {
    return (r->http_code == 404 || r->http_code == 204);
}

// https://stackoverflow.com/a/20490361
int
result_is_ok(result *r) {
    return (r->code == CURLE_OK &&
            r->http_code < 400 &&
            r->http_code != 204);
}

result *
result_new() {
    result *r = calloc(1, sizeof(result));
    result_init(r);
    return r;
}

char *
result_error_msg(result *r) {
    char *msg = NULL;
    if(r->code != CURLE_OK) {
        asprintf(&msg, "Error %d: %s\n", r->code, r->error);
    } else {
        if(r->http_code == 404) {
            asprintf(&msg, "Error %d (HTTP): %s\n", r->http_code, "No Content");
        } else {
            asprintf(&msg, "Error %d (HTTP): %s\n", r->http_code, r->data);
        }
    }
    return msg;
}

result *
result_error(int code, char *msg) {
    result *r = result_new();
    r->code  = code;
    r->error = strdup(msg);
    return r;
}

void
result_init(result *r) {
    r->error = NULL;
    r->data  = NULL;
    r->code  = CURLE_FAILED_INIT;
    r->n     = 0;
    r->filename = NULL;
}
void
result_from_curl(result *r, int code, char *data, size_t n) {
    /* Check for errors */
    r->code = code;
    if(r->code != CURLE_OK) {
        r->error = curl_easy_strerror(r->code);
    } else {
        r->data = data;
        r->n    = n;
    }
}

static void
find_unique_filename(char *base, char *file, size_t nfile) {
    int n = 0;
    strlcpy(file, base, nfile);
    while(TRUE) {
        if( access( file, F_OK ) == -1 ) {
            break;
        }
        snprintf(file, nfile, "%s.%d", base, n);
        n += 1;
    }
}

int
result_write_to_file(result *r, char *filename) {
    char tmp[64] = { 0 };
    FILE *fp = NULL;
    char file[4096] = { 0 };
    char base[4096] = { 0 };
    if(!filename && !r->filename) {
        printf("Error writing data to file: unknown filename\n");
        return 0;
    }
    strlcpy(base, (filename) ? filename : r->filename , sizeof(base));
    find_unique_filename(base, file, sizeof(file));

    if(!(fp = fopen(file, "w"))) {
        printf("Error writing data to file: could not open file: %s\n", file);
        return 0;
    }
    printf("Writing data to %s [%s]\n", file, data_size(r->n,tmp,sizeof(tmp)));
        if(fwrite(r->data, r->n, 1, fp) != 1) {
        printf("Error writing data to file: Incomplete write\n");
        return 0;
    }
    fclose(fp);
    return 1;
}

result *
request_full(char *url, char *post_data) {
    result *r = result_new();
    struct myprogress prog;
    dnld_params_t dnld_params;
    CURL *curl;

    zarray data;
    zarray_init(&data);

    curl_global_init(CURL_GLOBAL_DEFAULT);

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
        // Peer Verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        // Hostname Verificaiton
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        // Callback to collect data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)& data);

        // Callback to parse header data
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dnld_header_parse);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &dnld_params);

        // Transfer Information
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        // Setup POST if necessary
        if(post_data) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        }
        /* Perform the request, res will get the return code */
        int code = curl_easy_perform(curl);
        // Create Error result
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &r->http_code);
        r->filename = strdup(dnld_params.remote_fname);
        result_from_curl(r, code, data.data, data.n);

        // CURLINFO_CONTENT_LENGTH_DOWNLOAD_T -- Content Body size
            // https://stackoverflow.com/a/25878250 - Server provided File
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    clear_line();

    return r;
}

result *
request(char *url) {
    return request_full(url, NULL);
}


