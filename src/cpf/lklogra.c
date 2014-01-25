/** 
 * @file   lklogra.c
 * 
 * @brief  Parse a keyed logical/real array
 * 
 */

#include <stdio.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"



#include "bot.h"

/** 
 * Parse a keyed logical or real array command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param offOnFlt 
 *    On output
 *    - FALSE (0) is Off
 *    - TRUE (1) is On
 *    - 2 is array of Floats
 * @param nramn 
 *    Minimum number of reals to return
 * @param nramx 
 *    Maximum number of reals to return
 * @param ra 
 *    Real array on output
 * @param nra 
 *    Length of \p ra
 * @param nerr 
 *    Error Return Flag 
 *    - 0 on Success
 *    - 10000 on Error 
 * 
 * @return 
 *    - TRUE if the value was found
 *    - FALSE if the value was not found
 *
 * @bug Error return code is undefined.
 *
 * @date   970207:  Original version (merging lklog and lcra).
 *
 */
int
lklogra(char  *kkey, 
        int    kkey_s, 
        int   *offOnFlt, 
        int    nramn, 
        int    nramx, 
        double *ra, 
        int   *nra, 
        int   *nerr) {
  
	int lklogra_v;

	*nerr = 0 ;

	/* - Check for key. */
	lklogra_v = lckey( kkey,kkey_s );

	/* - Check for "ON" or "OFF" at next token.
	 *   Set logical variable to .TRUE. if not found. */
	if( lklogra_v ){
    if(lclog(offOnFlt)) { }
    else if(lcra(nramn, nramx, ra, nra)) {
      *offOnFlt = 2;
    } else {
      if( *nra < nramn ){
        char ktemp[50] ;
        sprintf (ktemp , "NEED AT LEAST %d REALS OR 'ON' OR 'OFF':$", nramn);
        cfmt ( ktemp , strlen ( ktemp ) + 1 ) ;
        cresp() ;
        *nerr = 10000 ;
      } 
		} 
	}
  
	return( lklogra_v );
}

