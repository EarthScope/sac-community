
#include <fern.h>

int
main() {
    size_t i = 0;
    // Create a Data Request
    // Origin(Lon, Lat):  -67.55, -13.84  (Deep Bolivia Event 1994)
    // Time:              1994/160 00:33:16 - 1994/160 01:03:16
    // Network:           IU,XE
    // Radius:            0 - 35 degrees
    fprintf(stderr, "Data Download init\n");
    Event *ev = event_from_id("usgs:usp0006dzc");
    fprintf(stderr, "event: %p\n", ev);
    if(!ev) {
        fprintf(stderr, "event undefined: %p\n", ev);
        return -1;
    }
    request *r = data_avail_new();
    data_avail_set_origin(r, -67.55, -13.84);
    // Alternative using an event (Location)
    // data_avail_set_origin(r, event_lon(ev), event_lat(ev));
    data_avail_set_radius(r, 0.0, 35.0);
    data_avail_set_network(r, "IU,XE");
    data_avail_set_channel(r, "BHZ");
    data_avail_set_station(r, "DOOR");
    data_avail_set_time_range(r,
                              timespec64_from_yjhmsf(1994, 160, 0, 33, 16, 0),
                              timespec64_from_yjhmsf(1994, 160, 1, 03, 16, 0));
    // Alternative using the event (time)
    // duration d = { 0, 0 };
    // duration_parse(&d, "30m");
    // timespec64 start = event_time(ev);
    // timespec64 end   = timespec64_add_duration(start, &dur);
    // data_avail_set_time(r, start, end);
    request_set_verbose(r, 1);
    // Get request and check result
    result *res = request_get(r);
    if(!result_is_ok(res)) {
        printf("request is not ok\n");
        return -1;
    }
    fprintf(stderr, "request is ok\n");
    // Parse the result into a data request
    data_request *dr = data_request_parse( result_data(res) );
    // Print a formatted data request
    data_request_write(dr, stdout);
    // Write the data request to an actual file (optional)
    // data_request_write_to_file(dr, "data.request");
    // The request can be split into multiple pieces based on request size in MB
    //    Optional, but can be useful for long duration requests that last longer
    //     than a traditional earthquake record.  It is ok, the libmseed library can
    //    magically merge the request files back together again, provided there
    //    are no data gaps.
    // data_request_chunks(dr, 100);
    // Download the data !!!
    // Download the all of the requests
    MS3TraceList *mst3k = NULL;
    int save_files  = 1;
    int unpack_data = 1;
    mst3k = data_request_download(dr, "data.request", "miniseed_prefix",
                                  save_files, unpack_data);
    // Convert miniseed trace list to sac files
    sac **s = NULL;
    s = miniseed_trace_list_to_sac(mst3k);
    // Insert station meta data for sac files
    int verbose = 1;
    sac_array_fill_meta_data(s, verbose);
    // Or from a file
    // sac_fill_meta_data_from_file(s, verbose, "station.xml");
    // sac_fill_meta_data_from_file(s, verbose, "station.json");
    // Insert event meta data if event is available
    // Use the Bolivian Event from above
    sac_array_fill_meta_data_from_event(s, ev, verbose);
    // Calculate distance, azimuth, ...
    for(i = 0; i < xarray_length(s); i++) {
        update_distaz(s[i]);  // Function in sacio
    }
    // Write out sac files
    //   A filename is defined in an extra meta data sac header
    //   The extra meta data is not written out to the file
    int nerr = 0;
    for(i = 0; i < xarray_length(s); i++) {
        sac_write(s[i], s[i]->m->filename, &nerr);
    }
    return 0;
}
