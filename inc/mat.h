
#ifndef _MAT_H_
#define _MAT_H_

#include "config.h"

void xmatc(int index, int *nerr);
void xCLOSEMAT(int *nerr);
void xMAT3C(int *nerr);
void xMAT(int *nerr);
void xsetmat(int *nerr);

#ifndef HAVE_MATLAB

void matlab_unavailable();

#endif /* HAVE_MATLAB */

#endif /* _MAT_H_ */
