
#include <stdio.h>
#include <sac.h>

#define MAX 10000

int
main() {

    float data[MAX];
    int nmax, npts, nerr, taper_type;
    float beg, dt, width;

    nmax = MAX;

    // Read in the data file
    rsac1("raw.sac", data, &npts, &beg, &dt, &nmax, &nerr, -1);

    // Set up taper parameters
    width = 0.05;    // Width to taper original data
    taper_type = 2;  // HANNING taper

    taper_width(data, npts, taper_type, width);

    wsac0("taperc.sac", data, data, &nerr, -1);

    if(sac_compare_to_file("taper_sac.sac", data, 1e-4, 0, 0)) {
        exit(1);
    }


    return 0;
}
