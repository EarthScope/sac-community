
#ifndef _CSV_H_
#define _CSV_H_

#include <stdlib.h>

typedef struct _csv_field csv_field;
typedef struct _csv csv;

void csv_float(char *in, void *meta, size_t off);
void csv_double(char *in, void *meta, size_t off);
void csv_string(char *in, void *meta, size_t off);
void csv_string_p(char *in, void *meta, size_t off);
void csv_datetime(char *in, void *meta, size_t off);

void **csv_parse(char *data, csv *csv);

struct _csv_field {
    char *key;
    void (*parse) (char *p, void *data, size_t off);
    size_t off;
};
struct _csv {
    char delim[2];
    size_t data_size;
    size_t nfields;
    csv_field fields[];
};

#endif /* _CSV_H_ */
