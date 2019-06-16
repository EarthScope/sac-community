
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sac_datetime.h"
#include "array.h"
#include "csv.h"

// External
char *rstrip(char *s);
char *lstrip(char *s);

void
csv_float(char *in, void *meta, size_t off) {
    float *f = (void *) meta + off;
    if(!in || !*in) {
        *f = 0.0;
        return;
    }
    if(sscanf(in, "%f", f) != 1) {
        fprintf(stdout, "csv: Error converting float: '%s'\n", in);
        *f = 0.0;
    }
}
void
csv_double(char *in, void *meta, size_t off) {
    double *d = (void *) meta + off;
    if(!in || !*in) {
        *d = 0.0;
        return;
    }
    if(sscanf(in, "%lf", d) != 1) {
        fprintf(stdout, "csv: Error converting float: '%s'\n", in);
        *d = 0.0;
    }
}
void
csv_string(char *in, void *meta, size_t off) {
    char *p = (void *) meta + off;
    if(!in || !*in) {
        p[0] = 0;
        return;
    }
    strcpy(p, in);
}
void
csv_string_p(char *in, void *meta, size_t off) {
    char *p = (void *) meta + off;
    if(!in || !*in) {
        p = strdup("");
        return;
    }
    p = strdup(in);
}
void
csv_datetime(char *in, void *meta, size_t off) {
    datetime *s = (void *) meta + off;
    if(!in || !*in) {
        datetime_free(s);
        return;
    }
    s = datetime_parse(in, s);
}

int
csv_find_header(csv *csv, char *key) {
    int lfound = -1;
    for(size_t j = 0; j < csv->nfields; j++) {
        if(strcmp(csv->fields[j].key, key) == 0) {
            lfound = j;
            break;
        }
    }
    if(lfound < 0) {
        printf("Encountered unknown Header Field: %s\n", key);
    }
    return lfound;
}

void **
csv_parse(char *data, csv *csv) {
    char *token = NULL;
    char *pline = NULL;
    int hdr[csv->nfields];
    int in_header = FALSE;
    void **cm = xarray_new('p');
    while((pline = strsep(&data, "\n")) != NULL) { // Split on Lines
        in_header = FALSE;
        char *line = rstrip(pline);
        if(strlen(line) == 0) {
            continue;
        }
        void *meta = NULL;
        if(line[0] == '#') {
            in_header = TRUE;
            line++;
        } else {
            meta = calloc(1, csv->data_size);
        }
        int i = 0;
        while((token = strsep(&line, csv->delim)) != NULL) {
            char *p = lstrip(rstrip(token));
            if(in_header) {
                if((hdr[i] = csv_find_header(csv, p)) < 0) {
                    goto ERROR;
                }
            } else {
                csv_field f = csv->fields[ hdr[i] ];
                f.parse(p, meta, f.off);
            }
            i++;
        }
        if(!in_header) {
            cm = xarray_append(cm, meta);
        }
    }
    return cm;
 ERROR:
    xarray_free(cm);
    cm = NULL;
    return NULL;

}

