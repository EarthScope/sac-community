/**
 * @file
 * @brief event data and event requests
 */
/**
 * @defgroup events events
 *
 * @brief Event data and requests
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sacio/timespec.h>

#include "array.h"
#include "event.h"
#include "cprint.h"
#include "request.h"

#include "json.h"
#include "chash.h"
#include "xml.h"
#include "defs.h"
#include "strip.h"
#include "urls.h"

Event **quake_xml_parse(char *data, size_t data_len, int verbose, char *cat);

/**
 * Event request
 */
struct event_req {};

/**
 * @brief Event data
 * @ingroup events
 */
struct Event {
    char eventid[EVENTID_LEN];     /**< @private eventid */
    timespec64 time;/**< @private origin time */
    char author[EVENT_ORIGIN_LEN];/**< @private author */
    double evla;/**< @private origin latitude  */
    double evlo;/**< @private origin longitude */
    double evdp;  /**< @private depth in kilometers */
    char catalog[EVENT_ORIGIN_LEN]; /**< @private catalog */
    double mag; /**< @private  magnitude */
    char magtype[EVENT_MAG_LEN]; /**< @private  magnitude type */
    char magauthor[EVENT_MAG_LEN]; /**< @private  magnitude author */
};

/**
 * Create a new Event
 *
 * @memberof Event
 * @ingroup events
 *
 * @return new event
 *
 * @warning User owns the event and is responsbile for freeing the underlying memory
 */
Event *
event_new() {
    Event *e = malloc(sizeof(Event));
    event_init(e);
    return e;
}

/**
 * Initialize an event
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 *
 * @note All values are set to 0 and characters strings are truncated
 *
 */
void
event_init(Event *e) {
    e->eventid[0] = 0;
    e->time.tv_sec = 0;
    e->time.tv_nsec = 0;
    e->evla      = 0.0;
    e->evlo      = 0.0;
    e->evdp      = 0.0;
    e->mag       = 0.0;
    e->catalog[0] = 0;
    e->author[0] = 0;
    e->magtype[0] = 0;
    e->magauthor[0] = 0;;
}

/**
 * Find an event if it exists from the id
 *
 * @memberof Event
 * @ingroup events
 *
 * @param    str  eventid
 *
 * @return Event if it was found, NULL on error
 *
 * @note If the event is not found, it will be requested
 *
 * @note This is the same as \ref event_find with a catalog checking
 *
 */
Event *
event_from_id(char *str) {
    Event *e = NULL;
    if(!str || *str == 0) {
        return NULL;
    }
    // Look for event id
    if(strncasecmp("usgs:", str,5) != 0 &&
       strncasecmp("gcmt:", str,5) != 0 &&
       strncasecmp("isc:", str,4) != 0) {
        return NULL;
    }
    if(!(e = event_find(str))) {
        return NULL;
    }
    return e;
}
/**
 * Print a single event
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e   event
 * @param fp  file pointer to print to, use stdout for normal printing
 *
 */
void
event_print(Event *e, FILE *fp) {
    char tmp[256];
    strftime64t(tmp, sizeof(tmp), "%FT%T", &e->time);
    fprintf(fp, "%19s %6.2f %7.2f %6.2f %4.2f ",
            tmp,
            e->evla, e->evlo, e->evdp,
            e->mag);
    fprintf(fp, "%-3s ", e->magtype);
    fprintf(fp, "%s/",   e->author);
    fprintf(fp, "%s ",   e->magauthor);
    fprintf(fp, "%s ",   e->catalog); // REMOVE THIS
    fprintf(fp, "%s\n",  e->eventid);
}

/**
 * Print an event collection
 *
 * @memberof Event
 * @ingroup events
 *
 * @param ev   events
 * @param fp   file pointer to print to, use stdout for normal printing
 *
 * @warning ev is expected to be an \ref xarray
 */
void
events_write(Event **ev, FILE *fp) {
    cfprintf(fp, "bold,black", "%-19s %-6s %-7s %-6s %-4s %-3s %s %s\n",
            "Origin", "Lat.", "Lon.", "Depth", "Mag.","","Agency", "EventID");
    for(size_t i = 0; i < xarray_length(ev); i++) {
        event_print(ev[i], fp);
    }
}
/**
 * Print an event collection to a file
 *
 * @memberof Event
 * @ingroup events
 *
 * @param ev    events
 * @param file  filename to write to
 *
 * @warning ev is expected to be an \ref xarray
 */
int
events_write_to_file(Event **ev, char *file) {
    FILE *fp;
    if(!(fp = fopen(file, "w"))) {
        printf("Error open file for writing: %s\n", file);
        return 0;
    }
    events_write(ev, fp);
    fclose(fp);
    return 1;
}

/**
 * Free an event
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e  event
 *
 */
void
event_free(Event *e) {
    FREE(e);
}


/**
 * Get an event's id
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 *
 * @return Event id
 *
 * @warning Value is owned by the event and the data must not be modified
 */
char *
event_id(Event *e) {
    if(!e) {
        return NULL;
    }
    return e->eventid;
}
/**
 * Get the events origin time
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 *
 * @return Event origin time
 *
 */
timespec64
event_time(Event *e) {
    if(!e) {
        timespec64 t = {0,0};
        return t;
    }
    return e->time;
}


/**
 * Get the event origin latitude
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 *
 * @return event latitude
 */
double
event_lat(Event *e) {
    if(e) {
        return e->evla;
    }
    return 0.0;
}
/**
 * Get the event origin longitude
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 *
 * @return event longitude
 */
double
event_lon(Event *e) {
    if(e) {
        return e->evlo;
    }
    return 0.0;
}
/**
 * Get the event origin depth
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 *
 * @return event depth
 */
double
event_depth(Event *e) {
    if(e) {
        return e->evdp;
    }
    return 0.0;
}
/**
 * Set the event origin time
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e  event
 * @param id new event id
 *
 * @param id value is copied
 *
 */
void
event_set_id(Event *e, char *id) {
    if(e) {
        fern_strlcpy(e->eventid, id, sizeof(e->eventid));
    }
}

/**
 * Set the event origin time
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e event
 * @param t timespec64 origin time
 *
 * @note timespec64 value is copied
 */
void
event_set_time(Event *e, timespec64 *t) {
    e->time = *t;
}

/**
 * Set the event origin magnitude
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e    event
 * @param mag  magnitude value
 *
 */
void
event_set_mag(Event *e, double mag) {
    if(e) {
        e->mag = mag;
    }
}
/**
 * Set the event origin magnitude type
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e     event
 * @param type  magnitude type
 *
 * @note magntiude type is copied
 */
void
event_set_magtype(Event *e, char *type) {
    if(e) {
        fern_strlcpy(e->magtype, type, sizeof(e->magtype));
    }
}
/**
 * Set the event origin magnitude author
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e       event
 * @param author  magnitude author
 *
 * @note magntiude author is copied
 */
void
event_set_magauthor(Event *e, char *author) {
    if(e) {
        fern_strlcpy(e->magauthor, author, sizeof(e->magauthor));
    }
}
/**
 * Set the event origin latitude
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e    event
 * @param lat  event latitude
 *
 */
void
event_set_latitude(Event *e, double lat) {
    if(e) {
        e->evla = lat;
    }
}
/**
 * Set the event origin longitude
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e    event
 * @param lon  event longitude
 *
 */
void
event_set_longitude(Event *e, double lon) {
    if(e) {
        e->evlo = lon;
    }
}
/**
 * Set the event origin depth, value in kilometers
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e     event
 * @param depth event depth
 *
 */
void
event_set_depth(Event *e, double depth) {
    if(e) {
        e->evdp = depth;
    }
}
/**
 * Set the event author
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e       event
 * @param author  author
 *
 * @note author value is copied
 */
void
event_set_author(Event *e, char *author) {
    if(e) {
        fern_strlcpy(e->author, author, sizeof(e->author));
    }
}
/**
 * Set the event catalog
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e         event
 * @param catalog   catalog
 *
 * @note author catalog is copied
 */
void
event_set_catalog(Event *e, char *catalog) {
    if(e) {
        fern_strlcpy(e->catalog, catalog, sizeof(e->catalog));
    }
}

/**
 * @brief Find and parse a time value in a json file
 *
 * @ingroup json
 * @memberof json
 *
 * @param root   json document
 * @param t      timevalue to place value into
 * @param ...    path of keys, ending in NULL
 *
 * @return 1 on success, 0 on failure
 *
 */
int
cjson_time64(const cJSON *root, timespec64 *t, ...) {
    const cJSON *cur = NULL;
    va_list ap;
    va_start(ap, t);
    cur = cjson_path_internal(root, ap);
    va_end(ap);
    if(!cur) {
        return 0;
    }
    if(!cJSON_IsString(cur)) {
        return 0;
    }
    if(! timespec64_parse(cur->valuestring, t)) {
        return 0;
    }
    return 1;
}

/**
 * Parse a set of events from json data
 *
 * @memberof Event
 * @ingroup events
 *
 * @param data     json data
 * @param datalen  length of data
 * @param verbose  be verbose in parsing
 * @param catalog  catalog to prepend to any eventid
 *
 * @return events "enclosed" in an xarray
 *
 */
Event **
event_from_json(char *data, size_t datalen, int verbose, char *catalog) {
    UNUSED(datalen);
    UNUSED(verbose);
    cJSON *json = NULL;
    Event **ev = NULL;
    Event *e = NULL;
    const cJSON *org = NULL;
    const cJSON *orgp = NULL;
    const cJSON *p = NULL;
    int weight  = 0;
    int weightp = 0;
    if(!(json = cJSON_Parse(data))) {
        goto done;
    }
    const cJSON *orgs = cjson_path(json, "properties", "products", "origin", NULL);
    // Find Preferred Origin based on Weight
    cJSON_ArrayForEach(org, orgs) {
        cjson_int(org, &weight, "preferredWeight", NULL);
        if(orgp == NULL) {
            orgp = org;
            weightp = weight;
        } else {
            if(weight > weightp) {
                orgp = org;
                weightp = weight;
            }
        }
    }
    // Fill Event
    e = event_new();
    cjson_string(orgp, e->eventid, EVENTID_LEN, "code", NULL);
    p = cjson_path(orgp, "properties", NULL);
    cjson_string(p, e->author, EVENT_ORIGIN_LEN, "origin-source", NULL);
    cjson_string(p, e->catalog, EVENT_ORIGIN_LEN, "origin-source", NULL);
    cjson_double(p, &e->evla, "latitude", NULL);
    cjson_double(p, &e->evlo, "longitude", NULL);
    cjson_double(p, &e->evdp, "depth", NULL);
    cjson_double(p, &e->mag, "magnitude", NULL);
    cjson_string(p, e->magtype, EVENT_MAG_LEN,  "magnitude-type", NULL);
    cjson_string(p, e->magauthor, EVENT_MAG_LEN, "magnitude-source", NULL);
    cjson_time64(p, &e->time, "eventtime", NULL);

    if(strlen(e->eventid) > 0) {
        char tmp[2*EVENTID_LEN];
        sprintf(tmp, "%s:%s", catalog, e->eventid);
        event_set_id(e, tmp);
    }
    ev = xarray_new_with_len('p', 1);
    ev[0] = e;
 done:
    cJSON_Delete(json);

    return ev;
}

/**
 * Get an Event from an eventid
 *
 * @memberof Event
 * @ingroup events
 *
 * @param id  event id
 *
 * @return Event associated with eventid, NULL on failure
 *
 * @note You probably want event_from_id() which will check if the event is known before calling this
 *
 * @note eventids should be of the form catalog:id where catalog is
 *    usgs, gcmt, and isc; and the id is from the respective catalog
 *
 * @warning User owns the Event and is responsible to free the underlying memoery
 *    using \ref event_free
 */
Event *
event_by_event_id(char *id) {
    Event *e = NULL;
    Event **ev = NULL;
    request *er = NULL;
    result *r = NULL;
    char *url = NULL;
    char cat[16] = {0};
    char *p;
    if(!id || *id == 0) {
        goto error;
    }
    fprintf(stderr, "Requesting event info for %s ...", id);

    // Get Catalog identifier
    p = strchr(id, ':');
    memcpy(cat, id, p-id);
    cat[p-id] = 0;

    // Create Request
    er = event_req_new();
    event_req_set_eventid(er, id);
    r = request_get(er);

    if(result_is_ok(r)) {
        if(is_xml(result_data(r))) {
            ev = quake_xml_parse(result_data(r), result_len(r), FALSE, cat);
        } else {
            ev = event_from_json(result_data(r), result_len(r), FALSE, cat);
        }
        if(xarray_length(ev) > 1) {
            printf("Multiple events found for eventid: %s\n", id);
            goto error;
        } else if (xarray_length(ev) == 0) {
            printf("No events found for eventid: %s\n", id);
            goto error;
        }
        e = ev[0];
        if(!event_exists(e)) {
            event_save(e);
        } else {
            event_free(e);
        }
    } else {
        printf("%s", result_error_msg(r));
        goto error;
    }

 error:
    xarray_free(ev);
    ev = NULL;
    FREE(url);
    RESULT_FREE(r);
    REQUEST_FREE(er);
    clear_line();
    return e;
}

// Global Event Store
dict *_EVENTS = NULL;

/**
 * Create the internal event storage
 *
 * @memberof Event
 * @ingroup events
 * @private
 * @return hashtable / dict for events
 *
 */
dict *
event_store() {
    if(!_EVENTS) {
        _EVENTS = dict_new();
    }
    return _EVENTS;
}

/**
 * Check if an event data exists
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e     event to check
 *
 * @return 1 if the event data is stored, 0 if the event data does not exist
 *
 */
int
event_exists(Event *e) {
    return (dict_get(event_store(), event_id(e)) != NULL);
}

/**
 * Save an event
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e   Event
 *
 * @note No checking is done to make sure the event is not overwriting an
 *    existing event
 *
 */
void
event_save(Event *e) {
    if(!e || strlen(event_id(e)) == 0) {
        return;
    }
    dict_put(event_store(), event_id(e), e);
}
/**
 * Find an event if it exists. You probably want event_from_id() which will check the format, then call
 *   this function
 *
 * @memberof Event
 * @ingroup events
 *
 * @param id  eventid
 *
 * @return Event if it was found, NULL on error
 *
 * @note If the event is not found, it will be requested
 *
 */
Event *
event_find(char *id) {
    Event *e = NULL;
    if(!id || *id == 0) {
        return NULL;
    }
    if(!(e = dict_get(event_store(), id))) {
        // If not Found, request from server
        e = event_by_event_id(id);
    }
    return e;
}
/**
 * Set default values for character string values
 *
 * @memberof Event
 * @ingroup events
 *
 * @param e  event
 *
 * @note Set author, magnitude-author, catalog, and eventid to "-"
 *    if the value is not set
 */
void
event_default(Event *e) {
    if(e) {
        if(e->author[0] == 0)    { fern_strlcpy(e->author, "-", EVENT_ORIGIN_LEN); }
        if(e->magauthor[0] == 0) { fern_strlcpy(e->magauthor, "-", EVENT_MAG_LEN); }
        if(e->catalog[0] == 0)   { fern_strlcpy(e->catalog, "-", EVENT_ORIGIN_LEN); }
        if(e->eventid[0] == 0)   { fern_strlcpy(e->eventid, "-", EVENTID_LEN); }
    }
}

/**
 * Initialize an event request
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e  request
 *
 * @note the IRIS event catalog at https://service.iris.edu/fdsnws/event/1 is
 *    set as the URL, nodata will return a 404 HTTP code and the format is xml
 */
void
event_req_init(request *e) {
    request_set_url(e, EVENT_IRIS);
    request_set_arg(e, "nodata", arg_int_new(404));
    request_set_arg(e, "format", arg_string_new("xml"));
}

/**
 * Create and initialize a new event request
 *
 * @memberof event_req
 *
 * @return request for events
 *
 * @warning User owns the request and is responsible for freeing the underlying data
 *    with request_free()
 * @note Initialization is done using event_req_init()
 *
 */
request *
event_req_new() {
    request *e = request_new();
    event_req_init(e);
    return e;
}

/**
 * Set the eventid keyword for an event
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e   event request
 * @param id  event id to get
 *
 * @note catalog is search based on the eventid prefix
 *  - usgs - https://earthquake.usgs.gov/fdsnws/event/1/
 *  - gcmt - https://service.iris.edu/fdsnws/event/1/
 *  - isc  - http://www.isc.ac.uk/fdsnws/event/1/
 *
 */
void
event_req_set_eventid(request *e, char *id) {
    char *eid = NULL;
    char catalog[512] = {0};
    fern_strlcpy(catalog, id, sizeof(catalog));
    if(!(eid = strchr(catalog, ':'))) {
        printf("Expected ':' in eventid, e.g. source:eventid\n");
        return;
    }
    *eid = 0;
    eid++;
    request_set_arg(e, "eventid", arg_string_new(eid));
    if(strcasecmp(catalog, "usgs") == 0) {
        request_set_url(e, EVENT_USGS);
        request_del_arg(e, "format");
        request_set_arg(e, "format", arg_string_new("geojson"));

    } else if(strcasecmp(catalog, "isc") == 0) {
        request_set_url(e, EVENT_ISC);

    } else if(strcasecmp(catalog, "gcmt") == 0) {
        request_set_url(e, EVENT_IRIS);
        request_set_arg(e, "catalog", arg_string_new("GCMT"));
    }
    eid--;
    *eid = ':';
}
/**
 * Set magnitude range for an event search
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e        event request
 * @param min_mag  minimum magnitude value
 * @param max_mag  maximum magnitude value
 *
 */
void
event_req_set_mag(request *e, double min_mag, double max_mag) {
    request_set_arg(e, "minmag", arg_double_new(min_mag));
    request_set_arg(e, "maxmag", arg_double_new(max_mag));
}

/**
 * Set time range for an event search
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e        event request
 * @param start    minimum time
 * @param end      maximum time
 *
 */
void
event_req_set_time_range(request *e, timespec64 start, timespec64 end) {
    request_set_arg(e, "start", arg_time_new(start));
    request_set_arg(e, "end", arg_time_new(end));
}
/**
 * Set depth range for an event search
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e         event request
 * @param mindepth  minimum depth
 * @param maxdepth  maximum depth
 *
 */
void
event_req_set_depth(request *e, double mindepth, double maxdepth) {
    request_set_arg(e, "mindepth", arg_double_new(mindepth));
    request_set_arg(e, "maxdepth", arg_double_new(maxdepth));
}
/**
 * Set catalog for an event search
 *
 * @memberof event_req
 *
 * @param e         event request
 * @param catalog   catalog to search
 *
 * @note Providers have multiple different catalogs
 */
void
event_req_set_catalog(request *e, char *catalog) {
    request_set_arg(e, "catalog", arg_string_new(catalog));
}
/**
 * Set distance radius for an event search
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e         event request
 * @param lon       search center, longitude
 * @param lat       search center, latitude
 * @param minr      minimum radius in degrees
 * @param maxr      maximum radius in degrees
 *
 */
void
event_req_set_radial(request *e, double lon, double lat,
                     double minr, double maxr) {
    request_set_arg(e, "lon", arg_double_new(lon));
    request_set_arg(e, "lat", arg_double_new(lat));
    request_set_arg(e, "minradius", arg_double_new(minr));
    request_set_arg(e, "maxradius", arg_double_new(maxr));
}

/**
 * Set region for an event search
 *
 * @memberof event_req
 * @ingroup events
 *
 * @param e         event request
 * @param minlon    minimum longitude, i.e. West
 * @param maxlon    maximum longitude, i.e. East
 * @param minlat    minimum latitude, i.e. South
 * @param maxlat    maximum latitude, i.e. North

 *
 */
void
event_req_set_region(request *e,
                     double minlon, double maxlon,
                     double minlat, double maxlat) {
    request_set_arg(e, "minlon", arg_double_new(minlon));
    request_set_arg(e, "maxlon", arg_double_new(maxlon));
    request_set_arg(e, "minlat", arg_double_new(minlat));
    request_set_arg(e, "maxlat", arg_double_new(maxlat));
}

