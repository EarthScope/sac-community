
#include <stdlib.h>

#include "string_utils.h"
#include "sac_datetime.h"
#include "array.h"
#include "csv.h"
#include "octopus.h"
#include "meta.h"

struct _station_meta {
    sid *sid;
    datetime *starttime;
    datetime *endtime;
    char level[32];
    char format[32];
};


csv channel_csv = {
    .delim = "|",
    .data_size = sizeof(channel_meta),
    .nfields = 17,
    .fields = {
        {"Network",           csv_string,   offsetof(channel_meta, net)},
        {"Station",           csv_string,   offsetof(channel_meta, sta)},
        {"Location",          csv_string,   offsetof(channel_meta, loc)},
        {"Channel",           csv_string,   offsetof(channel_meta, cha)},

        {"Latitude",          csv_float,    offsetof(channel_meta, stla)},
        {"Longitude",         csv_float,    offsetof(channel_meta, stlo)},
        {"Elevation",         csv_float,    offsetof(channel_meta, stel)},
        {"Depth",             csv_float,    offsetof(channel_meta, stdp)},
        {"Azimuth",           csv_float,    offsetof(channel_meta, cmpaz)},
        {"Dip",               csv_float,    offsetof(channel_meta, cmpinc)},

        {"SensorDescription", csv_string,   offsetof(channel_meta, desc)},
        {"Scale",             csv_float,    offsetof(channel_meta, scale)},
        {"ScaleFreq",         csv_float,    offsetof(channel_meta, scalefreq)},
        {"ScaleUnits",        csv_string,   offsetof(channel_meta, scaleunits)},
        {"SampleRate",        csv_float,    offsetof(channel_meta, samprate)},
        {"StartTime",         csv_datetime, offsetof(channel_meta, starttime)},
        {"EndTime",           csv_datetime, offsetof(channel_meta, endtime)},
    }
};

#define CHANNEL_META_LEN (sizeof(channel_csv)/sizeof(csv))


csv station_csv = {
    .delim = "|",
    .data_size = sizeof(station_data),
    .nfields = 8,
    .fields = {
        {"Network",   csv_string_p, offsetof(station_data, net)},
        {"Station",   csv_string_p, offsetof(station_data, sta)},
        {"Latitude",  csv_double,   offsetof(station_data, stla)},
        {"Longitude", csv_double,   offsetof(station_data, stlo)},
        {"Elevation", csv_double,   offsetof(station_data, stel)},
        {"SiteName",  csv_string_p, offsetof(station_data, sitename)},
        {"StartTime", csv_datetime, offsetof(station_data, start)},
        {"EndTime",   csv_datetime, offsetof(station_data, end)},
    }
};
void
channel_meta_print(channel_meta *m, csv* csv, int header) {
    char tmp[64];
    if(header) {
        printf("#");
        for(size_t i = 0; i < csv->nfields; i++) {
            printf("%s", csv->fields[i].key);
            if(i+1 != CHANNEL_META_LEN) {
                printf(" | ");
            }
        }
        printf("\n");
    }

    printf("%s|", m->net);
    printf("%s|", m->sta);
    printf("%s|", m->loc);
    printf("%s|", m->cha);
    printf("%f|", m->stla);
    printf("%f|", m->stlo);
    printf("%f|", m->stel);
    printf("%f|", m->stdp);
    printf("%f|", m->cmpaz);
    printf("%f|", m->cmpinc);
    printf("%s|", m->desc);
    printf("%e|", m->scale);
    printf("%f|", m->scalefreq);
    printf("%s|", m->scaleunits);
    printf("%f|", m->samprate);
    printf("%s|", datetime_to_iso8601(&m->starttime, tmp));
    printf("%s", datetime_to_iso8601(&m->endtime, tmp));
    printf("\n");
}

channel_meta **
channel_parse(char *data) {
    channel_meta **m = (channel_meta **) csv_parse(data, &channel_csv);
    for(size_t i = 0; i < xarray_length(m); i++) {
        channel_meta_print(m[i], &channel_csv, i == 0);
    }
    return m;
}

char *
station_meta_to_url(station_meta *sm) {
    char tmp[512] = { 0 };
    string out;
    string_init(&out);
    string_printf(&out, "https://service.iris.edu/fdsnws/station/1/query?");
    string_printf_append(&out, "%s", sid_query_string(sm->sid, tmp, sizeof(tmp)));
    string_printf_append(&out, "&starttime=%s", datetime_to_iso8601(sm->starttime, tmp));
    string_printf_append(&out, "&endtime=%s", datetime_to_iso8601(sm->endtime, tmp));
    string_printf_append(&out, "&format=%s", sm->format);
    string_printf_append(&out, "&level=%s", sm->level);
    return out.str;
}

void
station_data_init(station_data *s) {
    s->net = NULL;
    s->sta = NULL;
    s->stla = 0.0;
    s->stlo = 0.0;
    s->stel = 0.0;
    s->sitename = NULL;
    s->start = NULL;
    s->end = NULL;
}

station_data *
station_data_new() {
    station_data *s = calloc(1, sizeof(station_data));
    station_data_init(s);
    return s;
}

char *
station_data_to_string(station_data *s, int show_times) {
    char *tmp = NULL;
    char tmp1[64], tmp2[64];
    if(show_times) {
        asprintf(&tmp,
                 "%2s %5s %8.4f %9.4f %7.2f %10s %10s %s",
                 s->net, s->sta,
                 s->stla, s->stlo, s->stel,
                 datetime_to_iso8601(s->start, tmp1),
                 datetime_to_iso8601(s->end, tmp2),
                 s->sitename);
    } else {
        asprintf(&tmp,
                 "%2s %5s %8.4f %9.4f %7.2f %s",
                 s->net, s->sta,
                 s->stla, s->stlo, s->stel,
                 s->sitename);
    }
    return tmp;
}

void
station_meta_init(station_meta *sm) {
    sid_init(sm->sid);
    sm->starttime = NULL;
    sm->endtime = NULL;
    memset(sm->level, 0, sizeof(sm->level));
    memset(sm->format, 0, sizeof(sm->format));
}

station_meta *
station_meta_new() {
    station_meta *sm = calloc(1, sizeof(station_meta));
    station_meta_init(sm);
    return sm;
}

station_meta *
station_meta_from_parts(char *net, char *sta, char *loc, char *cha,
                        datetime *start, datetime *end) {
    station_meta *sm = station_meta_new();
    sid_set(sm->sid, net, sta, loc, cha);
    sm->starttime = start;
    sm->endtime   = end;
    strcpy(sm->level, "channel");
    strcpy(sm->format, "xml");
    return sm;
}

station_data **
station_data_from_csv(char *data) {
    return (station_data **) csv_parse(data, &station_csv);
}

