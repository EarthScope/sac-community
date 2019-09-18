/**
 * @file
 * @brief Data Requests
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include <libmseed/libmseed.h>

#include "array.h"
#include "datareq.h"
#include "request.h"
#include "cprint.h"
#include "miniseed_sac.h"

#include "chash.h"
#include "defs.h"
#include "strip.h"
#include "urls.h"

typedef struct breq_fast_line breq_fast_line;

/**
 * @brief Data request line, specifies only a single channel
 * @ingroup    data
 * @private
 */
struct breq_fast_line {
    char net[16];
    char sta[16];
    char loc[16];
    char cha[16];
    timespec64 t1;
    timespec64 t2;
};

typedef struct breq_fast      breq_fast;
/**
 * @brief      Single data request, made of multiple channels
 * @ingroup    data
 * @private
 */
struct breq_fast {
    int comment;  /**< if the data request is commented */
    dict *urls;   /**< urls where requests are made */
    char **lines; /**< individual data lines in the request */
};

/**
 * @brief Collection data requests, made to, possibly multiple data centers
 * @ingroup    data
 */

struct data_request {
    dict *pars;        /**< search parameters */
    breq_fast **reqs; /**< Individual Data Requests */
};




void             breq_fast_line_free(breq_fast_line *r);
breq_fast_line * breq_fast_line_new();
char *           breq_fast_line_format(breq_fast_line *x);

/**
 * @brief Data Availability Request
 * @ingroup data
 */
struct data_avail {};

void   data_request_init(data_request *f);
int    breq_fast_line_parse(char *line, breq_fast_line *x);
size_t breq_fast_line_size(breq_fast_line *x);
void   breq_fast_line_init(breq_fast_line *x);
void   breq_fast_init(breq_fast *f);
static int parse_key_value(char *in, char delim, char **key, char **val);

/**
 * @defgroup data data
 * @brief Data requests, availability, conversion, miniseed, sac
 */

/**
 * @brief Initialize a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r  data request
 *
 * @note Sets the request to https://service.iris.edu/irisws/fedcatalog/1 and
 *     - location = *
 *     - quality  = B
 *     - format   = request
 *     - nodata   = 404
 */
void
data_avail_init(request *r) {
    request_set_url(r, FEDCATALOG_IRIS);
    request_set_arg(r, "loc", arg_string_new("*"));
    request_set_arg(r, "quality", arg_string_new("B"));
    request_set_arg(r, "format", arg_string_new("request"));
    request_set_arg(r, "nodata", arg_int_new(404));
}

/**
 * @brief Create a new data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @return new data request, initialized
 *
 */
request *
data_avail_new() {
    request *r = request_new();
    data_avail_init(r);
    return r;
}

/**
 * @brief Set the region for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r        data request
 * @param minlon   Minimum Longitude, west
 * @param maxlon   Maximum Longitude, east
 * @param minlat   Minimum Latitude, south
 * @param maxlat   Maximum Latitude, north
 *
 */
void
data_avail_set_region(request *r, double minlon, double maxlon, double minlat, double maxlat) {
    request_set_arg(r, "minlon", arg_double_new(minlon));
    request_set_arg(r, "maxlon", arg_double_new(maxlon));
    request_set_arg(r, "minlat", arg_double_new(minlat));
    request_set_arg(r, "maxlat", arg_double_new(maxlat));
}

/**
 * @brief Set the search origin location for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r    data request
 * @param lon  Lonitude
 * @param lat  Latitude
 *
 * @note use this with data_avail_set_radius()
 *
 */
void
data_avail_set_origin(request *r, double lon, double lat) {
    request_set_arg(r, "lon", arg_double_new(lon));
    request_set_arg(r, "lat", arg_double_new(lat));
}
/**
 * @brief Set the search origin radius for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r     data request
 * @param minr  minimum radius in degrees
 * @param maxr  maximum radius in degrees
 *
 * @note use this with data_avail_set_origin()
 *
 */
void
data_avail_set_radius(request *r, double minr, double maxr) {
    request_set_arg(r, "minradius", arg_double_new(minr));
    request_set_arg(r, "maxradius", arg_double_new(maxr));
}
/**
 * @brief Set the time range for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r     data request
 * @param start start time for data request
 * @param end   end time for data request
 *
 */
void
data_avail_set_time_range(request *r, timespec64 start, timespec64 end) {
    request_set_arg(r, "start", arg_time_new(start));
    request_set_arg(r, "end",   arg_time_new(end));
}
/**
 * @brief Set the network for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r   data request
 * @param net networks to select
 *
 * @note use
 *      - wildcards * and ?
 *      - - for negation
 *      - lists are comma separated with no spaces
 */
void
data_avail_set_network(request *r, char *net) {
    request_set_arg(r, "net", arg_string_new(net));
}
/**
 * @brief Set the station for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r   data request
 * @param sta stations to select
 *
 * @note use
 *      - wildcards * and ?
 *      - - for negation
 *      - lists are comma separated with no spaces
 */
void
data_avail_set_station(request *r, char *sta) {
    request_set_arg(r, "sta", arg_string_new(sta));
}
/**
 * @brief Set the location for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r   data request
 * @param loc locations to select
 *
 * @note use
 *      - wildcards * and ?
 *      - - for negation
 *      - lists are comma separated with no spaces
 */
void
data_avail_set_location(request *r, char *loc) {
    request_set_arg(r, "loc", arg_string_new(loc));
}
/**
 * @brief Set the channel for a data request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r   data request
 * @param cha channels to select
 *
 * @note use
 *      - wildcards * and ?
 *      - - for negation
 *      - lists are comma separated with no spaces
 */
void
data_avail_set_channel(request *r, char *cha) {
    request_set_arg(r, "cha", arg_string_new(cha));
}

/**
 * @brief Set data quality of request
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r        breq_fast
 * @param quality  quality of the data, see \ref Quality
 *
 */
void
data_avail_set_quality(request *r, Quality quality) {

    switch(quality) {
    case QualityAll:
        request_set_arg(r, "quality", arg_string_new("*"));
        break;
    case QualityD:
    case QualityUnknown:
        request_set_arg(r, "quality", arg_string_new("D"));
        break;
    case QualityRaw:
        request_set_arg(r, "quality",  arg_string_new("R"));
        break;
    case QualityQual:
    case QualityQC:
        request_set_arg(r, "quality",  arg_string_new("Q"));
        break;
    case QualityModified:
    case QualityMerged:
        request_set_arg(r, "quality",  arg_string_new("M"));
        break;
    case QualityBest:
        request_set_arg(r, "quality",  arg_string_new("B"));
        break;
    }
}

/**
 * @brief Check is a data request is ok before sending
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r             data request to check
 * @param need_net_sta  if network and station is required
 *
 * @note check if the following fields are defined:
 *    - channel
 *    - start time
 *    - end time
 *    - network (if need_net_sta)
 *    - station (if need_net_sta)
 */
int
data_avail_is_ok(request *r, int need_net_sta) {

    if(!request_get_arg(r, "cha")) {
        return 0;
    }
    if(!request_get_arg(r, "start") || !request_get_arg(r, "end")) {
        return 0;
    }
    if(need_net_sta) {
        if(!request_get_arg(r, "net") || !request_get_arg(r, "sta")) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Set the data request based on the start time and a duration
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r    data request
 * @param d    duration
 *
 * @note new end time is calculated from the current start time and the duration
 */
void
data_avail_use_duration(request *r, duration *d) {
    Arg *a;
    timespec64 ts, te;
    if(!d) {
        return;
    }
    if(!(a = request_get_arg(r, "start"))) {
        return;
    }
    if(! arg_get_time(a, &ts)) {
        return;
    }
    te = timespec64_add_duration(ts, d);
    request_del_arg(r, "end");
    request_set_arg(r, "end", arg_time_new(te));
}

/**
 * @brief Fill a data request from a station file
 *
 * @memberof   data_avail
 * @ingroup    data
 *
 * @param r     data request
 * @param file  station file to create request from
 *
 * @return specific, long format data request for POSTing
 *
 * @note location, channel, start and end times are taken from the data request
 *
 * @warning user owns the data and is required to free the underlying memory
 *
 */
char *
data_avail_from_station_file(request *r, char *file) {
    char *key[] = {"loc", "cha", "start", "end"};
    char t1[128] = {0}, t2[128] = {0};
    char net[32] = {0};
    char sta[32] = {0};
    char loc[32] = {0};
    char cha[32] = {0};
    FILE *fp = NULL;
    char line[2048] = {0};
    char tmp[2048] = {0};
    char *req = NULL;
    size_t nalloc = 1024;
    size_t n = 0;

    for(size_t i = 0; i < 4; i++) {
        if(!request_get_arg(r, key[i])) {
            printf("Missing value for %s\n", key[i]);
            goto error;
        }
    }

    if(!(fp = fopen(file, "r"))) {
        printf("Error opening station file for data request: %s\n", file);
        goto error;
    }
    printf("Reading station file: %s\n", file);

    //string_printf_append(&req, "format=request\n");
    //string_printf_append(&req, "mergequality=true\n");
    //string_printf_append(&req, "mergesamplerate=true\n");
    arg_to_string(request_get_arg(r, "loc"),   loc, sizeof(loc));
    arg_to_string(request_get_arg(r, "cha"),   cha, sizeof(cha));
    arg_to_string(request_get_arg(r, "start"), t1,  sizeof(t1));
    arg_to_string(request_get_arg(r, "end"),   t2,  sizeof(t2));

    fgets(line, sizeof(line), fp); // Header Line
    while(fgets(line, sizeof(line), fp)) {
        if(sscanf(line, "%10s %10s", net, sta) != 2) {
            printf("Error reading station file on line: %s\n", line);
            goto error;
        }
        sprintf(tmp, "%-5s %-8s %-4s %-5s %s %s\n", net, sta, loc, cha, t1, t2);
        req = str_grow(req, &nalloc, n, strlen(tmp));
        n = fern_strlcat(req, tmp, nalloc);
    }

    return req;
 error:
    if(fp) {
        fclose(fp);
    }
    FREE(req);
    return NULL;
}

/**
 * @brief Convert the channels band code into samples per second (sps)
 *
 * @memberof   breq_fast
 * @ingroup    data
 *
 * @param band
 *
 * @private
 *
 * @return samples per second
 *
 * @note Values are approximate
 */
double
band_to_sps(char band) {
    double sps = 1.0;
    switch(band) {
    case 'F': sps =1000.0;     break;
    case 'G': sps =1000.0;     break;
    case 'D': sps = 500.0;     break;
    case 'C': sps = 250.0;     break;
    case 'E': sps = 100.0;     break; // ExtremelyShortPeriod
    case 'S': sps =  40.0;     break; // ShortPeriod
    case 'H': sps = 100.0;     break; // HighBroadBand
    case 'B': sps =  40.0;     break; // Broadband
    case 'M': sps =   5.0;     break; // MidPeriod
    case 'L': sps =   1.0;     break; // LongPeriod
    case 'V': sps =   0.1;     break; // VeryLongPeriod
    case 'U': sps =   0.01;    break; // UltraLongPeriod
    case 'R': sps =   0.00030; break; // ExtremelyLongPeriod
    case 'P': sps =   0.001;   break; // Does not exist
    case 'T': sps =   0.001;   break; // Only 3 channels
    case 'Q': sps =   0.05000; break;
    case 'A': sps =   1.0;     break; // Administrative
    case 'O': sps =   1.0;     break; // Opaque
    case 'W': sps =   1.0;     break; // Associated with Wind and Pressure
    }
    return sps;
}


/**
 * @brief Create an new data request
 *
 * @memberof   breq_fast
 * @ingroup    data
 * @private
 *
 * @return     new and initialized data request
 */
breq_fast *
breq_fast_new() {
    breq_fast *r = calloc(1, sizeof(breq_fast));
    breq_fast_init(r);
    return r;

}

/**
 * @brief Initialize a data request
 *
 * @memberof   breq_fast
 * @ingroup    data
 * @private
 *
 * @param f   request to initialize
 *
 */
void
breq_fast_init(breq_fast *f) {
    f->comment = FALSE;
    f->urls = dict_new();
    f->lines = xarray_new('p');
}
/**
 * @brief Join a list of strings
 *
 * @memberof   breq_fast
 * @ingroup    data
 *
 * @private
 *
 * @param items  Collection of character strings to join, enclosed in a xarray
 * @param s      joining character string
 *
 * @return list of character strings joined together
 *
 * @warning User owns the returned value and is responsible for freeing the underlying memory
 *    with free
 *
 */
char *
str_join(char **items, char *s) {
    char *out = NULL;
    size_t nalloc = 256;
    size_t nn = 0;
    size_t n = xarray_length(items);
    for(size_t i = 0; i < n; i++) {
        out = str_grow(out, &nalloc, nn, strlen(items[i]) + strlen(s));
        fern_strlcat(out, items[i], nalloc);
        fern_strlcat(out, s, nalloc);
        nn = strlen(out);
    }
    return out;
}

/**
 * @brief Estimate the total size of a  request
 *
 * @memberof breq_fast
 * @ingroup    data
 * @private
 *
 * @param f     data request
 *
 * @return estimated size of all requests in bytes
 *
 * @note see breq_fast_line_size() for assumptions
 *
 */
size_t
breq_fast_size(breq_fast *f) {
    size_t n = 0;
    size_t mem = 0;
    if(!f || !f->lines || xarray_length(f->lines) || !f->urls) {
        return 0;
    }
    n = xarray_length(f->lines);
    for(size_t i = 0; i < n; i++) {
        breq_fast_line x = { .net={0}, .sta={0}, .loc={0}, .cha={0},
                             .t1= {0,0}, .t2= {0,0} };
        breq_fast_line_parse(f->lines[i], &x);
        mem += breq_fast_line_size(&x);
    }
    return mem;
}


/**
 * @brief Free a data request
 *
 * @memberof   breq_fast
 * @ingroup    data
 * @private
 *
 * @param r   data request to free
 *
 */
void
breq_fast_free(breq_fast *r) {
    if(r) {
        dict_free(r->urls, free);
        xarray_free_items(r->lines, free);
        xarray_free(r->lines);
        FREE(r);
    }
}

/**
 * @brief Free a data request
 *
 * @memberof breq_fast
 * @ingroup    data
 *
 * @private
 *
 * @param f   data request to free
 *
 */
void
breq_fast_free_void(void *p) {
    breq_fast_free((breq_fast *) p);
}


/**
 * @brief Request data from a data center and download the requested data
 *
 * @memberof   breq_fast
 * @ingroup    data
 * @private
 *
 * @param f    data request
 *
 * @return \ref result of request
 *
 * @note URL to send request to is located in DATASELECTSERVICE key
 *
 */
result *
breq_fast_send(breq_fast *f) {
    int end_slash = 0;
    result *r = NULL;
    char *url = NULL;
    char *ds_url = NULL;
    char *req = NULL;
    request *fr = request_new();
    if(!f || !f->lines || xarray_length(f->lines) == 0 || ! f->urls) {
        return NULL;
    }
    if(!(ds_url = dict_get(f->urls, "DATASELECTSERVICE"))) {
        return NULL;
    }
    end_slash = ds_url[strlen(ds_url)-1] == '/';
    asprintf(&url, "%s%squery", ds_url, (!end_slash) ? "/" : "");
    request_set_url(fr, url);
    req = str_join(f->lines, "\n");
    r = request_post(fr, req);

    FREE(req);
    FREE(url);
    REQUEST_FREE(fr);
    return r;
}

/**
 * @brief Copy the urls from one data request to another
 *
 * @memberof   breq_fast
 * @ingroup    data
 * @private
 *
 * @param      dst   destination data request
 * @param      src   source data request
 *
 */
void
breq_fast_copy_urls(breq_fast *dst, breq_fast *src) {
    // Copy Keys from one Dict to another
    int j = 0;
    char **keys = dict_keys(src->urls);
    while(keys[j]) {
        dict_put(dst->urls, keys[j], strdup(dict_get(src->urls, keys[j])));
        j++;
    }
    dict_keys_free(keys);
    keys = NULL;
}
/**
 * @brief Create a data request from a data request line
 *
 * @memberof   breq_fast_line breq_fast
 * @ingroup    data
 * @private
 *
 * @param      x    data request line
 * @param      r    request
 *
 * @return     new  data request
 */
breq_fast *
breq_fast_from_breq_fast_line(breq_fast_line *x, breq_fast *r) {
    char *newline = NULL;
    breq_fast *fr = breq_fast_new();
    breq_fast_copy_urls(fr, r);
    newline = breq_fast_line_format(x);
    fr->lines = xarray_append(fr->lines, newline);
    return fr;
}

/**
 * @brief Split a data request into time chunks
 *
 * @memberof   breq_fast breq_fast_line
 * @ingroup    data
 * @private
 *
 * @param      x     data request line
 * @param      r     data request
 * @param      mem1  size of  data request line
 * @param      max   max size of data requests
 *
 * @return     collection of  data requests
 */
breq_fast **
breq_fast_time_split(breq_fast_line *x, breq_fast *r, size_t mem1, size_t max) {

    breq_fast *fr = NULL;
    //timespec64 t1 = {0,0}, t2 = {0,0};
    breq_fast **new = xarray_new('p');
    //t1 = x->t1;
    //t2 = x->t2;
    double nr = ceil( (float)mem1 / (float)max );
    int64_t ts = x->t2.tv_sec - x->t1.tv_sec;
    //printf("%f :: %f %lld\n", (float)ts/nr, nr*ceil((float)ts/nr), ts);
    int64_t dt = (int64_t) ceil((double) ts / nr);
    x->t2 = x->t1;

    int64_t t = 0;
    while(t < ts) {
        x->t2.tv_sec += dt;

        fr = breq_fast_from_breq_fast_line(x, r);
        new = xarray_append(new, fr);
        t += dt;
        x->t1 = x->t2;
    }
    return new;
}

#define FR_APPEND(fr, new, r, mem) do {           \
        if(xarray_length(fr->lines) > 0) {        \
            new = xarray_append(new, fr);         \
            fr = breq_fast_new();               \
            breq_fast_copy_urls(fr, r);         \
            mem = 0;                              \
        }                                         \
    } while(0);

#define FR_EXTEND(v, new)  do {                              \
        for(size_t k = 0; k < xarray_length(v); k++) {       \
            new = xarray_append(new, v[k]);                  \
        }                                                    \
        xarray_free(v);                                      \
    } while(0);


/**
 * @brief Create a new data request list
 *
 * @memberof    data_request
 * @ingroup     data
 *
 * @return      data request list
 */
data_request *
data_request_new() {
    data_request *f = calloc(1, sizeof(data_request));
    data_request_init(f);
    return f;
}

/**
 * @brief Initialize a  data requests collection
 *
 * @memberof data_request
 * @ingroup    data
 *
 * @param f federaed requests to initialize
 *
 */
void
data_request_init(data_request *f) {
    f->pars = dict_new();
    f->reqs = xarray_new('p');
}

/**
 * @brief Free a data request list
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @param      r    data request list to free
 *
 */
void
data_request_free(data_request *r) {
    if(r) {
        dict_free(r->pars, free);
        xarray_free_items(r->reqs, breq_fast_free_void);
        xarray_free(r->reqs);
        FREE(r);
    }
}

/**
 * @brief Parse a data request list
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @param      data    input data, text from fedcatalog URL
 *
 * @return     data request list
 *
 * @warning    User owns the output and is responsible for freeing the
 *             underlying memory with data_request_free()
 */
data_request *
data_request_parse(char *data) {
    char *line = NULL;
    int state = 0;
    char *line_orig = NULL;
    char *key = NULL, *value = NULL;
    int comment = 0;
    data_request *fdr = data_request_new();
    breq_fast  *fd = NULL;
    breq_fast_line x;
    breq_fast_line_init(&x);


    while((line = strsep(&data, "\n")) != NULL) {
        line_orig = line;
        //printf("line: '%s'\n", line);
        comment = 0;
        if(*line == '#') {
            comment = 1;
            line ++;
            while(*line == ' ') { line++; }
            if(*line == '#') {
                continue;
            }
        }
        int has_service = strstr(line, "SERVICE") != NULL;
        int has_datacenter = strncmp(line, "DATACENTER", 10) == 0;
        int empty = strlen(fern_rstrip(line)) == 0;
        //printf("service, datacenter, empty, state: %d %d %d %d\n", has_service, has_datacenter, empty, state);

        // State Determination
        if(state == 0) { // Expecting has_datacenter
            state = (has_datacenter) ? 1 : 0;
            if(state == 1) {
                fd = breq_fast_new();
                fd->comment = comment;
                fdr->reqs = xarray_append(fdr->reqs, fd);
            }
        } else if(state == 1) { // Expecting has_service or request line
            state = (has_service) ? 1 : 2 ;
        } else if(state == 2) { // Expecting request line or empty line
            state = (empty) ? 0 : 2 ;
        }
        // Action
        if(state == 0 && !empty) {
            if(parse_key_value(line, '=', &key, &value)) {
                dict_put(fdr->pars, key, value);
                FREE(key);
            } else {
                printf(" WARNING: Expected key=value for request parameters in data_request_parse\n");
                printf("          %s\n", line_orig);
            }
        } else if(state == 1) {
            if(parse_key_value(line, '=', &key, &value)) {
                dict_put(fd->urls, key, value);
                FREE(key);
            } else {
                printf(" WARNING: Expected key=value for service URLs in data_request_parse\n");
                printf("          %s\n", line_orig);
            }
        } else if(state == 2) {
            if(breq_fast_line_parse(line, &x)) {
                fd->lines = xarray_append(fd->lines, strdup(line));
            }
        }
    }
    if(xarray_length(fdr->reqs) == 0) {
        data_request_free(fdr);
        fdr = NULL;
    }
    return fdr;
}

/**
 * @brief Download data with a data request list
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @param      fdr          data request
 * @param      filename     data request filename
 * @param      prefix       prefix for miniseed output files
 * @param      save_files   save data to miniseed files
 * @param      unpack_data  unpack data and place into a Miniseed Trace List
 *
 * @return     miniseed trace list
 */
MS3TraceList *
data_request_download(data_request *fdr, char *filename, char *prefix,
                           int save_files, int unpack_data) {
    timespec64 t = {0,0};
    // Download data
    char date[64];
    MS3TraceList *mst3k = NULL;
    for(size_t i = 0; i < xarray_length(fdr->reqs); i++) {
        t = timespec64_now();
        char *dc = NULL;
        result *fr = NULL;
        breq_fast *r = fdr->reqs[i];
        if(r->comment) {
            continue;
        }
        strftime64t(date, sizeof(date), "%Y.%m.%d.%H.%M.%S", &t);
        dc = dict_get(r->urls, "DATACENTER");
        cprintf("", "Data Center: %s\n", dc);
        fr = breq_fast_send(r);
        printf("\t");
        if(result_is_ok(fr)) {
            if(save_files) {
                char file[2048] = {0};
                char dcname[128] = {0};
                char *p = NULL;
                fern_strlcpy(dcname, dc, sizeof(dcname));
                if((p = strchr(dcname, ','))) {
                    *p = 0;
                } else {
                    dcname[0] = 0;
                }
                snprintf(file, sizeof(file), "%s.%s.%s.mseed",
                         prefix, date, dcname);
                result_write_to_file_show(fr, file);
            }
            if(unpack_data) {
                if(!mst3k) {
                    mst3k = mstl3_init(NULL);
                }
                read_miniseed_memory(mst3k, result_data(fr),
                                     (uint64_t) result_len(fr));
            }
        } else if (result_http_code(fr) == 204) {
            cprintf("red,bold", "No data available\n");
        } else if (result_http_code(fr) == 404) {
            cprintf("red,bold", "No data available\n");
        } else {
            printf("%s\n", result_error_msg(fr));
        }
        RESULT_FREE(fr);
        r->comment = TRUE;
        data_request_write_to_file(fdr, filename);
    }
    return mst3k;
}

/**
 * @brief Split a data request list into chunks
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @param      fdr   data request list to split
 * @param      max   maximum size of chunk
 *
 */
void
data_request_chunks(data_request *fdr, size_t max) {
    size_t mem = 0, mem1 = 0;
    size_t i, j;
    size_t n = xarray_length(fdr->reqs);
    char *line = NULL;
    breq_fast *r = NULL;
    breq_fast *fr = NULL;
    breq_fast **tsplit = NULL;
    breq_fast_line x;
    breq_fast_line_init(&x);
    breq_fast **new = xarray_new('p');
    for(i = 0; i < n; i++) {
        r = fdr->reqs[i];
        fr = breq_fast_new();
        breq_fast_copy_urls(fr, r);
        mem = 0;
        for(j = 0; j < xarray_length(r->lines); j++) {
            line = r->lines[j];
            // printf("LINE: %s\n", line);
            if(!breq_fast_line_parse(line, &x)) {
                continue;
            }
            mem1 = breq_fast_line_size(&x);
            if(mem1 > max) {
                FR_APPEND(fr, new, r, mem);
                //printf("Splitting by time: mem: %zu max: %zu\n", mem1, max);
                // Need to split the request by time, rather than lines
                tsplit = breq_fast_time_split(&x, r, mem1, max);
                FR_EXTEND(tsplit, new);
            } else {
                // Append to Current Request
                fr->lines = xarray_append(fr->lines, strdup(line));
                mem += mem1;
                // Make a new Request if current one is too bug
                if(mem > max) {
                    FR_APPEND(fr, new, r, mem);
                }
            }
        }
        if(xarray_length(fr->lines) > 0) {
            new = xarray_append(new, fr);
        }
    }
    xarray_free_items(fdr->reqs, breq_fast_free_void);
    xarray_free(fdr->reqs);
    fdr->reqs = new;
}

#define COMMENT(r, fp) do { if(r->comment) { fprintf(fp, "# "); } } while(0);

/**
 * @brief Write a data request list to a file
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @param      fdr       data request
 * @param      filename file to write to
 *
 */
void
data_request_write_to_file(data_request *fdr, char *filename) {
    FILE *fp = NULL;
    if(!(fp = fopen(filename, "w"))) {
        return ;
    }
    data_request_write(fdr, fp);
    fclose(fp);
}
/**
 * @brief Write / print a data request_list
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @param      fdr   data request
 * @param      fp   file pointer, can be stdout
 *
 * @note       less information is printed out for ttys / stdout
 */
void
data_request_write(data_request *fdr, FILE *fp) {
    size_t i = 0, j = 0, n = 0, m = 0;
    int is_tty = isatty(fileno(fp));
    // Write Parameters
    if(!is_tty) {
        fprintf(fp, "## REQUEST PARAMETERS \n");
        char **keys = dict_keys(fdr->pars);
        for(j = 0; keys[j]; j++) {
            fprintf(fp, "%s=%s\n", keys[j], (char*) dict_get(fdr->pars, keys[j]));
        }
        for(j = 0; keys[j]; j++) { FREE(keys[j]); } FREE(keys);
        fprintf(fp,"\n");
    }
    n = xarray_length(fdr->reqs);
    for(i = 0; i < n; i++) {
        cfprintf(fp, "bold,black", "## REQUEST %zu/ %zu\n", i+1, n);
        breq_fast *r = fdr->reqs[i];
        COMMENT(r,fp);
        fprintf(fp, "%s=%s\n", "DATACENTER", (char *) dict_get(r->urls, "DATACENTER"));
        if(!is_tty) {
            char **keys = dict_keys(r->urls);
            for(j = 0 ; keys[j]; j++) {
                if(strcmp(keys[j], "DATACENTER") != 0) {
                    COMMENT(r,fp);
                    fprintf(fp, "%s=%s\n", keys[j], (char *) dict_get(r->urls, keys[j]));
                }
            }
            for(j = 0; keys[j]; j++) { FREE(keys[j]); } FREE(keys);
        }
        m = xarray_length(r->lines);
        for(j = 0; j < m; j++) {
            COMMENT(r,fp);
            fprintf(fp, "%s\n", r->lines[j]);
        }
        fprintf(fp, "\n");
    }
}

/**
 * @brief Parse a key=value pair
 *
 * @memberof   data_request
 * @ingroup    data
 *
 * @private
 *
 * @param      in     input string
 * @param      delim  delimiter to split on
 * @param      key    output key
 * @param      val    output value
 *
 * @return     1 on success, 0 on failure
 *
 * @warning    User owns the new key and value and is responsible
 *             for freeing the undelying memory with free
 */
static int
parse_key_value(char *in, char delim, char **key, char **val) {
    char *p;

    if(!(p = strchr(in, delim))) {
        return 0;
    }
    *p = 0;
    *key = fern_strip(in);
    *val = fern_strip(p+1);
    return 1;
}


/**
 * @brief Create a new  data request line
 *
 * @memberof   breq_fast_line
 * @ingroup    data
 * @private
 *
 * @return     new data request line
 *
 */
breq_fast_line *
breq_fast_line_new() {
    breq_fast_line *r = calloc(1, sizeof(breq_fast_line));
    breq_fast_line_init(r);
    return r;
}

/**
 * @brief Initialize a data request line
 *
 * @memberof   breq_fast_line
 * @ingroup    data
 * @private
 *
 * @param      x     data request line to initialize
 *
 */
void
breq_fast_line_init(breq_fast_line *x) {
    memset(x->net, 0, 16);
    memset(x->sta, 0, 16);
    memset(x->loc, 0, 16);
    memset(x->cha, 0, 16);
    x->t1.tv_sec  = 0;
    x->t1.tv_nsec = 0;
    x->t2.tv_sec  = 0;
    x->t2.tv_nsec = 0;
}

/**
 * @brief Free a data request line
 *
 * @memberof   breq_fast_line
 * @ingroup    data
 * @private
 *
 * @param      r     data request line to free
 *
 */
void
breq_fast_line_free(breq_fast_line *r) {
    if(r) {
        breq_fast_line_init(r);
        FREE(r);
    }
}


/**
 * @brief Parse a  request data line
 *
 * @memberof breq_fast_line
 * @ingroup    data
 * @private
 *
 * @param line    line to parse
 * @param x        request line to fill
 *
 * @return 1 on success, 0 on failure
 *
 * @note expected format is net sta loc chan start-time end-time
 *
 */
int
breq_fast_line_parse(char *line, breq_fast_line *x) {
    char start[64] = {0};
    char end[64] = {0};
    int64_t sec = 0;
    if(sscanf(line, "%15s %15s %15s %15s %63s %63s\n",
              x->net, x->sta, x->loc, x->cha, start, end) != 6) {
        printf(" WARNING: Cannot parse request line, skipping\n\t%s\n", line);
        return 0;
    }
    if(!timespec64_parse(start, &x->t1) ||
       !timespec64_parse(end,   &x->t2)) {
        printf(" WARNING: Cannot parse date/time, skipping\n");
        printf("\t%s\n", line);
        return 0;
    }
    if(timespec64_cmp(&x->t1, &x->t2) > 0) {
        printf(" WARNING: Start-time after end-time, skipping\n");
        printf("\t%s\n", line);
        return 0;
    }
    sec = x->t2.tv_sec - x->t1.tv_sec;
    if(sec > 60*60*24*366) {
        printf(" WARNING: Very long request duration: %lld years, skipping\n", sec/(60*60*24*366));
        printf("\t%s\n", line);
        return 0;
    }
    return 1;
}
/**
 * @brief Determine the size in bytes of a  request data line
 *
 * @memberof breq_fast_line
 * @ingroup    data
 *
 * @private
 *
 * @param x    data request line
 *
 * @return estimated size of request in bytes
 *
 * @note Assumptions:
 *    - 4 bytes per data point (floating point)
 *    - approximate samples per second from channel
 *
 */
size_t
breq_fast_line_size(breq_fast_line *x) {
    double sps   = 0.0;
    int64_t sec = 0.0;
    sec = x->t2.tv_sec - x->t1.tv_sec;
    sps = band_to_sps(x->cha[0]);
    return (size_t) ceil(sps * sec) * sizeof(float);
}
/**
 * @brief Format a data request line
 *
 * @memberof   breq_fast_line
 * @ingroup    data
 * @private
 *
 * @param      x  data request
 *
 * @return     formated character string
 */
char *
breq_fast_line_format(breq_fast_line *x) {
    char tmp1[64] = {0}, tmp2[64] = {0};
    char *newline = NULL;
    strftime64t(tmp1, sizeof(tmp1), "%FT%T.%3f", &x->t1);
    strftime64t(tmp2, sizeof(tmp2), "%FT%T.%3f", &x->t2);
    asprintf(&newline, "%s %s %s %s %s %s",
             x->net, x->sta, x->loc, x->cha, tmp1, tmp2);
    return newline;
}

