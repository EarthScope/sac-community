
#include <stdio.h>

#include <sacio/timespec.h>

#include "request.h"
#include "xml.h"

typedef struct station station;

/**
 * @brief station level meta data
 * @ingroup Stations
 */
struct station {
    char net[8];
    char sta[8];
    char loc[8]; //
    char cha[8];//
    double stla;
    double stlo;
    double stel;
    double stdp;//
    double az;//
    double dip; //// Angle from horizontal (-90 is up)
    char sensor_description[1024]; //
    double scale; //
    double scale_freq; //
    char scale_units[64]; //
    double sample_rate; //
    timespec64 start;
    timespec64 end;
    char sitename[1024];
};


station *  station_new();
void       station_free(station *s);
void       station_header(FILE *fp, int show_times);
char *     station_to_string(station *s, int show_times, char *dst, size_t n);
station ** station_xml_parse_from_raw(char *data, size_t data_len, int epochs, int verbose);
station ** station_xml_parse(xml *x, int epochs, int verbose);
void       stations_write(station **s, int show_time, FILE *fp);

station ** channel_xml_parse(xml *x, int verbose);
void       channel_header(FILE *fp);
char *     channel_to_string(station *s, char *dst, size_t n);
