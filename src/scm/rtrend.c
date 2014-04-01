
void
rtrend(float *data, int n, float yint, float slope, float b, float delta) {
  int i;
  float t;
  for(i = 0; i < n; i++) {
    t = b + delta * i;
    data[i] = data[i] - yint - slope * t;
  }
}

void
rtrend2(float *data, int n, float yint, float slope, float *t) {
  int i;
  for(i = 0; i < n; i ++) {
    data[i] = data[i] - yint - slope * t[i];
  }
}


void
rtrend_(float *data, int *n, float *yint, float *slope, float *b, float *delta) {
  rtrend(data,*n,*yint, *slope, *b, *delta);
}
void
rtrend__(float *data, int *n, float *yint, float *slope, float *b, float *delta) {
  rtrend(data,*n,*yint, *slope, *b, *delta);
}

void
rtrend2_(float *data, int *n, float *yint, float *slope, float *t) {
  rtrend2(data, *n, *yint, *slope, t);
}
void
rtrend2__(float *data, int *n, float *yint, float *slope, float *t) {
  rtrend2(data, *n, *yint, *slope, t);
}
