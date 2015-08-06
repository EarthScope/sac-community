/** 
 * @file   uom.h
 * 
 * @brief  Unary operations module
 * 
 */

#ifndef _UOM_H_
#define _UOM_H_

#include "mach.h"

#define	MDIFTP	5

/** 
 * @struct kmuom
 *   Unary Variables
 */
struct t_cmuom {
    int ndiftp;          /** */
    int idiftp;          /** */
    int ltrap;           /** */
} ;

/** 
 * @struct kmuom
 *   Unary Characters
 */
struct t_kmuom {
    char kdiftp[MDIFTP][9];
                          /** */
} ;

#define UOM_EXTERN         \
    extern struct t_kmuom kmuom;  \
    extern struct t_cmuom cmuom;

void dif2(float array[], int number, double step, float output[]);
void dif3(float array[], int number, double step, float output[]);
void dif5(float array[], int number, double step, float output[]);
void iniuom(void);
void xabs(int *nerr);
void xadd(int *nerr);
void xdif(int *nerr);
void xdiv(int *nerr);
void xexp(int *nerr);
void xexp10(int *nerr);
void xint(int *nerr);
void xlog(int *nerr);
void xlog10(int *nerr);
void xmul(int *nerr);
void xsqr(int *nerr);
void xsqrt(int *nerr);
void xsub(int *nerr);
void xuomc(int index, int *nerr);

#endif /* _UOM_H_ */
