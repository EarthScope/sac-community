/** 
 * @file   fir.h
 * 
 * @brief  Finite Impulse Response Filter
 * 
 */

#ifndef _FIR_H_
#define _FIR_H_

#define	MFIR	255

/** 
 * @struct kmfir 
 *    FIR Filter Characters
 *
 */
struct t_kmfir {
  char  kidfir[81];
} kmfir;


/** 
 * @struct kmfir 
 *    FIR Filter Variables
 *
 */
struct t_cmfir {
  int    ncfir;
  float  cfir[MFIR];
  float  dtfir;
} cmfir;

#ifdef DOINITS

   float *const Cfir = &cmfir.cfir[0] - 1;

#else

   extern float *const Cfir;

#endif

#endif /* _FIR_H_ */
