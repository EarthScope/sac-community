
void
rmean(float *data, int n, float mean) {
  int i;
  for(i = 0; i < n; i++) {
    data[i] -= mean;
  }
}


void rmean_(float *data, int *n, float *mean){
  rmean(data,*n,*mean);
}
void rmean__(float *data, int *n, float *mean){
  rmean(data,*n,*mean);
}
