

#include <stdio.h>
#include <sacio.h>
#include <sac.h>

#define MAX 10000

int
main() {

    int max = MAX;

    float y[MAX], out[MAX];
    float b, dt;
    int nerr, n, nout;

    float cutb = 10.0;
    float cute = 15.0;
    nout = MAX;

    rsac1("raw.sac", y, &n, &b, &dt, &max, &nerr, -1);

    cut(y, n, b, dt, cutb, cute, CUT_FILLZ, out, &nout);

    sac_compare("cut_sac.sac", out, nout, cutb, dt);

    wsac1("cutc.sac", out, &nout, &cutb, &dt, &nerr, -1);

    return 0;
}
