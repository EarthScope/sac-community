/** 
 * @file   lcrrcp.c
 * 
 * @brief  Parse a range check pair of reals
 * 
 */
#include <stdio.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a range checked real variable pair command construct
 * 
 * @param realmn 
 *    Minimum value
 * @param realmx 
 *    Maximum value
 * @param realv1 
 *    First real value
 * @param realv2 
 *    Second real value
 * 
 * @return 
 *    - TRUE if the real pair was found
 *    - FALSE if the real pair was not found
 *
 * @date   820622:  Original version.
 *
 */
int
lcrrcp(double  realmn, 
       double  realmx, 
       double *realv1, 
       double *realv2) {

	int lcrrcp_v;
	int nerr;
	float rv;
  Token *t;

	/* - Get real variable from next symbol.
	 * - Check variable against allowed range.
	 * - Perform standard command error recovery if not found.
	 * - Repeat for second real. */
L_2000:
  if((t = arg()) && token_is_number(t)) {
    //if( Itypcm[cmcom.jcom] == cmcom.inumbr ){
		lcrrcp_v = TRUE;
		rv = t->value;
		if( rv >= realmn && rv <= realmx ){
			*realv1 = rv;
      arg_next();
L_3000:
      if((t = arg()) && token_is_number(t)) {
        //if( Itypcm[cmcom.jcom] == cmcom.inumbr ){
				rv = t->value;
				if( rv >= *realv1 && rv <= realmx ){
					*realv2 = rv;
          arg_next();
				}
				else{
					cfmt( "OUTSIDE ALLOWED RANGE:",24 );
          fprintf(stdout," Allowed range is: %16.5g%16.5g\n", *realv1, realmx );
					cresp();
					if( lcmore( &nerr ) )
						goto L_3000;
					lcrrcp_v = TRUE;
				}
			}
			else{
				cfmt( "NEED A REAL VARIABLE:",23 );
				cresp();
				if( lcmore( &nerr ) )
					goto L_2000;
				lcrrcp_v = TRUE;
			}
		}
		else{
			cfmt( "OUTSIDE ALLOWED RANGE:",24 );
      fprintf(stdout," Allowed range is: %16.5g%16.5g\n", realmn, realmx );
			cresp();
			if( lcmore( &nerr ) )
				goto L_2000;
			lcrrcp_v = TRUE;
		}
	}
	else{
		lcrrcp_v = FALSE;
	}

	return( lcrrcp_v );
}

