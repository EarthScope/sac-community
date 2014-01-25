/** 
 * @file   gethv.c
 * 
 * @brief  Get and format a header variable
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "msg.h"
#include "bot.h"
#include "co.h"
#include "ucf.h"
#include "clf.h"
#include "dfm.h"
#include "bool.h"

/** 
 * Get and format a header variable
 * 
 * @param kname 
 *    Name of header variable to get 
 *    Form file,variable where
 *      - file is the name or number of the data file
 *      - variable is the name of the header variable    
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue 
 *    Formatted value of the header variable
 *     - 'Undefined' if the variable was undefined
 *     - 'Error' if an error occurred
 * @param kvalue_s 
 *    Length of \p kvalue
 * @param nerr 
 *    - 0 on Success
 *    - 
 *
 * @date   970129:  Add parameter (0) to cnvati.  0 means that if a string
 *                  of digits is too long, let it slide by.  maf 
 * @date   871103:  Changed return value of KVALUE for errors.
 * @date   871022:  Fixed a more subtle variation of last bug.
 * @date   871015:  Fixed bug in save/restore logic for header.
 * @date   870722:  Added descriptive error messages.
 * @date   870302:  Original version.
 *
 */
void 
gethv(char *kname, 
      int   kname_s, 
      char *kvalue, 
      int   kvalue_s, 
      int  *nerr) {

	int lsave;
	int ic1, ic2, icurdf, int_, isave, jdfl;
	int nc, ncerr, ntused;

	static int iolddf = 1;
  char *strtemp;
  char *tmp;
	*nerr = 0;

	/* - Determine the file number. */
	nc = indexb( kname,kname_s );
	ic1 = 1;
	ic2 = 1;
L_2000:
	if( kname[ic2 - 1] == ',' ){
		if( ic2 == 1 ){
			icurdf = iolddf;
		}
		else{
      int idx ;
 
      strtemp = malloc(ic2-ic1+2);
			for( idx = 0 ; idx < ic2-ic1+1 ; idx++ )
        strtemp[ idx ] = ' ' ;
      strtemp[ ic2-ic1+1 ] = '\0' ;
      strncpy(strtemp,kname+ic1 - 1,ic2-ic1);
      strtemp[ic2-ic1] = '\0';
      
			cnvati( strtemp, ic2-ic1+1, &int_, 0, &ncerr );
			/* add 0. maf 970129 */

                        free(strtemp);

			if( ncerr == 0 ){
				if( int_ >= 1 && int_ <= cmdfm.ndfl ){
					icurdf = int_;
				}
				else{
					*nerr = 1310;
					setmsg( "ERROR", *nerr );
					apimsg( int_ );
					fstrncpy( kvalue, kvalue_s-1, "ERROR", 5);
					goto L_8888;
				}
			}
			else{
				jdfl = 1;
L_3000:
        if((tmp = string_list_get(datafiles, jdfl-1))) {
          if(memcmp(tmp, kname+ic1-1, min(ic2-ic1,strlen(tmp))) == 0) {
            icurdf = jdfl;
          } else {
            jdfl = jdfl + 1;
            goto L_3000;
          }
        }
				else{
					*nerr = 1363;
					setmsg( "ERROR", *nerr );
          apcmsg2(&kname[ic1 - 1],ic2-ic1);
					fstrncpy( kvalue, kvalue_s-1, "ERROR", 5);
					goto L_8888;
				}
			}
		}
	}
	else if( ic2 < nc ){
		ic2 = ic2 + 1;
		goto L_2000;
	}
	else{
		*nerr = 1364;
		setmsg( "ERROR", *nerr );
		fstrncpy( kvalue, kvalue_s-1, "ERROR", 5);
		goto L_8888;
	}

	/* - Get the header from the memory manager if necessary. */

	if( icurdf != cmdfm.idflc ){
		if( cmdfm.idflc > 0 ){
			lsave = TRUE;
			isave = cmdfm.idflc;
			putfil( cmdfm.idflc, nerr );
			if( *nerr != 0 )
				goto L_8888;
		}
		else{
			lsave = FALSE;
		}
		getfil( icurdf, FALSE, &ntused, &ntused, &ntused, nerr );
		if( *nerr != 0 )
			goto L_8888;
	}
	else{
		lsave = FALSE;
	}

	/* - Format the requested header field. */

        strtemp = malloc(nc - (ic2 + 1) + 2);
        strncpy(strtemp,kname+ic2,nc - (ic2 + 1) + 1);
        strtemp[nc - (ic2 + 1) + 1] = '\0';

	formhv( strtemp, nc-(ic2+1)+2, 3, kvalue,kvalue_s, nerr );

        free(strtemp);

	if( *nerr > 0 ){
		fstrncpy( kvalue, kvalue_s-1, "ERROR", 5);
		goto L_8888;
	}
	else if( *nerr < 0 ){
		fstrncpy( kvalue, kvalue_s-1, "UNDEFINED", 9);
	}
	iolddf = icurdf;

	/* - Get the saved header from the memory manager if necessary. */

	if( lsave ){
		getfil( isave, FALSE, &ntused, &ntused, &ntused, nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

L_8888:
	return;
}

