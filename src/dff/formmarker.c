/** 
 * @file   formmarker.c
 * 
 * @brief  Format a time marker
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "bool.h"
#include "hdr.h"
#include "bot.h"
#include "ucf.h"
#include "co.h"

/** 
 * Format a time marker
 * 
 * @param time 
 *    Time of the marker
 * @param type 
 *    Type of marker
 * @param type_s 
 *     Length of \p type
 * @param output 
 *     Formatted output
 * @param output_s 
 *     Length of \p output
 * @param lok 
 *     - TRUE if value is defined
 *     - FALSE if value is not defined
 *
 * @date   870305:  Original version.
 *
 */
void 
formmarker(double  time, 
	   char   *type, 
	   int     type_s, 
	   char   *output, 
	   int     output_s, 
	   int    *lok) {

	int nc1, nc2;
        char *cattemp;

	if( time != SAC_FLOAT_UNDEFINED ){
                sprintf(output,"%16.5g",time);
		ljust( output,output_s );
		*lok = TRUE;
		nc1 = indexb( output,output_s );
		if( memcmp(type, SAC_CHAR_UNDEFINED,
			   min(strlen(type),strlen(SAC_CHAR_UNDEFINED))) != 0 ){
			nc2 = indexb( type,type_s );
                        cattemp = malloc(2+nc2+2);
                        strcpy(cattemp," (");
                        strncat(cattemp,type,nc2);
                        strcat(cattemp,")");
			subscpy( output, nc1, -1, output_s - 1, cattemp );
                        free(cattemp);
		}
		else{
			subscpy( output, nc1, -1, output_s - 1, " " );
		}
	}
	else{
                fstrncpy(output, output_s-1, "UNDEFINED", 9); 
		*lok = FALSE;
	}

	return;
}

