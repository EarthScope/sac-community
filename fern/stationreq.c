/**
 * @file
 * @brief Station Requests
 */
#include "stationreq.h"
#include "request.h"

/**
 * Station Request
 *
 */
struct station_req {};

void station_req_init(request *r);


/**
 * @brief      create a new station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    Create and initialize a new station request, see station_req_init()
 *
 * @return     new station request
 */
request *
station_req_new() {
    request *s = request_new();
    station_req_init(s);
    return s;
}

/**
 * @brief Initialize a station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    Initialize a station request with level = station, xml format and nondata = 404.  
 *             Requests are made to http://service.iris.edu/fdsnws/station/1/query
 *
 * @param      r   station requset to initialize
 *
 */
void
station_req_init(request *r) {
    request_set_url(r, "http://service.iris.edu/fdsnws/station/1/query?");
    request_set_arg(r, "level", arg_string_new("station"));
    request_set_arg(r, "format", arg_string_new("xml"));
    request_set_arg(r, "nodata", arg_int_new(404));
}

/**
 * @brief      set time range for station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    set the start and end times for a station request
 *
 * @param      r      station request
 * @param      start  starting time
 * @param      end    ending time
 *
 */
void
station_req_set_time_range(request *r, timespec64 start, timespec64 end) {
    request_set_arg(r, "start", arg_time_new(start));
    request_set_arg(r, "end", arg_time_new(end));
}

/**
 * @brief      set the network for the station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    use a comma separated list of networks, wildcards ? and * are accepted , along with - for negation
 *
 * @param      r    station request
 * @param      net  network to set
 *
 */
void
station_req_set_network(request *r, char *net) {
    request_set_arg(r, "net", arg_string_new(net));
}
/**
 * @brief      set the station for the station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    use a comma separated list for stations, wildcards ? and * are accepted , along with - for negation
 *
 * @param      r    station request
 * @param      sta  station to set
 *
 */
void
station_req_set_station(request *r, char *sta) {
    request_set_arg(r, "sta", arg_string_new(sta));
}
/**
 * @brief      set the location for the station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    use a comma separated list for locations, wildcards ? and * are accepted , along with - for negation, use -- for blank locations
 *
 * @param      r    station request
 * @param      loc  location to set
 *
 */
void
station_req_set_location(request *r, char *loc) {
    request_set_arg(r, "loc", arg_string_new(loc));
}
/**
 * @brief      set the channel for the station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    use a comma separated list for channels, wildcards ? and * are accepted , along with - for negation
 *
 * @param      r    station request
 * @param      cha  channel to set
 *
 */
void
station_req_set_channel(request *r, char *cha) {
    request_set_arg(r, "cha", arg_string_new(cha));
}

/**
 * @brief      set a rectangular region to search in for the station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    set a rectangular region to search for stations 
 *
 * @param      r      station request
 * @param      minlon minimum longitude, i.e. west
 * @param      maxlon maximum longitude, i.e. east
 * @param      minlat minimum latitude, i.e. south
 * @param      maxlat maximum latitude, i.e. north
 *
 */
void
station_req_set_region(request *r, double minlon, double maxlon,
                       double minlat, double maxlat) {
    request_set_arg(r, "minlon", arg_double_new(minlon));
    request_set_arg(r, "maxlon", arg_double_new(maxlon));
    request_set_arg(r, "minlat", arg_double_new(minlat));
    request_set_arg(r, "maxlat", arg_double_new(maxlat));
}

/**
 * @brief      set the origin for a radial search for the station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    set the origin location for a radial search
 *
 * @param      r      station request
 * @param      lon    longitude
 * @param      lat    latitude
 *
 */
void
station_req_set_origin(request *r, double lon, double lat) {
    request_set_arg(r, "lon", arg_double_new(lon));
    request_set_arg(r, "lat", arg_double_new(lat));
}

/**
 * @brief      set the radius parameters for a radial search for a station request
 *
 * @memberof station_req
 * @ingroup stations
 *
 * @details    set the min and maximum radius for a radial search
 *
 * @param      r     station request
 * @param      minr  minimum radius in degrees
 * @param      maxr  maximum radius in degrees
 *
 */
void
station_req_set_radius(request *r, double minr, double maxr) {
    request_set_arg(r, "minradius", arg_double_new(minr));
    request_set_arg(r, "maxradius", arg_double_new(maxr));
}

