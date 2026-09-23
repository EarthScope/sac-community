
//#define TESTING_URLS
#ifndef TESTING_URLS

#define EVENT_USGS        "https://earthquake.usgs.gov/fdsnws/event/1/query?"
#define EVENT_ISC         "http://www.isc.ac.uk/fdsnws/event/1/query?"
#define STATION_ESCOPE    "http://service.earthscope.org/fdsnws/station/1/query?"
#define FEDCATALOG_ESCOPE "https://service.earthscope.org/irisws/fedcatalog/1/query?"

#else

#define EVENT_USGS        "http://127.0.0.1:5000/irisws/event/1/query?"
#define EVENT_ISC         "http://127.0.0.1:5000/irisws/event/1/query?"
#define STATION_ESCOPE    "http://127.0.0.1:5000/irisws/station/1/query?"
#define FEDCATALOG_ESCOPE "http://127.0.0.1:5000/irisws/fedcatalog/1/query?"

#endif
