#include <string.h>

#include "string_utils.h"
#include "octopus.h"

struct _StationReq {
    char *net;
    char *sta;
    char *loc;
    char *cha;
    datetime *start;
    datetime *end;
    int box;
    double minlat;
    double maxlat;
    double minlon;
    double maxlon;
    int radius;
    double lat;
    double lon;
    double maxradius;
    double minradius;
    char *level;
    char *format;
    int nodata;
};

void
station_req_init(StationReq *r) {
    r->net = NULL;
    r->sta = NULL;
    r->loc = NULL;
    r->cha = NULL;
    r->start = NULL;
    r->end = NULL;
    r->box = 0;
    r->minlat = 0.0;
    r->maxlat = 0.0;
    r->minlon = 0.0;
    r->maxlon = 0.0;
    r->radius = 0;
    r->lat = 0.0;
    r->lon = 0.0;
    r->minradius = 0.0;
    r->maxradius = 0.0;
    r->level = strdup("station");
    r->format = strdup("xml");
    r->nodata = 404;
}

StationReq *
station_req_new() {
    StationReq *s = calloc(1, sizeof(StationReq));
    station_req_init(s);
    return s;
}

void
station_req_set_time_range(StationReq *r, datetime *start, datetime *end) {
    r->start = start;
    r->end   = end;
}
void
station_req_set_nslc(StationReq *r, char *net, char *sta,
                     char *loc, char *cha) {
    r->net = strdup(net);
    r->sta = strdup(sta);
    r->loc = strdup(loc);
    r->cha = strdup(cha);
}

void
station_req_set_region(StationReq *r, double minlon, double maxlon,
                       double minlat, double maxlat) {
    r->minlon = minlon;
    r->maxlon = maxlon;
    r->minlat = minlat;
    r->maxlat = maxlat;
    r->box = 1;
    r->radius = 0;
}
void
station_req_set_origin(StationReq *r, double lon, double lat) {
    r->lat = lat;
    r->lon = lon;
}
void
station_req_set_radius(StationReq *r, double minr, double maxr) {
    r->minradius = minr;
    r->maxradius = maxr;
    r->radius = 1;
    r->box    = 0;
}



char *
station_req_to_url(StationReq *r) {
    char tmp[64];
    string out;
    string_init(&out);
    string_append(&out, "http://service.iris.edu/fdsnws/station/1/query?");
    if(r->net && strlen(r->net) > 0) {
        string_printf_append(&out, "net=%s&", r->net);
    }
    if(r->sta && strlen(r->sta) > 0) {
        string_printf_append(&out, "sta=%s&", r->sta);
    }
    if(r->loc && strlen(r->loc) > 0) {
        string_printf_append(&out, "loc=%s&", r->loc);
    }
    if(r->cha && strlen(r->cha) > 0) {
        string_printf_append(&out, "cha=%s&", r->cha);
    }
    if(r->start) {
        string_printf_append(&out, "start=%s&",
                             datetime_to_iso8601(r->start, tmp));
    }
    if(r->end) {
        string_printf_append(&out, "end=%s&",
                             datetime_to_iso8601(r->end, tmp));
    }
    if(r->box) {
        string_printf_append(&out,
                             "minlat=%f&maxlat=%f&"
                             "minlon=%f&maxlon=%f&",
                             r->minlat, r->maxlat, r->minlon, r->maxlon);
    } else if(r->radius) {
        string_printf_append(&out,
                             "lat=%f&lon=%f&"
                             "minradius=%f&maxradius=%f&",
                             r->lat, r->lon, r->minradius, r->maxradius);
    }
    if(r->level) {
        string_printf_append(&out, "level=%s&", r->level);
    }
    string_printf_append(&out, "format=%s&", r->format);
    string_printf_append(&out, "nodata=%d", r->nodata);
    return out.str;
}

