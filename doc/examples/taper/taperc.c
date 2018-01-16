
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

    sac_compare("taper_sac.sac", data, npts, beg, dt);

    wsac0("taper.sac", data, data, &nerr, -1);

    return 0;
}
