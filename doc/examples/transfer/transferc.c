
#include <stdio.h>
#include <evalresp.h>
#include <sac.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define NMAX 10000


void
test_revalresp() {

    float y[NMAX], b, dt;
    int n, nmax, nerr;

    double limits[4] = { 0.002, 0.005, 12.0, 20.0 };

    /* Read in Raw Sac file */
    nmax = NMAX;
    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);
    if(nerr) {
        printf("Error reading file: %s nerr: %d \n", "raw.sac", nerr);
        return;
    }

    if(remove_evalresp_simple(y, n, dt, limits)) {
        printf("Error removing instrument with evalresp\n");
        return;
    }

    // write the deconvolved seismogram back to disk
    wsac0("deconvolved_evr.sac", NULL, y, &nerr,-1);

    // Check input/output
    sac_compare("deconvolved_from_evr.sac",y,n,b,dt);
    return;
}


void
test_rpolezero() {
    #define NMAX 10000

    float y[NMAX], b, dt;
    int n, nmax, nerr;

    double limits[4] = { 0.002, 0.005, 12.0, 20.0 };

    /* Read in Raw Sac file */
    nmax = NMAX;
    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);
    if(nerr) {
        printf("Error reading file: %s nerr: %d \n", "raw.sac", nerr);
        return;
    }

    if(remove_polezero_simple(y, n, dt, limits)) {
        printf("Error removing instrument with polezero\n");
        return;
    }

    // write the deconvolved seismogram back to disk
    wsac0("deconvolved_pz.sac", NULL, y, &nerr, -1);

    // Check input/output
    sac_compare("deconvolved_from_pz.sac", y,n,b,dt);
}

int
main() {
    printf("test evalresp\n");
    test_revalresp();
    printf("test polezero\n");
    test_rpolezero();
    return 0;
}
