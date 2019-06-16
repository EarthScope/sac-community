

#ifndef _OCTOPUS_H_
#define _OCTOPUS_H_

#include <libxml/tree.h>

#include "sac_datetime.h"

typedef struct _station_data station_data;
typedef struct _station_meta station_meta;
typedef struct _channel_meta channel_meta;
typedef struct _Event Event;
typedef struct _result result;
typedef struct _sid sid;
typedef struct _sacpz sacpz;
typedef struct _double_opt double_opt;
typedef struct _EventReq EventReq;
typedef struct _StationReq StationReq;
typedef struct _DataReq DataReq;

enum {
    DOWNLOAD = 1,
    AVAIL = 2,
};
typedef enum _Quality Quality;
enum _Quality {
    D        = 1,
    Raw      = 2,
    Qual     = 3,
    Modified = 4,
    Best     = 5,
    Merged   = 6,
    QC       = 7,
    Unknown  = 8,
};

result * request(char *url);
result * request_full(char *url, char *post_data);

result *result_new();
void    result_init(result *r);
char   *result_error_msg(result *r);
int     result_code(result *r);
int     result_http_code(result *r);
char   *result_data(result *r);
size_t  result_len(result *r);
int     result_is_ok(result *r);
result *result_error(int code, char *msg);
char   *result_filename(result *r);
int     result_write_to_file(result *r, char *filename);
int     result_is_empty(result *r);


struct _Event {
    char *eventid;
    datetime *time;
    char *author;
    double evla;
    double evlo;
    double evdp; // in kilometers
    char *catalog;
    double mag;
    char *magtype;
    char *magauthor;
};

void   event_init(Event *e);
Event *event_new();
void   event_print(Event *e, FILE *fp);
void   events_write(Event **ev, FILE *fp);
int    events_write_to_file(Event **ev, char *file);

Event **quake_xml_parse(char *data, size_t data_len, int verbose);

xmlDoc  *xml_init(char *data, size_t ndata);
xmlNode *xml_get_nslc(xmlDoc *doc, xmlNode *from,
                      char *net, char *sta, char *loc, char* cha,
                      datetime *start, datetime *end);
int xml_find_double(xmlDoc *doc, xmlNode *from,
                    const char *path, const char *key, double *value);
int xml_find_datetime(xmlDoc *doc, xmlNode *from,
                      const char *path, const char *key, datetime **t);
int xml_find_attr_string(xmlDoc *doc, xmlNode *from,
                         const char *path, const char *name, char **s);
int xml_find_string(xmlDoc *doc, xmlNode *from, const char *path, const char *key, char **s);

char * data_size(int64_t bytes, char *out, size_t n);

station_meta * station_meta_from_parts(char *net, char *sta,
                                       char *loc, char *cha,
                                       datetime *start, datetime *end);
char * station_meta_to_url(station_meta *sm);
char * station_data_to_string(station_data *s, int show_times);

station_data ** station_data_from_csv(char *data);

int  sid_is_ok(sid *s);
void sid_set(sid *s, char *net, char *sta, char *loc, char *cha);
void sid_init(sid *s);
char *sid_query_string(sid *s, char *dst, size_t n);
char *sid_join(sid *s, char *j, char *dst, size_t n);

sacpz *sacpz_new();
void   sacpz_set_time(sacpz *s, datetime *t);
void   sacpz_set_start(sacpz *s, datetime *t);
void   sacpz_set_end(sacpz *s, datetime *t);
void   sacpz_set_nslc(sacpz *s, char *net, char *sta, char *loc, char *cha);
int    sacpz_nslc_is_ok(sacpz *s);
char  *sacpz_to_url(sacpz *pz, char *kind);
char  *sacpz_filename(sacpz *pz, char *kind);

// Event Requests
EventReq *event_req_new();
void      event_req_set_mag(EventReq *e, double min_mag, double max_mag);
void      event_req_set_time_range(EventReq *e, datetime *start, datetime *end);
void      event_req_set_region(EventReq *e,
                               double minlon, double maxlon,
                               double minlat, double maxlat);
void      event_req_set_depth(EventReq *e, double mindepth, double maxdepth);
void      event_req_set_catalog(EventReq *e, char *catalog);
void      event_req_set_radial(EventReq *e, double lon, double lat,
                               double minr, double maxr);
char    * event_req_to_url(EventReq *e);


StationReq * station_req_new();
char *  station_req_to_url(StationReq *r);
void station_req_set_time_range(StationReq *r, datetime *start, datetime *end);
void station_req_set_nslc(StationReq *r, char *net, char *sta,
                          char *loc, char *cha);
void station_req_set_region(StationReq *r,
                            double minlon, double maxlon,
                            double minlat, double maxlat);
void station_req_set_origin(StationReq *r, double lat, double lon);
void station_req_set_radius(StationReq *r, double minr, double maxr);

DataReq *data_req_new();
char *   data_req_to_url(DataReq *r, int action);
void     data_req_set_time_range(DataReq *r, datetime *start, datetime *end);
void     data_req_set_nslc(DataReq *r, char *net, char *sta, char *loc, char *cha);
void     data_req_set_quality(DataReq *r, Quality quality);
int      data_req_is_ok(DataReq *r, int need_net_sta);
void     data_req_use_duration(DataReq *r, duration *d);
char *   data_req_from_station_file(DataReq *r, char *file);

#endif /* _OCTOPUS_H_ */
