
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "octopus.h"

struct _double_opt {
    double value;
    uint8_t set;
};

double_opt
double_opt_none() {
    double_opt d;
    d.value = 0.0;
    d.set = FALSE;
    return d;
}
double_opt
double_opt_some(double v) {
    double_opt d;
    d.value = v;
    d.set = TRUE;
    return d;
}

void
double_opt_append(string *s, char *key, double_opt *d) {
    if(d->set) {
        string_printf_append(s, "%s=%f&", key, d->value);
    }
}


struct _EventReq {
    double_opt minlon;
    double_opt maxlon;
    double_opt minlat;
    double_opt maxlat;
    double_opt minmag;
    double_opt maxmag;
    double_opt lat;
    double_opt lon;
    double_opt minradius;
    double_opt maxradius;
    double_opt mindepth;
    double_opt maxdepth;
    char catalog[16];
    datetime *start;
    datetime *end;
};

void
event_req_init(EventReq *e) {
    e->minlon = double_opt_none();
    e->maxlon = double_opt_none();
    e->minlat = double_opt_none();
    e->maxlat = double_opt_none();
    e->minmag = double_opt_none();
    e->maxmag = double_opt_none();
    e->mindepth = double_opt_none();
    e->maxdepth = double_opt_none();
    memset(e->catalog, 0, 16);
    e->start = NULL;
    e->end = NULL;
}

EventReq *
event_req_new() {
    EventReq *e = calloc(1, sizeof(EventReq));
    event_req_init(e);
    return e;
}

void
event_req_set_mag(EventReq *e, double min_mag, double max_mag) {
    e->minmag = double_opt_some(min_mag);
    e->maxmag = double_opt_some(max_mag);
}
void
event_req_set_time_range(EventReq *e, datetime *start, datetime *end) {
    e->start = start;
    e->end   = end;
}
void
event_req_set_depth(EventReq *e, double mindepth, double maxdepth) {
    e->mindepth = double_opt_some(mindepth);
    e->maxdepth = double_opt_some(maxdepth);
}
void
event_req_set_catalog(EventReq *e, char *catalog) {
    strlcpy(e->catalog, catalog, sizeof(e->catalog));
}
void
event_req_set_radial(EventReq *e, double lon, double lat,
                     double minr, double maxr) {
    e->lon = double_opt_some(lon);
    e->lat = double_opt_some(lat);
    e->minradius = double_opt_some(minr);
    e->maxradius = double_opt_some(maxr);
}
void
event_req_set_region(EventReq *e,
                     double minlon, double maxlon,
                     double minlat, double maxlat) {
    e->minlon = double_opt_some(minlon);
    e->maxlon = double_opt_some(maxlon);
    e->minlat = double_opt_some(minlat);
    e->maxlat = double_opt_some(maxlat);
}
char *
event_req_to_url(EventReq *e) {
    char tmp[64];
    string out;
    string_init(&out);

    string_append(&out, "https://service.iris.edu/fdsnws/event/1/query?");

    if(e->start) {
        string_printf_append(&out, "starttime=%s&",
                             datetime_to_iso8601(e->start, tmp));
    }
    if(e->end) {
        string_printf_append(&out, "endtime=%s&",
                             datetime_to_iso8601(e->end, tmp));
    }
    if(strlen(e->catalog) > 0) {
        string_printf_append(&out, "catalog=%s&", e->catalog);
    }
    double_opt_append(&out, "mindepth", &e->mindepth);
    double_opt_append(&out, "maxdepth", &e->maxdepth);
    double_opt_append(&out, "minmag",   &e->minmag);
    double_opt_append(&out, "maxmag",   &e->maxmag);
    double_opt_append(&out, "minlon",   &e->minlon);
    double_opt_append(&out, "maxlon",   &e->maxlon);
    double_opt_append(&out, "minlat",   &e->minlat);
    double_opt_append(&out, "maxlat",   &e->maxlat);

    double_opt_append(&out, "lon",       &e->lon);
    double_opt_append(&out, "lat",       &e->lat);
    double_opt_append(&out, "minradius", &e->minradius);
    double_opt_append(&out, "maxradius", &e->maxradius);

    string_printf_append(&out, "nodata=404");

    return out.str;
}
