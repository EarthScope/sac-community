/** 
 * @file   math.c
 * 
 * @brief  Extra math functionality
 * 
 */

#include <stdlib.h>
#include <math.h>
#include "co.h"

#include "config.h"

int 
min(int a,
    int b) {
   return ( a < b ? a : b );
}

int 
max(int a,
    int b) {
  return ( a > b ? a : b );
}


#ifdef MISSING_FUNC_COPYSIGN
double
copysign(double x, double y) {
  /* use atan2 to distinguish -0. from 0. */
  if (y > 0. || (y == 0. && atan2(y, -1.) > 0.)) {
    return fabs(x);
  } else {
    return -fabs(x);
  }
}
#endif

#if MISSING_FUNC_ROUND
double
round(double x) {
   double absx, y;
   absx = fabs(x);
   y = floor(absx);
   if (absx - y >= 0.5) {
     y += 1.0;
   }
   return copysign(y, x);
}
#endif

/* fmin, fmax, and labs are defined in the Standard C library on Mac OS X. */
#if MISSING_FUNC_FMIN
double 
fmin(double a,
     double b) {
  return ( a < b ? a : b );
}
#endif

#if MISSING_FUNC_FMAX
double 
fmax(double a,
     double b) {
  return ( a > b ? a : b );
}
#endif

#if MISSING_FUNC_LROUND
long int
lround(double z) {
  double min;
  if(z >= 0.0) {
    min = floor(z);
    if(z - min >= 0.5) {
      min += 1.0;
    }
    return (long int) min;
  } else {
    min = floor(-z);
    if(-z - min  >= 0.5) {
      min += 1.0;
    }
    min = min * -1.0;
    return (long int) min;
  }
}
#endif 

int 
isign(int a,
      int b) {
  return ( b >= 0 ? labs(a) : -(labs(a)) );
}

double 
sign(double a,
     double b) {
  return ( b >= 0.0 ? fabs(a) : -(fabs(a)) );
}

double 
powi(double   b,
     int      x) {

  return pow(b, (double)x);
}

int 
ipow(int b,
     int x) {
  int temp, i;

  if ( b == 0 ) return 0;
  if ( b == 1 ) return 1;

  if ( x < 0 ) return 0;
  if ( x == 0 ) return 1;
  if ( x == 1 ) return b;

  temp = b;
  for ( i = x-1; i > 0; i--) temp *= b;
  
  return temp;
}
