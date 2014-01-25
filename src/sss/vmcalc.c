
#include <math.h>

#include "sss.h"
#include "co.h"


#include "msg.h"

void /*FUNCTION*/ vmcalc(jvm, nerr)
int jvm, *nerr;
{
	float t0vmsq, vappsq;



	/*=====================================================================
	 * PURPOSE:  Calculate certain velocity model variables.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    810120:  Changed to output message retrieval from disk.
	 *    790831:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Check for valid velocity model number. */

	if( jvm <= 0 || jvm > MVM ){
		*nerr = 901;
		setmsg( "ERROR", *nerr );
		apcmsg( "VMCALC #1",10 );
		goto L_8888;
		}

	/* - Determine which velocity model to use. */

	/* -- Normal moveout calculations: */
	if( Ivm[jvm] == cmsss.inmo ){
		if( Lct0vm[jvm] && Lcvapp[jvm] ){
			if( Ndvm[jvm] == 2 && Ntvm[jvm] == 2 ){
				vappsq = (cmsss.dvm[jvm - 1][0]*cmsss.dvm[jvm - 1][0] - 
				 cmsss.dvm[jvm - 1][1]*cmsss.dvm[jvm - 1][1])/(cmsss.tvm[jvm - 1][0]*
				 cmsss.tvm[jvm - 1][0] - cmsss.tvm[jvm - 1][1]*cmsss.tvm[jvm - 1][1]);
				if( vappsq > 0. ){
					Vapp[jvm] = sqrt( vappsq );
					t0vmsq = cmsss.tvm[jvm - 1][0]*cmsss.tvm[jvm - 1][0] - 
					 cmsss.dvm[jvm - 1][0]*cmsss.dvm[jvm - 1][0]/vappsq;
					if( t0vmsq > 0. ){
						T0vm[jvm] = sqrt( t0vmsq );
						}
					else{
						*nerr = 5111;
						setmsg( "ERROR", *nerr );
						goto L_8888;
						}
					}
				else{
					*nerr = 5111;
					setmsg( "ERROR", *nerr );
					goto L_8888;
					}
				}
			else{
				*nerr = 5112;
				setmsg( "ERROR", *nerr );
				apimsg( Ndvm[jvm] );
				apimsg( Ntvm[jvm] );
				goto L_8888;
				}
			}
		else if( Lct0vm[jvm] ){
			if( Ndvm[jvm] >= 1 && Ntvm[jvm] >= 1 ){
				vappsq = Vapp[jvm]*Vapp[jvm];
				t0vmsq = cmsss.tvm[jvm - 1][0]*cmsss.tvm[jvm - 1][0] - 
				 cmsss.dvm[jvm - 1][0]*cmsss.dvm[jvm - 1][0]/vappsq;
				if( t0vmsq > 0. ){
					T0vm[jvm] = sqrt( t0vmsq );
					}
				else{
					*nerr = 5111;
					setmsg( "ERROR", *nerr );
					goto L_8888;
					}
				}
			else{
				*nerr = 5112;
				setmsg( "ERROR", *nerr );
				apimsg( Ndvm[jvm] );
				apimsg( Ntvm[jvm] );
				goto L_8888;
				}
			}
		else if( Lcvapp[jvm] ){
			if( Ndvm[jvm] >= 1 && Ntvm[jvm] >= 1 ){
				t0vmsq = T0vm[jvm]*T0vm[jvm];
				vappsq = cmsss.dvm[jvm - 1][0]*cmsss.dvm[jvm - 1][0]/
				 (powi(cmsss.tvm[jvm - 1][0],2) - t0vmsq);
				if( vappsq > 0. ){
					Vapp[jvm] = sqrt( vappsq );
					}
				else{
					*nerr = 5111;
					setmsg( "ERROR", *nerr );
					goto L_8888;
					}
				}
			else{
				*nerr = 5112;
				setmsg( "ERROR", *nerr );
				apimsg( Ndvm[jvm] );
				apimsg( Ntvm[jvm] );
				goto L_8888;
				}
			}

		/* -- Refracted wave calculations: */
		}
	else if( Ivm[jvm] == cmsss.irefr ){
		if( Lct0vm[jvm] && Lcvapp[jvm] ){
			if( Ndvm[jvm] == 2 && Ntvm[jvm] == 2 ){
				Vapp[jvm] = (cmsss.dvm[jvm - 1][0] - cmsss.dvm[jvm - 1][1])/
				 (cmsss.tvm[jvm - 1][0] - cmsss.tvm[jvm - 1][1]);
				T0vm[jvm] = cmsss.tvm[jvm - 1][0] - cmsss.dvm[jvm - 1][0]/
				 Vapp[jvm];
				}
			else{
				*nerr = 5112;
				setmsg( "ERROR", *nerr );
				apimsg( Ndvm[jvm] );
				apimsg( Ntvm[jvm] );
				goto L_8888;
				}
			}
		else if( Lct0vm[jvm] ){
			if( Ndvm[jvm] >= 1 && Ntvm[jvm] >= 1 ){
				T0vm[jvm] = cmsss.tvm[jvm - 1][0] - cmsss.dvm[jvm - 1][0]/
				 Vapp[jvm];
				}
			else{
				*nerr = 5112;
				setmsg( "ERROR", *nerr );
				apimsg( Ndvm[jvm] );
				apimsg( Ntvm[jvm] );
				goto L_8888;
				}
			}
		else if( Lcvapp[jvm] ){
			if( Ndvm[jvm] >= 1 && Ntvm[jvm] >= 1 ){
				Vapp[jvm] = cmsss.dvm[jvm - 1][0]/(cmsss.tvm[jvm - 1][0] - 
				 T0vm[jvm]);
				}
			else{
				*nerr = 5112;
				setmsg( "ERROR", *nerr );
				apimsg( Ndvm[jvm] );
				apimsg( Ntvm[jvm] );
				goto L_8888;
				}
			}

		}
	else{
		*nerr = 901;
		setmsg( "ERROR", *nerr );
		apcmsg( "VMCALC #2",10 );
		goto L_8888;
		}

L_8888:
	return;

} /* end of function */

