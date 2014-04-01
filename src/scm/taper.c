
#include <math.h>

#define PI M_PI

enum {
  TAPER_TYPE_COSINE = 1,
  TAPER_TYPE_HANNING = 2,
  TAPER_TYPE_HAMMING = 3,
};

void
taper_width_to_points(float width, float npts, int *ipts) {
  float tpts;
  tpts = width * (float)(npts + 1);
  if( (int)tpts < 2 ) {
    *ipts = 2;
  } else {
    *ipts = (int)tpts;
  }
}

void
taper(float *data, int n, int taper_type, int ipts) {
  int j;
  float f0, f1, omega, value;
  /* -- Taper each end.
   * --- Cosine taper.  (Yes, I know I'm computing a sine.) */
  if( taper_type == TAPER_TYPE_COSINE ){
    omega = PI/(2.*(float)( ipts ));
    for( j = 0; j <= (ipts - 1); j++ ){
      value = sin( omega*(float)( j ) );
      data[j]     *= value;
      data[n-j-1] *= value;
    }
  }
  /* --- Hanning and Hamming tapers differ only in coefficients. */
  else{
    if( taper_type == TAPER_TYPE_HANNING ){
      f0 = 0.50;
      f1 = 0.50;
    } else {
      f0 = 0.54;
      f1 = 0.46;
    }
    omega = PI/(float)( ipts );
    for( j = 0; j <= (ipts - 1); j++ ){
      value = f0 - f1*cos( omega*(float)( j ) );
      data[j]     *= value;
      data[n-j-1] *= value;
    }
  }
}

void taper_(float *data, int *n, int *taper_type, int *ipts) {
  taper(data, *n, *taper_type, *ipts);
}
void taper__(float *data, int *n, int *taper_type, int *ipts) {
  taper(data, *n, *taper_type, *ipts);
}

void taper_width_to_points_(float *width, float *npts, int *ipts) {
  taper_width_to_points(*width, *npts, ipts);
}
void taper_width_to_points__(float *width, float *npts, int *ipts) {
  taper_width_to_points(*width, *npts, ipts);
}
