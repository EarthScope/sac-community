/** 
 * @file   xelse.c
 * 
 * @brief  Parse "ELSE"
 * 
 */

#include "cnd.h"
#include "cpf.h"

/** 
 * Parse the action command "ELSE"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Succes
 *
 * @date   870817:  Original version.
 *
 */
void 
xelse(int *nerr) {
	*nerr = 0;
  if(arg()) {
    arg_msg("Unexpected characters after ELSE: ");
  }
	if( cnd.niflevel > 0 ){
		if( Lifresp[cnd.niflevel] ){
			skipif( nerr );
		}
	}
	else{
		*nerr = 1;
	}
	return;
}

