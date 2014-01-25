/** 
 * @file   getatw.c
 * 
 * @brief  Convert a relative time window to an absolute one
 * 
 */

#include <string.h>

#include "dfm.h"
#include "hdr.h"

#include "errors.h"


#include "msg.h"
#include "clf.h"
#include "bot.h"

/** 
 * Convert a relative time window to an absolute time window by 
 *   examing the header for the current data file
 * 
 * @param krtw 
 *    Two element array containing the starting and ending relative
 *    time picks
 * @param krtw_s
 *    Length of \p krtw 
 * @param ortw 
 *    Two element array containing the starting and ending relative
 *    time offsets.
 * @param tmin
 *    Minimum value of absolute time window on output
 * @param tmax 
 *    Maximum value of absolute time window on output
 * @param nofmin 
 *    Window offset in points of minimum
 * @param nlnwin 
 *    Number of points in the absolute time window
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_RELATIVE_TIME_PICK
 *    - ERROR_UNDEFINED_START_CUT_TIME
 *    - ERROR_UNDEFINED_STOP_CUT_TIME
 *
 * @date    860304:  Added calculation of NOFWIN and NLNWIN.
 * @date    820623:  Original version.
 *
 */
void 
getatw(char   *krtw, 
       int     krtw_s, 
       double *ortw, 
       double *tmin, 
       double *tmax, 
       int    *nofmin, 
       int    *nlnwin, 
       int    *nerr) {

#define KRTW(I_,J_)	(krtw+(I_)*(krtw_s)+(J_))

	int irtb, irte, nofmax, num;
	float rtrb, rtre ;
	double *const Ortw = &ortw[0] - 1;
  char *tmp;

	*nerr = 0;

	/* - Get indexes of start and stop time picks. */
	irtb = nequal( KRTW(0,0), (char*)kmdfm.kpick,9, MPICK );
	if( (irtb <= 0 || irtb == cmdfm.ipckn) || irtb == cmdfm.ipckg ){
		*nerr = ERROR_ILLEGAL_RELATIVE_TIME_PICK;
		setmsg( "ERROR", *nerr );
		goto L_8888;
	}
	irte = nequal( KRTW(1,0), (char*)kmdfm.kpick,9, MPICK );
	if( irte <= 0 || irte == cmdfm.ipckg ){
		*nerr = ERROR_ILLEGAL_RELATIVE_TIME_PICK;
		setmsg( "ERROR", *nerr );
		goto L_8888;
	}

	/* - Determine start absolute time window. */

	if( irtb == cmdfm.ipckz ){
		*tmin = Ortw[1];
	}
	else{
		rtrb = Fhdr[Ipckhd[irtb]];
		if( rtrb != cmhdr.fundef ){
			*tmin = rtrb + Ortw[1];
		}
		else{
			*tmin = *begin;
			*nerr = ERROR_UNDEFINED_START_CUT_TIME;
			setmsg( "ERROR", *nerr );
            tmp = string_list_get(datafiles, cmdfm.idflc-1);
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
	}

	/* - Determine stop absolute window. */

	if( irte == cmdfm.ipckz ){
		*tmax = Ortw[2];
	}
	else if( irte == cmdfm.ipckn ){
		num = (int)( Ortw[2] );
		*tmax = *tmin + *delta*(float)( num );
	}
	else{
		rtre = Fhdr[Ipckhd[irte]];
		if( rtre != cmhdr.fundef ){
			*tmax = rtre + Ortw[2];
		}
		else{
			*tmax = *ennd;
			*nerr = ERROR_UNDEFINED_STOP_CUT_TIME;
			setmsg( "ERROR", *nerr );
            tmp = string_list_get(datafiles, cmdfm.idflc-1);
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
	}

	/* - Determine offset and length of window in points. */

	*nofmin = (int)( (*tmin - *begin)/ *delta );
	nofmax = (int)( (*tmax - *begin)/ *delta );
	*nlnwin = nofmax - *nofmin + 1;

L_8888:
	return;

#undef	KRTW
} 

