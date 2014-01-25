
#include <string.h>

#include "icm.h"

#include "msg.h"

void /*FUNCTION*/ rstn(nfreq, delfrq, xre, xim, subtyp, subtyp_s, 
	 nerr)
int nfreq;
double delfrq, xre[], xim[];
char *subtyp;   int subtyp_s;
int *nerr;
{


	/*   .....RSTN Seismometer.....
	 *
	 *   .....Poles and zeros due to D. Breding (1983), Data Users' Guide
	 *        for the Regional Seismic Test Network (RSTN), Sandia Report
	 *        SAND82-2935.....
	 * */


	if( memcmp(subtyp,"CP",2) == 0 ){
		if( memcmp(subtyp+2,"KL",2) == 0 ){
			if( memcmp(subtyp+4,".Z",2) == 0 ){
				clz( nfreq, delfrq, xre, xim );
				}
			else if( memcmp(subtyp+4,".N",2) == 0 || memcmp(subtyp+4
			 ,".E",2) == 0 ){
				clh( nfreq, delfrq, xre, xim );
				}
			else{
				*nerr = 2105;
				setmsg( "ERROR", *nerr );
				apcmsg( "RSTN:",6 );
				apcmsg( subtyp,subtyp_s );
				goto L_8888;
				}
			}
		else if( memcmp(subtyp+2,"KM",2) == 0 ){
			if( memcmp(subtyp+4,".Z",2) == 0 ){
				cmz( nfreq, delfrq, xre, xim );
				}
			else if( memcmp(subtyp+4,".N",2) == 0 || memcmp(subtyp+4
			 ,".E",2) == 0 ){
				cmh( nfreq, delfrq, xre, xim );
				}
			else{
				*nerr = 2105;
				setmsg( "ERROR", *nerr );
				apcmsg( "RSTN:",6 );
				apcmsg( subtyp,subtyp_s );
				goto L_8888;
				}
			}
		else if( memcmp(subtyp+2,"KS",2) == 0 ){
			if( memcmp(subtyp+4,".Z",2) == 0 ){
				csz( nfreq, delfrq, xre, xim );
				}
			else if( memcmp(subtyp+4,".N",2) == 0 || memcmp(subtyp+4
			 ,".E",2) == 0 ){
				csh( nfreq, delfrq, xre, xim );
				}
			else{
				*nerr = 2105;
				setmsg( "ERROR", *nerr );
				apcmsg( "RSTN:",6 );
				apcmsg( subtyp,subtyp_s );
				goto L_8888;
				}
			}
		else{
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "RSTN:",6 );
			apcmsg( subtyp,subtyp_s );
			goto L_8888;
			}
		}
	else if( ((memcmp(subtyp,"NY",2) == 0 || memcmp(subtyp
	 ,"NT",2) == 0) || memcmp(subtyp,"ON",2) == 0) || memcmp(subtyp
	 ,"SD",2) == 0 ){
		if( memcmp(subtyp+2,"KL",2) == 0 ){
			rsl( nfreq, delfrq, xre, xim );
			}
		else if( memcmp(subtyp+2,"KM",2) == 0 ){
			rsm( nfreq, delfrq, xre, xim );
			}
		else if( memcmp(subtyp+2,"7S",2) == 0 ){
			rs7( nfreq, delfrq, xre, xim, subtyp );
			}
		else if( memcmp(subtyp+2,"KS",2) == 0 ){
			rsk( nfreq, delfrq, xre, xim, subtyp );
			}
		else{
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "RSTN:",6 );
			apcmsg( subtyp,subtyp_s );
			goto L_8888;
			}
		}
	else{
		*nerr = 2105;
		setmsg( "ERROR", *nerr );
		apcmsg( "RSTN:",6 );
		apcmsg( subtyp,subtyp_s );
		goto L_8888;
		}

L_8888:
	return;
} /* end of function */

