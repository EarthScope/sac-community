/** 
 * @file   xpickphase.c
 * 
 * @brief  Pick Phases
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"


#include "cpf.h"

/** 
 * Execute the command PICKPHASE which controls phases read by readcss
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970409:  Original version.
 *
 */
void 
xpickphase(int *nerr) {

    char prefsFileName [ MCPFN + 1 ] ;
    int nchar;

    *nerr = 0;

    /* - Look for order dependance key, FILE
     * if no tokens present, assume FILE by default.
     */
    if ( !lcmore( nerr ) || lckey ( "FILE$" , 6 ) ) {
	/* get file name if present. */
	if ( lcchar ( MCPFN , prefsFileName , MCPFN , &nchar ) ) {
	    prefsFileName [ nchar ] = '\0' ;
	    strcpy ( kmdfm.kprefsFileName , prefsFileName ) ;
	}
	/* get list of phases from user-defined file. */
	getprefs ( FALSE , TRUE ) ;
	return ;
    }

    /* - Look for order dependance key, AUTHOR */
    if ( lckey ( "AUTH#OR$" , 9 ) ) {
	/* get file name if present. */
        if ( lcchar ( MCPFN , prefsFileName , MCPFN , &nchar ) ) {
	    prefsFileName [ nchar ] = '\0' ;
	    strcpy ( kmdfm.kprefsFileName , prefsFileName ) ;
	}
	/* get authors and phases from the user-defined file. */
	getprefs ( TRUE , TRUE ) ;
	return ;
    }

    /* Loop through order independent keys, t0 - t9, picking up
       phases and authors. */
    lkt ( nerr ) ;
    return ;
}
