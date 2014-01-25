/** 
 * @file   cnvita.c
 * 
 * @brief  Convert an integer to a string
 * 
 */

#include <stdio.h>
#include <string.h>

#include "ucf.h"


#include "co.h"

/** 
 * Convert an integer \p intgr into its ASCII equivalent
 * 
 * @param intgr 
 *    Integer to convert into a string
 * @param kintgr 
 *    Returned string
 *     If number cannot be converted then string is set to 'BADINPUT'
 * @param kintgr_s 
 *    Length of \p kintgr
 *
 * @date   800102:  Original version.
 *
 */
void 
cnvita(int  intgr, 
       char     *kintgr, 
       int       kintgr_s)
{
	char kfmt[9];
	int ncf, nck;

	/* - Determine length of character variable. */
	nck = (kintgr_s - 1);

	/* - Create format statement. */

	strcpy( kfmt, "%" );
        ncf = 1;
	if( nck <= 9 ){
                sprintf(kfmt+ncf,"%1d",nck);
                ncf++;
                		}
	else if( nck <= 99 ){
                sprintf(kfmt+ncf,"%2d",nck);
                ncf += 2;
		}
	else{
                sprintf(kfmt+ncf,"%3d",nck);
                ncf += 3;
		}
        strcpy(kfmt+ncf,"d");

	/* - Encode integer into string. */

        if( sprintf(kintgr,kfmt,intgr) < 0 ) {
           fstrncpy(kintgr, kintgr_s - 1,"BADINPUT",8);
	}
	return;
} 

