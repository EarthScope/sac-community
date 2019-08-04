/**
 * @file
 * @brief xml specific parsing functions
 */
#include <time.h>
#include <string.h>

#include <sacio/timespec.h>


#include "station.h"
#include "cprint.h"
#include "event.h"
#include "array.h"

#include "chash.h"
#include "strip.h"
#include "xml.h"
#include "defs.h"

/**
 * @brief      find a time value in xml
 *
 * @memberof   xml
 * @ingroup    xml
 *
 * @details    find and return a time value at an xml node
 *
 * @param      x      xml document
 * @param      from   node to start search from, NULL for root node
 * @param      path   xml search path for node
 * @param      key    xml attribute, NULL for none
 * @param      t      output timespec64 value
 *
 * @return     1 on success, 0 on failure
 */
int
xml_find_time(xml *x, xmlNode *from, const char *path, const char *key, timespec64 *t) {
    char *s = NULL;
    if((xml_find_string(x, from, path, key, &s)) &&
       timespec64_parse(s, t)) {
        if(key) {
            FREE(s);
            s = NULL;
        }
        return 1;
    }
    return 0;
}
static int
xml_find_string_eventid(xml *x, xmlNode *from, const char *path, const char *key, char *eventid, size_t n) {
    UNUSED(x);
    UNUSED(path);
    xmlChar *sorg = NULL;
    char *p = NULL, *s = NULL;
    //char **v = (void *) data + q->off;
    if(!(sorg = xmlGetProp(from, (xmlChar *) key))) {
        printf("cannot find publicID in event\n");
        goto error;
    }
    s = (char *) sorg;
    char *kv = NULL;

    if(!(p = strchr(s, '?'))) {
        if(!(p = strchr(s, '/'))) {
            printf("No ? nor / in publicID in event\n");
            goto error;
        }
    }
    s = p;
    s++;
    // Extract the eventID from publicID "URL"
    while((kv = strsep(&s, "&"))) {
        if(strncmp(kv, "eventid=",8) == 0) {
            p = strchr(kv, '=');
            fern_strlcpy(eventid, p+1, n);
            FREE(sorg);
            return 1;
        }
        if(strncmp(kv, "evid=",5) == 0) {
            p = strchr(kv, '=');
            fern_strlcpy(eventid, p+1, n);
            FREE(sorg);
            return 1;
        }
    }

 error:
    fern_strlcpy(eventid, "", n);
    FREE(sorg);
    return 0;
}


static xmlXPathObject *
event_magnitude_from_agency(xml *x, xmlNode *base, char *agency) {
    xmlXPathObject *v = NULL;
    char path[4096] = {0};
    snprintf(path, sizeof(path),
             "q:magnitude/q:creationInfo/q:author[contains(text(),'%s')]/../..", agency);
    if((v = xml_find_all(x, base, (xmlChar *) path))) {
        return v;
    }

    snprintf(path, sizeof(path),
             "q:magnitude/q:creationInfo/q:agencyID[contains(text(),'%s')]/../..", agency);
    if((v = xml_find_all(x, base, (xmlChar *) path))) {
        return v;
    }
    return NULL;
}

static xmlXPathObject *
event_magnitude_from_agency_and_type(xml *x, xmlNode *base, char *agency, char *type) {
    xmlXPathObject *v = NULL;
    char path[4096] = {0};
    snprintf(path, sizeof(path),
             "q:magnitude/q:creationInfo/q:author[contains(text(),'%s')]/../../"
             "q:type[contains(translate(text(),'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'), '%s')]/..",
             agency, type);
    if((v = xml_find_all(x, base, (xmlChar *) path))) {
        return v;
    }
    snprintf(path, sizeof(path),
             "q:magnitude/q:creationInfo/q:agencyID[contains(text(),'%s')]/../../"
             "q:type[contains(translate(text(),'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'), '%s')]/..",
             agency, type);
    if((v = xml_find_all(x, base, (xmlChar *) path))) {
        return v;
    }
    return NULL;
}

static void
event_magnitude(xml *x, xmlNode *base, Event *e, char **agencies, char **mag_types) {
    xmlXPathObject *mags = NULL;
    xmlXPathObject *mags_a = NULL;
    xmlXPathObject *mags_at = NULL;
    xmlNode *mag = NULL;
    if(!(mags = xml_find_all(x, base, (xmlChar *) "q:magnitude"))) {
        return;
    }
    if(xpath_len(mags) == 1) {
        mag = xpath_index(mags, 0);
        goto single;
    }

    for(int i = 0; agencies[i]; i++) {
        // Find authors that contain "author"
        if(!(mags_a = event_magnitude_from_agency(x, base, agencies[i]))) {
            continue;
        }
        // Find Authors that contain "author" and have a "type" Magnitude
        //  convert to upper case for comparison
        for(int j = 0; mag_types[j]; j++) {
            if((mags_at = event_magnitude_from_agency_and_type(x, base, agencies[i], mag_types[j]))) {
                mag = xpath_index(mags_at, 0);
                goto single;
            }
        }
        // Agency found with no appropriate magnitudes, Use first magnitude
        mag = xpath_index(mags_a, 0);
        goto single;
    }
    mag = xpath_index(mags, 0);
    goto single;

 single:
    if(mag) {
        double magvalue = 0.0;
        char magtype[EVENT_MAG_LEN] = {0};
        char magauthor[EVENT_MAG_LEN] = {0};
        xml_find_double(x, mag, "q:mag/q:value", NULL, &magvalue);
        xml_find_string_copy(x, mag, "q:type", NULL, magtype, EVENT_MAG_LEN);
        xml_find_string_copy(x, mag, "q:creationInfo/q:agencyID", NULL, magauthor, EVENT_MAG_LEN);
        xml_find_string_copy(x, mag, "q:creationInfo/q:author", NULL, magauthor, EVENT_MAG_LEN);
        event_set_mag(e, magvalue);
        event_set_magtype(e, magtype);
        event_set_magauthor(e, magauthor);
    }
    xmlXPathFreeObject(mags);
    xmlXPathFreeObject(mags_a);
    xmlXPathFreeObject(mags_at);
}

static xmlXPathObject *
event_origin_from_agency(xml *x, xmlNode *base, char *agency) {
    xmlXPathObject *v = NULL;
    char path[4096] = {0};
    snprintf(path, sizeof(path),
             "q:origin/q:creationInfo/q:author[contains(text(),'%s')]/../..", agency);
    if((v = xml_find_all(x, base, (xmlChar *) path))) {
        return v;
    }
    snprintf(path, sizeof(path),
             "q:origin/q:creationInfo/q:agencyID[contains(text(),'%s')]/../..", agency);
    if((v = xml_find_all(x, base, (xmlChar *) path))) {
        return v;
    }
    return NULL;

}

static void
event_origin(xml *x, xmlNode *base, Event *e, char **agencies) {
    xmlXPathObject *orgs = NULL;
    xmlXPathObject *orgs_a = NULL;
    xmlNode *org = NULL;
    if(!(orgs = xml_find_all(x, base, (xmlChar *) "q:origin"))) {
        return;
    }
    if(xpath_len(orgs) == 1) {
        org = xpath_index(orgs, 0);
        goto single;
    }
    xmlXPathFreeObject(orgs);
    orgs = NULL;
    for(int i = 0; agencies[i]; i++) {
        if((orgs_a = event_origin_from_agency(x, base, agencies[i]))) {
            org = xpath_index(orgs_a, 0);
            goto single;
        }
    }
 single:
    if(org) {
        timespec64 t = {0,0};
        double lat = 0.0, lon = 0.0, depth = 0.0;
        char author[EVENT_ORIGIN_LEN] = {0};
        char catalog[EVENT_ORIGIN_LEN] = {0};

        xml_find_time(x, org, "q:time/q:value", NULL, &t);
        xml_find_double(x, org, "q:latitude/q:value", NULL, &lat);
        xml_find_double(x, org, "q:longitude/q:value", NULL, &lon);
        xml_find_double(x, org, "q:depth/q:value", NULL, &depth);
        xml_find_string_copy(x, org, "q:creationInfo/q:author", NULL, author, EVENT_ORIGIN_LEN);
        xml_find_string_copy(x, org, "q:creationInfo/q:agencyID", NULL, author, EVENT_ORIGIN_LEN);
        xml_find_string_copy(x, org, ".", "catalog", catalog, EVENT_ORIGIN_LEN);
        event_set_time(e, &t);
        event_set_latitude(e, lat);
        event_set_longitude(e, lon);
        event_set_depth(e, depth/1e3);
        event_set_author(e, author);
        event_set_catalog(e, catalog);
    }
    if(orgs) {
        xmlXPathFreeObject(orgs);
        orgs = NULL;
    }
    if(orgs_a) {
        xmlXPathFreeObject(orgs_a);
        orgs_a = NULL;
    }
}

/**
 * @brief      parse xml event data
 *
 * @details    parse xml event data into a collection of Event, encolsed in a \ref xarray
 *             Parsing proceeds as follows:
 *               - All `event` are found
 *               - eventid is from the `publicID` attribute, then `dataid` attribute if needed
 *               - magntiude is parsed
 *                 - all magntiudes are found
 *                 - search for preferred agencies (authors)
 *                   - if none of the agencies are found, use the first magnitude
 *                 - search for preferred magnitude types
 *                   - if none of the matntiudes types are found, use the first magntiude
 *               - origin is parsed
 *                 - find all origins
 *                   - search for origin from preferred agencies
 *                   - if not from preferred agencies, assume not found
 *               - all empty character fields are set to `-`
 *
 * @memberof   Event
 * @ingroup    events
 *
 * @param      data      xml data
 * @param      data_len  length of data
 * @param      verbose   be verbose while parsing
 * @param      cat       catalog to prepend to eventids
 *
 * @return     collection of Events
 *
 */
Event **
quake_xml_parse(char *data, size_t data_len, int verbose, char *cat) {
    size_t n = 0;
    xml *x = NULL;
    xmlXPathObject *evs = NULL;
    Event **out = NULL ; 

    char *agencies[] = { "official", "US", "NEIC", "USGS", "GCMT", "HRVD", "HRV", "ISC",  NULL };
    char *mag_types[] = { "MW", "MS", "MB", "ML", "MD", NULL };

    if(verbose) {
        printf("   Parsing quake.xml data\n");
    }
    if(!(x = xml_new(data, data_len))) {
        goto error;
    }
    if(verbose) {
        printf("   Searching for events\n");
    }
    // Find All Events
    if(!(evs = xml_find_all(x, NULL, (xmlChar *) "//q:event"))) {
        printf("   No events found\n");
        goto error;
    }
    if(verbose) {
        printf("   Parsing %zu events\n", xpath_len(evs));
    }
    n = xpath_len(evs);
    out = xarray_new_with_len('p', n);
    for(size_t i = 0; i < n; i++) {
        char eid[EVENTID_LEN] = {0};
        xmlNode *base = NULL;
        Event *e = event_new();
        memset(eid, 0, EVENTID_LEN);

        if(!(base = xpath_index(evs, i))) {
            printf("   Bad index on event collection :(\n");
            continue;
        }

        xml_find_string_eventid(x, base, ".", "publicID", eid, EVENTID_LEN);
        if(strlen(eid) == 0) {
            xml_find_string_copy(x, base, ".", "dataid", eid, EVENTID_LEN);
        }
        // Get magntiude based on agency and magnitude type
        event_magnitude(x, base, e, agencies, mag_types);
        event_origin(x, base, e, agencies);

        if(strlen(eid) != 0) {
            char tmp[EVENTID_LEN];
            sprintf(tmp, "%s:%s", cat, eid);
            event_set_id(e, tmp);
        }
        event_default(e);
        out[i] = e;

    }

 error:
    XPATH_FREE(evs);
    xml_free(x);
    return out;

}


/*
  ./xpath1 station2.xml 
     "//s:Network[@code='XE']/s:Station[@code='DOOR']/s:Latitude/text()" 
   "s=http://www.fdsn.org/xml/station/1"

*/
