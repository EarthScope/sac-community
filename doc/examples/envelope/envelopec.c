
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sac.h"
#include "sacio.h"

#define MAX 1000

int 
main(int argc, char *argv[]) {

    /* Local variables */
    int nlen, nerr, max;
    float yarray[10000], yenv[10000];
    float beg, delta;
    char *kname;

    max = MAX;

    kname = strdup("raw.sac");
    rsac1(kname, yarray, &nlen, &beg, &delta, &max, &nerr, SAC_STRING_LENGTH);

    if (nerr != 0) {
      fprintf(stderr, "Error reading in file: %s\n", kname);
      exit(-1);
    }

    envelope(nlen, yarray, yenv);

    wsac0("env.sac", yenv, yenv, &nerr, -1);

    if(sac_compare_to_file("env_sac.sac", yenv, 1e-4, 0, 0)) {
        exit(1);
    }

    return 0;
}
