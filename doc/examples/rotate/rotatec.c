

#include <stdio.h>
#include <math.h>
#include <sac.h>

#define NMAX 10000

void
rotate_gcp(float *in1, float *in2, int npts,
           float baz,
           float cmpaz1, float cmpinc1,
           float cmpaz2, float cmpinc2,
           float *out1, float *out2) {

    float delaz;
    double angle;
    int normal;
    if(fabs(cmpinc1 - 90.0) > 0.01 || fabs(cmpinc2 - 90.0) > 0.01) {
        printf("Data are not horizontal\n");
        return;
    }
    // Determine if the files are normal or reversed
    delaz = cmpaz2 - cmpaz1;
    if(fabs(delaz - 90) < 0.01 || fabs(-delaz - 270.0) < 0.01) {
        // delaz = 90 || delaz = -270
        normal = 1;
    } else if(fabs(-delaz - 90) < 0.01 || fabs(delaz - 270) < 0.01) {
        // delaz = -90 || delaz = 270
        normal = 0;
    } else {
        printf("Data are not orthogonal\n");
        return;
    }
    angle = baz + 180.0 - cmpaz1;

    rotate(in1, in2, npts, angle, normal, 1, out1, out2);
}


int
main() {
    // Demonstrate performing a rotation on a pair of signals using the SAC library

    int nmax = NMAX;
    int npts, nerr;
    float signal1[NMAX], signal2[NMAX];
    float rotated_signal1[NMAX], rotated_signal2[NMAX];
    float beg, dt;
    double angle;
    float  baz, cmpaz1;
    int lnpi, lnpo;

    // Read in the two signals to be rotated
    rsac1("signal1.sac", signal1, &npts, &beg, &dt, &nmax, &nerr, -1);
    getfhv("cmpaz", &cmpaz1, &nerr, -1);
    getfhv("baz", &baz, &nerr, -1);

    rsac1("signal2.sac", signal2, &npts, &beg, &dt, &nmax, &nerr, -1);

    // Set up parameters for rotation
    lnpi = 1;     // input signals have "normal" polarity
    lnpo = 1;     // output signals have "normal polarity

    angle = baz + 180.0 - cmpaz1;

    rotate(signal1, signal2, npts, angle, lnpi, lnpo, rotated_signal1, rotated_signal2);

    sac_compare("signalr.sac", rotated_signal1, npts, beg, dt);
    sac_compare("signalt.sac", rotated_signal2, npts, beg, dt);

    // write the seismogram with trend removed back to disk
    wsac0("rotated1.sac", rotated_signal1, rotated_signal1, &nerr, -1);
    wsac0("rotated2.sac", rotated_signal2, rotated_signal2, &nerr, -1);

    return 0;
}
