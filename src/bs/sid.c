
#include <stdlib.h>
#include <string.h>

#include "octopus.h"

char *rstrip(char *b);

struct _sid {
    char network[9];
    char station[9];
    char location[9];
    char channel[9];
};


void
sid_init(sid *s) {
    memset(s->network,  0, sizeof(s->network));
    memset(s->station,  0, sizeof(s->station));
    memset(s->location, 0, sizeof(s->location));
    memset(s->channel,  0, sizeof(s->channel));
}

static void
strset(char *dst, size_t n, char *src) {
    memset(dst, 0, n);
    if(src) {
        strlcpy(dst, src, n);
        rstrip(dst);
        if(strlen(dst) == 0) {
            strlcpy(dst, "--", n);
        }
    }
}


void
sid_set(sid *s, char *net, char *sta, char *loc, char *cha) {
    strset(s->network, sizeof(s->network), net);
    strset(s->station, sizeof(s->station), sta);
    strset(s->location, sizeof(s->location), loc);
    strset(s->channel, sizeof(s->channel), cha);
}

int
sid_is_ok(sid *s) {
    return
        strlen(s->network)  > 0 &&
        strlen(s->station)  > 0 &&
        strlen(s->channel)  > 0 &&
        strlen(s->location) > 0;
}

char *
sid_query_string(sid *s, char *tmp, size_t n) {
    if(strlen(s->network) > 0) {
        snprintf(tmp, n, "%snet=%s&", tmp, s->network);
    }
    if(strlen(s->station) > 0) {
        snprintf(tmp, n, "%ssta=%s&", tmp, s->station);
    }
    if(strlen(s->location) > 0) {
        snprintf(tmp, n, "%sloc=%s&", tmp, s->location);
    }
    if(strlen(s->channel) > 0) {
        snprintf(tmp, n, "%scha=%s&", tmp, s->channel);
    }
    size_t k = strlen(tmp);
    if(tmp[k-1] == '&') {
        tmp[k-1] = 0;
    }
    return tmp;
}

static char *
empty_if_wild(char *v) {
    //char out[2] = "";
    if(strchr(v, '*') || strchr(v, '?')) {
        return "";
    }
    return v;
}

char *
sid_join(sid *s, char *j, char *dst, size_t n) {
    snprintf(dst, n,
             "%s%s"
             "%s%s"
             "%s%s"
             "%s",
             empty_if_wild(s->network), j,
             empty_if_wild(s->station), j,
             empty_if_wild(s->channel), j,
             empty_if_wild(s->location));
    return dst;
}
