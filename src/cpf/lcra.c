/** 
 * @file   lcra.c
 * 
 * @brief  Parse a real array
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a real array command construct
 * 
 * @param nramn 
 *    Minimum number of reals to return
 * @param nramx 
 *    Maximum number of reals to return
 * @param ra 
 *    Real array on output
 * @param nra 
 *    Number of reals return on output
 * 
 * @return 
 *    - TRUE if the real array was found
 *    - FALSE if the real array was not found
 *
 * @date   820914:  Was not setting function value upon return.
 * @date   820721:  Original version.
 *
 */
int
lcra(int     nramn, 
     int     nramx, 
     double *ra, 
     int    *nra) {

	int lcra_v;
	int nerr;
  char *cattemp;
  Token *t;

	double *const Ra = &ra[0] - 1;

	/* - Pop real variables off command until:
	 *   (1) Maximum number is reached  OR
	 *   (2) Next token is not a real variable. */
	*nra = 0;
	nerr = 0;
L_2000:
  while(*nra < nramx && (t = arg()) && token_is_number(t)) {
		*nra = *nra + 1;
		Ra[*nra] = t->value;
    arg_next();
	}
	if( *nra < nramn ){
	  /* - Perform standard error recovery if minimum 
	   *   number of reals not found. */
    asprintf(&cattemp, "NEED AT LEAST %5d REALS:$", nramn);
		cfmt(cattemp, strlen(cattemp) );
		cresp();
    free(cattemp);
		if( lcmore( &nerr ) )
			goto L_2000;
		lcra_v = TRUE;
		goto L_8888;
	}
  
	/* - If we get to here with no errors, set function value to .TRUE. */
	lcra_v = nerr == 0;

L_8888:
	return( lcra_v );


}

