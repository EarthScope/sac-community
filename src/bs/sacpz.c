
#include "octopus.h"
#include "string_utils.h"

char *rstrip(char *s);

struct _sacpz {
    sid *sid;
    datetime *time;
    datetime *starttime;
    datetime *endtime;
};



void
sacpz_init(sacpz *pz) {
    sid_init(pz->sid);
    pz->time      = NULL;
    pz->starttime = NULL;
    pz->endtime   = NULL;
}

sacpz *
sacpz_alloc() {
    sacpz *pz = malloc(sizeof(sacpz));
    sacpz_init(pz);
    return pz;
}

sacpz *
sacpz_new() {
    sacpz *s = sacpz_alloc();
    sacpz_init(s);
    return s;
}

sacpz *
sacpz_new_from_nslc(char *net, char *stat, char *loc, char *chan) {
    sacpz *pz = sacpz_alloc();
    sid_set(pz->sid, net, stat, loc, chan);
    return pz;
}

static char *
datetime_to_sacpz_time(datetime *t, char *dst) {
    int ms = t->nanosecond / 1000000;
    sprintf(dst, "%04d.%03d.%02d.%02d.%02d.%04d",
            t->year,
            t->doy,
            t->hour,
            t->minute,
            t->second,
            ms);
    return dst;
}


char *
sacpz_to_url(sacpz *pz, char *kind) {

    char tmp[256];
    string out;
    string_init(&out);

    string_printf(&out, "https://service.iris.edu/irisws/%s/1/query?%s",
                  rstrip(kind),
                  sid_query_string(pz->sid, tmp, sizeof(tmp)));
    if(pz->time) {
        datetime_to_iso8601(pz->time, tmp);
        string_printf_append(&out, "&time=%s", tmp);
    } else {
        if(pz->starttime) {
            //fprintf(stderr, "ADDING START TIME\n");
            datetime_to_iso8601(pz->starttime, tmp);
            string_printf_append(&out, "&starttime=%s", tmp);
        }
        if(pz->endtime) {
            //fprintf(stderr, "ADDING END TIME\n");
            datetime_to_iso8601(pz->endtime, tmp);
            string_printf_append(&out, "&endtime=%s", tmp);
        }
    }
    return out.str;
}

char *
sacpz_filename(sacpz *pz, char *kind) {
    char tmp[256];
    string out;
    string_init(&out);
    if(strcmp(kind, "sacpz") == 0) {
        string_printf(&out, "SAC_PZs_%s", sid_join(pz->sid, "_", tmp, sizeof(tmp)));
    } else if(strcmp(kind, "resp") == 0) {
        string_printf(&out, "RESP.%s", sid_join(pz->sid, ".", tmp, sizeof(tmp)));
    } else {
        printf("Unknown response type: '%s', expected sacpz or resp\n", kind);
        return NULL;
    }
    if(pz->time) {
        string_printf_append(&out, "_%s", datetime_to_sacpz_time(pz->time, tmp));
    } else if(pz->starttime && pz->endtime) {
        string_printf_append(&out, "_%s", datetime_to_sacpz_time(pz->starttime, tmp));
        string_printf_append(&out, "_%s", datetime_to_sacpz_time(pz->endtime, tmp));
    }
    return out.str;
}

void
sacpz_set_time(sacpz *s, datetime *t) {
    s->time = t;
}
void
sacpz_set_start(sacpz *s, datetime *t) {
    s->starttime = t;
}
void
sacpz_set_end(sacpz *s, datetime *t) {
    s->endtime = t;
}
void
sacpz_set_nslc(sacpz *s, char *net, char *sta, char *loc, char *cha) {
    sid_set(s->sid, net, sta, loc, cha);
}
int
sacpz_nslc_is_ok(sacpz *s) {
    return sid_is_ok(s->sid);
}

