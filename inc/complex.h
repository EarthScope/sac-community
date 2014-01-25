/** 
 * @file   complex.h
 *
 * @brief  Complex variable type
 * 
 */

#ifndef  _COMPLEX_H_
#define _COMPLEX_H_

typedef struct complexf_t complexf;

/** 
 * @struct Complex Type (Floating Point)
 *    
 */
struct complexf_t {
  float re;        /** Real Part */
  float im;        /** Imaginary Part */
};


double   aimag      ( complexf fc);
complexf cmplxadd   ( complexf c1, 
                      complexf c2);
double   cmplxtof   ( complexf c);
complexf cmplxcj    ( complexf c);
complexf cmplxmul   ( complexf c1, 
                      complexf c2);
complexf flttocmplx ( double d1, 
                      double d2);
complexf cmplxsub   ( complexf c1, 
                      complexf c2);
double   cmplxabs   ( complexf c);
double   cmplxang   ( complexf c);
complexf cmplxsqrt  ( complexf c);
complexf cmplxdiv   ( complexf c1, 
                      complexf c2);
complexf cmplxlog   ( complexf c);
complexf cmplxexp   ( complexf c);
complexf cmplxpow   ( complexf c, 
                      double d);
complexf cmplxneg   ( complexf c);


#endif /* _COMPLEX_H_ */
