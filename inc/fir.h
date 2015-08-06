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
    char kidfir[81];
} ;

/** 
 * @struct kmfir 
 *    FIR Filter Variables
 *
 */
struct t_cmfir {
    int ncfir;
    float cfir[MFIR];
    float dtfir;
} ;

#define FIR_EXTERN \
    extern struct t_kmfir kmfir;                \
    extern struct t_cmfir cmfir;                \
    extern float *const Cfir;


#endif /* _FIR_H_ */
