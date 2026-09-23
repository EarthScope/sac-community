
#include <math.h>

#include "errors.h"

#include "cpf.h"
#include "msg.h"
#include "amf.h"
#include "hdr.h"
#include "dff.h"
#include "bbs.h"
#include "bot.h"
#include "defs.h"
#include "string_utils.h"

#include <fern/fern.h>
#include <fern/urls.h>
#include <libmseed/libmseed.h>

#define WARN(s) do {      \
        warning(3264, s); \
        outmsg();         \
        clrmsg();         \
    } while(0)

Event **quake_xml_parse(char *data, size_t data_len, int verbose, char *cat);
sac ** sac_data(); // iniam.c

Event * event_by_event_id(char *id);
int sac_fill_meta_data(sac **files, int verbose);
void sac_fill_meta_data_from_event(sac *s, Event *ev, int verbose);

/**
 * @brief      get a file extension
 *
 * @details    get a file extension from a string. Extension is
 *             assumed to be the after the final period.
 *
 * @param      file
 *
 * @return     file extension or NULL. Return value is within the string
 */
static char *
file_extension(char *file) {
    char *p;
    if((p = strrchr(file, '.'))) {
        p++;
    }
    return p;
}

/**
 * @brief      Parse a keyed duration value
 *
 * @details    Parse a keyed duration value
 *
 * @param      kkey    key to check for
 * @param      d       duration on success
 *
 * @return     TRUE on success, FALSE on error or no match
 */
int
lkdur(char *kkey, duration *d) {
    Token *t = NULL;

    if(!lckey(kkey, strlen(kkey)+1)) {
        return FALSE;
    }
    if(!(t = arg()) || !is_string_like(t->type)) {
        return FALSE;
    }

    if(! (duration_parse(t->str, d))) {
        return FALSE;
    }
    arg_next();
    return TRUE;
}

/**
 * @brief      Parse an event id
 *
 * @details    Parse an event id, e.g. usgs:1234566, isc:438302, ...
 *
 * @param      Newly allocated event on success
 *
 * @return     TRUE on success, FALSE on error
 */
int
levent(Event **e) {
    Event *tmp = NULL;
    Token *t;
    if(!(t = arg()) || !is_string_like(t->type)) {
        return FALSE;
    }
    if(!(tmp = event_from_id(t->str))) {
        if(strncasecmp("usgs:", t->str,5) == 0 ||
           strncasecmp("gcmt:", t->str,5) == 0 ||
           strncasecmp("isc:", t->str,4) == 0) {
            printf("Error: Could not resolve event: %s\n", t->str);
        }
        return FALSE;
    }
    *e = tmp;
    arg_next();
    return TRUE;
}

#define   EVENT_LIKE    1<<1
#define   TIME_LIKE     1<<2
#define   DURATION_LIKE 1<<3
#define   NOW_LIKE      1<<4

/**
 * @brief      Parse a time like value
 *
 * @details    Parse a value which represents time.
 *
 * @param      str     Input string to parse
 * @param      t0      Reference time for duration
 * @param      which   Kind of time to parse
 *                     - EVENT_LIKE
 *                     - TIME_LIKE
 *                     - DURATION_LIKE
 *                     - NOW_LIKE
 * @param      t       Output Time on sucess
 *
 * @return     0 on failure, 1 on success
 */
int
parse_time_like(char *str, timespec64 *t0, int which, timespec64 *t) {
    duration d;
    Event *e = NULL;
    if(!str || *str == 0) {
        return 0;
    }
    if(which & EVENT_LIKE && (e = event_from_id(str))) {
        *t = event_time(e);
        return 1;
    }
    if(which & TIME_LIKE && timespec64_parse(str, t)) {
        return 1;
    }

    if(which & DURATION_LIKE && t0 && (duration_parse(str, &d))) {
        *t = timespec64_add_duration(*t0, &d);
        return 1;
    }
    if(which & NOW_LIKE && strcasecmp(str, "now") == 0) {
        *t = timespec64_now();
        return 1;
    }
    /*if(0) {
        char dst[128] = {0};
        timespec64 v = {0,0};
        timespec64_parse(str, &v);
        strftime64t(dst, sizeof(dst), "%Y-%m-%dT%T.%3fZ", &v);
        printf("-->'%s'\n   '%s'\n", t->str, dst);
        }*/

    return 0;
}

/**
 * @brief      Parse a keyed set of time values
 *
 * @details    Parse a keyed set of time values, length is either 1 or 2
 *
 * @param      kkey    Key to check for
 * @param      vp      Output allocated array of time values, values
 *                     are sorted if two are found
 *
 * @return     FALSE on error, TRUE on success
 *
 */
int
lktn(char *kkey, timespec64 ***vp) {
    Token *t;
    timespec64 *tmp = NULL;
    timespec64 ts = {0,0};
    timespec64 **v = xarray_new('p');

    if(!lckey(kkey, -1)) {
        return FALSE;
    }
    if(!(t = arg()) || !is_string_like(t->type)) {
        return FALSE;
    }
    if(!parse_time_like(t->str, NULL,
                            TIME_LIKE | EVENT_LIKE | NOW_LIKE,
                            &ts)) {
        return FALSE;
    }
    tmp = calloc(1, sizeof(timespec64));
    *tmp = ts;
    v = xarray_append(v, tmp);
    arg_next();

    if(!(t = arg()) || !is_string_like(t->type)) {
        goto done;
    }
    if(!parse_time_like(t->str, v[0], TIME_LIKE | DURATION_LIKE, &ts)) {
        goto done;
    }
    tmp = calloc(1, sizeof(timespec64));
    *tmp = ts;
    v = xarray_append(v, tmp);
    arg_next();
 done:
    if(xarray_length(v) == 2 && timespec64_cmp(v[0],v[1]) > 0) {
        tmp = v[0];
        v[0] = v[1];
        v[1] = tmp;

    }
    *vp = v;
    return TRUE;

}

/**
 * @brief      Parse a keyed time-like pair
 *
 * @details    Parse a keyed time-like pair, (time,time), (time,duration)
 *
 * @param      kkey    Key
 * @param      t1      output earlies time
 * @param      t2      output later time
 *
 * @return     TRUE on success, FALSE on error
 */
int
lktp(char *kkey, timespec64 *t1, timespec64 *t2) {
    Token *t = NULL;
    if(!lckey(kkey, -1)) {
        return FALSE;
    }

    if(!(t = arg()) || !is_string_like(t->type)) {
        return FALSE;
    }

    if(!parse_time_like(t->str, NULL, TIME_LIKE | EVENT_LIKE | NOW_LIKE, t1)) {
        return FALSE;
    }
    arg_next();

    if(!(t = arg()) || !is_string_like(t->type)) {
        return FALSE;
    }
    if(!parse_time_like(t->str, t1, TIME_LIKE | DURATION_LIKE, t2)) {
        return FALSE;
    }
    arg_next();

    // Make sure times are ordered
    if(timespec64_cmp(t1, t2) > 0) {
        timespec64 tmp = *t1;
        *t1 = *t2;
        *t2 = tmp;
    }
    return TRUE;

}


#define SetMag    1<<1
#define SetTime   1<<2
#define SetRegion 1<<3
#define SetDepth  1<<4
#define SetRadial 1<<5
#define SetOrigin 1<<6
#define SetEvent 1<<7
#define SetNetwork 1<<8
#define SetStation 1<<9
#define SetChannel 1<<10
#define SetLocation 1<<11


/**
 * @brief      Limit a value
 *
 * @details    Limit a value to between min and max, assuming wrap around
 *             value return is min <= value < max assuming a total range of
 *             (max - min).  Values < min have (max-min) added until within
 *             range. Values >= max have (max-min) subtraced until within
 *             range
 *
 * @param      v    Initial value
 * @param      min  Min value
 * @param      max  Max value
 *
 * @return     limited value
 */
static double
limit_range(double v, double min, double max) {
    double dv = max - min;
    if(!isfinite(v)) {
        return v;
    }
    while(v < min) {
        v += dv;
    }
    while(v >= max) {
        v -= dv;
    }
    return v;
}


/**
 * @brief      Make an event request
 *
 * @details    Make an event request for command EVENT
 *
 * @param      nerr   Error return code
 *
 */
void
event_request(int *nerr) {
    int n = 0;
    int catalog = 5;
    int verbose = 0;
    int search = 0;
    double v[4] = {0.0, 0.0, 0.0, 0.0 };
    timespec64 t1 = {0,0}, t2 = {0,0};
    char outfile[2048] = {0};
    char bbvar[2048] = {0 };
    char cat[8];
    request *e = NULL;
    Event **ev = NULL;
    Event *ev1 = NULL;
    result *r = NULL;

    char catalogs[6][9] = {"GCMT    ",
                           "ISC     ",
                           "NEIC    ",
                           "CMT     ",
                           "USGS    ",
                           "PDE     ",
    };
    int ncatalogs = sizeof(catalogs)/sizeof(char*);

    e = event_req_new();

    /* Argument Parsing */
    while(lcmore(nerr)) {
        if(lkra("M#AG$", -1, 1, 2, v, &n)) {
            if(n == 1) { // Single Value - Minimum Magnitude
                v[1] = 10.0;
            }
            if(v[1] < v[0]) { // Sort v[0] and v[1]
                v[2] = v[1];
                v[1] = v[0];
                v[0] = v[2];
            }
            event_req_set_mag(e, v[0], v[1]);
            search |= SetMag;
        }
        else if(lckey("verbose$", -1)) {
            verbose = 1;
        }
        else if(lktp("T#IME$", &t1, &t2)) {
            event_req_set_time_range(e, t1, t2);
            search |= SetTime;
        }
        else if(lkra("REG#ION$", -1, 4, 4, v, &n)) {
            event_req_set_region(e, v[0], v[1], v[2], v[3]);
            search |= SetRegion;
        }
        /* If event id found, set time range at +1m,-1m */
        else if(levent(&ev1)) {
            duration d = {0,0};
            timespec64 t1 = {0,0}, t2 = {0,0};
            duration_parse("-1m", &d);
            t1 = timespec64_add_duration(event_time(ev1), &d);
            duration_parse("+1m", &d);
            t2 = timespec64_add_duration(event_time(ev1), &d);
            event_req_set_time_range(e, t1, t2);
            search |= SetTime;
        }
        /* Set Center location with min/max radius */
        else if(lkra("rad#ial$", -1, 4, 4, v, &n) ||
                lkra("rad#ius$", -1, 4, 4, v, &n) ) {
            if(v[2] < -90.0 || v[2] > 90.0 ||
               v[3] < -90.0 || v[3] > 90.0) {
                error(*nerr = 3264, "Latitudes should be between -90 and 90\n");
                goto error;
            }
            v[0] = limit_range(v[0], -180.0, 180.0);
            v[1] = limit_range(v[1], -180.0, 180.0);
            event_req_set_radial(e, v[0], v[1], v[2], v[3]);
            search |= SetRadial;
        }
        /* Set min/max depth */
        else if(lkra("D#EPTH$", -1, 2, 2, v, &n)) {
            event_req_set_depth(e, v[0], v[1]);
            search |= SetDepth;
        }
        /* Output event file */
        else if(lkchar2("OUT#FILE$", outfile, sizeof(outfile))) { }
        /* Select catalog to search */
        else if(lclist((char *)catalogs, 9, ncatalogs, &catalog)) { }
        /* Set blackboard variable to store event id(s) */
        else if(lkchar2("to#$", bbvar, sizeof(bbvar))) { }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if(search == 0) {
        error(*nerr = 3264, "Not enough search parameters: "
              "Use mag, time, depth, or region\n");
        goto error;
    }
    /* Do not allow region and radial search at the same time */
    if(search == (SetRegion | SetRadial)) {
        error(*nerr = 3264, "Cannot use Region and Radial searches together\n");
        goto error;
    }
    /* Set Catalog to search */
    switch(catalog) {
    case 1:
    case 4:
        request_set_url(e, EVENT_USGS);
        event_req_set_catalog(e, "gcmt");
        strlcpy(cat, "gcmt", sizeof(cat));
        break;
    case 2:
        request_set_url(e, EVENT_ISC);
        strlcpy(cat, "isc", sizeof(cat));
        break;
    case 3:
    case 5:
    case 6:
        request_set_url(e, EVENT_USGS);
        strlcpy(cat, "usgs", sizeof(cat));
        break;
    }
    /* If Time range is not set, set time range to -1000yr,+1day */
    if(! (search & SetTime) ) {
        duration d = { 0, 0};
        t2 = timespec64_now();
        t1 = t2;
        duration_parse("+1d", &d);
        t2 = timespec64_add_duration(t2, &d);
        duration_parse("-10cent", &d);
        t1 = timespec64_add_duration(t1, &d);
        event_req_set_time_range(e, t1, t2);
    }
    if(*nerr != SAC_OK) {
        goto error;
    }
    /* Verbose */
    request_set_verbose(e, verbose);

    /// Download Data
    r = request_get(e);
    if(!result_is_ok(r)) {
        if(result_is_empty(r)) {
            printf("No Event found matching search parameter\n");
        } else {
            printf("%s", result_error_msg(r));
        }
        goto error;
    }

    // Parse XML Data
    if(!(ev = quake_xml_parse(result_data(r), result_len(r), verbose, cat))) {
        printf("Error parsing quake xml format\n");
        goto error;
    }

    // Print Out Events
    events_write(ev, stdout);

    // Save Files if desired, xml or text
    if(strlen(outfile) > 0) {
        char *ext = file_extension(outfile);
        if(ext && strcmp(ext, "xml") == 0) {
            result_write_to_file_show(r, outfile);
        } else {
            events_write_to_file(ev, outfile);
        }
    }

    // Set Blackboard variable if desired
    if(strlen(bbvar) > 0) {
        char *val = NULL;
        size_t nalloc = 1024;
        size_t n = 0;
        for(size_t i = 0; i < xarray_length(ev); i++) {
            char *id = event_id(ev[i]);
            val = str_grow(val, &nalloc, n, strlen(id)+1);
            n = strlcat(val, id, nalloc);
            n = strlcat(val, " ", nalloc);
        }
        setbb(bbvar, VAR_STRING, val);
        FREE(val);
    }

    // Store events in magical Global State
    for(size_t i = 0; i < xarray_length(ev); i++) {
        if(!event_exists(ev[i])) {
            event_save(ev[i]);
        } else {
            event_free(ev[i]);
        }
    }

    xarray_free(ev);

 error:
    RESULT_FREE(r);
    REQUEST_FREE(e);
    return;
}

/**
 * @brief      Write station data to a file
 *
 * @details    Write station data to a file
 *
 * @param      stat       station data
 * @param      filename   filename to save file to
 * @param      show_time  save on/off times for stations
 *
 * @return     0 on failure, 1 on success
 */
int
stations_write_to_file(station **stat, char *filename, int show_time) {
    char tmp[256] = {0};
    FILE *fp = NULL;
    size_t n = 0;
    if(!(fp = fopen(filename, "w"))) {
        printf("Error opening station file for writing: %s\n", filename);
        return 0;
    }
    station_header(fp, show_time);
    n = xarray_length(stat);
    for(size_t i = 0; i < n; i++) {
        fprintf(fp, "%s\n", station_to_string(stat[i], show_time, tmp, sizeof(tmp)));
    }
    fclose(fp);
    return 1;
}


int
channels_write_to_file(station **stat, char *filename) {
    char tmp[2048] = {0};
    FILE *fp = NULL;
    size_t n = 0;
    if(!(fp = fopen(filename, "w"))) {
        printf("Error opening station file for writing: %s\n", filename);
        return 0;
    }
    channel_header(fp);
    n = xarray_length(stat);
    for(size_t i = 0; i < n; i++) {
        fprintf(fp, "%s\n", channel_to_string(stat[i], tmp, sizeof(tmp)));
    }
    fclose(fp);
    return 1;
}


/**
 * @brief      Request station data
 *
 * @details    Request station data, command STATION
 *
 * @param      nerr   status error code
 *
 */
void
station_request(int *nerr) {
    char tmp[2048] = {0};
    char net[128] = {0}, cha[128] = {0}, loc[128] = {0}, sta[128] = {0};
    int verbose = 0;
    int epochs = 0;
    int quiet = 0;
    int output_level = 1;
    char keys_level[][9] = {"station ", "channel "};
    int show_time = 0;
    char filename[256] = { 0 };
    Event *ev = NULL;
    request *sr = NULL;
    double v[4] = {0.,0.,0.,0.};
    int n = 0;
    timespec64 t1 = {0,0}, t2 = {0,0};
    result *r = NULL;
    station **s = NULL;
    xml *x = NULL;
    int set = 0;
    sr = station_req_new();

    /* Argument Parsing */
    while(lcmore(nerr)) {
        if(0) {  }
        /* Verbose for request data */
        else if(lckey("verbose$", -1)) { verbose = 1; }
        /* Channel level output */
        else if(lklist("level$", -1, (char *) keys_level, 9, 2, &output_level)) { }
        /* To show the station data */
        else if(lckey("quiet", -1)) { quiet = 1; }
        /* To show individual epochs for a station */
        else if(lklog("epochs$", -1, &epochs)) { }
        /* To show on and off times */
        else if(lckey("show#times$", -1)) { show_time = 1; }
        /* Set time range to search */
        else if(lktp("T#IME$", &t1, &t2)) {
            station_req_set_time_range(sr, t1, t2);
            set |= SetTime;
        }
        else if(lkchar2("STA#TION$", sta, sizeof(sta))) {
            rstrip_char(sta, '"');
            lstrip_char(sta, '"');
            station_req_set_station(sr, sta);
            set |= SetStation;
        }
        else if(lkchar2("NET#WORK$", net, sizeof(net))) {
            station_req_set_network(sr, net);
            set |= SetNetwork;
        }
        else if(lkchar2("CHA#NNEL$", cha, sizeof(cha))) {
            station_req_set_channel(sr, cha);
            set |= SetChannel;
        }
        else if(lkchar2("LOC#ATION$",loc, sizeof(loc))) {
            rstrip_char(loc, '"');
            lstrip_char(loc, '"');
            station_req_set_location(sr, loc);
            set |= SetLocation;
        }
        else if(lkchar2("OUT#FILE$", filename, sizeof(filename))) { }
        else if(lkra("reg#ion$", -1, 4, 4, v, &n)) {
            station_req_set_region(sr, v[0], v[1], v[2], v[3]);
            set |= SetRegion;
        }
        else if(lkra("ori#gin$", -1, 2, 2, v, &n)) {
            station_req_set_origin(sr, v[0], v[1]);
            set |= SetOrigin;
        }
        else if(lkra("rad#ial$", -1, 2, 2, v, &n) ||
                lkra("rad#ius$", -1, 2, 2, v, &n)) {
            station_req_set_radius(sr, v[0], v[1]);
            if(set & SetEvent) {
                station_req_set_origin(sr, event_lon(ev), event_lat(ev));
            }
            set |= SetRadial;
        }
        else if(levent(&ev)) {
            timespec64 t  = event_time(ev);
            timespec64 t2 = event_time(ev);
            station_req_set_time_range(sr, t, t2);
            if(set & SetRadial) {
                station_req_set_origin(sr, event_lon(ev), event_lat(ev));
            }
            set |= SetEvent;
        }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if(set == 0) {
        error(*nerr = 3264, "No station search parameters given\n");
        goto error;
    }
    if(set & SetRadial && set & SetRegion) {
        error(*nerr = 3264, "Cannot use Region and Radial searches together\n");
        goto error;
    }
    // Set NSLC for the Request
    if(*nerr != SAC_OK) {
        goto error;
    }
    request_set_verbose(sr, verbose);
    /// Make the Request
    r = request_get(sr);

    if(!(x = xml_merge_results(r, NULL, "//s:Network"))) {
        goto error;
    }
    if(output_level == 1) {
        if(!(s = station_xml_parse(x, epochs, verbose))) {
            goto error;
        }
        if(!quiet) {
            // Print out Station Data
            station_header(stdout, show_time);
            for(size_t i = 0; i < xarray_length(s); i++) {
                printf("%s\n", station_to_string(s[i], show_time,
                                                 tmp, sizeof(tmp)));
            }
        }
    } else if(output_level == 2) {
        if(!(s = channel_xml_parse(x, verbose))) {
            goto error;
        }
        if(!quiet) {
            // Print out Channel Data
            channel_header(stdout);
            for(size_t i = 0; i < xarray_length(s); i++) {
                printf("%s\n", channel_to_string(s[i], tmp, sizeof tmp));
            }
        }
    }
    // Write Station Data to a File if desired, xml or text
    if(strlen(filename) > 0) {
        char *ext = file_extension(filename);
        if(ext && strcmp(ext, "xml") == 0) {
            result_write_to_file_show(r, filename);
        } else {
            if(output_level == 1) {
                stations_write_to_file(s, filename, show_time);
            } else if(output_level == 2) {
                channels_write_to_file(s, filename);
            }
        }
    }
    for(size_t i = 0; i < xarray_length(s); i++) {
        station_free(s[i]);
        s[i] = NULL;
    }
    xarray_free(s);
 error:
    xml_free(x);
    REQUEST_FREE(sr);
    RESULT_FREE(r);
    return;
}

char *rstrip(char *s);
char *lstrip(char *s);

#define NINE 9

char *
slurp(char *file, size_t *np) {
    size_t n = 0;
    char *data = NULL;
    FILE *fp = NULL;
    if(!(fp = fopen(file, "r"))) {
        printf("Error opening file: %s\n", file);
        return NULL;
    }
    fseek (fp, 0, SEEK_END);
    n = ftell (fp);
    fseek (fp, 0, SEEK_SET);
    data = calloc(n + 1, sizeof(char));
    if(fread (data, 1, n, fp) != n) {
        FREE(data);
        return NULL;
    }
    fclose(fp);
    *np = n;
    return data;
}



enum {
    DOWNLOAD = 1,
    AVAIL = 2,
};

/**
 * @brief      Data Request, command DATA
 *
 * @details    Data request, command DATA
 *
 * @param      nerr   status error code
 *
 */
void
data_request_f(int *nerr) {
    char net[128] = { 0 }, sta[128] = { 0 }, loc[128] = { 0 }, cha[128] = { 0 };
    char infile[2048] = { 0 };
    char outfile[2048] = { 0 };
    char reqfile[2048] = { 0 };
    char prefix[2048] = { 0 };
    int nr = 0;
    int verbose = 0;
    int to_sac   = 0;
    int to_mem   = 0;
    int more     = 0;
    int to_mseed = 0;
    double v[4] = {0., 0., 0., 0. };
    double chunk_size = 200. * 1024.0 * 1024.0;
    Event *ev = NULL;
    duration d = { Duration_None, 0 };
    request *dr = NULL;
    result *r = NULL;
    data_request *fdr = NULL;
    MS3TraceList *mst3k = NULL;
#define NACTIONS 2
    int action = AVAIL;
    timespec64 t1 = {0,0}, t2 = {0,0};
    int set = 0;
    //char actions[NACTIONS][NINE] = {"download",
    //                                "avail   "};
#define NQUALS  9
    int qual = 5;
    char qualities[NQUALS][NINE] = {"D       ",
                                    "Raw     ",
                                    "Quality ",
                                    "Modified",
                                    "Best    ",
                                    "Merged  ",
                                    "QC      ",
                                    "Unknown ",
                                    "All     ",
    };
    strlcpy(prefix, "fdsnws", sizeof(prefix));
    dr = data_avail_new();
    *nerr = 0;

    /* Argument Parsing */
    while(lcmore(nerr)) {
        if(0) { }
        else if(lckey("verbose$", -1)) { verbose = 1; }
        //else if(lclist((char *)actions, NINE, NACTIONS, &action)) { }
        else if(lclist((char *)qualities, NINE, NQUALS, &qual)) {
            data_avail_set_quality(dr, qual);
        }
        else if(lkreal("MAX$", -1, &chunk_size)) {
            chunk_size = chunk_size * 1024.0 * 1024.0;
        }
        else if(lkchar2("req#uest$", reqfile, sizeof(reqfile))) { }
        else if(levent(&ev)) {
            timespec64 ts = event_time(ev);
            timespec64 te = event_time(ev);
            data_avail_set_time_range(dr, ts, te);
            if(set & SetRadial) {
                data_avail_set_origin(dr, event_lon(ev), event_lat(ev));
            }
            set |= SetEvent;
        }
        else if(lkdur("dur#ation$", &d)) { }
        else if(lkra("reg#ion$", -1, 4, 4, v, &nr)) {
            data_avail_set_region(dr, v[0], v[1], v[2], v[3]);
            set |= SetRegion;
        }
        else if(lkra("origin#$", -1, 2, 2, v, &nr)) {
            data_avail_set_origin(dr, v[0], v[1]);
            set |= SetOrigin;
        }
        else if(lkra("rad#ial$", -1, 2, 2, v, &nr) ||
                lkra("rad#ius$", -1, 2, 2, v, &nr)) {
            data_avail_set_radius(dr, v[0], v[1]);
            set |= SetRadial;
            if(set & SetEvent) {
                data_avail_set_origin(dr, event_lon(ev), event_lat(ev));
            }
        }
        else if(lktp("T#IME$", &t1, &t2)) {
            data_avail_set_time_range(dr, t1, t2);
            set |= SetTime;
        }
        else if(lkchar2("STA#TION$",  sta, sizeof(sta))) {
            data_avail_set_station(dr, sta);
            set |= SetStation;
        }
        else if(lkchar2("NET#WORK$",  net, sizeof(net))) {
            data_avail_set_network(dr, net);
            set |= SetNetwork;
        }
        else if(lkchar2("CHA#NNEL$",  cha, sizeof(cha))) {
            data_avail_set_channel(dr, cha);
            set |= SetChannel;
        }
        else if(lkchar2("LOC#ATION$", loc, sizeof(loc))) {
            rstrip_char(loc, '"');
            lstrip_char(loc, '"');
            data_avail_set_location(dr, loc);
            set |= SetLocation;
        }
        else if(lkchar2("IN$", infile, sizeof(infile))) { }
        else if(lkchar2("OUT$", outfile, sizeof(outfile))) { }
        else if(lkchar2("prefix$", prefix, sizeof(prefix))) { }
        else if(lckey("mseed", -1))  { to_mseed = 1; }
        else if(lckey("miniseed", -1))  { to_mseed = 1; }
        else if(lckey("sac", -1))  { to_sac = 1; }
        else if(lckey("read", -1)) { to_mem = 1; }
        else if(lckey("more", -1)) { more = 1; }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if(set == 0 && strlen(reqfile) == 0) {
        error(*nerr = 3264, "Not enough search parameters for data search\n"
              "%14sUse time, station, event, or region / radius\n", "");
        goto error;
    }
    if(set & SetRegion && set & SetRadial) {
        error(*nerr = 3264,"Cannot use Region and Radial searches together");
        goto error;
    }
    if(*nerr != SAC_OK) {
        goto error;
    }
    // If data was requested, download the data
    if(to_sac || to_mem || to_mseed) {
        action = DOWNLOAD;
    }

    if(!to_sac && !to_mem && !to_mseed) {
        to_mseed = 1;
    }
    request_set_verbose(dr, verbose);

    /* Read Request file if specified */
    if(strlen(reqfile) > 0) {
        size_t n = 0;
        char *data = NULL;
        if(!(data = slurp(reqfile, &n))) {
            goto error;
        }
        fdr = data_request_parse(data);
        if(!fdr) {
            error(*nerr= 3264, "Error parsing data request file");
            goto error;
        }
        FREE(data);
    } else {
        // Use Duration if available
        if(d.type != Duration_None) {
            data_avail_use_duration(dr, &d);
        }
        // Availability Request, construct from station file
        if(strlen(infile) > 0 ) {
            char *req = NULL;
            request *dr0 = data_avail_new();
            if(verbose) {
                printf("Contructing request from file: %s\n", infile);
            }
            if(!(req = data_avail_from_station_file(dr, infile))) {
                goto error;
            }
            request_set_verbose(dr0, verbose);
            /* Make the Availability request */
            r = request_post(dr0, req);
            REQUEST_FREE(dr0);
        } else {
            /* Make the Availability request */
            r = request_get(dr);
        }
        /* Check the request result */
        if(!result_is_ok(r)) {
            printf("%s", result_error_msg(r));
            goto error;
        }
        /* Parse the data request result */
        fdr = data_request_parse(result_data(r));
        if(!fdr) {
            error(*nerr = 3264, "Error parsing data request file");
            goto error;
        }
        /* Split the request into managable chunks */
        data_request_chunks(fdr, (size_t) chunk_size);
        if(strlen(outfile) > 0) {
            data_request_write_to_file(fdr, outfile);
        }
        // Write out the the screen if not downloading
        if(action == AVAIL) {
            data_request_write(fdr, stdout);
            cprintf("red,bold",
                   "No Data Downloaded, use miniseed, sac, or read to download data\n");
            goto error; /* Its ok, its not really an error */
        }
    }

    /* Data Download from here */

    /* Set output filename for the request  */
    char *filename = NULL;
    if(strlen(outfile) > 0) {
        filename = outfile;
    } else if(strlen(reqfile) > 0) {
        filename = reqfile;
    } else {
        filename = result_filename(r);
    }

    // Download actual data
    mst3k = data_request_download(fdr, filename, prefix, to_mseed, to_sac | to_mem);
    if(to_mem && !more) {
        sacclear();
    }
    if(mst3k && (to_sac || to_mem)) {
        char tmp[64] = {0};
        sac **out = miniseed_trace_list_to_sac(mst3k);
        sac_array_fill_meta_data(out, verbose);
        for(size_t i = 0; i < xarray_length(out); i++) {
            sac_fill_meta_data_from_event(out[i], ev, verbose);
            update_distaz(out[i]);
            if(to_sac) {
                cprintf("green", "\tWriting data to %s [%s]\n",
                        out[i]->m->filename, data_size(sac_size(out[i]),tmp,sizeof(tmp)));
                sac_write(out[i], out[i]->m->filename, nerr);
            }
            if(to_mem) {
                sacput(out[i]);
            } else {
                sac_free(out[i]);
                out[i] = NULL;
            }
        }
        xarray_free(out);
        out = NULL;
        if(to_mem && saclen() > 0) {
            setrng();
        }
    }

 error:
    if(mst3k) {
        mstl3_free(&mst3k, 1);
        mst3k = NULL;
    }
    data_request_free(fdr);
    fdr = NULL;

    REQUEST_FREE(dr);
    RESULT_FREE(r);
    return;
}

/**
 * @brief      Get a CMT id from an Event ID (IRIS)
 *
 * @details    Get a CMT id from an Event ID (IRIS)
 *
 * @param      eventid   Event ID
 * @param      verbose   Report the progress
 *
 * @return     cmtid on success, NULL on failure
 */
char *
cmtid_from_eventid(char *eventid, int verbose) {
    request *req = NULL;
    result *r = NULL;
    char *cmtid = NULL;
    if(!(req = request_new())) {
        printf("Error creating request\n");
        goto done;
    }
    request_set_url(req, "https://ds.iris.edu/spudservice/momenttensor/ids?");
    request_set_arg(req, "eventid", arg_string_new(eventid));
    request_set_verbose(req, verbose);
    r = request_get(req);
    if(!result_is_ok(r)) {
        printf("%s\n", result_error_msg(r));
        goto done;
    }
    cmtid = rstrip( result_free_move_data(r) );
    r = NULL;
 done:
    REQUEST_FREE(req);
    RESULT_FREE(r);
    return cmtid;
}

/**
 * @brief      Get CMTSOLUTION from CMT id (IRIS)
 *
 * @details    Get CMTSOLUTION from CMT id
 *
 * @param      cmtid    CMT id
 * @param      verbose  Report progress
 *
 * @return     CMTSOLUTION on success, NULL on failure
 */
char *
cmtsolution_from_cmtid(char *cmtid, int verbose) {
    request *req = NULL;
    result *r = NULL;
    char *cmtsol = NULL;
    char url[1024] = {0};

    snprintf(url, sizeof(url),
             "https://ds.iris.edu/spudservice/momenttensor/%s/cmtsolution",
             cmtid);
    if(!(req = request_new())) {
        printf("Error creating request\n");
        goto done;
    }
    request_set_url(req, url);
    request_set_verbose(req, verbose);
    r = request_get(req);
    if(!(result_is_ok(r))) {
        printf("%s\n", result_error_msg(r));
        goto done;
    }
    cmtsol = result_free_move_data(r);
    r = NULL;
 done:
    REQUEST_FREE(req);
    RESULT_FREE(r);
    return cmtsol;
}

/**
 * @brief      Check if string starts with a string
 *
 * @details    Check if string starts with a string
 *
 * @param      str    String to test
 * @param      pat    Pattern to see if string starts with
 *
 * @return     1 on success, 0 on failure
 */
int
starts_with(char *str, char *pat) {
    return (strncmp(pat, str, strlen(pat)) == 0);
}

/**
 * @brief      Get CMTSOLUTION from kevnm
 *
 * @details    Get CMTSOLUTION from kevnm
 *
 * @param      files    sac files
 * @param      ev       event
 * @param      verbose  report progress
 *
 */
void
get_cmtsolution_from_kevnm(sac **files, Event *ev, int verbose) {
    sac *s = NULL;
    dict *evs = dict_new(); /* Local event dictionary */
    if(ev) {
        /* Put event into dictionary */
        dict_put(evs, event_id(ev), NULL);
    }
    /* Check if kevnm exists and save name */
    for(size_t i = 0; i < xarray_length(files); i++) {
        s = files[i];
        if(strcmp(s->h->kevnm, SAC_CHAR_UNDEFINED) != 0) {
            if(!dict_get(evs, s->h->kevnm)) {
                dict_put(evs, s->h->kevnm, NULL);
            }
        }
    }
    /* For each unique event, find and download CMTSOLUTION */
    char **keys = dict_keys(evs);
    for(size_t i = 0; keys[i]; i++) {
        if(starts_with(keys[i], "gcmt:")) {
            // Convert GCMT ID to CMTSOLUTION ID
            //   GCMT ID => 884490
            // http://ds.iris.edu/spudservice/momenttensor/ids?eventid=369471
            //   => 884490
            // http://ds.iris.edu/spudservice/momenttensor/884490/cmtsolution
            char *p = NULL;
            char *cmtid = NULL;
            char *cmtsol = NULL;
            if(!(p = strchr(keys[i], ':'))) {
                printf("Error finding ':' in event-name %s\n", keys[i]);
                continue;
            }
            p++;
            if((cmtid = cmtid_from_eventid(p, verbose)) &&
               (cmtsol = cmtsolution_from_cmtid(cmtid, verbose))) {
                printf("%s\n", cmtsol);
            }
            FREE(cmtid);
            FREE(cmtsol);
        } else if(starts_with(keys[i], "usgs:")) {
            continue;
            // Request event data from USGS Service
            // Parse CMT Data from xml
            /*
            char *cmtsol = NULL;
            if((cmtsol = cmtsolution_from_usgs_eventid(keys[i], verbose))) {
                printf("%s\n", cmtsol);
            }
            FREE(cmtsol);
            */
        }
    }
    dict_keys_free(keys);
    dict_free(evs, NULL);
    evs = NULL;
    return;
}

/**
 * @brief      meta data request, command METADATA
 *
 * @details    meta data request, command METADATA
 *
 * @param      nerr    status error code
 *
 */
void
meta_request(int *nerr) {
    int verbose = 0;
    int cmt = 0;
    char file[2048] = {0};
    Event *ev = NULL;
    *nerr = SAC_OK;

    /* Argument parsing */
    while(lcmore(nerr)) {
        if(0) {}
        else if(lckey("verbose$", -1)) { verbose = 1; }
        else if(levent(&ev)) { }
        else if(lkchar2("file$", file, sizeof(file))) { }
        else if(lckey("cmt$", -1)) { cmt = 1; }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if(*nerr != SAC_OK) {
        goto error;
    }
    if(saclen() == 0) {
        goto error;
    }

    sac **files = sac_data();

    /* Fill meta data from "file" or header data */
    if(strlen(file) > 0) {
        sac_array_fill_meta_data_from_file(files, verbose, file);
    } else {
        sac_array_fill_meta_data(files, verbose);
    }

    /* Update data for event */
    for(size_t i = 0; i < xarray_length(files); i++) {
        sac_fill_meta_data_from_event(files[i], ev, verbose);
        update_distaz(files[i]);
    }

    /* Download CMTSOLUTION if desired */
    if(cmt) {
        get_cmtsolution_from_kevnm(sac_data(), ev, verbose);
    }

 error:
    return;
}


static int str_is(char *a, char *b) {
    return strcmp(a, b) == 0;
}
static int
sac_str_def(char *s) {
    return ! str_is(s, SAC_CHAR_UNDEFINED) ;
}

/**
 * @brief      response request, command RESPONSE
 *
 * @details    response request, command RESPONSE
 *
 * @param      nerr   status error code
 *
 */
void
response_request(int *nerr) {
    char file[2048] = {0};
    char nslc[128] = { 0 };
    char keys[6][9] = {"sacpz   ",
                       "resp    ",
                       "evalresp",
                       "pz      ",
                       "polezero",
                       "evresp"};
    int kind = 1;
    ResponseType type = ResponseSacPZ;
    timespec64 t = {0,0};
    timespec64 **ts = NULL;
    request *pz = response_new();
    result *r = NULL;
    duration d = {Duration_None, 0};
    int set = 0;
    int verbose = 0;
    *nerr = SAC_OK;

    /* Argument parsing */
    while( lcmore(nerr) ) {
        if(lclist((char *)keys, 9, 6, &kind)) {
            switch(kind) {
            case 1:
            case 4:
            case 5:
                type = ResponseSacPZ;
                break;
            case 2:
            case 3:
            case 6:
                type = ResponseResp;
                break;
            }
            response_set_kind(pz, type);
        }
        else if(lckey("verbose$",  -1)){
            request_set_verbose(pz, TRUE);
            verbose = TRUE;
        }
        else if(lkchar2("STA#TION$",  nslc, sizeof(nslc))) {
            response_set_station(pz, nslc);
        }
        else if(lkchar2("NET#WORK$",  nslc, sizeof(nslc))) {
            response_set_network(pz, nslc);
        }
        else if(lkchar2("CHA#NNEL$",  nslc, sizeof(nslc))) {
            response_set_channel(pz, nslc);
        }
        else if(lkchar2("LOC#ATION$", nslc, sizeof(nslc))) {
            rstrip_char(nslc, '"');
            lstrip_char(nslc, '"');
            response_set_location(pz, nslc);
        }
        else if(lktn("TIME$", &ts)) {
            if(xarray_length(ts) == 1) {
                response_set_time(pz, *ts[0] );
                set |= SetTime;
            } else if (xarray_length(ts) == 2) {
                response_set_start(pz, *ts[0] );
                response_set_end(pz, *ts[1] );
                set |= SetTime;
            }
        }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if(*nerr != SAC_OK) {
        goto error;
    }
    int stat = response_status(pz);
    if( (stat & ResponseNSLC) == ResponseNSLC ) {
        if(saclen() > 0) {
            WARN("Retrieving response from command options (data exists in memory)");
        }
        /* If Time range is not set, set to -200years, +1year */
        if((set & SetTime) == 0) {
            duration_parse("-200y", &d);
            t = timespec64_add_duration(timespec64_now(), &d);
            response_set_start(pz, t);

            duration_parse("+1y", &d);
            t = timespec64_add_duration(timespec64_now(), &d);
            response_set_end(pz, t);
        }
        /* Request response */
        r = request_get(pz);
        /* Check response */
        if(!result_is_ok(r)) {
            printf("%s", result_error_msg(r));
            goto error;
        }
        /* Remove time range if not specified */
        if((set & SetTime) == 0) {
            request_del_arg(pz, "start");
            request_del_arg(pz, "end");
        }
        /* Write response to a file */
        result_write_to_file_show(r, response_filename(pz, file, sizeof(file)));
    } else if(saclen() > 0) {
        if( stat != 0 ) {
            WARN("Retrieving response from headers in files (some command options provided)");
        }
        /* Get Response for all files in memory */
        REQUEST_FREE(pz);
        for( int i = 0 ; i < saclen(); i++) {
            /* -- Get the next file in DFL, moving header to CMHDR. */
            sac *s;
            if (!(s = sacget(i, TRUE, nerr))) {
                goto error;
            }
            if(sac_str_def(s->h->knetwk) &&
               sac_str_def(s->h->kstnm) &&
               sac_str_def(s->h->kcmpnm)) {
                char p[16];

                if(!sac_str_def(s->h->khole) || strlen(s->h->khole) == 0) {
                    strlcpy(p, "--", sizeof(p));
                } else {
                    strlcpy(p, s->h->khole, sizeof(p));
                }
                pz = response_new();

                response_set_kind(pz, type);
                if(verbose) {
                    request_set_verbose(pz, TRUE);
                }
                response_set_kind(pz, kind);
                response_set_location(pz, p);

                strlcpy(p, s->h->knetwk, sizeof(p));
                response_set_network (pz, rstrip(p));

                strlcpy(p, s->h->kstnm, sizeof(p));
                response_set_station (pz, rstrip(p));

                strlcpy(p, s->h->kcmpnm, sizeof(p));
                response_set_channel (pz, rstrip(p));

                if(sac_get_time(s, SAC_B, &t)) {
                    response_set_start(pz, t);
                }
                if(sac_get_time(s, SAC_E, &t)) {
                    response_set_end(pz, t);
                }
                r = request_get(pz);
                if(!result_is_ok(r)) {
                    char pp[64] = {0};
                    sac_fmt(pp, sizeof(pp), "%Z", s);
                    printf("%s %s", pp, result_error_msg(r));
                    continue;
                }
                result_write_to_file_show(r,
                           response_filename(pz, file, sizeof(file)));
                RESULT_FREE(r);
                REQUEST_FREE(pz);
            } else {
                error(*nerr = 3264,
                      "Response request either requires a data file with meta data\n"
                      "    knetwm, kstnm, khole, kcmpnm [kzdate/kztime]\n"
                      "    or net, sta, loc, and cha [time/start/end]");
                char *names[] = {"Network", "Station", "Location/Hole", "Channel" };
                int bits[] = {ResponseNetwork, ResponseStation, ResponseLocation, ResponseChannel };
                for(int i = 0; i < 4; i++) {
                    if((stat & bits[i]) == 0) {
                        mprint("   -- Missing: %s\n", names[i]);
                    }
                }
            }
        }
    } else {
        error(*nerr = 3264,
              "Response request either requires a data file with meta data\n"
              "    knetwm, kstnm, khole, kcmpnm [kzdate/kztime]\n"
              "    or net, sta, loc, and cha [time/start/end]");
        char *names[] = {"Network", "Station", "Location/Hole", "Channel" };
        int bits[] = {ResponseNetwork, ResponseStation, ResponseLocation, ResponseChannel };
        for(int i = 0; i < 4; i++) {
            if((stat & bits[i]) == 0) {
                mprint("   -- Missing: %s\n", names[i]);
            }
        }
        goto error;
    }
 error:
    REQUEST_FREE(pz);
    RESULT_FREE(r);

    return;
}

