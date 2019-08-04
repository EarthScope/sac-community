/** 
 * @file   complex.h
 *
 * @brief  Complex variable type
 * 
 */

#ifndef  _SAC_COMPLEX_H_
#define _SAC_COMPLEX_H_

#include <sacio/sacio.h>

double aimag(complexf fc);
complexf cmplxadd(complexf c1, complexf c2);
double cmplxtof(complexf c);
complexf cmplxcj(complexf c);
complexf cmplxmul(complexf c1, complexf c2);
complexf flttocmplx(double d1, double d2);
complexf cmplxsub(complexf c1, complexf c2);
double cmplxabs(complexf c);
double cmplxang(complexf c);
complexf cmplxsqrt(complexf c);
complexf cmplxdiv(complexf c1, complexf c2);
complexf cmplxlog(complexf c);
complexf cmplxexp(complexf c);
complexf cmplxpow(complexf c, double d);
complexf cmplxneg(complexf c);

complexd dbltocmplx(double d1, double d2);
complexd dcmplxneg(complexd c);
complexd dcmplxsqrt(complexd c);
double dcmplxabs(complexd c);
double dcmplxang(complexd c);
complexd dcmplxsub(complexd c1, complexd c2);
complexd dcmplxmul(complexd c1, complexd c2);
complexd dcmplxadd(complexd c1, complexd c2);
complexd dcmplxpow(complexd c, double d);
complexd dcmplxlog(complexd c);
complexd dcmplxexp(complexd c);
complexd dcmplxdiv(complexd c1, complexd c2);
double daimag(complexd c);
double dcmplxtof(complexd c);



#endif /* _SAC_COMPLEX_H_ */
