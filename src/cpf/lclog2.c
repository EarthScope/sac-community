/** 
 * @file   lclog2.c
 * 
 * @brief  Parse a logical variable
 * 
 */

#include "cpf.h"
#include "bool.h"

/** 
 * Parse a logical variable command construct.
 * 
 * @param ktrue 
 *    Token corresponding to TRUE
 * @param ktrue_s 
 *    Length of \p ktrue
 * @param kfalse 
 *    Token corresponding to FALSE
 * @param kfalse_s 
 *    Length of \p kfalse
 * @param logv 
 *    Logical variable found
 * 
 * @return 
 *    - TRUE if the logical variable was found
 *    - FALSE if the logical variable was not found
 *
 * @date   820622:  Original version.
 *
 */
int
lclog2(char *ktrue, 
       int   ktrue_s, 
       char *kfalse, 
       int   kfalse_s, 
       int  *logv) {

	int lclog2_v;

	/* - Check for the "true" token. */
	if( lckey( ktrue,ktrue_s ) ){
		*logv = TRUE;
		lclog2_v = TRUE;
	}
	/* - Check for the "false" token. */
	else if( lckey( kfalse,kfalse_s ) ){
		*logv = FALSE;
		lclog2_v = TRUE;
	}
	/* - Neither token found. */
	else{
		lclog2_v = FALSE;
	}

	return( lclog2_v );
}

