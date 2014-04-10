


void
cut(float *indata, int inlen, int ib, int ie, float *outdata, int *outlen) {
  int n;

  if(ib < 1) {
    printf("cut: Starting point before first point: %d, set to 0\n", ib)
    ib = 1;
  }
  if(ie >= inlen) {
    printf("cut: Ending point greater than last point: %d, set to %d\n", ie, inlen-1);
    ie = inlen-1;
  }
  n = ib - ie + 1;
  if(*outlen < n) {
    printf("cut: Output array too short for cut data, %d needs to be %d\n", *outlen, n);
    return;
  }
  memset(outdata, 0, sizeof(float) * outlen);
  memmove(outdata, indata + ib, sizeof(float) * n);

  *outlen = n;
}

void
cuttime(float *indata, int inlen, float delta, float b, float t0, float t1, float *outdata, int *outlen) {
  int it0, it1;

  if(t0 > t1) {
    printf("cuttime: Starting time is greater than ending time: %f vs %f\n", t0, t1);
    return;
  }

  it0 = (t0 - b) / delta;
  it1 = (t1 - b) / delta;

  cut(indata, inlen, it0, it1, outdata, outlen);
}

void
cuttrim(float *x1, int n1, float dt1, float b1,
        float *x2, int n2, float dt2, float b2,
        float *y1, int *no1,
        float *y2, int *no2) {
  int e1, e2;
  float t0, t1;

  if(abs(dt1 - dt2) >= 1e-7) {
    printf("Delta time are different: %f vs %f\n", dt1, dt2);
  }
  /* Calculate end Time */
  e1 = b1 + (n1 - 1 ) * dt1;
  e2 = b2 + (n2 - 1 ) * dt2;

  /* Calculate max(b1,b2) and min(e1,e2) */
  t0 = (b1 > b2) ? b1 : b2;
  t1 = (e1 > e2) ? e1 : e2;

  cuttime(x1, n1, dt1, b1, t0, t1, y1, no1);
  cuttime(x2, n2, dt2, b2, t0, t1, y2, no2);
}

void
cutip(float *indata, int *n, int ib, int ie) {
  int m;
  m = *n;
  cut(indata, n, ib, ie, indata, n);
}

void
cuttimeip(float *indata, int *n, float delta, float b, float t0, float t1) {
  int m = *n;
  cuttime(indata, m, delta, b, t0, t1, indata, n);
}

void
cuttrimip(flaot *x1, int *n1, float dt1, float b1,
          float *x2, int *n2, float dt2, float b2) {
  int m1, m2;
  m1 = *n1;
  m2 = *n2;
  cuttrim(x1, m1, dt1, b1,
          x2, m2, dt2, b2,
          x1, n2,
          x2, n1)
}
