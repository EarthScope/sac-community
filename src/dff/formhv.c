/** 
 * @file   formhv.c
 * 
 * @brief  Format a header variable
 * 
 */

#include <stdio.h>
#include <string.h>

#include "dff.h"
#include "bool.h"
#include "bot.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "ucf.h"

/** 
 * Format a header variable into a text string
 * 
 * @param kname 
 *    Header variable name
 * @param kname_s 
 *    Length of \p kname
 * @param iform 
 *    Type of ouput format
 *    - 1 gives name followed by " = " followed by value.
 *    - 2 gives name followed by ": " followed by value.
 *    - 3 gives value only.
 * @param kout 
 *    Output string
 * @param kout_s 
 *    Length of \p kout
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - -1 if \p kname is undefined in the current header
 *            then \p kout is still formatted in this case
 *    - -2 if \p iform has a bad value
 *
 * @date   970128:  Now allows floats 7 digits. maf
 * @date   961212:  Modified to add INCLUSIVE option to the lh command.  maf
 * @date   841025:  Original version.
 *
 */
void 
formhv(char  *kname, 
       int    kname_s, 
       int    iform, 
       char  *kout, 
       int    kout_s, 
       int   *nerr) {

	char kvalue[41];
	int lok, lok2 = FALSE, linc ;  
	int icat, item, nc;

	*nerr = 0;

        memset(&(kvalue[0]), ' ', 40);
	kvalue[ 40 ] = '\0' ;

	/* if cmhdr.linc and .llh are both TRUE, so is linc. maf 961212 */
	linc = cmhdr.linc && cmhdr.llh ;

	/* - Determine type and location of header field. */
	hdrfld( kname,kname_s, &icat, &item, &lok );

	if( lok ){
	        /* lok was true coming out of hdrfld().  maf 961212 */
		lok2 = TRUE ;	
		if( icat == cmlhf.icatf ){
			lok = Fhdr[item] != cmhdr.fundef ;
			if( lok ||  linc ) {
                                sprintf(kvalue,"%#16.6e",Fhdr[item]); 
				ljust( kvalue,41 );
			}
		}
		else if( icat == cmlhf.icatn ){
			lok = Nhdr[item] != cmhdr.nundef ;
			if( lok ||  linc  ){	
                                sprintf(kvalue,"%10d", Nhdr[item]);
				ljust( kvalue,41 );
			}
		}
		else if( icat == cmlhf.icati ){
			lok = Ihdr[item] != cmhdr.iundef ;
			if( lok )
				fstrncpy(kvalue, 40, kmlhf.kdiv[Ihdr[item] - 1],
					 strlen(kmlhf.kdiv[Ihdr[item] - 1]));
			else if ( linc )
				strcpy( kvalue, "UNDEFINED                               " );
		} 
		else if( icat == cmlhf.icatl ){
			lok = TRUE;
			if( Lhdr[item] ){
				strcpy( kvalue, "TRUE                                    " );
			}
			else{
				strcpy( kvalue, "FALSE                                   " );
			}
		}
		else if( icat == cmlhf.icatk ){
			lok = memcmp(kmhdr.khdr[item - 1],kmhdr.kundef,min(strlen(kmhdr.khdr[item - 1]),
                                                         strlen(kmhdr.kundef))) != 0 ;
      if( lok ||  linc  ) {
        memset(kvalue, ' ', sizeof(kvalue));
        strncpy(kvalue, kmhdr.khdr[item-1], strlen(kmhdr.khdr[item-1]));
        kvalue[strlen(kmhdr.khdr[item-1])] = 0;
			}
		} /* end if( icat == cmlhf.icatk ) */
		else if( icat == cmlhf.icata ){
			lok = lgahdr( kname,kname_s, kvalue,41 );
		}
	} 

	if( !lok2 || ( !lok && !linc ) ) {
		strcpy( kvalue, "Undefined                               " );
		*nerr = -1;
	}

	if( iform == 1 ){
		nc = indexb( kname,kname_s );
                fstrncpy(kout, kout_s-1, kname, nc);
                fstrncpy(kout+nc, kout_s-1-nc, " = ", 3);
                fstrncpy(kout+nc+3, kout_s-1-nc-3, kvalue, strlen(kvalue));
	}
	else if( iform == 2 ){
		nc = indexb( kname,kname_s );
                fstrncpy(kout, kout_s-1, kname, nc);
                fstrncpy(kout+nc, kout_s-1-nc, ": ", 2);
                fstrncpy(kout+nc+2, kout_s-1-nc-2, kvalue, strlen(kvalue));
	}
	else if( iform == 3 ){
                fstrncpy(kout, kout_s-1, kvalue, strlen(kvalue));
	}
	else{
		*nerr = -2;
		strcpy( kvalue, "Bad format number                       " );
	}

	return;
}

