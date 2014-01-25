/** 
 * @file   indexa.c
 * 
 * @brief  Search for a character in a string
 * 
 */

/** 
 * Search for the occurance of a single character within a character string.
 *   The search can be performed forward or backwards, and for the 
 *   non-occurance of the character.
 * 
 * @param string 
 *    String to search
 * @param string_s 
 *    Length of \p stirng
 * @param kchar 
 *    Character to search for, or not
 * @param lfwd 
 *    - TRUE - Forward Search
 *    - FALSE - Reverse Search
 * @param locc 
 *    - TRUE - Search for the first occurance of \p kchar 
 *    - FALSE - Search for the first non-occurance \p kchar 
 *
 * @return 
 *    The index of the character if found, 0 otherwise
 *    
 *
 * @note Example:
 *    CHARACTER*MCMSG STRING
 *    STRING='$aabcde$'
 *    IC1=INDEXA2(STRING,'c',.TRUE.,.TRUE.)
 *    IC2=INDEXA2(STRING,'a',.TRUE.,.FALSE.)
 *    IC3=INDEXA2(STRING,'a',.FALSE.,.TRUE.)
 *    IC4=INDEXA2(STRING,'e',.FALSE.,.FALSE.)
 *
 *    For this example, IC1 would be 5, IC2 would be 4,
 *    IC3 would be 3, and IC$ would be 6.
 *
 * @date   920326:  Changed parameter char to kchar.
 * @date   830505:  Original version.
 *
 */
int 
indexa(char *string, 
       int   string_s, 
       int   kchar, 
       int   lfwd, 
       int   locc) {

	int ic, ic1, ic3, indexa_v, nc, do_count;

	/* - Determine length of character string. */
	nc = (string_s - 1);
        if ( nc == 0 ) nc = 1;

	/* - Set up loop parameters based on search direction. */

	if( lfwd ){
		ic1 = 1;
		/* ic2 = nc; */
		ic3 = 1;
	}
	else{
		ic1 = nc;
		/* ic2 = 1; */
		ic3 = -1;
	}

	/* - Search each character in input string for requested character. */

	indexa_v = 0;
	for( ic = ic1, do_count = nc; do_count > 0; ic += ic3, do_count-- ){
		if( string[ic - 1] == kchar && locc ){
			indexa_v = ic;
			goto L_8888;
		}
		else if( string[ic - 1] != kchar && !locc ){
			indexa_v = ic;
			goto L_8888;
		}
	}

L_8888:
	return( indexa_v );

}

