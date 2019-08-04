libfern Documentation
=====================

Overview
--------

The libfern library, assocated fern binary, and inferface in sac provides a interface to:
   - event searches
   - download data (miniseed format)
   - station searches
   - apply station meta-data
   - apply event meta-data
   - search for available data
   - convert miniseed to sac
   - write miniseed and sac files

Examples using fern and libfern
---------------------------
- [Event Search](#Event-Search)
- [Station Search](#Station-Search)
- [Search based on Event](#Search-based-on-Event)
- [Data Availability Search](#Data-Availability-Search)
- [Data Download to miniseed](#Data-Download-to-miniseed)
- [Data Download to sac](#Data-Download-to-sac)
- [Event Search with libfern](#event-fern)
- [Station Search with libfern](#event-fern)
- [Data Download with libfern](#data-fern)

### <a name="Event-Search">Event Search</a>

Description        | Argument
-------------------|---------
Event Search       | `-E`
Magntiude: 9 - 10  | `-m 9/10`
Time: 1950 - 2020  | `-t 1950/01/01 2020/01/01`

~~~~~{.sh}
    fern -E -m 9/10 -t 1950/01/01 2020/01/01
    Origin              Lat.   Lon.    Depth  Mag.     Agency EventID
    2011-03-11T05:46:24  38.30  142.37  29.00 9.10 mww US/official - usgs:official20110311054624120_30
    2004-12-26T00:58:53   3.29   95.98  30.00 9.10 mw  US/official - usgs:official20041226005853450_30
    1964-03-28T03:36:16  60.91 -147.34  25.00 9.20 mw  iscgem/official - usgs:official19640328033616_30
    1960-05-22T19:11:20 -38.14  -73.41  25.00 9.50 mw  iscgem/official - usgs:official19600522191120_30
    1952-11-04T16:58:30  52.62  159.78  21.60 9.00 mw  iscgem/official - usgs:official19521104165830_30
~~~~~

### <a name="Station-Search">Station Search</a>

Description                   | Argument
------------------------------|---------
Station Search                | `-S`
Time: 1999/01/01 - 1999/01/02 | `-t 1999/01/01 +1day`
Network: IU                   | `-n IU`
verbose: On                   | `-v`

~~~~~{.sh}
    fern -S -t 1999/01/01 +1day -n IU
    Net Sta   Lat.     Lon.      Elev.   SiteName
    IU  ADK    51.8823 -176.6842  130.00 Adak, Aleutian Islands, Alaska
    IU  AFI   -13.9093 -171.7773  706.00 Afiamalu, Samoa
    IU  ANMO   34.9459 -106.4572 1850.00 Albuquerque, New Mexico, USA
    ...
    IU  XMAS    2.0448 -157.4457   20.00 Kiritimati Island, Republic of Kiribati
    IU  YAK    62.0310  129.6805  110.00 Yakutsk, Russia
    IU  YSS    46.9587  142.7604  150.00 Yuzhno Sakhalinsk, Russia
~~~~~

### <a name="Search-based-on-Event">Search based on Event</a>

Description                             | Argument
----------------------------------------|---------
Event search                            | `-E`
Start time  160th day in 1994           | `-t 1994/160`
Magntiude 8 - 10                        | `-m 8/10`
Station search                          | `-S`
Time: event origin time                 | `-e usgs:usp0006dzc`
Network: IU, XE                         | `-n IU,XE`
Radial search from event to 35 degrees  | `-r0/35 -e usgs:usp0006dzc`

~~~~~{.sh}
    fern -E -t 1994/160 +1d -m 8/10
    Origin              Lat.   Lon.    Depth  Mag.     Agency EventID
    1994-06-09T00:33:16 -13.84  -67.55 631.30 8.20 mw  US/HRV - usgs:usp0006dzc

    fern -S -e usgs:usp0006dzc -n IU,XE -r0/35
    Net Sta   Lat.     Lon.      Elev.   SiteName
    IU  BOCO    4.5869  -74.0432 3160.00 Bogota, Colombia
    IU  SJG    18.1091  -66.1500  420.00 San Juan, Puerto Rico
    XE  CHIT  -20.0756  -66.8851 3862.00 -
    ...
    XE  SICA  -17.2924  -67.7490 4065.00 -
    XE  TACA  -18.8279  -66.7331 3810.00 -
    XE  YUNZ  -19.1581  -65.0700 2896.00 -

    # Equivalent to the following
    fern -S -t 1994-06-09T00:33:16 +0m  -n IU,XE -O -67.55/-13.84  -r0/35
    Net Sta   Lat.     Lon.      Elev.   SiteName
    IU  BOCO    4.5869  -74.0432 3160.00 Bogota, Colombia
    IU  SJG    18.1091  -66.1500  420.00 San Juan, Puerto Rico
    XE  CHIT  -20.0756  -66.8851 3862.00 -
    ...
    XE  SICA  -17.2924  -67.7490 4065.00 -
    XE  TACA  -18.8279  -66.7331 3810.00 -
    XE  YUNZ  -19.1581  -65.0700 2896.00 -
~~~~~

### <a name="Data-Availability-Search">Data Availability Search</a>

Description                             | Argument
----------------------------------------|---------
Start time: Event origin time           | `-e usgs:usp0006dzc`
End time: Event origin time +30 minutes | `-e usgs:usp0006dzc -d +30m`
Network: XE                             | `-n XE`
Channel: BHZ                            | `-c BHZ`

~~~{.sh}
    fern -D available  -e 'usgs:usp0006dzc' -n XE -d +30m -c BHZ
    ## REQUEST 1/ 1
    DATACENTER=IRISDMC,http://ds.iris.edu
    XE CHIT -- BHZ 1994-06-09T00:33:16 1994-06-09T01:03:16
    XE CHUQ -- BHZ 1994-06-09T00:33:16 1994-06-09T01:03:16
    XE COLL -- BHZ 1994-06-09T00:33:16 1994-06-09T01:03:16
    ...
    XE SICA -- BHZ 1994-06-09T00:33:16 1994-06-09T01:03:16
    XE TACA -- BHZ 1994-06-09T00:33:16 1994-06-09T01:03:16
    XE YUNZ -- BHZ 1994-06-09T00:33:16 1994-06-09T01:03:16
~~~

### <a name="Data-Download-to-miniseed">Data Download to miniseed</a>

Description                             | Argument
----------------------------------------|---------
Download miniseed data                  | `-D miniseed`
Start time from event origin            | `-e usgs:usp0006dzc`
End time from event origin + 30 minutes | `-e usgs:usp0006dzc -d +30m`
Network: XE                             | `-n XE`
Channel: BHZ                            | `-n BHZ`

~~~{.sh}
    fern -D miniseed   -e 'usgs:usp0006dzc' -n XE -d +30m -c BHZ
    Data Center: IRISDMC,http://ds.iris.edu
            Writing data to fdsnws.2019.07.20.10.56.28.IRISDMC.mseed [968.00 KiB]
~~~

### <a name="Data-Download-to-sac">Data Download to sac</a>

Description                             | Argument
----------------------------------------|---------
Download miniseed data                  | `-D sac`
Start time from event origin            | `-e usgs:usp0006dzc`
End time from event origin + 30 minutes | `-e usgs:usp0006dzc -d +30m`
Network: XE                             | `-n XE`
Channel: BHZ                            | `-n BHZ`
Station: DOOR                           | `-s DOOR`

~~~{.sh}
    fern -D sac   -e 'usgs:usp0006dzc' -n XE -v -d +30m -c BHZ -s DOOR
    Data Center: IRISDMC,http://ds.iris.edu
    Working on file: XE.DOOR..BHZ.M.1994.160.003321.sac [ OK ]
            Writing data to XE.DOOR..BHZ.M.1994.160.003321.sac [ 70.73 KiB]
~~~

#### Meta Data Insertion
**Station meta data** is autoomatically set if sac conversion is requested:
    - Station Latitude (stla)
    - Station Longitude (stlo)
    - Station Elevation (stel)
    - Station Depth (stdp) , always 0.0
    - Component Azimuth (cmpaz)
    - Component Inclination (cmpinc)

**Event meta data** is automatically set of an event parameter is used and sac conversion is requested:
    - Event Latitude (evla)
    - Event Longitude (evlo)
    - Event Depth (evdp)
    - Event Elevation (evel), always 0.0
    - Event Name (kevnm)
    - Origin Time (kzdate, kztime)
    - Reference Time (iztype) set to IO, i.e. event origin time
    - All other times are shifted to the reference time
    - The following are computed:
      - Distance (dist)
      - Great Circle Arc (gcarc)
      - Azimuth  (az)
      - Back azimuth (baz)

Examples using libfern
----------------------

#### <a name="event-fern">Event Search with libfern</a>

~~~{.c}
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
~~~

#### <a name="station-fern">Station Search with libfern</a>

~~~{.c}
    // Construct a request
    // Time:     1994/160 - 1994/161
    // Network:  XE
    // Channel:  BH?
    // Region:   -180/180/-90/0
    request *r = station_req_new();
    station_req_set_time_range(r,
                               timespec64_from_yjhmsf(1994, 160, 0, 0, 0, 0),
                               timespec64_from_yjhmsf(1994, 161, 0, 0, 0, 0));
    station_req_set_network(r, "XE");
    station_req_set_channel(r, "BH?");
    station_req_set_region(r, -180.0, 180.0, -90.0, 0.0);
    request_set_verbose(r, 1);
    // Get request and check result
    result *res = request_get(r);
    if(!result_is_ok(res)) {
        return -1;
    }
    // Parse station xml output
    int verbose   = 1;
    int epochs    = 1;
    int show_time = 1;
    station **st = station_xml_parse(result_data(res), result_len(res), epochs, verbose);
    if(st == NULL) {
        return -1;
    }
    // Print out the stations
    stations_write(st, show_time, stdout);
~~~

#### <a name="data-fern">Data Download with libfern</a>

~~~{.c}
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
~~~

Requirements
------------

- libmseed - https://github.com/iris-edu/libmseed
- libsacio (IRIS/LLNL) - http://ds.iris.edu/ds/nodes/dmc/forms/sac/
- libsacio (BSD) - http://https://github.com/savage13/sacio
- libcurl - https://curl.haxx.se/libcurl/ (On most systems)
- libxml2 - http://www.xmlsoft.org/ (On most systems)
- cJSON - https://github.com/DaveGamble/cJSON (Included)
- 64-bit time https://github.com/evalEmpire/y2038 (Included)

Downloading and installing
--------------------------

Please report issues to the project.

If you would like to contribute to the project please file Pull Requests and/or create issues for discussion at the libfern project.

License
-------

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this software except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
