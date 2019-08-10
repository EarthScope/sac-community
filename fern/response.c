
#include <stdio.h>
#include <string.h>
#include "response.h"
#include "request.h"

char *rstrip_fern(char *s);

/**
 * @brief      Initialize a response request
 *
 * @details    Initialize a response request by setting the url to the `sacpz` web service and setting the `nodata` parameter to 404
 *
 * @param      pz  response request
 *
 */
void
response_init(request *pz) {
    request_set_url(pz, "https://service.iris.edu/irisws/sacpz/1/query?");
    request_set_arg(pz, "nodata", arg_int_new(404));
}

/**
 * @brief      Create a new response request
 *
 * @details    Create and initialize a new response request
 *
 * @return     newly created response request
 */
request *
response_new() {
    request *pz = request_new();
    response_init(pz);
    return pz;
}

/**
 * @brief      Change the type of the response requested
 *
 * @details    Set the requested response type to either `sacpz` or `evalresp`. This will set the url to either the `sacpz` or the `resp` web service url.
 *
 * @param      pz   response request
 * @param      rt   response type
 *
 */
void
response_set_kind(request *pz, ResponseType rt) {
    switch(rt) {
    case ResponseSacPZ:
        request_set_url(pz, "https://service.iris.edu/irisws/sacpz/1/query?");
        break;
    case ResponseResp:
        request_set_url(pz, "https://service.iris.edu/irisws/resp/1/query?");
        break;
    }
}

/**
 * @brief      Create a new response request
 *
 * @details    Create a response request from the network, station, location and channel
 *
 * @param      net   Network
 * @param      sta   Station
 * @param      loc   Location
 * @param      cha   Channel
 *
 * @return     newly created response request
 */
request *
response_new_from_nslc(char *net, char *sta, char *loc, char *cha) {
    request *r = response_new();
    response_set_network(r, net);
    response_set_station(r, sta);
    response_set_location(r, loc);
    response_set_channel(r, cha);
    return r;
}

/**
 * @brief      check a character string for wildcards
 *
 * @details    If a stirng contains `*` or `?` or is `--`, return ``, otherwise return the string
 *
 * @private
 *
 * @param      v   character string to check
 *
 * @return     returned input character string if missing wildcards, otherwise a blank character string
 */
static char *
empty_if_wild(char *v) {
    //char out[2] = "";
    if(strchr(v, '*') || strchr(v, '?') || strcmp(v, "--") == 0 ) {
        return "";
    }
    return v;
}

/**
 * @brief      get a filename for the response
 *
 * @details    get a filename for the response in the format of
 *              - SAC_PZs_net_sta_loc_cha_time
 *              - SAC_PZs_net_sta_loc_cha_start_end
 *              - RESP.net.sta.loc.cha
 *
 * @param      pz   response request
 * @param      dst  destination character string
 * @param      n    length of dst
 *
 * @return     return type
 */
char *
response_filename(request *pz, char *dst, size_t n) {
    char *key[] = {"net", "sta", "loc", "cha"};
    Arg *a1 = NULL, *a2 = NULL;
    char tmp[128] = { 0 };;
    const char *url = request_get_url(pz);
    
    if(strstr(url, "sacpz")) {
        snprintf(dst, n, "SAC_PZs_");
        for(size_t i = 0; i < 4; i++) {
            arg_to_string(request_get_arg(pz, key[i]), tmp, sizeof(tmp));
            snprintf(dst, n, "%s%s_", dst, empty_if_wild(tmp));
        }
        if((a1 = request_get_arg(pz, "time"))) {
            snprintf(dst, n, "%s%s", dst, arg_to_string(a1, tmp, sizeof(tmp)));
        } else if((a1 = request_get_arg(pz, "start")) &&
                  (a2 = request_get_arg(pz, "end"))) {
            snprintf(dst, n, "%s%s", dst,
                     arg_to_string(a1, tmp, sizeof(tmp)));
            snprintf(dst, n, "%s_%s", dst,
                     arg_to_string(a2, tmp, sizeof(tmp)));
        }
    } else if(strstr(url, "resp")) {
        snprintf(dst, n, "RESP");
        for(size_t i = 0; i < 4; i++) {
            arg_to_string(request_get_arg(pz, key[i]), tmp, sizeof(tmp));
            snprintf(dst, n, "%s.%s", dst, empty_if_wild(tmp));
        }
    }
    return dst;
}
/**
 * @brief      set the time
 *
 * @details    set the time for the response request
 *
 * @param      s  response request
 * @param      t  timespec64 value
 *
 */
void
response_set_time(request *s, timespec64 t) {
    request_set_arg(s, "time", arg_time_new(t));
}
/**
 * @brief      set the start time
 *
 * @details    set the start time for the response request
 *
 * @param      s  response request
 * @param      t  start timespec64 value
 *
 */
void
response_set_start(request *s, timespec64 t) {
    request_set_arg(s, "start", arg_time_new(t));
}
/**
 * @brief      set the end time
 *
 * @details    set the ent time for the response request
 *
 * @param      s  response request
 * @param      t  end timespec64 value
 *
 */
void
response_set_end(request *s, timespec64 t) {
    request_set_arg(s, "end", arg_time_new(t));
}
/**
 * @brief      set the network
 *
 * @details    set the network for the response request
 *
 * @param      s    response request
 * @param      net  network
 *
 */
void
response_set_network(request *s, char *net) {
    request_set_arg(s, "net", arg_string_new(net));
}
/**
 * @brief      set the station
 *
 * @details    set the station for the response request
 *
 * @param      s    response request
 * @param      sta  station
 *
 */
void
response_set_station(request *s, char *sta) {
    request_set_arg(s, "sta", arg_string_new(sta));
}
/**
 * @brief      set the location
 *
 * @details    set the location for the response request
 *
 * @param      s    response request
 * @param      loc  location
 *
 */
void
response_set_location(request *s, char *loc) {
    request_set_arg(s, "loc", arg_string_new(loc));
}
/**
 * @brief      set the channel
 *
 * @details    set the channel for the response request
 *
 * @param      s    response request
 * @param      cha  channel
 *
 */
void
response_set_channel(request *s, char *cha) {
    request_set_arg(s, "cha", arg_string_new(cha));
}

/**
 * @brief      check if the response is ok
 *
 * @details    check if the response contains network, station, location and channel
 *
 * @param      s   response request to check
 *
 * @return     1 on success, 0 on failure
 */
int
response_is_ok(request *s) {
    char *keys[] = {"net", "sta", "loc", "cha" };
    for(size_t i = 0; i < 4; i++) {
        if(!request_get_arg(s, keys[i])) {
            return 0;
        }
    }
    return 1;
}

