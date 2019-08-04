
#include <stdio.h>

#include <sacio/timespec.h>

#include "request.h"

typedef struct station station;

/**
 * @brief station level meta data
 * @ingroup Stations
 */
struct station {
    char *net;
    char *sta;
    double stla;
    double stlo;
    double stel;
    char *sitename;
    timespec64 start;
    timespec64 end;
};


station *  station_new();
void       station_free(station *s);
void       station_header(FILE *fp, int show_times);
char *     station_to_string(station *s, int show_times, char *dst, size_t n);
station ** station_xml_parse(char *data, size_t data_len, int epochs, int verbose);
void       stations_write(station **s, int show_time, FILE *fp);

