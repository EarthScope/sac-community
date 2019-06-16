
#include "string_utils.h"
#include "octopus.h"

struct _DataReq {
    char *net;
    char *loc;
    char *sta;
    char *cha;
    char quality;
    datetime *start;
    datetime *end;
};

void
data_req_init(DataReq *r) {
    r->net = NULL;
    r->sta = NULL;
    r->cha = NULL;
    r->loc = strdup("*");
    r->quality = 'B';
    r->start = NULL;
    r->end = NULL;
}

DataReq *
data_req_new() {
    DataReq *r = calloc(1, sizeof(DataReq));
    data_req_init(r);
    return r;
}

void
data_req_set_time_range(DataReq *r, datetime *start, datetime *end) {
    r->start = start;
    r->end   = end;
}
void
data_req_set_nslc(DataReq *r, char *net, char *sta, char *loc, char *cha) {
    r->net = strdup(net);
    r->sta = strdup(sta);
    r->loc = strdup(loc);
    r->cha = strdup(cha);
}
void
data_req_set_quality(DataReq *r, Quality quality) {

    switch(quality) {
    case D:
    case Unknown:
        r->quality = 'D'; break;
    case Raw:
        r->quality = 'R'; break;
    case Qual:
    case QC:
        r->quality = 'Q'; break;
    case Modified:
    case Merged:
        r->quality = 'M'; break;
    case Best:
        r->quality = 'B'; break;
    }
}

int
data_req_is_ok(DataReq *r, int need_net_sta) {

    if(!r->cha) {
        return 0;
    }
    if(!r->start || !r->end) {
        return 0;
    }
    if(need_net_sta) {
        if(!r->net || !r->sta) {
            return 0;
        }
    }
    return 1;
}

void
data_req_use_duration(DataReq *r, duration *d) {
    if(!d || !r->start) {
        return;
    }
    datetime_free(r->end);
    r->end = datetime_add_duration(r->start, d);
}


char *
data_req_to_url(DataReq *r, int action) {
    /// Data request would be from
    ///   - Reading a station file and eventid (time) + duration
    ///   - Taking a station search parameter and eventid (time + location)
    ///       - Make the stations request
    ///       - Make the availability request
    ///   - After avail request, then the data request
    ///   - How to restart a data download after stopping?
    ///     - Chunk up request and save
    char tmp[64];
    string out;
    string_init(&out);
    if(action == DOWNLOAD) {
        string_append(&out, "https://service.iris.edu/fdsnws/dataselect/1/query?");
    } else if (action == AVAIL) {
        string_append(&out, "https://service.iris.edu/irisws/availability/1/extent?");
    }
    string_printf_append(&out, "quality=%c&", r->quality);
    if(r->net)   { string_printf_append(&out, "net=%s&", r->net); }
    if(r->sta)   { string_printf_append(&out, "sta=%s&", r->sta); }
    if(r->loc)   { string_printf_append(&out, "loc=%s&", r->loc); }
    if(r->cha)   { string_printf_append(&out, "cha=%s&", r->cha); }
    if(r->start) { string_printf_append(&out, "start=%s", datetime_to_iso8601(r->start, tmp)); }
    if(r->end)   { string_printf_append(&out, "end=%s", datetime_to_iso8601(r->end, tmp)); }

    string_chomp_char(&out, '&');
    return out.str;
}

char *
data_req_from_station_file(DataReq *r, char *file) {
    char tmp1[64], tmp2[64], net[64], sta[64];
    string req;
    FILE *fp = NULL;
    char line[2048] = {0};

    string_init(&req);
    
    if(!(fp = fopen(file, "r"))) {
        printf("Error opening station file for data request: %s\n", file);
        goto error;
    }
    printf("Reading station file: %s\n", file);

    string_printf_append(&req, "format=request\n");
    string_printf_append(&req, "mergequality=true\n");
    string_printf_append(&req, "mergesamplerate=true\n");
    while(fgets(line, sizeof(line), fp)) {
        if(sscanf(line, "%s %s", net, sta) != 2) {
            printf("Error reading station file on line: %s\n", line);
            goto error;
        }
        string_printf_append(&req, "%-5s %-8s %-4s %-5s %s %s\n",
                             net, sta, r->loc, r->cha,
                             datetime_to_iso8601(r->start, tmp1),
                             datetime_to_iso8601(r->end, tmp2));
    }

    return req.str;
 error:
    if(req.str) {
        free(req.str);
        req.str = NULL;
    }
    return NULL;
}
