/** 
 * @file   append.c
 * 
 * @brief  Append on character string to another
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dbh.h"


#include "co.h"

/** 
 * Append one character string to another
 * 
 * @param line 
 *    String to copy to
 * @param line_s 
 *    Length of string \p line
 * @param exp 
 *    String to copy from with a delimiter at the first character.
 *    Strings are contained withing the delimiters: @Hello I am a string @
 * @param exp_s 
 *    Length of string \p exp
 *
 * @return Nothing
 *
 * @author Dave B. Harris
 *
 * \date 810724  Last Modified
 * \date 071022  Documented/Reviewed
 *
 */
void 
append(char *line, 
       int   line_s, 
       char *exp, 
       int   exp_s)
{
	char delim;
	int iptr, jptr;
        char *strtemp;


	/*  LOCATE END OF LINE
	 * */
	iptr = (line_s - 1);
L_1:
	;
	if( line[iptr - 1] != ' ' ){
		goto L_2;
		}
	else{
		iptr = iptr - 1;
		if( iptr < 1 ){
			return;
			}
		}
	goto L_1;
L_2:
	;

	/*  OBTAIN DELIMITER ( FIRST CHARACTER OF EXP )
	 * */
	delim = exp[0];

	/*  APPEND EXPRESSION IF NOT NULL
	 * */
	if( exp[1] != delim ){

		/*    LOCATE END OF EXPRESSION
		 * */
		jptr = 2;
L_3:
		;
		if( exp[jptr] == delim ){
			goto L_4;
			}
		else{
			jptr = jptr + 1;
			if( jptr > (exp_s - 1) ){
				return;
				}
			}
		goto L_3;
L_4:
		;

		/*    APPEND EXPRESSION TO LINE
		 * */
                strtemp = malloc(jptr);
                strncpy(strtemp,exp+1,jptr-1);
                strtemp[jptr-1] = '\0';
		subscpy( line, iptr, (line_s - 1) - 1, line_s - 1, strtemp);
                free(strtemp);

		}

	return;
} 

