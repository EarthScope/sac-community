
#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdio.h>

#include "request.h"

#define EVENT_ORIGIN_LEN 16
#define EVENT_MAG_LEN 16
#define EVENTID_LEN 64

typedef struct Event Event;

void       event_init(Event *e);
Event    * event_new();
void       event_free(Event *e);
void       event_print(Event *e, FILE *fp);
void       events_write(Event **ev, FILE *fp);
int        events_write_to_file(Event **ev, char *file);
char     * event_id(Event *e);
timespec64 event_time(Event *e);
double     event_lat(Event *e);
double     event_lon(Event *e);
double     event_depth(Event *e);

void       event_set_mag(Event *e, double mag);
void       event_set_magtype(Event *e, char *type);
void       event_set_magauthor(Event *e, char *author);
void       event_set_time(Event *e, timespec64 *t);
void       event_set_latitude(Event *e, double lat);
void       event_set_longitude(Event *e, double lon);
void       event_set_depth(Event *e, double depth);
void       event_set_author(Event *e, char *author);
void       event_set_catalog(Event *e, char *catalog);
void       event_set_id(Event *e, char *id);

void       event_default(Event *e);

Event   ** event_from_json(char *data, size_t datalen, int verbose, char *catalog);

Event *    event_from_id(char *str);
Event *    event_find(char *id);
int        event_exists(Event *e);
void       event_save(Event *e);
Event *    event_by_event_id(char *id);


// Event requests
request *event_req_new();
void     event_req_set_mag(request *e, double min_mag, double max_mag);
void     event_req_set_time_range(request *e, timespec64 start, timespec64 end);
void     event_req_set_region(request *e,
                              double minlon, double maxlon,
                              double minlat, double maxlat);
void     event_req_set_depth(request *e, double mindepth, double maxdepth);
void     event_req_set_catalog(request *e, char *catalog);
void     event_req_set_radial(request *e, double lon, double lat,
                              double minr, double maxr);
void     event_req_set_eventid(request *e, char *id);


Event **quake_xml_parse(char *data, size_t data_len, int verbose, char *cat);

#endif /* _EVENT_H_ */
