
#include "ncpf.h"

int /*FUNCTION*/ nstrlensp(kstring, kstring_s)
char *kstring;   int kstring_s;
{
	int ic, nldefined, nstrlensp_v;

	/*=====================================================================
	 * PURPOSE: To count the length of char string, until terminated by
	 *          the first space (blank).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    string:   String to count. [c]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nstrlensp:  count returned. [i]
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *    910419:   Initial copy.
	 *===================================================================== */
	/* PROCEDURE: */
	nldefined = (kstring_s - 1);
	for( ic = 1; ic <= nldefined; ic++ ){
		if(( kstring[ic - 1] == ' ') || (kstring[ic-1] == '\0') )
			goto L_2000;
		}
L_2000:
	nstrlensp_v = ic - 1;

	return( nstrlensp_v );
} /* end of function */

