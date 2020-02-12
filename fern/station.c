/**
 * @file
 * @brief Station data
 */
#include <string.h>
#include <stdlib.h>

#include <sacio/timespec.h>

#include "station.h"
#include "array.h"
#include "cprint.h"
#include "request.h"
#include "strip.h"
#include "xml.h"
#include "chash.h"
#include "defs.h"

int xml_find_time(xml *x, xmlNode *from, const char *path, const char *key, timespec64 *t);

/**
 * @defgroup stations stations
 *
 * @brief Station data and requests
 *
 */

/**
 * @brief Initialize Station data
 *
 * @memberof station
 * @ingroup stations
 *
 * @param s   station to initialize
 *
 */
void
station_init(station *s) {
    memset(s->net, 0, sizeof s->net);
    memset(s->sta, 0, sizeof s->sta);
    memset(s->loc, 0, sizeof s->loc);
    memset(s->cha, 0, sizeof s->cha);
    s->stla = 0.0;
    s->stlo = 0.0;
    s->stel = 0.0;
    s->stdp = 0.0;
    s->az = 0.0;
    s->dip = 0.0;
    memset(s->sensor_description, 0, sizeof s->sensor_description);
    s->scale = 0.0;
    s->scale_freq = 0.0;
    memset(s->scale_units, 0, sizeof s->scale_units);
    s->sample_rate = 0.0;
    s->start.tv_sec = 0;
    s->start.tv_nsec = 0;
    s->end.tv_sec = 0;
    s->end.tv_nsec = 0;
    memset(s->sitename, 0, sizeof s->sitename);
}
/**
 * @brief Create a new station struct
 *
 * @memberof station
 * @ingroup stations
 *
 * @return new station struct
 *
 * @warning User owns the data and is responsible for freeing the underlying memory with
 *    \ref station_free
 *
 */
station *
station_new() {
    station *s = calloc(1, sizeof(station));
    station_init(s);
    return s;
}

/**
 * @brief Free a station
 *
 * @memberof station
 * @ingroup stations
 *
 * @param s  station to free
 *
 */
void
station_free(station *s) {
    FREE(s);
}

/**
 * @brief Print the header for a station
 *
 * @memberof station
 * @ingroup stations
 *
 * @param fp          file pointer to print to, can be stdout
 * @param show_times  displace on / off times
 *
 */
void
station_header(FILE *fp, int show_times) {
    if(show_times) {
        cfprintf(fp, "black,bold",
                 "%-3s %-5s %-8s %-9s %-7s %-19s %-19s %s\n",
                 "Net", "Sta",
                 "Lat.", "Lon.", "Elev.",
                 "TimeOn", "TimeOff",
                 "SiteName");


    } else {
        cfprintf(fp, "black, bold",
                 "%-3s %-5s %-8s %-9s %-7s %s\n",
                 "Net", "Sta",
                 "Lat.", "Lon.", "Elev.",
                 "SiteName");
    }
}


/**
 * @brief Convert a station to a string
 *
 * @memberof station
 * @ingroup stations
 *
 * @param s          station to convert
 * @param show_times whether to show on / off times
 * @param dst        output character string
 * @param n          size of dst
 *
 * @return output character string
 *
 */
char *
station_to_string(station *s, int show_times, char *dst, size_t n) {
    char tmp1[64], tmp2[64];
    if(show_times) {
        strftime64t(tmp1, sizeof(tmp1), "%FT%T", &s->start);
        strftime64t(tmp2, sizeof(tmp2), "%FT%T", &s->end);
        snprintf(dst, n,
                 "%-3s %-5s %8.4f %9.4f %7.2f %19s %19s %s",
                 s->net, s->sta,
                 s->stla, s->stlo, s->stel,
                 tmp1, tmp2,
                 s->sitename);
    } else {
        snprintf(dst, n,
                 "%-3s %-5s %8.4f %9.4f %7.2f %s",
                 s->net, s->sta,
                 s->stla, s->stlo, s->stel,
                 s->sitename);
    }
    return dst;
}

station **
station_xml_parse_from_raw(char *data, size_t data_len, int epochs, int verbose) {
    xml *x = NULL;
    station **s = NULL;
    if(verbose) {
        printf("   Parsing station.xml data\n");
    }
    if((x = xml_new(data, data_len))) {
        s = station_xml_parse(x, epochs, verbose);
    }
    xml_free(x);
    return s;
}

int
xml_find_time_or_2599(xml *x, xmlNode *from, const char *path, const char *key, timespec64 *t) {
    if(!xml_find_time(x, from, path, key, t)) {
        timespec64_parse("2599-12-31T23:59:59", t);
    }
    return 1;
}

/**
 * @brief Parse station xml data
 *
 * @memberof station
 * @ingroup stations
 *
 * @param data      station xml meta data
 * @param data_len  length of data
 * @param epochs    if true, assume unique on/off times, else only use net.sta.loc.cha
 * @param verbose   be verbose during parsing
 *
 * @return collection of \ref station, enclosed in a \ref xarray, NULL on error
 *
 * @warning User owns the station collection and is responsible for freeing
 *   the underlying memory with \ref station_free and xarray_free()
 *
 */
station **
station_xml_parse(xml *x, int epochs, int verbose) {
    xmlXPathObject *nets = NULL;
    xmlXPathObject *stas = NULL;
    station **out = NULL;
    dict *d = NULL;
    int err = 0;

    if(!x) {
        return NULL;
    }

    if(verbose) {
        printf("   Searching for networks\n");
    }
    if(!(nets = xml_find_all(x, NULL, (xmlChar *) "//s:Network"))) {
        printf("   No Networks Found\n");
        goto error;
    }

    out = xarray_new('p');
    if(!epochs) {
        d = dict_new();
    }

    for(size_t i = 0; i < xpath_len(nets); i++) {
        char netcode[16] = {0};
        xmlNode *net = xpath_index(nets, i);
        if(verbose) {
            printf("   Searching for station\n");
        }
        if(!(stas = xml_find_all(x, net, (xmlChar *) "s:Station"))) {
            printf("Cound not find stations in network\n");
            continue;
        }
        if(!(xml_find_string_copy(x, net, ".", "code", netcode, sizeof netcode))) {
            printf("Error finding netcode\n");
            continue;
        }
        for(size_t j = 0; j < xpath_len(stas); j++) {
            xmlNode *sta = xpath_index(stas, j);
            station *s = station_new();

            fern_strlcpy(s->net, netcode, sizeof s->net);
            xml_find_string_copy(x, sta, ".", "code", s->sta, sizeof s->sta);
            xml_find_double(x, sta, "s:Latitude", NULL, &s->stla);
            xml_find_double(x, sta, "s:Longitude", NULL, &s->stlo);
            xml_find_double(x, sta, "s:Elevation", NULL, &s->stel);
            xml_find_string_copy(x, sta, "s:Site/s:Name", NULL, s->sitename, sizeof s->sitename);
            xml_find_time(x, sta, ".", "startDate", &s->start);
            xml_find_time_or_2599(x, sta, ".", "endDate", &s->end);

            if(epochs) {
                out = xarray_append(out, s);
            } else {
                char key[128];
                snprintf(key, sizeof(key), "%s.%s", s->net, s->sta);
                if(! dict_get(d, key)) {
                    dict_put(d, key, s);
                } else {
                    if(err == 0) {
                        cprintf("bold,red", "Warning: Multiple instances of net.sta, likely mutiple epochs\n");
                        err = 1;
                    }
                    station_free(s);
                }
            }
        }
        if(stas) {
            xmlXPathFreeObject(stas);
        }
    }
    if(! epochs) {
        char **keys = dict_keys(d);
        int i = 0;
        while(keys[i]) {
            out = xarray_append(out, dict_get(d, keys[i]));
            i++;
        }
        dict_keys_free(keys);
        dict_free(d, NULL);
        d = NULL;
        int net_stat_sort(const void *a, const void *b);
        qsort((void *) out, (size_t)xarray_length(out), sizeof(station *),
              net_stat_sort);
    }
 error:
    XPATH_FREE(nets);
    return out;
}

/**
 * @brief Parse station xml data
 *
 * @memberof station
 * @ingroup stations
 *
 * @param data      station xml meta data
 * @param data_len  length of data
 * @param epochs    if true, assume unique on/off times, else only use net.sta.loc.cha
 * @param verbose   be verbose during parsing
 *
 * @return collection of \ref station, enclosed in a \ref xarray, NULL on error
 *
 * @warning User owns the station collection and is responsible for freeing
 *   the underlying memory with \ref station_free and xarray_free()
 *
 */
station **
channel_xml_parse(xml *x, int verbose) {
    xmlXPathObject *nets = NULL;
    xmlXPathObject *stas = NULL;
    xmlXPathObject *chas = NULL;
    station **out = NULL;

    if(!x) {
        return NULL;
    }

    if(verbose) {
        printf("   Searching for networks\n");
    }
    if(!(nets = xml_find_all(x, NULL, (xmlChar *) "//s:Network"))) {
        printf("   No Networks Found\n");
        goto error;
    }

    out = xarray_new('p');

    for(size_t i = 0; i < xpath_len(nets); i++) {
        char netcode[16] = {0};
        char stacode[16] = {0};
        xmlNode *net = xpath_index(nets, i);
        if(verbose) {
            printf("   Searching for station\n");
        }
        if(!(stas = xml_find_all(x, net, (xmlChar *) "s:Station"))) {
            printf("Cound not find stations in network\n");
            continue;
        }
        if(!(xml_find_string_copy(x, net, ".", "code", netcode, sizeof netcode))) {
            printf("Error finding netcode\n");
            continue;
        }
        for(size_t j = 0; j < xpath_len(stas); j++) {
            xmlNode *sta = xpath_index(stas, j);
            if(!(xml_find_string_copy(x, sta, ".", "code", stacode, sizeof stacode))) {
                printf("Error finding stacode\n");
                continue;
            }
            if(!(chas = xml_find_all(x, sta, (xmlChar *) "s:Channel"))) {
                printf("Cound not find stations in network\n");
                continue;
            }
            for(size_t k = 0; k < xpath_len(chas); k++) {
                station *s = station_new();
                xmlNode *cha = xpath_index(chas, k);

                fern_strlcpy(s->net, netcode, sizeof s->net);
                fern_strlcpy(s->sta, stacode, sizeof s->sta);
                xml_find_string_copy(x, cha, ".", "code", s->cha, sizeof s->cha);
                xml_find_string_copy(x, cha, ".", "locationCode", s->loc, sizeof s->loc);

                xml_find_double(x, cha, "s:Latitude", NULL, &s->stla);
                xml_find_double(x, cha, "s:Longitude", NULL, &s->stlo);
                xml_find_double(x, cha, "s:Elevation", NULL, &s->stel);
                xml_find_double(x, cha, "s:Depth", NULL, &s->stdp);

                xml_find_double(x, cha, "s:Azimuth", NULL, &s->az);
                xml_find_double(x, cha, "s:Dip", NULL, &s->dip);

                xml_find_string_copy(x, cha, "s:Sensor/s:Description", NULL, s->sensor_description, sizeof s->sensor_description);
                xml_find_double(x, cha, "s:Response/s:InstrumentSensitivity/s:Value", NULL, &s->scale);
                xml_find_double(x, cha, "s:Response/s:InstrumentSensitivity/s:Frequency", NULL, &s->scale_freq);
                xml_find_string_copy(x, cha, "s:Response/s:InstrumentSensitivity/s:InputUnits/s:Name", NULL, s->scale_units, sizeof s->scale_units);
                xml_find_double(x, cha, "s:SampleRate", NULL, &s->sample_rate);

                xml_find_time(x, cha, ".", "startDate", &s->start);
                xml_find_time_or_2599(x, cha, ".", "endDate", &s->end);

                xml_find_string_copy(x, sta, "s:Site/s:Name", NULL, s->sitename, sizeof s->sitename);
                out = xarray_append(out, s);
            }
            XPATH_FREE(chas);
        }
        XPATH_FREE(stas);
    }
 error:
    XPATH_FREE(nets);
    return out;
}

/**
 * @brief Write station data collection to a file, could be stdout
 *
 * @memberof station
 * @ingroup stations
 *
 * @param s          collection of stations, assumed encolsed in a \ref xarray
 * @param show_time  whether to display on / off times
 * @param fp         file pointer to write to, could be stdout
 *
 */
void
stations_write(station **s, int show_time, FILE *fp) {
    char tmp[1024] = {0};
    // Print out Station Data
    station_header(fp, show_time);
    for(size_t i = 0; i < xarray_length(s); i++) {
        fprintf(fp, "%s\n", station_to_string(s[i], show_time,
                                              tmp, sizeof(tmp)));
    }
}

/**
 * @brief Sort function for (net, sta) alphanumerically
 *
 * @memberof station
 * @ingroup stations
 *
 * @param a first (net, sta) pair
 * @param b first (net, sta) pair
 *
 * @return 0 if equal, -1 if is a < b, +1 a > b
 */
int
net_stat_sort(const void *a, const void *b) {
    int n = 0;
    const station *pa = *(station **) a;
    const station *pb = *(station **) b;

    if((n = strcmp(pa->net, pb->net)) != 0) {
        return n;
    }
    return strcmp(pa->sta, pb->sta);
}


void
channel_header(FILE *fp) {
    cfprintf(fp, "black, bold",
             "#Network | Station | Channel | Latitude | Longitude | Elevation | Depth"
             "| Azimuth | Dip | SensorDescription | Scale | ScaleFreq | ScaleUnits | SampeRate | StartTime | EndTime\n");
}
char *
channel_to_string(station *s, char *dst, size_t n) {
    char tmp1[64] = {0}, tmp2[64] = {0};
    strftime64t(tmp1, sizeof(tmp1), "%FT%T", &s->start);
    strftime64t(tmp2, sizeof(tmp2), "%FT%T", &s->end);
    snprintf(dst, n, "%s|%s|%s|%s|"
              "%f|%f|%.1f|%.1f|"
              "%.1f|%.1f|"
              "%s|"
              "%E|%.1f|%s|%.3f|"
              "%s|%s",
              s->net, s->sta, s->loc, s->cha,
              s->stla, s->stlo, s->stel, s->stdp,
              s->az, s->dip,
              s->sensor_description,
              s->scale, s->scale_freq, s->scale_units, s->sample_rate,
              tmp1, tmp2);
    return dst;
}
