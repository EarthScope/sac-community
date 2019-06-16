
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "vars/chash.h"

#include "octopus.h"
#include "meta.h"

#define UNUSED(x) (void) x

/// External
#include "array.h"
station_data * station_data_new();
char *rstrip(char *s);

xmlXPathObject * xml_find_all(xmlDoc * doc, xmlNode *from, const xmlChar* path);


typedef struct _quake_to_event quake_to_event;
struct _quake_to_event {
    char *path;
    char *key;
    int (*get) (xmlDoc *doc, xmlNode *from, void *data, quake_to_event *q);
    size_t off;
};

static int
xml_find_datetime_f(xmlDoc *doc, xmlNode *from, void *data, quake_to_event *q) {
    datetime **t = (void *) data + q->off;
    return xml_find_datetime(doc, from, q->path, q->key, t);
}
static int
xml_find_double_f(xmlDoc *doc, xmlNode *from, void *data, quake_to_event *q) {
    double *v = (void *) data + q->off;
    return xml_find_double(doc, from, q->path, q->key, v);
}
static int
xml_find_string_p(xmlDoc *doc, xmlNode *from, void *data, quake_to_event *q) {
    UNUSED(doc);
    char *p = NULL, *s = NULL;
    char **v = (void *) data + q->off;
    if(!(s = (char *) xmlGetProp(from, (xmlChar *) q->key))) {
        printf("cannot find publicID in event\n");
        *v = strdup("");
        return 0;
    }
    if((p = strrchr(s, '='))) { // Extract the eventID from publicID "URL"
        *v = strdup(p+1);
    } else {
        *v = strdup(s);
    }

    return 1;
}
static int
xml_find_string_f(xmlDoc *doc, xmlNode *from, void *data, quake_to_event *q) {
    char *s = NULL;
    char **v = (void *) data + q->off;
    if(!(xml_find_string(doc, from, q->path, q->key, &s))) {
        *v = strdup("");
        return 0;
    }
        //    }
    *v = strdup(s);
    rstrip(*v);
    return 1;
}


quake_to_event q2e[] = {
    {"q:event",                            "publicID", xml_find_string_p,   offsetof(Event, eventid)},
    {"q:origin/q:time/q:value",             NULL,      xml_find_datetime_f, offsetof(Event, time)},
    {"q:origin/q:creationInfo/q:author",    NULL,      xml_find_string_f,   offsetof(Event, author)},
    {"q:origin/q:latitude/q:value",         NULL,      xml_find_double_f,   offsetof(Event, evla)},
    {"q:origin/q:longitude/q:value",        NULL,      xml_find_double_f,   offsetof(Event, evlo)},
    {"q:origin/q:depth/q:value",            NULL,      xml_find_double_f,   offsetof(Event, evdp)},
    {"q:origin",                            "catalog", xml_find_string_f,   offsetof(Event, catalog)},
    {"q:magnitude/q:mag/q:value",           NULL,      xml_find_double_f,   offsetof(Event, mag)},
    {"q:magnitude/q:type",                  NULL,      xml_find_string_f,   offsetof(Event, magtype)},
    {"q:magnitude/q:creationInfo/q:author", NULL,      xml_find_string_f,   offsetof(Event, magauthor)},
};
#define EVENT_LEN (sizeof(q2e) / sizeof(quake_to_event))


xmlDoc*
xml_init(char *data, size_t ndata) {
    xmlDoc *doc = xmlReadMemory(data, ndata, "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        return NULL;
    }
    return doc;
}

size_t
xpath_len(xmlXPathObject* res) {
    if(!res || !res->nodesetval) {
        return 0;
    }
    return res->nodesetval->nodeNr;
}


xmlNode *
xpath_index(xmlXPathObject* v, size_t i) {
    if(!v || !v->nodesetval) {
        return NULL;
    }
    if(i >= (size_t) v->nodesetval->nodeNr) {
        return NULL;
    }
    return v->nodesetval->nodeTab[i];
}

xmlNode *
xml_find(xmlDoc *doc, xmlNode *from, const xmlChar *path) {
    xmlNode * e = NULL;
    xmlXPathObject * result = xml_find_all(doc, from, path);
    size_t n = xpath_len(result);
    if(n == 0) {
        goto error;
    }
    if(n > 1) {
        printf("Multiple [%zu] nodes found\n", n);
        goto error;
    }
    e = result->nodesetval->nodeTab[0];
 error:
    if(result) {
        xmlXPathFreeObject(result);
    }
    return e;
}

xmlNode *
xml_get_text_node(xmlNode * parent) {
    if(parent->type == XML_TEXT_NODE) {
        return parent;
    }
    xmlNode * child = parent->children;
    while(child && child->type != XML_TEXT_NODE) {
        child = child->next;
    }
    return child;
}
int
xml_find_string(xmlDoc *doc, xmlNode *from, const char *path, const char *key, char **s) {
    xmlNode *v = NULL, *txt = NULL;
    *s = NULL;
    if(key) {
        if(xml_find_attr_string(doc, from, path, key, s)) {
            return 1;
        }
    } else {
        if((v = xml_find(doc, from, (xmlChar *)path)) &&
           (txt = xml_get_text_node(v)) &&
           (txt->content)) {
            *s = (char *) txt->content;
            return 1;
        }
    }
    return 0;
}

int
xml_find_datetime(xmlDoc *doc, xmlNode *from, const char *path, const char *key, datetime **t) {
    char *s;
    if((xml_find_string(doc, from, path, key, &s)) &&
       (*t = datetime_parse(s, *t))) {
        return 1;
    }
    return 0;
}

int
xml_find_double(xmlDoc *doc, xmlNode *from, const char *path, const char *key, double *value) {
    char *end = NULL, *s = NULL;
    if(xml_find_string(doc, from, path, key, &s)) {
        *value = strtod(s, &end);
        if(*end == 0 && errno != ERANGE) {
            return 1;
        }
    }
    return 0;
}

int
xml_find_attr_string(xmlDoc *doc, xmlNode *from, const char *path, const char *name, char **s) {
    xmlNode *v = NULL;
    if((v = xml_find(doc, from, (xmlChar *) path)) &&
       (*s = (char *) xmlGetProp(v, (xmlChar *) name))) {
        return 1;
    }
    return 0;
}


xmlXPathObject *
xml_find_all(xmlDoc * doc, xmlNode *from, const xmlChar* path) {
    xmlXPathContext *context = xmlXPathNewContext(doc);
    if (context == NULL) {
        printf("Error in xmlXPathNewContext\n");
        return NULL;
    }
    xmlXPathRegisterNs(context,
                       BAD_CAST "s",
                       BAD_CAST "http://www.fdsn.org/xml/station/1");
    xmlXPathRegisterNs(context,
                       BAD_CAST "q",
                       BAD_CAST "http://quakeml.org/xmlns/bed/1.2");

    xmlXPathObject *result = xmlXPathNodeEval(from, path, context);
    //xmlXPathObjectPtr result = xmlXPathEval(path, context);
    if (result == NULL) {
        printf("Error in xmlXPathNodeEval\n");
        printf("%s\n", path);
        return NULL;
    }
    if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
        xmlXPathFreeObject(result);
        return NULL;
    }
    if (context) {
        xmlXPathFreeContext(context);
    }
    return result;
}

Event **
quake_xml_parse(char *data, size_t data_len, int verbose) {
    xmlDoc *doc;
    xmlXPathObject *evs;
    Event **out = xarray_new('p');
    
    if(verbose) {
        printf("   Parsing quake.xml data\n");
    }
    if(!(doc = xml_init(data, data_len))) {
        printf("Error initializing xml parser\n");
        //*nerr = 3264;
        goto error;
    }
    
    if(verbose) {
        printf("   Searching for events\n");
    }
    if(!(evs = xml_find_all(doc, doc->children, (xmlChar *) "//q:event"))) {
        printf("   No events found\n");
        goto error;
    }
    if(verbose) {
        printf("   Parsing %zu events\n", xpath_len(evs));
    }
    for(size_t i = 0; i < xpath_len(evs); i++) {
        xmlNode *base;
        Event *e = event_new();

        if(!(base = xpath_index(evs, i))) {
            printf("   Bad index on event collection :(\n");
            continue;
        }

        for(size_t j = 0; j < EVENT_LEN; j++) {
            (q2e[j]).get(doc, base, e, &q2e[j]);
        }
        out = xarray_append(out, e);
    }

 error:
    if(doc) {
        xmlFreeDoc(doc);
    }
    return out;

}

xmlNode *
xml_get_nslc(xmlDoc *doc, xmlNode *from, char *net, char *sta, char *loc, char* cha,
              datetime *start, datetime *end) {
    char path[256];
    sprintf(path,
            "//s:Network[@code='%s']"
            "/s:Station[@code='%s']"
            "/s:Channel[@code='%s' and "
            "@locationCode='%s']",
            net, sta, cha, loc);

    xmlXPathObject *objs = xml_find_all(doc, from, (xmlChar *) path);
    if(!objs) {
        printf("Cound not find %s.%s.%s.%s\n", net,sta,loc,cha);
    }
    xmlNode *obj = NULL;
    xmlChar *s0 = NULL;
    xmlChar *s1 = NULL;
    datetime t0, t1;

    for(size_t i = 0; i < xpath_len(objs); i++) {
        if(!(obj = xpath_index(objs, i))) {
            continue;
        }
        if(!(s0 = xmlGetProp(obj, (xmlChar *) "startDate"))) {
            continue;
        }
        if(!(s1 = xmlGetProp(obj, (xmlChar *) "endDate"))) {
            continue;
        }
        if(!(datetime_parse((char *)s0, &t0))) { continue; }
        if(!(datetime_parse((char *)s1, &t1))) { continue; }

        // https://stackoverflow.com/a/325964
        if(datetime_compare(start, &t1, dt_LE) &&
           datetime_compare(end,   &t0, dt_GE)) {
            return obj;
        }
    }
    printf("Could not find %s.%s.%s.%s at specified time\n",
           net, sta, loc, cha);
    return NULL;
}



quake_to_event s2s[] = {
    {".",              "code",     xml_find_string_f,   offsetof(station_data, sta)},
    {"s:Latitude",     NULL,       xml_find_double_f,   offsetof(station_data, stla)},
    {"s:Longitude",    NULL,       xml_find_double_f,   offsetof(station_data, stlo)},
    {"s:Elevation",    NULL,       xml_find_double_f,   offsetof(station_data, stel)},
    {"s:Site/s:Name",  NULL,       xml_find_string_f,   offsetof(station_data, sitename)},
    {".",     "startDate", xml_find_datetime_f, offsetof(station_data, start)},
    {".",     "endDate",   xml_find_datetime_f, offsetof(station_data, end)},
};

station_data **
station_xml_parse(char *data, size_t data_len, int epochs, int verbose) {
    xmlDoc *doc = NULL;
    xmlXPathObject *nets = NULL;
    xmlXPathObject *stas = NULL;
    station_data **out = NULL;
    dict *d = NULL;
    
    if(verbose) {
        printf("   Parsing station.xml data\n");
    }
    if(!(doc = xml_init(data, data_len))) {
        printf("Error initializing xml parser\n");
        goto error;
    }
    if(verbose) {
        printf("   Searching for networks\n");
    }
    if(!(nets = xml_find_all(doc, doc->children, (xmlChar *) "//s:Network"))) {
        printf("   No Networks Found\n");
        goto error;
    }

    out = xarray_new('p');
    if(!epochs) {
        d = dict_new();
    }

    for(size_t i = 0; i < xpath_len(nets); i++) {
        char *netcode = NULL;
        xmlNode *net = xpath_index(nets, i);
        if(verbose) {
            printf("   Searching for station\n");
        }
        if(!(stas = xml_find_all(doc, net, (xmlChar *) "s:Station"))) {
            printf("Cound not find stations in network\n");
            continue;
        }
        if(!(xml_find_string(doc, net, ".", "code", &netcode))) {
            printf("Error finding netcode\n");
            continue;
        }
        for(size_t j = 0; j < xpath_len(stas); j++) {
            xmlNode *sta = xpath_index(stas, j);
            station_data *s = station_data_new();
            s->net = strdup(netcode);

            for(size_t k = 0; k < sizeof(s2s)/sizeof(quake_to_event); k++) {
                (s2s[k]).get(doc, sta, s, &s2s[k]);
            }
            if(epochs) {
                out = xarray_append(out, s);
            } else {
                char *key = NULL;
                asprintf(&key, "%s.%s", s->net, s->sta);
                if(! dict_get(d, key)) {
                    dict_put(d, key, s);
                }
            }
        }
    }
    if(! epochs) {
        char **keys = dict_keys(d);
        int i = 0;
        while(keys[i]) {
            out = xarray_append(out, dict_get(d, keys[i]));
            i++;
        }
        dict_free(d, NULL);
        d = NULL;

        int net_stat_sort(const void *a, const void *b);
        qsort((void *) out, (size_t)xarray_length(out), sizeof(station_data *),
              net_stat_sort);

    }
 error:
    return out;
}

int
net_stat_sort(const void *a, const void *b) {
    int n = 0;
    const station_data *pa = *(station_data **) a;
    const station_data *pb = *(station_data **) b;

    if((n = strcmp(pa->net, pb->net)) != 0) {
        return n;
    }
    return strcmp(pa->sta, pb->sta);
}
