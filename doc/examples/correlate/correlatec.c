
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sac.h>
#include <assert.h>


void
vprint(char *pre, float *v, int n, float dt, float b) {
    int i;
    printf("%s n: %d\n", pre, n);
    for(i = 0; i < n; i++) {
        if(fabs(v[i]) > 1e-5) {
            printf("%5s %d: %f (amp) %f (time) \n", "", i,v[i], correlate_time(dt, b, i));
        }
    }
}

void
correlate_files(char *file1, char *file2, float amp_max, float t_max, int verbose) {
    float tolerance = 1e-6;
    int i,n;
    float a[1024];
    float b[1024];
    float *c;
    float ba, bb, bc;
    int na, nb, nmax, nerr;
    float dt;
    nmax = 1024;

    if(verbose) {
        printf("correlate files: %s %s\n", file1, file2);
    }
    rsac1(file1, a, &na, &ba, &dt, &nmax, &nerr, -1);
    if(nerr) {
        printf("Error reading file: %s nerr: %d \n", file1, nerr);
        exit(1);
    }
    rsac1(file2, b, &nb, &bb, &dt, &nmax, &nerr, -1);
    if(nerr) {
        printf("Error reading file: %s nerr: %d \n", file2, nerr);
        exit(1);
    }
    n = na+nb-1;
    c = calloc(n, sizeof(float));
    // Compute Cross Correlation Function
    correlate(a, na, b, nb, c, n);
    // Start time of cross correlation function
    bc = correlate_time_begin(dt, na, nb, ba, bb);
    // Compute maximum value of cross correlation function
    i = correlate_max(c, n);
    if(verbose) {
        vprint("a", a,na,dt,ba);
        vprint("b", b,nb,dt,bb);
        vprint("c", c,n,dt,bc);
        printf("max time %f amp: %f\n", correlate_time(dt, bc, i), c[i]);
    }
    // Check result versus expected value (within a tolerance)
    assert(fabs(correlate_time(dt, bc, i) - t_max) < tolerance);
    assert(fabs(c[i] - amp_max) < tolerance);
}

int
main() {

    correlate_files("imp02.sac", "imp06.sac", 1.0,  0.4, 0);
    correlate_files("imp02.sac", "imp12.sac", 1.0,  1.0, 0);
    correlate_files("imp02.sac", "imp02n.sac",1.0, -1.0, 0);
    correlate_files("imp02n.sac", "imp06.sac",1.0,  1.4, 0);
    correlate_files("imp02n.sac", "imp12.sac",1.0,  2.0, 0);

    correlate_files("imp06.sac", "imp02.sac", 1.0, -0.4, 0);
    correlate_files("imp12.sac", "imp02.sac", 1.0, -1.0, 0);
    correlate_files("imp02n.sac", "imp02.sac",1.0,  1.0, 0);
    correlate_files("imp06.sac", "imp02n.sac",1.0, -1.4, 0);
    correlate_files("imp12.sac", "imp02n.sac",1.0, -2.0, 0);

    return 0;
}
