
#include <string.h>

#include "ncpf.h"

int /*FUNCTION*/ indexs(text, ntext, string, nstring, lfwd, locc)
char *text;
int ntext;
char *string;
int nstring;
int lfwd, locc;
{
	int ic, ic1, ic3, indexs_v, 
	 jc, do_count;

	/*=====================================================================
	 * PURPOSE: Searches for the occurance of a  character string within
	 *          a text string.  The search can be performed in the
	 *          forward or backward directions.  The search can be made
	 *          for the occurance of the non-occurance of the character.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    text:    Text string to search. [c]
	 *    ntext:   Number of characters in text. [i]
	 *    string:  Text  string to search for. [c]
	 *    nstring: Number of characters in string. [i]
	 *    lfwd:    Forward search flag. [l]
	 *             Set to .TRUE. if string is to be forward searched
	 *             and .FALSE. if string is to search backward.
	 *    locc:    Occurance flag. [l]
	 *             Set to .TRUE. if search is for first occurance of CHAR
	 *             and .FALSE. if search is for first non-occurrance.
	 *=====================================================================
	 * FUNCTION VALUE:
	 *    indexs:  The index of the beginning of the string if found, 
	 *             0 otherwise. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  SERVICE/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881228:  Original version based on indexa.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  881228
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set up loop parameters based on search direction. */
	if( lfwd ){
		ic1 = 1;
		/* ic2 = ntext - nstring + 1; */
		ic3 = 1;
		}
	else{
		ic1 = ntext - nstring + 1;
		/* ic2 = 1; */
		ic3 = -1;
		}

	/* - Search each substring in text for requested string. */

	indexs_v = 0;
	jc = ic1 + nstring - 1;
        do_count = ntext - nstring + 1;
	for( ic = ic1; do_count > 0; ic += ic3, do_count-- ){
		if( memcmp(text+ic - 1,string,nstring) == 0 && locc ){
			indexs_v = ic;
			goto L_8888;
			}
		else if( memcmp(text+ic - 1,string,nstring) != 0 && !locc ){
			indexs_v = ic;
			goto L_8888;
			}
		jc = jc + ic3;
		}

L_8888:
	return( indexs_v );

} /* end of function */

