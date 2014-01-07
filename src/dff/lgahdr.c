/** 
 * @file   lgahdr.c
 * 
 * @brief  Get an auxillary header field
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "co.h"
#include "bot.h"
#include "hdr.h"
#include "ucf.h"
#include "dfm.h"
#include "bool.h"

extern sac *CURRENT;
#include "clf.h"

/** 
 * Get an auxillary header field from the current SAC file
 * 
 * @param kfield 
 *    Header field to get 
 *    - KZDATE, KZTIME, KSTCMP, FILENAME, NAME, AM, OM, FM, T[1..9]M
 * @param kfield_s 
 *    Length of \p kname
 * @param kvalue 
 *    Auxillary field value
 * @param kvalue_s 
 *    Length of \p kvalue
 * 
 * @return 
 *    - TRUE if the header value was found and defined
 *    - FALSE if the header value was not found or not defined
 *
 * @date   900419:  Fixed bug in computing length of kfield.
 * @date   880419:  Converted input field to uppercase before comparison.
 * @date   850516:  Allowed NAME as an alias for FILENAME.
 * @date   841026:  Added FILENAME field.
 * @date   801101:  Added logic to treat an undefined station name.
 * @date   801018:  Original version.
 *
 */
int
lgahdr(char *kfield, 
       int   kfield_s, 
       char *kvalue, 
       int   kvalue_s) {

	char kcmpaz[9], kcmpin[9], ktemp[9];
	int lgahdr_v;
	int nc, nerr;
    char *cattemp;
    char *tmp;
    sac *s;

    s = CURRENT;
	/* - Convert the name to upper case before doing any tests. */
	nc = min( (kfield_s - 1), SAC_HEADER_STRING_LENGTH_FILE );
	modcase( TRUE, kfield, nc, ktemp );


	/* - Compute the requested auxiliary header field from actual header
	 *   fields, making sure that each one is defined. */

	/* -- KZDATE:  Zero date field: */

	if( memcmp(ktemp,"KZDATE",6) == 0 ){
		if( s->h->nzyear != cmhdr.nundef && s->h->nzjday != cmhdr.nundef ){
			nc = (kvalue_s - 1);
			kadate( s->h->nzyear, s->h->nzjday, nc, kvalue,kvalue_s, &nerr );
			if( nerr == 0 ){
				lgahdr_v = TRUE;
				}
			else{
				fstrncpy( kvalue, kvalue_s-1, "BAD FIELD", 9);
				lgahdr_v = FALSE;
				}
			}
		else{
			fstrncpy( kvalue, kvalue_s-1, "UNDEFINED", 9);
			lgahdr_v = FALSE;
			}

		/* -- KZTIME:  Zero time field: */

		}
	else if( memcmp(ktemp,"KZTIME",6) == 0 ){
		if( ((s->h->nzhour != cmhdr.nundef && s->h->nzmin != cmhdr.nundef) && 
		 s->h->nzsec != cmhdr.nundef) && s->h->nzmsec != cmhdr.nundef ){
			nc = (kvalue_s - 1);
			katime( s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, nc, kvalue,kvalue_s, 
			 &nerr );
			if( nerr == 0 ){
				lgahdr_v = TRUE;
				}
			else{
				fstrncpy( kvalue, kvalue_s-1, "BAD FIELD", 9);
				lgahdr_v = FALSE;
				}
			}
		else{
			fstrncpy( kvalue, kvalue_s-1, "UNDEFINED", 9);
			lgahdr_v = FALSE;
			}

		/* -- KSTCMP:  Station component field: */

		}
	else if( memcmp(ktemp,"KSTCMP",6) == 0 ){
		if( memcmp(s->h->kstnm,kmhdr.kundef,9) != 0 ){
			lgahdr_v = TRUE;
			nc = indexb( s->h->kstnm,9 );
			fstrncpy( kvalue, kvalue_s-1, s->h->kstnm , nc );
			if( memcmp(s->h->kcmpnm,kmhdr.kundef,9) != 0 ){
                                cattemp = malloc(2+strlen(s->h->kcmpnm)+1);
                                strcpy(cattemp,"  ");
                                strcat(cattemp,s->h->kcmpnm);
				subscpy( kvalue, nc, -1, kvalue_s - 1, cattemp );
                                free(cattemp);
				}
			else if( s->h->cmpaz != cmhdr.fundef && s->h->cmpinc != cmhdr.fundef ){
				if( s->h->cmpaz == 0. && s->h->cmpinc == 0. ){
					subscpy( kvalue, nc, -1, kvalue_s - 1, "  VERT"
					  );
					}
				else if( s->h->cmpaz == 0 && s->h->cmpinc == 90. ){
					subscpy( kvalue, nc, -1, kvalue_s - 1, "  NORTH"
					  );
					}
				else if( s->h->cmpaz == 90. && s->h->cmpinc == 90. ){
					subscpy( kvalue, nc, -1, kvalue_s - 1, "  EAST"
					  );
					}
				else{
					cnvita( (int)( s->h->cmpaz + 0.5 ), kcmpaz,9 );
					ljust( kcmpaz,9 );
                                        cattemp = malloc(2+3+1);
                                        strcpy(cattemp,"  ");
                                        strncat(cattemp,kcmpaz,3);
					subscpy( kvalue, nc, nc + 4, kvalue_s - 1, cattemp );
                                        free(cattemp);
					nc = nc + 5;
					if( s->h->cmpinc != 90. ){
						cnvita( (int)( s->h->cmpinc + 0.5 ), kcmpin,9 );
						ljust( kcmpin,9 );
                                                cattemp = malloc(2+2+1);
                                                strcpy(cattemp,"  ");
                                                strncat(cattemp,kcmpin,2);
						subscpy( kvalue, nc, nc + 3, kvalue_s - 
						                            1, cattemp );
                                                free(cattemp);
						}
					}
				}
			}
		else{
			fstrncpy( kvalue, kvalue_s-1, "UNDEFINED", 9);
			lgahdr_v = FALSE;
			}

		/* -- FILENAME:  Name of data file field: */

		}
	else if( memcmp(ktemp,"FILENAME",8) == 0 || memcmp(ktemp,"NAME",4) == 0 ){
        tmp = s->m->filename;
        if(tmp) {
            fstrncpy(kvalue, kvalue_s-1, tmp, strlen(tmp)+1);
        }else{
            fstrncpy(kvalue,kvalue_s-1," ",1);
        }
        lgahdr_v = TRUE;

		/* -- XMARKER:  X time pick and KX descriptor where
		 *              (where X is A, O, F, T0, T1, ... T9) */

		}
	else if( memcmp(ktemp,"AM",2) == 0 ){
		formmarker( s->h->a, s->h->ka,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"OM",2) == 0 ){
		formmarker( s->h->o, s->h->ko,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"FM",2) == 0 ){
		formmarker( s->h->f, s->h->kf,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T0M",3) == 0 ){
		formmarker( s->h->t0, s->h->kt0,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T1M",3) == 0 ){
		formmarker( s->h->t1, s->h->kt1,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T2M",3) == 0 ){
		formmarker( s->h->t2, s->h->kt2,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T3M",3) == 0 ){
		formmarker( s->h->t3, s->h->kt3,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T4M",3) == 0 ){
		formmarker( s->h->t4, s->h->kt4,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T5M",3) == 0 ){
		formmarker( s->h->t5, s->h->kt5,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T6M",3) == 0 ){
		formmarker( s->h->t6, s->h->kt6,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T7M",3) == 0 ){
		formmarker( s->h->t7, s->h->kt7,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T8M",3) == 0 ){
		formmarker( s->h->t8, s->h->kt8,9, kvalue,kvalue_s, &lgahdr_v );
		}
	else if( memcmp(ktemp,"T9M",3) == 0 ){
		formmarker( s->h->t9, s->h->kt9,9, kvalue,kvalue_s, &lgahdr_v );

		/* -- Invalid field: */

		}
	else{
		fstrncpy( kvalue, kvalue_s-1, "INVALID FIELD", 13);
		lgahdr_v = FALSE;

		}

	return( lgahdr_v );
}
