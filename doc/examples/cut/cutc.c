

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

    cut(y, n, b, dt, cutb, cute, SAC_CUT_FILLZ, out, &nout);

    setnhv("npts", &nout, &nerr, -1);
    setfhv("b", &cutb, &nerr, -1);

    wsac0("cutc.sac", out, out, &nerr, -1);

    if(sac_compare_to_file("cut_sac.sac", out, 1e-4, 0, 0) != 0) {
        exit(1);
    }

    return 0;
}
