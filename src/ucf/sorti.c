/** 
 * @file   sorti.c
 * 
 * @brief  Bubble Sort integer array
 * 
 */

#include "ucf.h"
#include "bool.h"

/** 
 * Sort integer array (Bubble Sort)
 * 
 * @param iain 
 *    Input integer array
 * @param niain 
 *    Length of \p iain
 * @param lincr 
 *    - TRUE to sort in increasing order
 *    - FALSE to sort in decreasing order
 * @param iaout 
 *    Output integer array
 * 
 * @date   810000:  Original version.
 *
 */
void 
sorti(int  *iain, 
      int   niain, 
      int   lincr, 
      int  *iaout) {

	int lagain;
	int i, j, j1, j2, nd2;

	int *const Iain = &iain[0] - 1;
	int *const Iaout = &iaout[0] - 1;


	/* COPY INPUT ARRAY TO OUTPUT ARRAY */
	for( j = 1; j <= niain; j++ ){
		Iaout[j] = Iain[j];
		}

	/* SORT HERE IF NIAIN GT 1 */

	if( niain > 1 ){
L_2000:
		lagain = FALSE;
		for( j = 1; j <= (niain - 1); j++ ){
			if( Iaout[j] > Iaout[j + 1] ){
				i = Iaout[j];
				Iaout[j] = Iaout[j + 1];
				Iaout[j + 1] = i;
				lagain = TRUE;
				}
			}
		if( lagain )
			goto L_2000;

		/* REVERSE ORDER OF OUTPUT ARRAY IF REQUESTED */

		if( !lincr ){
			nd2 = niain/2;
			j2 = niain;
			for( j1 = 1; j1 <= nd2; j1++ ){
				i = Iaout[j1];
				Iaout[j1] = Iaout[j2];
				Iaout[j2] = i;
				j2 = j2 - 1;
				}
			}
		}

	return;
}

