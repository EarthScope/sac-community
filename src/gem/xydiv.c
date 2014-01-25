/** 
 * @file   xydiv.c
 * 
 * @brief  Control the Y Divisions 
 * 
 */

#include "gem.h"
#include "bool.h"


#include "cpf.h"

void
ydiv_nice(int flag) {
  cmgem.ydiv_spacing_on  = (flag == TRUE) ? FALSE : TRUE; /* Y Division "Increment"; spacing between divisons: cmgem.ydiv  */
  cmgem.ydiv_number_on = (flag == TRUE) ? FALSE : TRUE; /* Y Division "Number"; number of divisions: cmgem.ydiv_number_on        */
}

void 
ydiv_increment(float z) {
  cmgem.ydiv_spacing_on  = TRUE;
  cmgem.ydiv_number_on = FALSE;
  cmgem.ydiv_spacing   = z;    /* Spacing between Divisions */
}

void
ydiv_number(int n) {
  cmgem.ydiv_spacing_on = FALSE;
  cmgem.ydiv_number_on = TRUE;
  cmgem.ydiv_number = n;  /* Number of Divisions */
}

void
ydiv_power(int flag) {
  cmgem.lypowr = flag; /* Division displayed as a power */
}


/** 
 * Parse the command "ydiv" and set the Y Divisions
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
xydiv(int *nerr) {

  double tmp;
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Set up nice numbering. */
		if( lckey( "NI$",4 ) ){
                  ydiv_nice(TRUE);
                  /* -- Set up fixed division spacings. */
                }
		else if( lkreal( "I$",3, &tmp ) ){
      cmgem.ydiv_spacing = (float) tmp;
			cmgem.ydiv_spacing_on = TRUE;
			cmgem.ydiv_number_on = FALSE;

			/* -- Set up a fixed number of divisions. */
			}
		else if( lkirc( "NU$",4, 1, 100, &cmgem.ydiv_number ) ){
			cmgem.ydiv_spacing_on = FALSE;
			cmgem.ydiv_number_on = TRUE;

			/* -- Turn power labeling on/off. */
			}
		else if( lklog( "P$",3, &cmgem.lypowr ) ){

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

