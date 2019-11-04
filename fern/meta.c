/**
 * @file
 * @brief Meta data information
 */
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <sacio/sacio.h>
#include <sacio/timespec.h>

#include "meta.h"
#include "event.h"
#include "datareq.h"
#include "array.h"
#include "cprint.h"
#include "station.h"

#include "xml.h"
#include "slurp.h"
#include "defs.h"
#include "strip.h"
#include "urls.h"


#define KEYN 64 /**< @private */
#define MAXF 20 /**< @private */

/**
 * @defgroup meta meta
 * @brief Meta data requests and insertion
 *
 */

typedef struct meta_data meta_data;
/**
 * @brief channel level meta data
 * @ingroup meta
 * @private
 */
struct meta_data {
    char key[MAXF][KEYN]; /**< @private Key values for meta data */
};


/**
 * @brief Print out a sac floating point value
 *
 * @memberof sac
 *
 * @param s     sac
 * @param name  header name
 * @param hdr   header index
 *
 */
void
sac_show_float(sac *s, char *name, int hdr) {
    double v = 0.0;
    if(!sac_get_float(s, hdr, &v)) {
        return;
    }
    printf("\t%8s     %12.4f\n", name, v);
}
/**
 * @brief Print out a sac string value
 *
 * @memberof sac
 *
 * @param s     sac file
 * @param name  header name
 * @param hdr   header index
 *
 */
void
sac_show_string(sac *s, char *name, int hdr) {
    char v[20];
    if(!sac_get_string(s, hdr, v, sizeof(v))) {
        return;
    }
    printf("\t%8s %16s\n", name, v);
}


/**
 * @brief Compare a sac header value with a string
 *
 * @memberof sac
 *
 * @param sac   sac file
 * @param hdr   header index value
 * @param value character string
 *
 * @return 1 if the strings are the same, 0 if different
 *
 * Comparison is conducted on both the full string and a trimmed string
 * A special case is taken for KHOLE or Location
 *   If the value is "" or "--"  and the khole value is "--      " or "-12345  "
 *   it is considered a match
 */
int
sac_strcmp(sac *s, int hdr, char *value) {
    char h[20] = {0};
    sac_get_string(s, hdr, h, sizeof(h));
    //printf("'%s' <=> '%s'\n", h, value);
    if(hdr == SAC_LOC) {
        if((strcmp(value, "--") == 0 || strlen(value) == 0) &&
           (strcmp(h, "--      ") == 0     || strcmp(h, SAC_CHAR_UNDEFINED) == 0 )) {
            //printf("   Match Loc/Hole Undefined\n");
            return 1;
        }
    }
    if(strcmp(h, value) == 0) {
        //printf("   Match\n");
        return 1;
    }
    fern_rstrip(h);
    //printf("'%s' <=> '%s'\n", h, value);
    if(strcmp(h, value) == 0) {
        //printf("   Match\n");
        return 1;
    }
    //printf("   No match\n");
    return 0;
}

/**
 * @brief Get an double floating point value from a station xml file
 *
 * @memberof xml
 *
 * @param x        xml doc
 * @param s        sac file with header info
 * @param which    sac header value to identify which header value
 * @param value    output floating point value
 *
 * @return 1 on success, 0 on failure
 *
 * @note possible values for which are SAC_STLA, SAC_STLO, SAC_STEL, SAC_STDP, SAC_CMPAZ, SAC_CMPINC
 * @note values for SAC_CMPINC will have 90.0 added to them to convert from SEED format to SAC format
 *
 */
int
station_xml_get_double(xml *x, sac *s, int which, double *value) {
    char start[128] = {0};
    char end[128] = {0};
    timespec64 tb = {0,0};
    timespec64 te = {0,0};
    timespec64 sb = {0,0};
    timespec64 se = {0,0};
    char *fmt =  "//s:Network[@code='%N']/s:Station[@code='%S']"
        "/s:Channel[@locationCode='%H' and @code='%C']";
    char path[2048] = {0};
    char path2[2048] = {0};

    sac_get_time(s, SAC_B, &sb);
    sac_get_time(s, SAC_E, &se);

    sac_fmt(path, sizeof(path), fmt, s);
    xmlXPathObject *objs = xml_find_all(x, NULL, (xmlChar *) path);
    if(!objs) {
        return 0;
    }
    for(size_t i = 0; i < xpath_len(objs); i++) {
        xmlNode *node = xpath_index(objs, i);
        if(!xml_find_string_copy(x, node, ".", "startDate", start, sizeof start) ||
           !xml_find_string_copy(x, node, ".", "endDate", end, sizeof end)) {
            printf("Error finding startDate or endDate in channel for metadata\n");
            printf("%s\n", path);
            continue;
        }
        if(!timespec64_parse(start, &tb) || !timespec64_parse(end, &te)) {
            printf("Error parsing datetime start %s end %s\n", start, end);
            continue;
        }
        if(timespec64_cmp(&tb, &se) <= 0 && timespec64_cmp(&te, &sb) >= 0) {
            memset(path2, 0, sizeof path2);
            switch(which) {
            case SAC_STLA:   fern_strlcat(path2, "s:Latitude", sizeof(path)); break;
            case SAC_STLO:   fern_strlcat(path2, "s:Longitude", sizeof(path)); break;
            case SAC_STEL:   fern_strlcat(path2, "s:Elevation", sizeof(path)); break;
            case SAC_STDP:   fern_strlcat(path2, "s:Depth", sizeof(path)); break;
            case SAC_CMPAZ:  fern_strlcat(path2, "s:Azimuth", sizeof(path)); break;
            case SAC_CMPINC: fern_strlcat(path2, "s:Dip", sizeof(path)); break;
            }
            if(xml_find_double(x, node, path2, NULL, value)) {
                if(which == SAC_CMPINC) {
                    *value = *value + 90;
                }
                return 1;
            } else {
                printf("Error finding %s for %s\n", path2, path);
            }
        }
    }
    return 0;
}

/**
 * @brief Get an floating point value from a station xml file
 *
 * @memberof xml
 *
 * @param x       xml doc
 * @param s       sac file with header info
 * @param which   sac header value to identify which header value
 * @param value   output floating point value
 *
 * @return 1 on success, 0 on failure
 *
 * @note possible values for which are SAC_STLA, SAC_STLO, SAC_STEL, SAC_STDP, SAC_CMPAZ, SAC_CMPINC
 * @note values for SAC_CMPINC will have 90.0 added to them to convert from SEED format to SAC format
 * @note calls station_xml_get_double)
 *
 */
int
station_xml_get_float(xml *x, sac *s, int which, float *value) {
    double v = 0.0;
    int retval = station_xml_get_double(x, s, which, &v);
    *value = (float) v;
    return retval;
}

/**
 * @brief Count the number of characters in a line
 *
 * @private
 * @ingroup meta
 * @memberof meta_data
 *
 * @param line  Line to count within
 * @param c     character to count the number of
 *
 * @return number of c in line
 *
 */
int
count_chars(char *line, char c) {
    int n = 0;
    char *p = line;
    while(p && (p = strchr(p, c))) {
        n++;
        p++;
    }
    return n;
}

/**
 * @brief Parse meta data from a input line
 *
 * @memberof meta_data
 * @ingroup  meta
 * @private
 *
 * @param line line to parse meta data from
 * @param delim  delimiter which separates columes
 *
 * @return meta_data
 *
 * @note values are copied from the line
 *
 */
meta_data *
meta_data_from_line(char *line, char *delim) {
    int i = 0;
    char *tok = NULL;
    meta_data *m = calloc(sizeof(meta_data), 1);
    memset(m->key, 0, MAXF * KEYN);
    while((tok = strsep(&line, delim))) {
        tok = fern_lstrip(fern_rstrip(tok));
        fern_strlcpy(m->key[i], tok, KEYN);
        i++;
        if(i >= MAXF) {
            break;
        }
    }
    return m;
}

/**
 * @brief Check is meta data line matches a sac file
 *
 * @memberof meta_data
 * @ingroup  meta
 * @private
 *
 * @param s  sac file
 * @param m  meta data (net, sta, loc, cha, start, end)
 *
 * @return 1 on a match, 0 on no match
 *
 */
int
sac_matches_nslc(sac *s, meta_data *m) {
    return (sac_strcmp(s, SAC_NET, m->key[0]) &&
            sac_strcmp(s, SAC_STA, m->key[1]) &&
            sac_strcmp(s, SAC_LOC, m->key[2]) &&
            sac_strcmp(s, SAC_CHA, m->key[3]));
}

/**
 * Meta data fields
 *
 */
enum {
    META_NET = 0, /**< @private Network */
    META_STA, /**< @private Station */
    META_LOC, /**< @private Location */
    META_CHA, /**< @private Channel */
    META_STLA, /**< @private Latitude */
    META_STLO, /**< @private Longitude */
    META_STEL, /**< @private Elevation */
    META_STDP, /**< @private Depth */
    META_CMPAZ, /**< @private Component Azimuth */
    META_CMPINC, /**< @private Component Inclination */
    META_KINST, /**< @private Instrument Name */
    META_SCALE, /**< @private  Scale Factor */
    META_SCALE_FREQ, /**< @private  Frequency at Scale Factor */
    META_SCALE_UNITS, /**< @private  Scale Factor Units*/
    META_SAMP_RATE, /**< @private  Sample rate */
    META_START, /**< @private Start time / On */
    META_END, /**< @private  End time / Off */
};

/**
 * @brief Check if a time range matches a meta data line
 *
 * @memberof meta_data
 * @ingroup  meta
 * @private
 *
 * @param sb  start time (typically from a sac file)
 * @param se  end time (typically from a sac file)
 * @param m   meta data line
 *
 * @return 1 on match, 0 on no match
 *
 * @note If sb or se do not exist or they do not exist in the meta data, it is considered a match
 *
 * @note See https://stackoverflow.com/a/325964 for time overlap function
 */

int
sac_matches_time(timespec64 *sb, timespec64 *se, meta_data *m) {
    timespec64 tb = {0,0} , te = {0,0};
    // sac times do not exist
    if(!se || !sb) {
        return 1;
    }
    // meta data time fields do not exist
    if(strlen(m->key[META_START]) == 0 || strlen(m->key[META_END]) == 0) {
        return 1;
    }
    // error parsing meta data keys
    if(!timespec64_parse(m->key[META_START], &tb)) {
        printf("Error parsing start time: '%s'\n", m->key[META_START]);
        return 1;
    }
    if(!timespec64_parse(m->key[META_END], &te)) {
        printf("Error parsing end time: '%s'\n", m->key[META_END]);
        return 1;
    }
    // Date Times overlap
    if(timespec64_cmp(&tb, se) <= 0 && timespec64_cmp(&te, sb) >= 0) {
        return 1;
    }
    // Date Times do not overlap
    return 0;
}

/**
 * @brief Fill meta data for a collection of sac files
 *
 * @memberof meta_data
 * @ingroup  meta
 * @private
 *
 * @param files    sac files, must be wrapped in an \ref xarray
 * @param data     xml meta data
 * @param ndata    length of data
 * @param verbose  be verbose while parsing and setting
 *
 * @return 1 on success, 0 on error
 *
 * @note fields filled are:
 *    - stla - Station Latitude
 *    - stlo - Station Longitude
 *    - stel - Station Elevation
 *    - stdp - Station Depth
 *    - cmpaz - Component Azimuth
 *    - cmpinc - Component Inclination
 */
int
sac_fill_meta_data_from_xml(sac **files, xml *x, int verbose) {
    sac *s = NULL;

    char *key[] = {"stla", "stlo", "stel",
                   "stdp", "cmpaz", "cmpinc" };
    int fid[] = { SAC_STLA, SAC_STLO, SAC_STEL,
                  SAC_STDP, SAC_CMPAZ, SAC_CMPINC };
    if(!x) {
        return 0;
    }
    for(size_t i = 0; i < xarray_length(files); i++) {
        s = files[i];
        cprintf("black,bold", "Working on file: ");
        printf("%s", s->m->filename);
        if(verbose) {
            printf("\n");
        }
        int rv = 0;
        for(int j = 0; j < 6; j++) {
            int rvi = 0;
            float v = 0.0;
            if((rvi = station_xml_get_float(x, s, fid[j], &v))) {
                sac_set_float(s, fid[j], v);
            }
            rv += rvi;
        }
        if(!verbose) {
            printf(" [ ");
            if(rv == 6) {
                cprintf("green,bold", "OK");
            } else {
                cprintf("red,bold", "Error %d/%d", rv, 6);
            }
            printf(" ]\n");
        } else {
            for(int j = 0; j < 6; j++) {
                sac_show_float(s, key[j], fid[j]);
            }
        }
    }
    return 1;
}

/**
 * @brief Parse station meta data from a file
 *
 * @memberof meta_data
 * @ingroup  meta
 * @private
 *
 * @param file         filename to get meta data from
 * @param verbose      be verbose in parsing
 * @param seed_cmpinc  output value to add to Component Inclincation
 *
 * @return \ref meta_data
 *
 * @note From mseed2sac, https://github.com/iris-edu/mseed2sac:
 *     - When comma separators are used the dip field (CMPINC) is assumed to be in
 *       the SAC convention (degrees down from vertical up/outward), if vertical bars
 *       are used the dip field is assumed to be in the SEED convention (degrees down
 *       from horizontal) and converted to SAC convention.
 *
 *     - delimiter = , (comma)     ==> seed_cmpinc =  0.0
 *     - delimiter = | (bar/pipe)  ==> seed_cmpinc = 90.0
 *
 *     - See option -msi in mseed2sac:
 *        - Convert any component inclination values in a metadata file from SEED (dip)
 *          to SAC convention, this is a simple matter of adding 90 degrees.
 *
 */
meta_data **
station_meta_parse(char *file, int verbose, float *seed_cmpinc) {
    char *p = NULL;
    char line[4096] = {0};
    FILE *fp = NULL;
    char delim[2] = {0};
    int nfields = 0;
    if(!(fp = fopen(file, "r"))) {
        printf("Error: File does not exist: %s\n", file);
        return NULL;
    }
    meta_data **ms = xarray_new('p');

    while(fgets(line, sizeof(line), fp)) {
        p = line;
        if(line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = 0;
        }
        while(isspace(*p)) {
            p++;
        }
        if(*p == '#') {
            if(verbose) {
                printf("meta-data: Skipping comment: %s\n", line);
            }
            continue;
        }
        if(delim[0] == 0) {
            if(strchr(line,'|')) {
                delim[0] = '|'; // miniseed (inclination)
                *seed_cmpinc = 90.0;
            } else {
                delim[0] = ','; // sac convection (dip)
                *seed_cmpinc = 0.0;
            }
        }
        nfields = count_chars(line, delim[0]);
        if(nfields < 4) {
            printf("meta-data: Skipping line, too few fields [%d]: %s\n", nfields, line);
        }
        meta_data *m = meta_data_from_line(line, delim);
        ms = xarray_append(ms, m);
    }
    return ms;
}

#define IS_OK(v) do {               \
    if(!v) {                        \
        printf("[ ");               \
        cprintf("green,bold","OK"); \
        printf(" ]");               \
    }                               \
    printf("\n");                   \
    } while(0);

/**
 * @brief Report no meta data could be found for a sac file
 *
 * @private
 *
 * @memberof meta_data
 * @ingroup  meta
 *
 * @param s        sac file to complain about
 * @param verbose  be verbose in complaining
 *
 */
static
void no_meta_data(sac *s, int verbose) {
    char nslc[64] = {0};
    if(verbose) {
        sac_fmt(nslc, sizeof(nslc), "%Z", s);
        cprintf("", "\n");
        cprintf("red,bold", "\tWARNING: Could not find meta data for %s\n", nslc);
    } else {
        printf("[ ");                           \
        cprintf("red,bold","No Metadata");      \
        printf(" ]\n");                         \
    }
}
/**
 * @brief Find a match within a meta data collection
 *
 * @memberof meta_data
 * @ingroup  meta
 * @private
 *
 * @param ms  \ref meta_data collection, must be enclosed in an \ref xarray
 * @param s   sac file to find meta data for
 *
 * @return matching meta_data
 *
 * @note meta data is matched on Network, Station, Location, Channel, On and Off times
 */
meta_data *
meta_data_find_match(meta_data **ms, sac *s) {
    timespec64 sb = {0,0} , se = {0,0};
    meta_data *mi = NULL;
    size_t n = 0;
    if(!ms || !s) {
        goto done;
    }
    n = xarray_length(ms);
    if(n == 0) {
        goto done;
    }
    sac_get_time(s, SAC_B, &sb);
    sac_get_time(s, SAC_E, &se);

    for(size_t j = 0; j < n; j++) {
        meta_data *m = ms[j];
        if( sac_matches_nslc(s, m) && sac_matches_time(&sb, &se, m)) {
            mi = m;
            goto done;
        }
    }
 done:
    return mi;
}

/**
 * @brief Fill meta data for a collection of sac files from a text or XML file
 *
 * @memberof meta_data
 * @ingroup  meta
 *
 * @param files     collection of sac files, must be a enclosed in an \ref xarray
 * @param verbose   be verbose about setting meta data
 * @param file      file to take data from, can be either an xml file or text file
 *
 */
void
sac_array_fill_meta_data_from_file(sac **files, int verbose, char *file) {
    float seed_cmpinc = 0.0;
    char *endptr = NULL;
    char *fields[] = {"net","sta","loc","cha",
                      "stla", "stlo","stel","stdp",
                      "cmpaz","cmpinc","kinst"};
    int fid[] = {SAC_NET,SAC_STA,SAC_LOC,SAC_CHA,
                 SAC_STLA, SAC_STLO, SAC_STEL, SAC_STDP, SAC_CMPAZ,
                 SAC_CMPINC, SAC_INST};
    meta_data **ms = NULL;

    if(is_xml_file(file)) {
        size_t n = 0;
        char *data = NULL;
        xml *x = NULL;
        data = slurp(file, &n);
        if(!data || (x = xml_new(data, n)) == NULL) {
            return;
        }
        sac_fill_meta_data_from_xml(files, x, verbose);
        xml_free(x);
        FREE(data);
        return;
    }

    ms = station_meta_parse(file, verbose, &seed_cmpinc);
    if(!ms) {
        return;
    }

    for(size_t i = 0; i < xarray_length(files); i++) {
        meta_data *m = NULL;
        sac *s = files[i];
        cprintf("black,bold", "Working on file: ");
        cprintf("","%s ", s->m->filename);
        if(!(m = meta_data_find_match(ms, s))) {
            no_meta_data(s, verbose);
            continue;
        }
        IS_OK(verbose);
        for(size_t k = 4; k < sizeof(fid)/sizeof(int); k++) {
            int f = fid[k];
            if(strlen(m->key[k]) > 0) {
                if(f == SAC_KINST) {
                    sac_set_string(s, f, m->key[k]);
                    if(verbose) {
                        sac_show_string(s, fields[k], f);
                    }
                } else {
                    double v = 0.0;
                    v = strtod(m->key[k], &endptr);
                    if(f == SAC_CMPINC) {
                        v += seed_cmpinc;
                    }
                    sac_set_float(s, f, v);
                    if(verbose) {
                        sac_show_float(s, fields[k], f);
                    }
                }
            }
        }
    }
    for(size_t i = 0; i < xarray_length(ms); i++) {
        FREE(ms[i]);
    }
    xarray_free(ms);
    ms = NULL;
}

xml *
xml_merge_results(result *r1, result *r2, char *path) {
    xml *x1 = NULL;
    xml *x2 = NULL;
    // If neither request is ok, return with error
    if(!result_is_ok(r1) && !result_is_ok(r2)) {
        if(r1) {
            printf("%s\n", result_error_msg(r1));
        } else if(r2) {
            printf("%s\n", result_error_msg(r2));
        } else {
            printf("Error getting station data\n");
        }
        goto done;
    }

    // Convert requested data to xml
    if(r1 && result_is_ok(r1) && !(x1 = xml_new(result_data(r1), result_len(r1)))) {
        printf("Error parsing xml\n");
        goto done;
    }
    if(r2 && result_is_ok(r2) && !(x2 = xml_new(result_data(r2), result_len(r2)))) {
        printf("Error parsing xml\n");
        xml_free(x1);
        goto done;
    }

    if(!x1 && !x2) {
        // If neither xml exists, return
        return NULL;
    } else if(x1 && x2 == NULL) {
        // Do nothing, if only 1st xml exists
    } else if(x1 == NULL && x2) {
        // Shift the 2nd xml then 1st, if it only exists
        x1 = x2;
        x2 = NULL;
    } else if(x1 && x2) {
        // Merge if both xml exist into the 1st, at network level
        xml_merge(x1, x2, path);
    }
 done:
    xml_free(x2);
    return x1;
}

/**
 * @brief Fill meta data for a collection of sac files by request
 *
 * @memberof meta_data
 * @ingroup  meta
 *
 * @param files    collection of sac files, must be enclosed in a \ref xarray
 * @param verbose  be verbose when setting meta data
 * @param ph5      get data also from ph5 web service
 *
 * @return 1 always
 *
 * @note meta data is requested using IRIS Station request, level = channel
 *
 */
int
sac_array_fill_meta_data(sac **files, int verbose, int ph5) {
    sac *s = NULL;
    request *sm = NULL;
    result *r[2] = {NULL,NULL};
    char *data = NULL;
    size_t nalloc = 2048;
    size_t n = 0;
    char line[2048] = {0};
    xml *x = NULL;

    // Station Meta Request Build
    data = str_grow(data, &nalloc, n, strlen("level=channel\n"));
    n = fern_strlcat(data, "level=channel\n", nalloc);
    for(size_t i = 0; i < xarray_length(files); i++) {
        s = files[i];
        if(! sac_hdr_defined(s, SAC_NET, SAC_STA, SAC_CHA, NULL) ) {
            printf("Insufficient net,sta,cha,time to retrieve station meta data\n");
            continue;
        }
        sac_fmt(line, sizeof(line), "%R\n", s);
        data = str_grow(data, &nalloc, n, strlen(line));
        n = fern_strlcat(data, line, nalloc);
    }

    // Request Station Meta Data
    sm = request_new();
    request_set_verbose(sm, verbose);

    request_set_url(sm, STATION_IRIS);
    r[0] = request_post(sm, data);

    if(ph5) {
        request_set_url(sm, STATION_IRIS_PH5);
        r[1] = request_post(sm, data);
    }

    if(!(x = xml_merge_results(r[0], r[1], "//s:Network"))) {
        goto error;
    }

    sac_fill_meta_data_from_xml(files, x, verbose);

 error:
    xml_free(x);
    RESULT_FREE(r[0]);
    RESULT_FREE(r[1]);
    REQUEST_FREE(sm);
    FREE(data);
    return 1;
}

/**
 * @brief Fill meta data associated with an event in multiple sac file
 *
 * @memberof meta_data
 * @ingroup  meta
 *
 * @param s        sac files, encolsed in an xarray
 * @param ev       event data, if NULL, nothing is done
 * @param verbose  be verbose when setting meta data
 *
 * @note Fields set are
 *    - evla - Event Latitude
 *    - evla - Event Longitude
 *    - evla - Event Depth
 *    - evla - Event Elevation - Always 0.0
 *    - kevnm - Event ID - Only if <= 16 characters
 *    - kzdate and kztime - Event Origin time
 *    - iztype = IO (Origin)
 *
 * @note All time picks are shifted so the origin time is 0.0, See sac_set_time()
 */
void
sac_array_fill_meta_data_from_event(sac **s, Event *ev, int verbose) {
    for(size_t i = 0; i < xarray_length(s); i++) {
        sac_fill_meta_data_from_event(s[i], ev, verbose);
    }
}

/**
 * @brief Fill meta data associated with an event in a sac file
 *
 * @memberof meta_data
 * @ingroup  meta
 *
 * @param s        sac file
 * @param ev       event data, if NULL, nothing is done
 * @param verbose  be verbose when setting meta data
 *
 * @note Field set are
 *    - evla - Event Latitude
 *    - evla - Event Longitude
 *    - evla - Event Depth
 *    - evla - Event Elevation - Always 0.0
 *    - kevnm - Event ID - Only if <= 16 characters
 *    - kzdate and kztime - Event Origin time
 *    - iztype = IO (Origin)
 *
 * @note All time picks are shifted so the origin time is 0.0, See sac_set_time()
 */
void
sac_fill_meta_data_from_event(sac *s, Event *ev, int verbose) {

    if(ev) {
        if(verbose) {
            printf("Setting event parameters from '%s'\n", event_id(ev));
        }
        sac_set_float(s, SAC_EVLA, event_lat(ev));
        sac_set_float(s, SAC_EVLO, event_lon(ev));
        sac_set_float(s, SAC_EVDP, event_depth(ev));
        sac_set_float(s, SAC_EVEL, 0.0);

        if(strlen(event_id(ev)) <= 16) {
            sac_set_string(s, SAC_EVENT, event_id(ev));
        } else {
            cprintf("red,bold",
                    "Warning: eventid too long (%lu) to store in header\n",
                    strlen(event_id(ev)));
            fern_strlcpy(s->h->kevnm, SAC_CHAR_UNDEFINED_2, strlen(s->h->kevnm));
        }
        sac_set_time(s, event_time(ev));

        if(verbose) {
            char *ekey[] = {"evla","evlo","evdp","evel","kevnm"};
            int eid[] = {SAC_EVLA, SAC_EVLO, SAC_EVDP, SAC_EVEL, SAC_EVENT};
            for(int j = 0; j < 4; j++) {
                sac_show_float(s, ekey[j], eid[j]);
            }
            sac_show_string(s, ekey[4], eid[4]);

            char dst[128] = {0};
            timespec64 t = event_time(ev);
            strftime64t(dst, sizeof(dst), "%FT%T.%3f", &t);
            printf("\t%8s     %s\n", "origin", dst);
        }
    }
}

