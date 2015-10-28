

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "icm.h"
#include "ucf.h"
#include "bot.h"
#include "sac_datetime.h"
#include "debug.h"
#include "SACHeader.h"

#ifdef WIN32
#define pointer char *
#else
#define pointer void *
#endif

#define KEY_CONSTANT  "CONSTANT"
#define KEY_ZEROS     "ZEROS"
#define KEY_POLES     "POLES"
#define KEY_STAR      '*'

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

int
is_numexp(char c) {
    return (isdigit(c) || c == '.' || c == '+' || c == '-' || c == 'e' ||
            c == 'E' || c == 'd' || c == 'D');
}

void
polezero_comment_float(char *p, pzmeta_t * meta, pzcomment_t * c) {
    char *pp;
    char *s;
    int nerr;
    float *f;
    nerr = 1;
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

station_id_t *
station_id_new(char *net, char *stat, char *loc, char *chan,
               int year, int doy, int hour, int min, int sec, int msec) {
    station_id_t *s;
    s = (station_id_t *) malloc(sizeof(station_id_t));
    if(!s) {
        return NULL;
    }
    s->net  = strdup(net);
    s->stat = strdup(stat);
    s->loc  = strdup(loc);
    s->chan = strdup(chan);
    s->ref  = datetime_new();
    datetime_set_year(s->ref, year);
    datetime_set_doy(s->ref, doy);
    datetime_set_hour(s->ref, hour);
    datetime_set_minute(s->ref, min);
    datetime_set_second(s->ref, sec);
    datetime_set_nanosecond(s->ref, msec*1e6);
    return s;
}

station_id_t *
station_id_from_sac(sac *s) {
    return station_id_new(s->h->knetwk, s->h->kstnm,
                          s->h->khole, s->h->kcmpnm,
                          s->h->nzyear, s->h->nzjday,
                          s->h->nzhour, s->h->nzmin,
                          s->h->nzsec, s->h->nzmsec);
}

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

/*
 * Find the first occurrence of find in s, ignore case.
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


int
sdef(char *s) {
    return (s && strlen(s) > 0);
}

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
                return pz;
            }
            meta_used = polezero_meta_copy(meta);
        } else if(startswith(kline, KEY_POLES, TRUE)) {
            if(sscanf(kline + strlen(KEY_POLES), "%d", &pz->npole) != 1 || pz->npole < 0) {
                pz->nerr = 2108;
                pz->line = strdup(kiline);
                return pz;
            }
            state = POLES;
            pz->poles = (complexd *) calloc(pz->npole, sizeof(complexd));
        } else if(startswith(kline, KEY_ZEROS, TRUE)) {
            if(sscanf(kline + strlen(KEY_ZEROS), "%d", &pz->nzero) != 1 || pz->nzero < 0) {
                pz->nerr = 2109;
                pz->line = strdup(kiline);
                return pz;
            }
            state = ZEROS;
            pz->zeros = (complexd *) calloc(pz->nzero, sizeof(complexd));
        } else if(state == POLES || state == ZEROS) {
            double re, im;
            if(sscanf(kline, "%lg %lg", &re, &im) != 2) {
                pz->nerr = (state == POLES) ? 2126 : 2127;
                pz->line = strdup(kiline);
                return pz;
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
            return pz;
        }
    }
    if(!feof(fp)) {
        pz->nerr = 114;
    }
    return pz;
}
