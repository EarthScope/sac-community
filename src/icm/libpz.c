/**
 * @file Polezero functions
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <glob.h>
#include <float.h>

#include "amf.h"
#include "lhf.h"
#include "hdr.h"
#include "icm.h"
#include "dff.h"
#include "ucf.h"
#include "bot.h"
#include "sac_datetime.h"
#include "debug.h"
#include "SacHeader.h"
#include "libpz.h"

#ifdef WIN32
#define pointer char *
#else
#define pointer void *
#endif

#define KEY_CONSTANT  "CONSTANT"
#define KEY_ZEROS     "ZEROS"
#define KEY_POLES     "POLES"
#define KEY_STAR      '*'


/**
 * Parse a polezero comment value
 *
 * Arguments:
 *   - `p` - Input line, assumed to be split at a ':'
 *
 * Returns:
 *   - Value string after the ':'
 *
 */
static char *
polezero_comment_token(char *p) {
    if (!p) {
        return p;
    }
    p = strchr(p, ':');
    if (p) {
        p = p + 1;
        p = lstrip(p);
        p = rstrip(p);
    }
    return p;
}

/**
 * Parse a comment as a string
 *
 * Arguments:
 *   - `p` - Input line
 *   - `meta` - Metadata for the polezero file section
 *   - `c` - Comment type, here uses the offset of the value in meta
 *
 * Parsed value is stored at (meta + c->off) as a `char *`
 *
 */
void
polezero_comment_string(char *p, pzmeta_t * meta, pzcomment_t * c) {
    char *pp;
    char **s;
    s = (char **) ((pointer) meta + c->off);
    if (*s) {
        free(*s);
        *s = NULL;
    }
    pp = polezero_comment_token(p);
    if (!pp || !*pp) {
        *s = NULL;
        return;
    }
    *s = strdup(pp);
    if (!*s) {
        fprintf(stdout, "polezero-comment: Error converting string: '%s'\n", p);
    }
}
/**
 * Parse a comment as a datetime
 *
 * Arguments:
 *   - `p` - Input line
 *   - `meta` - Metadata for the polezero file section
 *   - `c` - Comment type, here uses the offset of the value in meta
 *
 * Parsed value is stored in (meta + c->off) as a datatime
 */
void
polezero_comment_datetime(char *p, pzmeta_t * meta, pzcomment_t * c) {
    char *pp;
    datetime **s;
    s = (datetime **) ((char *) meta + c->off);
    *s = NULL;
    pp = polezero_comment_token(p);
    if (!pp || !*pp) {
        datetime_free(*s);
        return;
    }
    *s = datetime_parse(pp, *s);
    if (!*s) {
        fprintf(stdout, "polezero-comment: Error parsing datetime: '%s'\n", p);
    }
}

/**
 *  Determine if a characeter is within 0-9.+-eEdD
 */
/*
static int
is_numexp(char c) {
    return (isdigit(c) || c == '.' || c == '+' || c == '-' || c == 'e' ||
            c == 'E' || c == 'd' || c == 'D');
}
*/

/**
 * Parse a comment as a float
 *
 * Arguments:
 *   - `p` - Input line
 *   - `meta` - Metadata for the polezero file section
 *   - `c` - Comment type, here uses the offset of the value in meta
 *
 * Parsed value is stored in (meta + c->off) as a float
 */
void
polezero_comment_float(char *p, pzmeta_t * meta, pzcomment_t * c) {
    char *pp;
    float *f;

    f = (float *) ((pointer) meta + c->off);
    pp = polezero_comment_token(p);
    if (!pp || !*pp) {
        *f = 0.0;
        return;
    }
    if(sscanf(pp, "%g", f) != 1) {
        fprintf(stdout, "polezero-comment: Error converting float: '%s'\n", p);
        *f = 0.0;
    }
    return;
    /* s = strdup(pp); */
    /* pp = s; */
    /* /\* Find the end of the number *\/ */
    /* while (pp && is_numexp(*pp)) { */
    /*     pp++; */
    /* } */
    /* /\* Truncate the string *\/ */
    /* if (pp && *pp) { */
    /*     *pp = 0; */
    /* } */
    /* *f = cdouble(s, &nerr); */
    /* free(s); */
    /* s = NULL; */
    /* if (nerr != 0) { */
    /*     fprintf(stdout, "polezero-comment: Error converting float: '%s'\n", p); */
    /*     *f = 0.0; */
    /* } */
}

/**
 * Available comment metadata tags
 *
 * Key - String of value
 * Type - string, datetime, or float
 * Loation - where the values is stored
 *
 */
pzcomment_t pzc[] = {
    {" network ", polezero_comment_string, offsetof(pzmeta_t, net)},
    {" station ", polezero_comment_string, offsetof(pzmeta_t, stat)},
    {" channel ", polezero_comment_string, offsetof(pzmeta_t, chan)},
    {" component ", polezero_comment_string, offsetof(pzmeta_t, chan)},
    {" location ", polezero_comment_string, offsetof(pzmeta_t, loc)},
    {" description ", polezero_comment_string, offsetof(pzmeta_t, descrip)},
    {" input_unit ", polezero_comment_string, offsetof(pzmeta_t, input_unit)},
    {" output_unit ", polezero_comment_string, offsetof(pzmeta_t, output_unit)},
    {" insttype ", polezero_comment_string, offsetof(pzmeta_t, instrument_type)},
    {" start ", polezero_comment_datetime, offsetof(pzmeta_t, start)},
    {" end ", polezero_comment_datetime, offsetof(pzmeta_t, end)},
    {" created ", polezero_comment_datetime, offsetof(pzmeta_t, created)},
    {" latitude ", polezero_comment_float, offsetof(pzmeta_t, lat)},
    {" longitude ", polezero_comment_float, offsetof(pzmeta_t, lat)},
    {" elevation ", polezero_comment_float, offsetof(pzmeta_t, elev)},
    {" depth ", polezero_comment_float, offsetof(pzmeta_t, depth)},
    {" dip ", polezero_comment_float, offsetof(pzmeta_t, dip)},
    {" azimuth ", polezero_comment_float, offsetof(pzmeta_t, az)},
    {" sample rate ", polezero_comment_float, offsetof(pzmeta_t, sample_rate)},
    {" instgain ", polezero_comment_float, offsetof(pzmeta_t, gain)},
    {" comment ", polezero_comment_string, offsetof(pzmeta_t, comment)},
    {" sensitivity ", polezero_comment_float, offsetof(pzmeta_t, sensitivity)},
    {" a0 ", polezero_comment_float, offsetof(pzmeta_t, a0)},
};


/**
 * Initialize the Polezero metadata struct 
 */
void
polezero_meta_init(pzmeta_t * meta) {
    meta->net = NULL;
    meta->stat = NULL;
    meta->chan = NULL;
    meta->loc = NULL;
    meta->created = NULL;
    meta->start = NULL;
    meta->end = NULL;
    meta->descrip = NULL;
    meta->lat = 0.0;
    meta->lon = 0.0;
    meta->elev = 0.0;
    meta->depth = 0.0;
    meta->dip = 0.0;
    meta->az = 0.0;
    meta->sample_rate = 0.0;
    meta->input_unit = NULL;
    meta->output_unit = NULL;
    meta->instrument_type = NULL;
    meta->gain = 0.0;
    meta->sensitivity = 0.0;
    meta->comment = NULL;
    meta->a0 = 0.0;
}

/**
 * Copy and remove trailing whitespace from a string
 */
char *
strdup_rstrip(char *s) {
    if(!s) {
        return strdup("");
    }
    return rstrip(strdup(s));
}

/**
 * Create a new station id
 *
 * Arguments:
 *   - `net` - Network name (CI, TA, IU, II, ...)
 *   - `stat` - Station name (PAS, BKS, BORG, ...)
 *   - `loc` - Location name (00, 10, 20, ...)
 *   - `chan` - Channel name (BHZ, HHZ, LH1, ...)
 *   - `year` - Reference year
 *   - `doy` - Reference day of the year
 *   - `hour` - Refernce Hour
 *   - `min` - Reference Minute
 *   - `sec` - Reference Second
 *   - `msec` - Reference Milliseconds
 *
 * All times are specificed in UTC
 *
 * Returns:
 *   - Initialize Station ID struct
 *
 */
station_id_t *
station_id_new(char *net, char *stat, char *loc, char *chan,
               int year, int doy, int hour, int min, int sec, int msec) {
    station_id_t *s;
    s = (station_id_t *) malloc(sizeof(station_id_t));
    if(!s) {
        return NULL;
    }
    s->net  = strdup_rstrip(net);
    s->stat = strdup_rstrip(stat);
    s->loc  = strdup_rstrip(loc);
    s->chan = strdup_rstrip(chan);
    s->ref  = datetime_new();
    datetime_set_year(s->ref, year);
    datetime_set_doy(s->ref, doy);
    datetime_set_hour(s->ref, hour);
    datetime_set_minute(s->ref, min);
    datetime_set_second(s->ref, sec);
    datetime_set_nanosecond(s->ref, msec*1e6);
    datetime_doy2ymd(s->ref);
    datetime_normalize(s->ref);
    return s;
}

/**
 * Crate a Station ID from a sac file
 *
 * Arguments:
 *   - `s` - Sac file to create station id struct from 
 *
 * Returns:
 *   - Initialize Station ID struct
 *
 */
station_id_t *
station_id_from_sac(sac *s) {
    return station_id_new(s->h->knetwk, s->h->kstnm,
                          s->h->khole, s->h->kcmpnm,
                          s->h->nzyear, s->h->nzjday,
                          s->h->nzhour, s->h->nzmin,
                          s->h->nzsec, s->h->nzmsec);
}

/**
 * Allocate and initialize a new polezero struct
 *
 * Returns:
 *   Newly allocated polezero struct with a
 *   - constant of 1.0
 *   - no poles
 *   - no zeros
 *
 */
pz_t *
polezero_new() {
    pz_t *pz;
    pz = (pz_t *) malloc(sizeof(pz_t));
    if(!pz) {
        return NULL;
    }
    pz->constant = 1.0;
    pz->nzero = 0;
    pz->npole = 0;
    pz->zeros = NULL;
    pz->poles = NULL;
    pz->nerr = 0;
    pz->line = NULL;
    return pz;
}

/**
 * Free a polezero struct
 *
 * Arguments:
 *   - `pz` - Polezero struct to free
 *
 */
void
polezero_free(pz_t *pz) {
    if(pz) {
        FREE(pz->zeros);
        FREE(pz->poles);
        FREE(pz->line);
        FREE(pz);
    }
}

/**
 * Allocate and initialize polezero metadata struct
 *
 * Returns:
 *   A newly allocated and initialized polezero metadata struct
 *
 */
pzmeta_t *
polezero_meta_new() {
    pzmeta_t *meta;
    meta = (pzmeta_t *) malloc(sizeof(pzmeta_t));
    if (!meta) {
        fprintf(stderr, "Error allocating space for polezero meta data\n");
        return NULL;
    }
    polezero_meta_init(meta);
    return meta;
}

#define STRINGCOPY(to, from, field) do {                        \
                if(from->field) {                               \
                        to->field = strdup( from->field );      \
                } else {                                        \
                        to->field = strdup(" ");                \
                }                                               \
        } while(0)


/**
 * Copy a polezero metadata struct
 *
 * Arguments:
 *   - `m` - Polezero metadata struct to copy
 *
 * Returns:
 *   Newly allocated polezero metadata struct with values from input
 *
 */
pzmeta_t *
polezero_meta_copy(pzmeta_t * m) {
    pzmeta_t *new;
    if (!m) {
        return NULL;
    }
    new = polezero_meta_new();
    if (!new) {
        return NULL;
    }
    STRINGCOPY(new, m, net);
    STRINGCOPY(new, m, stat);
    STRINGCOPY(new, m, chan);
    STRINGCOPY(new, m, loc);
    STRINGCOPY(new, m, input_unit);
    STRINGCOPY(new, m, output_unit);
    STRINGCOPY(new, m, instrument_type);
    STRINGCOPY(new, m, comment);

    new->created = datetime_copy(m->created);
    new->start = datetime_copy(m->start);
    new->end = datetime_copy(m->end);
    new->lat = m->lat;
    new->lon = m->lon;
    new->elev = m->elev;
    new->depth = m->depth;
    new->dip = m->dip;
    new->az = m->az;
    new->sample_rate = m->sample_rate;
    new->gain = m->gain;
    new->sensitivity = m->sensitivity;
    new->a0 = m->a0;

    return new;
}

/**
 * Free a polezero metadata struct
 *
 * Arguments:
 *   - `meta` - Polezero metadata struct to free
 */
void
polezero_meta_free(pzmeta_t * meta) {
    if (!meta) {
        return;
    }
    FREE(meta->net);
    FREE(meta->stat);
    FREE(meta->chan);
    FREE(meta->loc);
    FREE(meta->descrip);
    FREE(meta->input_unit);
    FREE(meta->output_unit);
    FREE(meta->instrument_type);
    datetime_free(meta->start);
    datetime_free(meta->end);
    datetime_free(meta->created);
    FREE(meta);
}

/**
 * Find the first occurrence of find in s, ignore case.
 *
 * Arguments:
 *   - `s` - Input string to look in
 *   - `find` - String to find in s
 *
 * Returns:
 *   - If `s` is NULL, return NULL
 *   - If `find` is not in `s`, return NULL
 *   Otherwise a pointer to the first occurance of `find` in `s`
 *
 */
char *
strcasestr_bsd(const char *s, const char *find) {
    char c, sc;
    size_t len;
    if (!s) {
        return NULL;
    }
    if ((c = *find++) != 0) {
        c = (char) tolower((unsigned char) c);
        len = strlen(find);
        do {
            do {
                if ((sc = *s++) == 0)
                    return (NULL);
            } while ((char) tolower((unsigned char) sc) != c);
        } while (strncasecmp(s, find, len) != 0);
        s--;
    }
    return ((char *) s);
}

/**
 * Parse a polezero comment
 *
 * Arguments:
 *   - `line` - Input comment line to parse
 *   - `meta` - Polezero metadata struct to fill
 *
 */
void
polezero_comment_parse(char *line, pzmeta_t * meta) {
    int i, n;
    char *p;
    n = sizeof(pzc) / sizeof(pzcomment_t);
    for (i = 0; i < n; i++) {
        p = strcasestr_bsd(line, pzc[i].key);
        if (p != NULL && !strcasestr_bsd(line, "channel flag")) {
            pzc[i].parse(p, meta, &(pzc[i]));
        }
    }
    return;
}



/**
 * Determine if string (char *) is not-NULL and its length is > 0
 *
 * Arguments:
 *   - `s` - Input char * (string)
 *
 * Returns:
 *   1 if s is non-NULL and its length is > 0
 *
 */
int
sdef(char *s) {
    return (s && strlen(s) > 0);
}

/**
 * Determine if the polezero section is correct for a set of data identifiers
 *
 * Arguments:
 *   - `meta` - Polezero metadata, derived from section comments
 *   - `filetime` - Datetime struct identifying a time of the data
 *   - `stat` - Station name
 *   - `net` - Network name
 *   - `loc` -  Location name
 *   - `chan` -  Channel name
 *
 * Returns:
 *   If values are defined in both input value and `meta`
 *     - 0 / False if `stat`, `net`, `loc`, or `chan` does not match those in `meta`
 *     - 0 / False if `filetime` is not within the the `meta` time range
 *     - 1 / True if `stat`, `net`, `loc` and `chan` match those in the `meta`
 *             and the `filetime` is within the `meta` time range
 *   If values are not defined in input or `meta`
 *     - 1 / True always
 *
 */
int
polezero_is_correct_block(pzmeta_t * meta, datetime * filetime, char *stat,
                          char *net, char *loc, char *chan) {
    if ((sdef(meta->stat) && sdef(stat) && strcasecmp(stat, meta->stat) != 0) ||
        (sdef(meta->net) && sdef(net) && strcasecmp(net, meta->net) != 0) ||
        (sdef(meta->loc) && sdef(loc) && strcasecmp(loc, meta->loc) != 0) ||
        (sdef(meta->chan) && sdef(chan) && strcasecmp(chan, meta->chan) != 0)) {
        return 0;
    }
    if (filetime && datetime_status(meta->start) == DATETIME_OK &&
        datetime_status(meta->end) == DATETIME_OK) {
        return datetime_in_span(filetime, meta->start, meta->end);
    }
    return TRUE;
}

/**
 * Determine if the polezero section is correct for a station id
 *
 * Arguments:
 *   - `meta` - Polezero metadata struct 
 *   - `s` - Station Id 
 *
 * Returns:
 *   True if the metadata matches the station id
 *
 */
int
polezero_is_correct_block_stat(pzmeta_t *meta, station_id_t *s) {
    return polezero_is_correct_block(meta, s->ref, s->stat, s->net, s->loc, s->chan);
}

enum {
    NONE = 0,
    POLES = 1,
    ZEROS = 2,
};


static int
startswith(char *str, char *prefix, int ignore_case) {
    int n,m;
    n = strlen(prefix);
    m = strlen(str);
    if(n > m) {
        return 0;
    }
    if(ignore_case) {
        return strncasecmp(str, prefix, n) == 0;
    }
    return strncmp(str, prefix, n) == 0;
}

/*
void
polezero_parse_(char *filename, char *net, char *sta, char *loc, char *cha, char *when, int filename_s, int net_s, int sta_s, int loc_s, int cha_s, int when_s) {
    
}
*/

/**
 * Parse a polezero file
 *
 * Arguments:
 *   - `filename` - File to read and parse
 *   - `stat` - Station id
 *
 * Returns:
 *   Polezero struct
 *
 * Errors:
 *   - 0 - No Error
 *   - 114 - Unexpected end of file
 *   - 1301 - File not found
 *   - 2110 - Illegal option is Polezero file, typically an unknown keyword
 *   - 2108 - Error prasing number of poles or poles < 0
 *   - 2109 - Error prasing number of zeros or zeros < 0
 *   - 2118 - Constant value could not be parse, is 0, infinite, or NaN
 *   - 2126 - Error parsing pole value
 *   - 2127 - Error parsing zero value
 *   - 2128 - Cannot find polezero section matching file
 *
 *   The offending line is saved in the line variable of the polezero struct
 *
 */
pz_t *
polezero_parse(char *filename, station_id_t *stat) {
    pz_t* pz;
    pzmeta_t *meta, *meta_used;
    FILE *fp;
    int state, ip, iz;
    char kiline[MCMSG+1];
    char *kline;

    pz = polezero_new();
    if(!(fp = fopen(filename, "r"))) {
        pz->nerr = 1301;
        return pz;
    }

    meta = polezero_meta_new();
    meta_used = NULL;

    state = NONE;
    ip = iz = 0;

    memset(kiline, 0, sizeof(kiline));

    while(fgetsp(kiline, MCMSG + 1, fp) != NULL) {
        kline = kiline;

        /* Remove Leading Whitespace */
        while(kline && isspace(*kline)) {
            kline++;
        }

        /* Skip Empty Lines */
        if(strlen(kline) == 0) {
            continue;
        }

        if(kline[0] == KEY_STAR) {
            polezero_comment_parse(kline, meta);
        } else if(!polezero_is_correct_block_stat(meta, stat)) {

        } else if(startswith(kline, KEY_CONSTANT, TRUE)) {
            if(sscanf(kline + strlen(KEY_CONSTANT), "%lg", &pz->constant) != 1 ||
               isnan(pz->constant) || pz->constant == 0 || pz->constant == HUGE_VAL ||
               pz->constant == HUGE_VAL ) {
                pz->nerr = 2118;
                pz->line = strdup(kiline);
                goto ERROR;
            }
            meta_used = (meta_used) ? meta_used : polezero_meta_copy(meta) ;
        } else if(startswith(kline, KEY_POLES, TRUE)) {
            if(sscanf(kline + strlen(KEY_POLES), "%d", &pz->npole) != 1 || pz->npole < 0) {
                pz->nerr = 2108;
                pz->line = strdup(kiline);
                goto ERROR;
            }
            state = POLES;
            pz->poles = (complexd *) calloc(pz->npole, sizeof(complexd));
            meta_used = (meta_used) ? meta_used : polezero_meta_copy(meta) ;
        } else if(startswith(kline, KEY_ZEROS, TRUE)) {
            if(sscanf(kline + strlen(KEY_ZEROS), "%d", &pz->nzero) != 1 || pz->nzero < 0) {
                pz->nerr = 2109;
                pz->line = strdup(kiline);
                goto ERROR;
            }
            state = ZEROS;
            pz->zeros = (complexd *) calloc(pz->nzero, sizeof(complexd));
            meta_used = (meta_used) ? meta_used : polezero_meta_copy(meta) ;
        } else if(state == POLES || state == ZEROS) {
            double re, im;
            if(sscanf(kline, "%lg %lg", &re, &im) != 2) {
                pz->nerr = (state == POLES) ? 2126 : 2127;
                pz->line = strdup(kiline);
                goto ERROR;
            }
            if(state == POLES) {
                pz->poles[ip].re = re;
                pz->poles[ip].im = im;
                ip++;
            } else {
                pz->zeros[iz].re = re;
                pz->zeros[iz].im = im;
                iz++;
            }
        } else {
            pz->nerr = 2110;
            pz->line = strdup(kiline);
            goto ERROR;
        }
    }
    if(!feof(fp)) {
        pz->nerr = 114;
    }
    if(!meta_used) {
        pz->nerr = 2128;
    }
 ERROR:
    polezero_meta_free(meta);
    polezero_meta_free(meta_used);
    fclose(fp);
    return pz;
}

/**
 * Compute the frequency spacing
 *
 * Arguments:
 *   - `n` - Length of time series
 *   - `dt` - Time spacing of data points
 *
 * Returns:
 *   Frequency spacing (df) = 1 / (n*dt)
 */
double
dfreq(int n, double dt) {
    return 1.0 / (n * dt);
}

/**
 * Compute Frequenies 
 *
 * Arguments:
 *   - `n` - Number of frequencies to compute
 *   - `f0` - Initial frequency
 *   - `dt` - Frequency spacing
 *
 * Returns:
 *   Newly allocated frequencies [f0, ..., f0 + df * (n-1)]
 *
 */
double *
fftfreq(int n, double f0, double df) {
    double *f = calloc(n, sizeof(double));
    for(int i = 0; i < n; i++) {
        f[i] = f0 + (i * df);
    }
    return f;
}

/**
 * Allocate and initialize a new Transfer struct
 *
 * Arguments:
 *   - `n` - Length of time series
 *   - `dt` - Time spacing of time series
 *
 * Returns:
 *   Newly allocated and initialized Transfer struct
 *
 */
Transfer *
transfer_new(int n, double dt) {
    Transfer *t = calloc(1, sizeof(Transfer));
    // Data Values
    t->n      = n;
    t->dt     = dt;
    // Derived Values
    t->nfft   = next2(n);
    t->nfreqs = t->nfft/2 + 1;
    t->df     = dfreq(t->nfft, dt);
    t->f      = fftfreq(t->nfft, 0.0, t->df);
    return t;
}

/**
 * Free a Transfer struct
 *
 * Arguments:
 *   - `t` - Transfer struct to free
 *
 */
void
transfer_free(Transfer *t) {
    if(!t) {
        return;
    }
    FREE(t->f);
    FREE(t);
}

/**
 * Allocate a response
 *
 * Arguments:
 *   - `n` - Length of allocated response
 *
 * Returns:
 *   - Newly allocated response, with all values at 0.0
 *
 */
Response *
response_alloc(int n) {
    Response *r = calloc(1, sizeof(Response));
    int nfft = next2(n);
    r->re = calloc(nfft, sizeof(double));
    r->im = calloc(nfft, sizeof(double));
    r->n  = n;
    return r;
}

/**
 * Free a response
 *
 * Arguments:
 *   - `r` - Response to free
 *
 */
void
response_free(Response *r) {
    if(!r) {
        return;
    }
    FREE(r->re);
    FREE(r->im);
    FREE(r);
}

/**
 * Crate a response with a value of 1+0i
 *
 * Arguments:
 *   - `n` - Length of response to create
 *
 * Returns:
 *   Response will all values at 1+0i
 *
 */
Response *
response_ones(int n) {
    Response *r = response_alloc(n);
    for(int i = 0; i < n; i++) {
        r->re[i] = 1.0;
    }
    return r;
}

/**
 * Create a response from a interwoven complex array
 *
 * Arguments:
 *   - `r2` - Complex array with alternating real/imaginary values
 *   - `n` - Length of the complex array, e.g. number of complex values
 *
 * Returns:
 *   Response with values from `r2`
 *
 */
Response *
response_from_2vec(double *r2, int n) {
    Response *r = response_alloc(n);
    int j = 0;
    for(int i = 0; i < n; i++) {
        r->re[i] = r2[j++];
        r->im[i] = r2[j++];
    }
    return r;
}

/**
 * Take the reciprical of a response
 *
 * Arguments:
 *   - `r` - Response to take the reciprical of, done in place
 *
 * len = Real(r)**2 + Imag(r)**2
 *  if len < FLT_MIN:
 *     1/r = 0+0i
 *  else:
 *    Real(1/r) =  Real(r) * len 
 *    Imag(1/r) = -Imag(r) * len
 *
 */
void
response_recip(Response *r) {
    for(int i = 0; i < r->n; i++)  {
        double re = r->re[i];
        double im = r->im[i];
        double len = re*re + im*im;
        if(len < FLT_MIN) {
            r->re[i] = 0.0;
            r->im[i] = 0.0;
        } else {
            len = 1.0 / len;
            r->re[i] =  re * len;
            r->im[i] = -im * len;
        }
    }
}

/**
 * Create a response from a polezero struct
 *
 * Arguments:
 *   - `pz` - Polezero file
 *   - `t` - Transfer struct
 *
 * Compute the response of a polezero file based on the length and sampling 
 *   of the desired transfer function
 *
 */
Response *
response_from_polezero(pz_t *pz, Transfer *t) {
    Response *r = response_alloc(t->nfreqs);
    getrand(t->nfreqs, t->df, pz->constant,
            pz->nzero, pz->zeros,
            pz->npole, pz->poles,
            r->re, r->im);
    return r;
}

/**
 *  Split a Station ID into component parts
 * 
 * Arguments:
 *  - `id` - Instrument Identifier, NET.STA.LOC.CHA (Input)
 *  - `net` - Network (Output)
 *  - `sta` - Station (Output)
 *  - `loc` - Location (Output)
 *  - `cha` - Channel (Output)
 *
 *  If `id` is `*` then the `id` is populated from the current sac file
 *    using `sac_station_id()`
 *
 */
void
sac_station_id_split(char *id, char *net, char *sta, char *loc, char *cha) {
    char *tok;
    char *p, *p0;
    if(strcmp(id, "*") == 0) {
        char sid[64];
        sac_station_id(sid);
        p = strdup(sid);
    } else {
        p = strdup(id);
    }
    p0 = p;
    for(int i = 0; i < 4; i++) {
        if((tok = strsep(&p, "."))) {
            switch(i) {
            case 0: strcpy(net,tok); break;
            case 1: strcpy(sta,tok); break;
            case 2: strcpy(loc,tok); break;
            case 3: strcpy(cha,tok); break;
            }
        }
    }
    FREE(p0);
}

/**
 * Format a string based on sac header values
 *
 * Arguments:
 *   - `s` - sac file to use as input data
 *   - `fmt` - input format of the string
 *
 * Returns:
 *   Formated output string
 *
 * Only %{header_variable} keys are understood and integer and strings are available
 *
 * Examples:
 *   - "delta: %{delta}"
 *   - "%{knetwk}.%{kstnm}.%{khole}.%{kcmpnm}"
 *   - "%{}.%{kstnm}.%{khole}.%{kcmpnm}"

 *
 */
char *
sac_format_string(sac *s, char *fmt) {
    char *p;
    char *out;
    string *str = string_new("");
    p = fmt;
    while(*p) {
        if(*p == '%') {
            p++; if(!*p) { break; }
            if(*p == '%') {
                str = string_append(str, "%");
                p++;
                break;
            }
            if(*p == '{') {
                char *e;
                p++; if(!*p) { break; }
                if((e = index(p,'}'))) {
                    int icat, item, lfound;
                    char *colon;
                    int n = e-p;
                    char *fmt = NULL;
                    char *key = calloc(n+1, sizeof(char));
                    strncpy(key, p, n);
                    key[n] = 0;
                    if((colon = index(key, ':'))) {
                        fmt = strdup(colon+1);
                        *colon = 0;
                    }
                    hdrfld(key, strlen(key)+1, &icat, &item, &lfound);
                    if(lfound) {
                        switch(icat) {
                        case STRING_TYPE: {
                            char *v = khdr(s, item);
                            if(SAC_CHAR_DEFINED(v)) {
                                char *v2 = strdup(v);
                                rstrip(v2);
                                str = string_printf_append(str, (fmt) ? fmt : "%s", v2);
                            }
                        }
                            break;
                        case INT_TYPE: {
                            int v = VALUE(nhdr(s, item));
                            if(SAC_INT_DEFINED(v)) {
                                str = string_printf_append(str, (fmt) ? fmt : "%d", v);
                            }
                        }
                            break;
                        }
                    }
                    FREE(fmt);
                    p = e;
                    p++;
                }
            }
        } else {
            str = string_printf_append(str, "%c", *p);
            p++;
        }
    }
    out = strdup(string_string(str));
    string_free(&str);
    return out;
}

/**
 *  Create Station ID, NET.STA.LOC.CHA from active sac file
 *
 * Arguments:
 *  - `id` - Output station id, length must be at least 20
 *
 */
void
sac_station_id(char *id) {
    sac *s;
    if(!(s = sacget_current())) {
        printf("Error getting current sac file\n");
        return;
    }
    char *str = sac_format_string(s, "%{knetwk}.%{kstnm}.%{khole}.%{kcmpnm}");
    strcpy(id, str);
    FREE(str);
}

void
sac_station_id_(char *id, int id_s) {
    char id_c[64];
    memset(id_c, 0, sizeof(id_c));
    sac_station_id(id_c);
    fstrset(id_c, id, id_s);
}
void
sac_station_id__(char *id, int id_s) {
    sac_station_id_(id, id_s);
}

/**
 * Generate a SAC Polezero filename from a Station id
 *
 * Arguments:
 *   - `id` - Station id string: NET.STAT.LOC.CHAN (Input)
 *   - `pzfile` - Polezero filename (Output), length must be at least 32
 *
 */
void
sac_station_id_to_polezero(char *id, char *pzfile) {
    char sta[10],net[10],loc[10],cha[10];
    sac_station_id_split(id, net, sta, loc, cha);
    sprintf(pzfile, "SAC_PZs_%s_%s_%s_%s_", net,sta,cha,loc);
}

/**
 *  Create Reference time, YYYY,DDD,HH:MM:SS, from active sac file
 *
 * Arguments:
 *  - `when` - Output reference time, length must be at least 20
 *
 */
void
sac_reference_time(char *when) {
    sac *s;
    char *str;
    if(!(s = sacget_current())) {
        printf("Error getting current sac file\n");
        return;
    }
    str = sac_format_string(s, "%{nzyear:%04d},%{nzjday:%03d},%{nzhour:%02d}:%{nzmin:%02d}:%{nzsec:%02d}");
    strcpy(when, str);
    FREE(str);
}


void
sac_reference_time_(char *when, const int when_s) {
    char when_c[64];
    memset(when_c, 0, sizeof(when_c));
    sac_reference_time(when_c);
    fstrset(when_c, when, when_s);
}
void
sac_reference_time__(char *when, const int when_s) {
    sac_reference_time_(when, when_s);
}



datetime * datetime_parse(char *s, datetime *t);

/**
 * Find polezero files 
 *
 * Arguments:
 *   - `dir` - directory to search (Input), if NULL use current directory
 *   - `id` - Station Id string: NET.STAT.LOC.CHAN (Input)
 *   - `n` - Number of files found (Output)
 *
 * Returns:
 *   Array of polezero file names found
 *
 */
char **
pzfiles_find(char *dir, char *id, int *n) {
    glob_t g;
    int i;
    char **out;
    char *pat;
    char net[10], sta[10], loc[10], cha[10];

    sac_station_id_split(id, net, sta, loc, cha);

    if(dir) {
        asprintf(&pat, "%s/SAC_PZs_%s_%s_%s_%s_*", dir,net,sta,cha,loc);
    } else {
        asprintf(&pat, "SAC_PZs_%s_%s_%s_%s_*", net,sta,cha,loc);
    }
    *n = 0;
    glob(pat, 0, NULL, &g);
    FREE(pat);
    if(g.gl_pathc <= 0) {
        globfree(&g);
        return NULL;
    }
    *n = g.gl_pathc;
    out = (char **) malloc(sizeof(char *) * (*n));
    for(i = 0; i < (*n); i++) {
        out[i] = strdup(g.gl_pathv[i]);
    }
    globfree(&g);
    return out;
}

/**
 * Attempt to read a polezero file
 *
 * Arguments:
 *   - `pzfile` - Filename of a polezero file to read
 *          if NULL or `*`, the current directory is searched for files
 *   - `id` - Station id string: NET.STAT.LOC.CHAN
 *   - `when` - Reference time: YYYY,DDD,HH:MM:SS
 *
 * Returns:
 *   Polezero file
 *   Error values are found in the nerr field of the polezero struct
 *   nerr = 0 on success
 *
 */
pz_t *
polezero_try_read(char *pzfile, char *id, char *when) {
    char *dir = NULL;
    pz_t *pz = NULL;
    station_id_t stat;
    
    stat.net  = calloc(10, sizeof(char));
    stat.stat = calloc(10, sizeof(char));
    stat.loc  = calloc(10, sizeof(char));
    stat.chan = calloc(10, sizeof(char));

    sac_station_id_split(id, stat.net, stat.stat, stat.loc, stat.chan);
    stat.ref = datetime_parse(when, NULL);

    if(!pzfile || strcmp(pzfile, "*") == 0) {
        int n;
        char **out = NULL;
        if(!(out = pzfiles_find(dir, id, &n))) {
            return NULL;
        }
        for(int i = 0; i < n; i++) {
            if(pz) {
                polezero_free(pz);
            }
            pz = polezero_parse(out[i], &stat);
            if(pz->nerr == 0) {
                break;
            }
        }
        for(int i = 0; i < n; i++) {
            FREE(out[i]);
        }
        FREE(out);
    } else {
        pz = polezero_parse(pzfile, &stat);
    }
    return pz;
}


/**
 * Remove a polezero response from data
 *
 * Arguments:
 *   - `data` - data time series, data is overwritten in return
 *   - `n` - Length of data
 *   - `dt` - Time sampling of data
 *   - `limits` - Frequency Limits over which response is removed
 *   - `id` - Station id string: NET.STAT.LOC.CHAN 
 *          `*` to use the current sac file station id string
 *   - `when` - Reference time: YYYY,DDD,HH:MM:SS
 *          `*` to use the current sac file time
 *   - `pzfile` - Polezero file containing response
 *          `*` to search the current directory for matching responses
 *
 */
int
remove_polezero(float *data, int n, float dt, double limits[4], char *id, char *when, char *pzfile) {

    Response *from;
    pz_t *pz = NULL;

    Transfer *t = transfer_new(n, dt);

    if(strcmp(when, "*") == 0) {
        char when_auto[64];
        sac_reference_time(when_auto);
        when = when_auto;
    }

    // Search for Polezero file it non-specified or wildcard given
    if(!(pz = polezero_try_read(pzfile, id, when))) {
        return -1;
    }
    if(pz->nerr) {
        int nerr = pz->nerr;
        polezero_free(pz);
        return nerr;
    }

    // FROM Response
    from = response_from_polezero(pz, t);
    response_recip(from);

    // TO Response
    Response *to = response_ones(t->nfreqs);

    ztransfer(data, n, (double)dt,
              from->re, from->im,
              to->re, to->im,
              t->nfreqs, t->nfft, t->df, limits);

    for(int i = 0; i < n; i++) {
        data[i] = from->re[i];
    }
    response_free(from);
    response_free(to);
    transfer_free(t);
    return 0;
}


int
remove_polezero_(float *data, int *n, float *dt, double *limits,
                 const char *id, const char *when, const char *pzfile,
                 int id_s,
                 int when_s,
                 int pzfile_s) {


    char *id_c     = fstrdup((char *) id,     id_s);
    char *when_c   = fstrdup((char *) when,   when_s);
    char *pzfile_c = fstrdup((char *) pzfile, pzfile_s);

    int retval =  remove_polezero(data, *n, *dt, limits, id_c, when_c, pzfile_c);

    FREE(id_c);
    FREE(when_c);
    FREE(pzfile_c);

    return retval;
}

int
remove_polezero__(float *data, int *n, float *dt, double *limits,
                  const char *id, const char *when, const char *pzfile,
                  const int id_s,
                  const int when_s,
                  const int pzfile_s) {
    return remove_polezero_(data, n, dt, limits, id, when, pzfile, id_s, when_s, pzfile_s);
}

/**
 * Remove a polezero reseponse
 *
 * Arguments:
 *   - `data` - data time series, data is overwritten in return
 *   - `n` - Length of data
 *   - `dt` - Time sampling of data
 *   - `limits` - Frequency Limits over which response is removed
 *
 * This is equivalent to calling::
 *
 *    remove_polezero(data, n, dt, limits, "*", "*", "*")
 *
 */
int
remove_polezero_simple(float *data, int n, float dt, double limits[4]) {
    return remove_polezero(data, n, dt, limits, "*", "*", "*");
}
int
remove_polezero_simple_(float *data, int *n, float *dt, double *limits) {
    return remove_polezero_simple(data, *n, *dt, limits);
}
int
remove_polezero_simple__(float *data, int *n, float *dt, double *limits) {
    return remove_polezero_simple(data, *n, *dt, limits);
}

/**
 * Comparison functions
 */

#define ATOL 1e-8
#define RTOL 1e-5

/**
 * Determine if values are close
 *
 * Arguments:
 *   - `a` - First value
 *   - `b` - Seconds value
 *   - `atol` - Absolute tolerance
 *   - `rtol` - Relative tolerance
 *
 * Return:
 *   1/True if |a-b| <= atol + rtol * |b|
 *   0/False otherwise
 *
 */
int isclosef_par(float a, float b, float atol, float rtol) {
    return fabs(a-b) <= atol + rtol * fabs(b);
}
int isclosef_par_(float *a, float *b, float *atol, float *rtol) {
    return isclosef_par(*a,*b,*atol,*rtol);
}
int isclosef_par__(float *a, float *b, float *atol, float *rtol) {
    return isclosef_par(*a,*b,*atol,*rtol);
}
/**
 *  Detemrine if values are close
 *
 * Arguments:
 *   - `a` - First value
 *   - `b` - Second value
 *
 * Return:
 *   1/True if |a-b| <= 1e-8 + 1e-5 * |b|
 *   0/False otherwise
 */
int isclosef  (float  a, float  b) { return isclosef_par( a,  b, ATOL, RTOL); }
int isclosef_ (float *a, float *b) { return isclosef_par(*a, *b, ATOL, RTOL); }
int isclosef__(float *a, float *b) { return isclosef_par(*a, *b, ATOL, RTOL); }

/**
 * Detemine if two arrays are close in value
 *
 * Arguments:
 *   - `a` - First array
 *   - `b` - Second array
 *   - `n` - Length of `a` and `b`
 *   - `atol` - Absolute Tolerance
 *   - `rtol` - Relative Tolerance
 *
 * Return:
 *   1/True if |a-b| <= atol + rtol * |b|
 *   0/False otherwise
 *   for all values
 *
 */
int
allclosef_par(float *a, float *b, int n, float atol, float rtol) {
    int i;
    for(i = 0; i < n; i++) {
        if(! isclosef_par(a[i], b[i], atol, rtol)) {
            return 0;
        }
    }
    return 1;
}

/**
 * Detemine if two arrays are close in value
 *
 * Arguments:
 *   - `a` - First array
 *   - `b` - Second array
 *   - `n` - Length of `a` and `b`
 *
 * Return:
 *   1/True if |a-b| <= 1e-8 + 1e-5 * |b|
 *   0/False otherwise
 *   for all values
 *
 */

int allclosef  (float *a, float *b, int  n) { return allclosef_par(a, b,  n, ATOL, RTOL); }
int allclosef_ (float *a, float *b, int *n) { return allclosef_par(a, b, *n, ATOL, RTOL); }
int allclosef__(float *a, float *b, int *n) { return allclosef_par(a, b, *n, ATOL, RTOL); }

sac *sac_read(char *filename, int *nerr);

/**
 * Compare a sac file data to that in memory
 *
 * Arguments:
 *   - `file` - Filename of sac file to compare
 *   - `y` - Data of to compare to sac file
 *   - `n` - Length of `y`
 *   - `b` - Begin value of time
 *   - `dt - Time sampling
 *
 * Return:
 *   1/True if `dt`, `b', `n` and all data points are close in value
 *   0/False othersize
 *
 */
int
sac_compare(char *file, float *y, int n, float b, float dt) {
    int nerr;
    sac *s = sac_read(file, &nerr);
    if(!isclosef(s->h->b, b)) {
        printf("b-value differs: %e %e\n", s->h->b, b);
        return 0;
    }
    if(!isclosef(s->h->delta, dt)) {
        printf("delta differs: %e %e\n", s->h->delta, dt);
        return 0;
    }
    if(n != s->h->npts) {
        printf("npts differs: %d %d\n", s->h->npts, n);
        return 0;
    }
    if(!allclosef(s->y, y, n)) {
        printf("data differs\n");
        return 0;
    }
    return 1;
}

int
sac_compare_(char *file, float *y, int *n, float *b, float *dt, int file_s) {
    char *file_c = fstrdup(file, file_s);
    int retval = sac_compare(file_c, y, *n, *b, *dt);
    FREE(file_c);
    return retval;
}
int
sac_compare__(char *file, float *y, int *n, float *b, float *dt, int file_s) {
    return sac_compare_(file, y, n, b, dt, file_s);
}
