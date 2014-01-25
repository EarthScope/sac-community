/** 
 * @file   xxdiv.c
 * 
 * @brief  Control the X Divisions
 * 
 */

#include "gem.h"
#include "bool.h"


#include "cpf.h"

void
xdiv_nice(int flag) {
  /* X Division "Increment"; spacing between divisons: cmgem.xdiv_spacing  */
  cmgem.xdiv_spacing_on  = (flag == TRUE) ? FALSE : TRUE; 

  /* X Division "Number"; number of divisions: cmgem.xdiv_number        */
  cmgem.xdiv_number_on = (flag == TRUE) ? FALSE : TRUE; 
}

void 
xdiv_increment(float z) {
  cmgem.xdiv_spacing_on = TRUE;
  cmgem.xdiv_number_on  = FALSE;
  cmgem.xdiv_spacing    = z;    /* Spacing between Divisions */
}

void
xdiv_number(int n) {
  cmgem.xdiv_spacing_on = FALSE;
  cmgem.xdiv_number_on  = TRUE;
  cmgem.xdiv_number     = n;  /* Number of Divisions */
}

void
xdiv_power(int flag) {
  cmgem.lxpowr = flag; /* Division displayed as a power */
}

/** 
 * Parse the command "xdiv" and set the X Divisions
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Error
 *
 * @date   840531:  Added power labeling flag option.
 * @date   820610:  Original version (from GEMCOM.)
 *
 */
void 
xxdiv(int *nerr) {

  double tmp;
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Set up nice numbering. */
		if( lckey( "NI$",4 ) ){
                  xdiv_nice(TRUE);
                  /* -- Set up fixed division spacings. */
                }
		else if( lkreal( "I$",3, &tmp ) ){
      cmgem.xdiv_spacing = (float) tmp;
			cmgem.xdiv_spacing_on = TRUE;
			cmgem.xdiv_number_on = FALSE;

			/* -- Set up a fixed number of divisions. */
			}
		else if( lkirc( "NU$",4, 1, 100, &cmgem.xdiv_number ) ){
			cmgem.xdiv_spacing_on = FALSE;
			cmgem.xdiv_number_on = TRUE;

			/* -- Turn power labeling on/off. */
			}
		else if( lklog( "P$",3, &cmgem.lxpowr ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;
		}

       
	return;

}
