
void
interp(float *in, int nlen, float *out, int newlen, float bval, float eval, float dt, float tstart, float dtnew, float eps) {
  int j;
  float xnew;
  for( j = 0; j <= (newlen - 1); j++ ){
    xnew = tstart + (j  * dtnew);
    if( xnew >= bval && xnew <= eval ){
      wigint( (float*)&bval, in, nlen, dt, eps, xnew, &out[j]);
    }
  }
}

void
interp2(float *in, int nlen, float *out, int newlen, float bval, float eval, float *t, float tstart, float dtnew, float eps) {
  int j;
  float xnew;
  for(j = 0; j < newlen; j++){
    xnew = tstart + (j  * dtnew);
    if(xnew >= bval && xnew <= eval) {
      wigint( t, in, nlen, 0.0, eps, xnew, &out[j] );
    }
  }
}

void
interp_(float *in, int *nlen, float *out, int *newlen, float *bval, float *eval, float *dt, float *tstart, float *dtnew, float *eps) {
  interp(in, *nlen, out, *newlen, *bval, *eval, *dt, *tstart, *dtnew, *eps);
}

void
interp__(float *in, int *nlen, float *out, int *newlen, float *bval, float *eval, float *dt, float *tstart, float *dtnew, float *eps) {
  interp(in, *nlen, out, *newlen, *bval, *eval, *dt, *tstart, *dtnew, *eps);
}
void
interp2_(float *in, int *nlen, float *out, int *newlen, float *bval, float *eval, float *t, float *tstart, float *dtnew, float *eps) {
  interp2(in, *nlen, out, *newlen, *bval, *eval, t, *tstart, *dtnew, *eps);
}
void
interp2__(float *in, int *nlen, float *out, int *newlen, float *bval, float *eval, float *t, float *tstart, float *dtnew, float *eps) {
  interp2(in, *nlen, out, *newlen, *bval, *eval, t, *tstart, *dtnew, *eps);
}
