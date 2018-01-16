
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sacio.h>
#include <sac.h>

#define MAX        4000


int 
main(int argc, char *argv[]) {

    /* Local variables */
    int na, nb, nerr, max, n;

    float beg, delta;
    char *kname;

    float ya[MAX], yb[MAX], yc[MAX];

    max = MAX;

    memset(ya, 0, sizeof(float) * MAX);
    memset(yb, 0, sizeof(float) * MAX);
    memset(yc, 0, sizeof(float) * MAX);

    /* Read in the first file  */
    kname = strdup("convolvec_in1.sac");
    rsac1(kname, ya, &na, &beg, &delta, &max, &nerr, SAC_STRING_LENGTH);

    if (nerr != 0) {
      fprintf(stderr, "Error reading in file(%d): %s\n", nerr, kname);
      exit(-1);
    }
    /* Read in the second file  */
    kname = strdup("convolvec_in2.sac");
    rsac1(kname, yb, &nb, &beg, &delta, &max, &nerr, SAC_STRING_LENGTH);

    if (nerr != 0) {
      fprintf(stderr, "Error reading in file: %s\n", kname);
      exit(-1);
    }

    convolve(ya, na, yb, nb, yc, na+nb-1);

    nerr = sac_compare("convolvec_out_sac1.sac", yc, na+nb-1, 0.0, delta);

    n  = na+nb-1;
    wsac1("convolvec_out.sac", yc, &n, &beg, &delta, &nerr, -1);

    return 0;
}
