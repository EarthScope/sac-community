
#include "cpf.h"
#include "msg.h"
#include "errors.h"
#include "vars/chash.h"

#include "octopus.h"
#include "array.h"
#include "debug.h"

Event * event_by_event_id(char *id);

static char *
file_extension(char *file) {
    char *p;
    if((p = strrchr(file, '.'))) {
        p++;
    }
    return p;
}

dict *EVENTS = NULL;

Event *
event_find(char *id) {
    Event *e = NULL;
    if(!EVENTS) {
        EVENTS = dict_new();
    }
    if(!(e = dict_get(EVENTS, id))) {
        // If not Found, request from server
        e = event_by_event_id(id);
    }
    return e;
}

void
event_save(Event *e) {
    if(!e && !e->eventid) {
        return;
    }
    dict_put(EVENTS, strdup(e->eventid), e);
}


Event *
event_by_event_id(char *id) {
    Event *e = NULL;
    Event **ev = NULL;
    string out;
    printf("Requesting event info for %s\n", id);
    string_init(&out);
    string_printf_append(&out,
                         "https://service.iris.edu/fdsnws/event/1/query?"
                         "eventid=%s&format=xml&nodata=404", id);

    result *r = request(out.str);

    if(result_is_ok(r)) {
        ev = quake_xml_parse(result_data(r), result_len(r), FALSE);
        if(xarray_length(ev) > 1) {
            printf("Multiple events found for eventid: %s\n", id);
            goto error;
        } else if (xarray_length(ev) == 0) {
            printf("No events found for eventid: %s\n", id);
            goto error;
        }
        e = ev[0];
        event_save(e);
    } else {
        printf("%s", result_error_msg(r));
        goto error;
    }

 error:
    xarray_free(ev);
    ev = NULL;
    FREE(out.str);
    return e;
}


int
lkdur(char *kkey, duration **d) {
    Token *t = NULL;

    if(!lckey(kkey, strlen(kkey)+1)) {
        return FALSE;
    }
    if(!(t = arg())) {
        return FALSE;
    }
    if(! (*d = duration_parse(t->str))) {
        return FALSE;
    }
    arg_next();
    return TRUE;
}


int
levent(Event **e) {
    Token *t;
    char id[128];
    if(!(t = arg())) {
        return FALSE;
    }
    // Look for event id
    if(sscanf(t->str, "id:%s", id) != 1) {
        return FALSE;
    }
    if(!(*e = event_find(id))) {
        return FALSE;
    }
    arg_next();
    return TRUE;
}

int
lktp(char *kkey, datetime **t1, datetime **t2) {
    Token *t = NULL;
    Event *e = NULL;
    duration *d = NULL;

    if(!lckey(kkey, -1)) {
        return FALSE;
    }

    if(!(t = arg())) {
        return FALSE;
    }
    if(levent(&e)) {
        *t1 = datetime_copy(e->time);
    } else if((*t1 = datetime_parse(t->str, NULL))) {
    } else {
        return FALSE;
    }
    arg_next();

    if(!(t = arg())) {
        return FALSE;
    }
    if((*t2 = datetime_parse(t->str, NULL))) {
    } else if((d = duration_parse(t->str))) {
        *t2 = datetime_add_duration(*t1, d);
    } else {
        return FALSE;
    }
    arg_next();
    return TRUE;

}


#define EventMag    1<<1
#define EventTime   1<<2
#define EventRegion 1<<3
#define EventDepth  1<<4
#define EventRadial 1<<5


void
event_request(int *nerr) {
    int n = 0;
    int catalog = 1;
    int verbose = 0;
    int search = 0;
    double v[4] = {0.0, 0.0, 0.0, 0.0 };
    datetime *t1 = NULL, *t2 = NULL;
    char outfile[2048] = {0};

    char *url;
    EventReq *e = NULL;
    Event **ev = NULL;
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

    while(lcmore(nerr)) {
        if(lkra("M#AG$", -1, 2, 2, v, &n)) {
            event_req_set_mag(e, v[0], v[1]);
            search |= EventMag;
        }
        else if(lckey("verbose$", -1)) {
            verbose = 1;
        }
        else if(lktp("T#IME$", &t1, &t2)) {
            event_req_set_time_range(e, t1, t2);
            search |= EventTime;
        }
        else if(lkra("REG#ION$", -1, 4, 4, v, &n)) {
            event_req_set_region(e, v[0], v[1], v[2], v[3]);
            search |= EventRegion;
        }
        else if(lkra("RAD#IAL$", -1, 4, 4, v, &n)) {
            event_req_set_radial(e, v[0], v[1], v[2], v[3]);
            search |= EventRadial;
        }
        else if(lkra("D#EPTH$", -1, 2, 2, v, &n)) {
            event_req_set_depth(e, v[0], v[1]);
            search |= EventDepth;
        }
        else if(lkchar2("OUT#FILE$", outfile, sizeof(outfile))) { }
        else if(lclist((char *)catalogs, 9, ncatalogs, &catalog)) {
            switch(catalog) {
            case 1:
            case 4:
                event_req_set_catalog(e, "GCMT"); break;
            case 2:
                event_req_set_catalog(e, "ISC"); break;
            case 3:
            case 5:
            case 6:
                event_req_set_catalog(e, "NEIC+PDE"); break;
            }
        }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if(search == 0) {
        printf("Not enough search parameters: Use mag, time, depth, region\n");
        *nerr = 3264;
    }
    if(search == (EventRegion | EventRadial)) {
        printf("Cannot use Region and Radial search together\n");
        *nerr = 3264;
    }
    if(*nerr != SAC_OK) {
        goto error;
    }

    // Create URL
    if(!(url = event_req_to_url(e))) {
        error(*nerr = 3264, "Error constructing event request");
        goto error;
    }
    if(verbose) {
        printf("%s\n", url);
    }

    /// Download Data
    r = request(url);
    if(!result_is_ok(r)) {
        if(result_is_empty(r)) {
            printf("No Event found matching search parameter\n");
        } else {
            printf("%s", result_error_msg(r));
        }
        goto error;
    }

    // Parse XML Data
    if(!(ev = quake_xml_parse(result_data(r), result_len(r), verbose))) {
        printf("Error parsing quake xml format\n");
        goto error;
    }

    // Print Out Events
    events_write(ev, stdout);

    // Save Files if desired
    if(strlen(outfile) > 0) {
        char *ext = file_extension(outfile);
        if(strcmp(ext, "xml") == 0) {
            result_write_to_file(r, outfile);
        } else if(strcmp(ext, "txt") == 0) {
            events_write_to_file(ev, outfile);
        } else {
            events_write_to_file(ev, outfile);
        }
    }

    // Store events in Global State
    if(!EVENTS) {
        EVENTS = dict_new();
    }
    for(size_t i = 0; i < xarray_length(ev); i++) {
        event_save(ev[i]);
    }

    xarray_free(ev);

    FREE(url);
 error:
    return;
}

int
stations_write_to_file(station_data **stat, char *filename) {
    FILE *fp = NULL;
    size_t n = 0;
    if(!(fp = fopen(filename, "w"))) {
        printf("Error opening station file for writing: %s\n", filename);
        return 0;
    }
    n = xarray_length(stat);
    for(size_t i = 0; i < n; i++) {
        fprintf(fp, "%s\n", station_data_to_string(stat[i], FALSE));
    }
    fclose(fp);
    return 1;
}

station_data ** station_xml_parse(char *data, size_t data_len, int epochs, int verbose);

void
station_request(int *nerr) {
    char net[128] = {0}, cha[128] = {0}, loc[128] = {0}, sta[128] = {0};
    int verbose = 0;
    int epochs = 0;
    int show_time = 0;
    char filename[256] = { 0 };
    Event *ev = NULL;
    StationReq *sr = NULL;
    double v[4] = {0.,0.,0.,0.};
    int n = 0;
    datetime *t1 = NULL, *t2 = NULL;
    char *url = NULL;
    result *r = NULL;
    station_data **s = NULL;

    sr = station_req_new();

    while(lcmore(nerr)) {
        if(0) {  }
        else if(lckey("verbose$", -1)) { verbose = 1; }
        else if(lklog("epochs$", -1, &epochs)) { }
        else if(lckey("show#times$", -1)) { show_time = 1; }
        else if(lktp("T#IME$", &t1, &t2)) {
            station_req_set_time_range(sr, t1, t2);
        }
        else if(lkchar2("STA#TION$", sta, sizeof(sta))) { }
        else if(lkchar2("NET#WORK$", net, sizeof(net))) { }
        else if(lkchar2("CHA#NNEL$", cha, sizeof(cha))) { }
        else if(lkchar2("LOC#ATION$",loc, sizeof(loc))) { }
        else if(lkchar2("OUT#FILE$", filename, sizeof(filename))) { }
        else if(lkra("reg#ion$", -1, 4, 4, v, &n)) {
            station_req_set_region(sr, v[0], v[1], v[2], v[3]);
        }
        else if(lkra("ori#gin$", -1, 2, 2, v, &n)) {
            station_req_set_origin(sr, v[0], v[1]);
        }
        else if(lkra("rad#ial$", -1, 2, 2, v, &n)) {
            station_req_set_radius(sr, v[0], v[1]);
        }
        else if(levent(&ev)) {
            datetime *t = datetime_copy(ev->time);
            station_req_set_time_range(sr, t, t);
            station_req_set_origin(sr, ev->evlo, ev->evla);
        }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    // Set NSLC for the Request
    station_req_set_nslc(sr, net, sta, loc, cha);
    if(*nerr != SAC_OK) {
        goto error;
    }
    // Construct the URL
    url = station_req_to_url(sr);
    if(verbose) {
        printf("%s\n", url);
    }
    /// Make the Request
    r = request(url);

    if(!result_is_ok(r)) {
        printf("%s", result_error_msg(r));
        goto error;
    }
    // Check for Raw Text, if so, parse as CSV
    char *data = result_data(r);
    if(data[0] == '#') {
        if(!(s = station_data_from_csv(data))) {
            printf("error parsing station data\n");
            goto error;
        }
    }
    // Check for XML, if so, parse that
    if(data[0] == '<') {
        if(!(s = station_xml_parse(data, result_len(r), epochs, verbose))) {
            printf("error parsing station.xml data\n");
            goto error;
        }
    }
    // Print out Station Data
    for(size_t i = 0; i < xarray_length(s); i++) {
        printf("%s\n", station_data_to_string(s[i], show_time));
    }
    // Write Station Data to a File if desired
    if(strlen(filename) > 0) {
        char *ext = file_extension(filename);
        if(strcmp(ext, "xml") == 0) {
            result_write_to_file(r, filename);
        } else if(strcmp(ext, "txt") == 0) {
            stations_write_to_file(s, filename);
        } else {
            stations_write_to_file(s, filename);
        }
    }

 error:
    FREE(url);
    return;
}
