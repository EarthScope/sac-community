
#include <stdio.h>
#include "sss.h"
#include "bool.h"


#include "cpf.h"

void /*FUNCTION*/ xvelocitymode(nerr)
int *nerr;
{
	int lflip;
	int iflip, jvm;
	float flip;
  double tmp;


	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command VELOCITYMODEL.
	 *          This command sets stack velocity model parameters in SSS.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    sss:     mvm
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lvm, ivm, vapp, tovm, vappi, t0vmi, ntvm, ndvm, lcvapp,
	 *             tvm, dvm
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lclog, lclisss, lckey, lcreal, lcra,
	 *             setmsg, apimsg
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    850821:  Added parsing of velocity model number.
	 *    821207:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850821
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
    jvm = 0;
	/* - Parse positional tokens here. */

L_900:
	if( lcmore( nerr ) ){

		/* -- "n": select stack velocity model number. */
		if( lcirc( 1, 2, &jvm ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "MODEL NUMBER MISSING",22 );
			cresp();
			goto L_900;
			}
		}
    if(jvm == 0) {
        fprintf(stderr, "Velocity Model: Model Number Missing\n");
        return;
    }
	/* - Loop on remaining tokens in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "ON|OFF":  turn velocity model on or off. */
		if( lclog( &Lvm[jvm] ) ){

			/* -- "REFRACTEDWAVE|NORMALMOVEOUT":  change type of velocity model.
			 *    (Also allow "NMO" as abbreviation for "NORMALMOVEOUT".) */
			}
		else if( lclist( (char*)kmsss.kvmtp,9, cmsss.nvmtp, &Ivm[jvm] ) ){
			if( Ivm[jvm] == 3 )
				Ivm[jvm] = 2;
			Lvm[jvm] = TRUE;

			/* -- "FLIP":  flip or interchange velocity models 1 and 2. */
			}
		else if( lckey( "FLIP$",6 ) ){
			lflip = Lvm[1];
			Lvm[1] = Lvm[2];
			Lvm[2] = lflip;
			/* iflip = Ivm[1]; */
			Ivm[1] = Ivm[2];
			Ivm[2] = Ivm[1];
			flip = Vapp[1];
			Vapp[1] = Vapp[2];
			Vapp[2] = flip;
			flip = T0vm[1];
			T0vm[1] = T0vm[2];
			T0vm[2] = flip;
			flip = Vappi[1];
			Vappi[1] = Vappi[2];
			Vappi[2] = flip;
			flip = T0vmi[1];
			T0vmi[1] = T0vmi[2];
			T0vmi[2] = flip;
			iflip = Ntvm[1];
			Ntvm[1] = Ntvm[2];
			Ntvm[2] = iflip;
			iflip = Ndvm[1];
			Ndvm[1] = Ndvm[2];
			Ndvm[2] = iflip;
			lflip = Lcvapp[1];
			Lcvapp[1] = Lcvapp[2];
			Lcvapp[2] = lflip;
			lflip = Lct0vm[1];
			Lct0vm[1] = Lct0vm[2];
			Lct0vm[2] = lflip;
			flip = cmsss.tvm[0][0];
			cmsss.tvm[0][0] = cmsss.tvm[1][0];
			cmsss.tvm[1][0] = flip;
			flip = cmsss.dvm[0][0];
			cmsss.dvm[0][0] = cmsss.dvm[1][0];
			cmsss.dvm[1][0] = flip;
			flip = cmsss.tvm[0][1];
			cmsss.tvm[0][1] = cmsss.tvm[1][1];
			cmsss.tvm[1][1] = flip;
			flip = cmsss.dvm[0][1];
			cmsss.dvm[0][1] = cmsss.dvm[1][1];
			cmsss.dvm[1][1] = flip;

			/* -- "VAPP CALCULATE|v":  define apparent velocity properties. */
			}
		else if( lckey( "VAPP$",6 ) ){

			if( lckey( "CALCULATE$",11 ) ){
				Lcvapp[jvm] = TRUE;
				}
			else if( lcreal( &tmp ) ){
        Vapp[jvm] = (float) tmp;
				Lcvapp[jvm] = FALSE;
      }
			else{
				cfmt( "ILLEGAL OPTION:",17 );
				cresp();
				/*            IF(LCMORE(NERR))GO TO 1100 */
				}

			/* -- "T0VM CALCULATE|v":  define intercept time properties. */
			}
		else if( lckey( "T0VM$",6 ) ){

			if( lckey( "CALCULATE$",11 ) ){
				Lct0vm[jvm] = TRUE;
				}
			else if( lcreal( &tmp ) ){
        T0vm[jvm] = (float) tmp;
				Lct0vm[jvm] = FALSE;
				}
			else{
				cfmt( "ILLEGAL OPTION:",17 );
				cresp();
				/*            IF(LCMORE(NERR))GO TO 1200 */
				}

			/* -- "VAPPI v":  set apparent velocity increment. */
			}
		else if( lkreal( "VAPPI$",7, &tmp ) ){
      Vappi[jvm] = (float) tmp;
			/* -- "T0VMI v":  set intercept time increment. */
			}
		else if( lkreal( "T0VMI$",7, &tmp ) ){
      T0vmi[jvm] = (float) tmp;
			/* -- "DVM v1 [v2]":  set VM distance array. */
			}
		else if( lkra( "DVM$",5, 1, 2, &cmsss.dvm[jvm - 1][0], &Ndvm[jvm] ) ){

			/* -- "TVM v1 [v2]":  set VM time array. */
			}
		else if( lkra( "TVM$",5, 1, 2, &cmsss.tvm[jvm - 1][0], &Ntvm[jvm] ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

} /* end of function */

