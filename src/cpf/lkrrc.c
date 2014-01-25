/** 
 * @file   lkrrc.c
 * 
 * @brief  Parse a keyed range checked real
 * 
 */

#include <stdio.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed range checked real variable command construct
 * 
 * @param kkey 
 *    Keyword to search fro
 * @param kkey_s 
 *    Length of \p kkey
 * @param realmn 
 *    Minimum value
 * @param realmx 
 *    Maximum value
 * @param realv 
 *    Real variable on output
 * 
 * @return 
 *    - TRUE if real value was found
 *    - FALSE if real value was not found
 *
 * @date   901157:  Split line 63 to two lines, was longer than 72. wct
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lkrrc(char   *kkey, 
      int     kkey_s, 
      double  realmn, 
      double  realmx, 
      double *realv) {

	int nerr;
	double rv;
  Token *t;

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }


L_2000:
  if((t = arg()) && token_is_number(t)) {
    rv = t->value;
    if( rv >= realmn && rv <= realmx ){
      *realv = rv;
      arg_next();
    } else {
      cfmt( "OUTSIDE ALLOWED RANGE:",24 );
      fprintf(stdout, " Allowed range is: %12.5g%12.5g\n",realmn, realmx);
      cresp();
      if( lcmore( &nerr ) )
        goto L_2000;
    }
  } else {
    cfmt( "NEED A REAL VARIABLE:",23 );
    cresp();
    if( lcmore( &nerr ) )
      goto L_2000;
  }

	return TRUE;
}

