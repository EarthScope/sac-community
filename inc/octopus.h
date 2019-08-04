

#ifndef _OCTOPUS_H_
#define _OCTOPUS_H_

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <libmseed/libmseed.h>
#include <fern/fern.h>

typedef enum _ResponseType ResponseType;
enum _ResponseType {
    ResponseSacPZ = 1,
    ResponseResp = 2,
};


Event **quake_xml_parse(char *data, size_t data_len, int verbose, char *cat);

// SACPZ
request *sacpz_new();
void   sacpz_set_time(request *s, timespec64 t);
void   sacpz_set_start(request *s, timespec64 t);
void   sacpz_set_end(request *s, timespec64 t);
void   sacpz_set_network(request *s, char *net);
void   sacpz_set_station(request *s, char *sta);
void   sacpz_set_location(request *s, char *loc);
void   sacpz_set_channel(request *s, char *cha);
void   sacpz_set_kind(request *s, ResponseType kind);
int    sacpz_is_ok(request *s);
char  *sacpz_filename(request *pz, char *dst, size_t n);




void result_write_to_file_show(result *r, char *file);


#endif /* _OCTOPUS_H_ */
