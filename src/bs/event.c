
#include <stdlib.h>

#include "array.h"
#include "octopus.h"

#define FREE(x) do {  \
        if(x) {       \
            free(x);  \
            x = NULL; \
        }             \
    } while(0);


void
event_init(Event *e) {
    e->eventid = NULL;
    e->time = NULL;
    e->author = NULL;
    e->evla = 0.0;
    e->evlo = 0.0;
    e->evdp = 0.0;
    e->catalog = NULL;
    e->mag = 0.0;
    e->magtype = NULL;
    e->magauthor = NULL;
}
Event *
event_new() {
    Event *e = malloc(sizeof(Event));
    event_init(e);
    return e;
}

void
event_print(Event *e, FILE *fp) {
    char tmp[256];
    fprintf(fp, "%-10s %s %8.3f %9.4f %7.2f %6s %5.2f %4s %6s %6s\n",
            e->eventid, datetime_to_iso8601(e->time, tmp),
            e->evla, e->evlo, e->evdp/1e3, e->author,
            e->mag, e->magtype, e->magauthor,
            e->catalog);
}

void
events_write(Event **ev, FILE *fp) {
    fprintf(fp, "EventID    Origin                Lat.    Lon.       Depth  Author Mag.  Type  Author  Catalog\n");
    for(size_t i = 0; i < xarray_length(ev); i++) {
        event_print(ev[i], fp);
    }
}
int
events_write_to_file(Event **ev, char *file) {
    FILE *fp;
    if(!(fp = fopen(file, "w"))) {
        printf("Error open file for writing: %s\n", file);
        return 0;
    }
    events_write(ev, fp);
    fclose(fp);
    return 1;
}

void
event_free(Event *e) {
    if(e) {
        FREE(e->eventid);
        if(e->time) {
            datetime_free(e->time);
        }
        FREE(e->author);
        FREE(e->catalog);
        FREE(e->magtype);
        FREE(e->magauthor);
        free(e);
        e = NULL;
    }
}

