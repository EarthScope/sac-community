
#include <stdio.h>
#include <stdlib.h>

#include "slurp.h"
#include "defs.h"

char *
slurp(char *file, size_t *np) {
    size_t n = 0;
    char *data = NULL;
    FILE *fp = NULL;
    if(!(fp = fopen(file, "r"))) {
        printf("Error opening file: %s\n", file);
        return NULL;
    }
    fseek (fp, 0, SEEK_END);
    n = (size_t) ftell (fp);
    fseek (fp, 0, SEEK_SET);
    data = calloc(n, sizeof(char));
    if(fread (data, 1, n, fp) != n) {
        FREE(data);
        return NULL;
    }
    fclose(fp);
    *np = n;
    return data;
}

