
void
int_trap(float *y, int npts, double delta) {
    int i;
    double hstep, totint, prtint;

    hstep = 0.5 * delta;
    totint = 0.0;
    for(i = 0; i < npts-1; i++) {
        prtint = hstep * (y[i] + y[i+1]);
        totint = totint + prtint;
        y[i] = totint;
    }
}

void
int_rect(float *y, int npts, double delta) {
    int i;
    y[0] = delta * y[0];
    for(i = 1; i < npts; i++) {
        y[i] = delta * y[i] + y[i-1];
    }
}

void
int_trap_(float *y, int *npts, double *delta) {
    int_trap(y, *npts, *delta);
}
void
int_trap__(float *y, int *npts, double *delta) {
    int_trap(y, *npts, *delta);
}
void
int_rect_(float *y, int *npts, double *delta) {
    int_rect(y, *npts, *delta);
}
void
int_rect__(float *y, int *npts, double *delta) {
    int_rect(y, *npts, *delta);
}
