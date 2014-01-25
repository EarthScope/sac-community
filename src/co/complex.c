/** 
 * @file   complex.c
 * 
 * @brief  Complex data type
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "complex.h"

double 
aimag(complexf c) {
  return(c.im);
}

complexf 
cmplxadd(complexf c1,
	 complexf c2) {
  c1.re += c2.re;
  c1.im += c2.im;
  return(c1);
}

double 
cmplxtof(complexf c) {
  return(c.re);
}

complexf 
cmplxcj(complexf c) {
  c.im = -c.im;
  return(c);
}

complexf 
cmplxmul(complexf c1,
	 complexf c2) {
  complexf c3;
  
  c3.re = (c1.re * c2.re) - (c1.im * c2.im);
  c3.im = (c1.re * c2.im) + (c1.im * c2.re);
  return(c3);
}

complexf 
flttocmplx(double d1,
	   double d2) {
  complexf c;
  c.re = (float) d1;
  c.im = (float) d2;
  return(c);
}

complexf 
cmplxsub(complexf c1,
	 complexf c2) {
  c1.re -= c2.re;
  c1.im -= c2.im;
  return(c1);
}

double 
cmplxabs(complexf c) {
  return(sqrt((double)((c.re*c.re) + (c.im*c.im))));
} 

double 
cmplxang(complexf c) {
  double d;

  if( fabs(c.im) < 1.0e-14) { /* Real Number */
      if ( c.re > 0 ) {
          d = 0;
      } else if ( c.re < 0 ) {
          d = M_PI ;
      } else {
          fprintf(stderr, "complex: Attempt to take angle (argument) of 0+0i\n");
          return 0.0;
      }
  } else if ( fabs(c.re) < 1.0e-14 ) { /* Imaginary Number */
      if ( c.im > 0 ) {
          d = M_PI_2; 
      } else if ( c.re < 0 ) {
          d = -M_PI_2; 
      } else {
          fprintf(stderr, "complex: Attempt to take angle (argument) of 0+0i\n");
          return 0.0;
      }
  } else {
      d = atan(c.im/c.re);
      if(c.re < 0.0) {
          if(c.im < 0.0) {
              d -= M_PI;
          } else {
              d += M_PI;
          }
      }
  }

  return(d);
}

complexf 
cmplxsqrt(complexf c) {

  double sqrtsave, angle;

  sqrtsave = sqrt(cmplxabs(c));
  angle = cmplxang(c);

  c.re = (float) (sqrtsave*cos(angle/2.0));
  c.im = (float) (sqrtsave*sin(angle/2.0));

  if(c.re < 0.0) {
    c.re = -c.re;
    c.im = -c.im;
  }
  else if (c.re < 1.0e-14 && c.re > -1.0e-14 && c.im < 0.0)
          c.im = -c.im;

  return(c);
}

complexf 
cmplxdiv(complexf c1,
	 complexf c2) {
  complexf c;
  float f;

  if (c2.re == 0.0 && c2.im == 0.0) {
    printf("complex divide by zero-cmplxdiv\n");
    exit(1);
  }

  f = c2.re*c2.re + c2.im*c2.im;

  c.re = (c1.re*c2.re + c1.im*c2.im)/f;
  c.im = (c2.re*c1.im - c1.re*c2.im)/f;

  return(c);
}

complexf 
cmplxlog(complexf c) {
  complexf c1;

  c1.re = (float) log(cmplxabs(c));
  c1.im = (float) cmplxang(c);

  return(c1);
}

complexf 
cmplxexp(complexf c) {
  double d;

  if(c.re == 0.0) d = 1.0;
  else            d = exp(c.re);

  if(c.im == 0.0){
    c.re = (float) d;
    return(c);
  }

  c.re = (float)( d*cos(c.im) );
  c.im = (float)( d*sin(c.im) );

  return(c);
}

complexf 
cmplxpow(complexf c,
	 double   d) {
  if(c.re == 0.0 && c.im == 0.0) return(c);

  c = cmplxlog(c);
  c.re = (float) ( d*c.re );
  c.im = (float) ( d*c.im );

  return(cmplxexp(c));
}

complexf 
cmplxneg(complexf c) {
  c.re = -c.re;
  c.im = -c.im;
  return(c);
}
