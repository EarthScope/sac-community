
#ifndef _STATIONREQ_H_
#define _STATIONREQ_H_

#include <sacio/timespec.h>

#include "request.h"

// Station Requests
request * station_req_new();
void station_req_set_time_range(request *r, timespec64 start, timespec64 end);
void station_req_set_region(request *r,
                            double minlon, double maxlon,
                            double minlat, double maxlat);
void station_req_set_origin(request *r, double lat, double lon);
void station_req_set_radius(request *r, double minr, double maxr);
void station_req_set_network(request *r, char *net);
void station_req_set_station(request *r, char *sta);
void station_req_set_location(request *r, char *loc);
void station_req_set_channel(request *r, char *cha);

#endif /* _STATIONREQ_H_ */
