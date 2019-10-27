
//#define TESTING_URLS
#ifndef TESTING_URLS

#define EVENT_IRIS      "https://service.iris.edu/fdsnws/event/1/query?"
#define EVENT_USGS      "https://earthquake.usgs.gov/fdsnws/event/1/query?"
#define EVENT_ISC       "http://www.isc.ac.uk/fdsnws/event/1/query?"
#define STATION_IRIS    "http://service.iris.edu/fdsnws/station/1/query?"
#define STATION_IRIS_PH5 "http://service.iris.edu/ph5ws/station/1/query?"
#define RESPONSE_SACPZ  "https://service.iris.edu/irisws/sacpz/1/query?"
#define RESPONSE_RESP   "https://service.iris.edu/irisws/resp/1/query?"
#define FEDCATALOG_IRIS "https://service.iris.edu/irisws/fedcatalog/1/query?"

#else

#define EVENT_IRIS      "http://127.0.0.1:5000/irisws/event/1/query?"
#define EVENT_USGS      "http://127.0.0.1:5000/irisws/event/1/query?"
#define EVENT_ISC       "http://127.0.0.1:5000/irisws/event/1/query?"
#define STATION_IRIS    "http://127.0.0.1:5000/irisws/station/1/query?"
#define STATION_IRIS_PH5 "http://127.0.0.1:5000/irisws/station/1/query?"
#define RESPONSE_SACPZ  "http://127.0.0.1:5000/irisws/sacpz/1/query?"
#define RESPONSE_RESP   "http://127.0.0.1:5000/irisws/resp/1/query?"
#define FEDCATALOG_IRIS "http://127.0.0.1:5000/irisws/fedcatalog/1/query?"

#endif
