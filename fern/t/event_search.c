
#include <fern.h>

int
main() {
    // Contstruct a request
    //  Magnitude: 8 - 10
    //  Time:      1994/160 - 1994/161
    //  Depth:     400.0 - 700.0 km
    //  Verbose:   On
    request *r = event_req_new();
    request_set_url(r, "https://earthquake.usgs.gov/fdsnws/event/1/query?");
    event_req_set_mag(r, 8.0, 10.0);
    event_req_set_depth(r, 400.0, 700.0);
    event_req_set_time_range(r,
                             timespec64_from_yjhmsf(1994, 160, 0, 0, 0, 0),
                             timespec64_from_yjhmsf(1994, 161, 0, 0, 0, 0));
    request_set_verbose(r, 1);

    // Get request and check result
    result *res = request_get(r);
    if(!result_is_ok(res)) {
        return -1;
    }
    // Parse quake xml output
    int verbose = 1;
    char catalog[5] = "usgs";
    Event **ev = quake_xml_parse(result_data(res), result_len(res), verbose, catalog);

    // Print out the events
    events_write(ev, stdout);
    return 0;
}
