
#include "sss.h"
#include "bool.h"


#include "cpf.h"

void /*FUNCTION*/ xdistancewind(nerr)
int *nerr;
{
	int ntused;
  double tmp[2];
	/*=====================================================================
	 * PURPOSE:  To execute the DISTANCEWINDOW command.
	 *           This command controls the distance window properties
	 *           of the record section plot (PLOTRECORDSECTION).
	 *=====================================================================
	 * Output ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:  1001.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    sss:     kdwun, ndwun
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     idwun, idwop, dwwid, dwlim
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lckey, lcra, lclist, lkreal
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970514:  If no option is specified, it now assumes FIXED.  maf
	 *    860304:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  860304
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "USEDATA":  use the minimum and maximum distance from DFL. */
		if( lckey( "USEDATA$",9 ) ){
			cmsss.idwop = 1;
		}

		/* -- "WIDTH v":  use minimum distance from DFL but force width to fixed value. */
		else if( lkreal( "WIDTH$",7, &tmp[0] ) ){
			cmsss.idwop = 2;
      cmsss.dwwid = (float) tmp[0];
    }

		/* -- "UNITS KILOMETERS|DEGREES":  set distance window units.
		 *    (Also allow "KM" as abbreviation for "KILOMETERS".) */
		else if( lklist( "UNITS$",7, (char*)kmsss.kdwun,9, MDWUN, 
		 &cmsss.idwun ) ){
			if( cmsss.idwun == 3 )
				cmsss.idwun = 1;
			if( cmsss.idwun != cmsss.ndwun ){
				if( cmsss.idwun == 1 ){
					Dwlim[1] = Dwlim[1]*RKMPERDG;
					Dwlim[2] = Dwlim[2]*RKMPERDG;
				}
				else{
					Dwlim[1] = Dwlim[1]/RKMPERDG;
					Dwlim[2] = Dwlim[2]/RKMPERDG;
				}
			}
			cmsss.ndwun = cmsss.idwun;
		}

    /* -- "FIXED v1 v2":  fix data window to be between v1 and v2. */
    else if( lkra( "FIXED$",7, 2, 2, tmp, &ntused ) ){
      cmsss.idwop = 3;
      cmsss.dwlim[0] = tmp[0];
      cmsss.dwlim[1] = tmp[1];
    }
    /* -- "v1 v2":  fix data window to be between v1 and v2. */
		else if ( lcra ( 2 , 2 , tmp , &ntused ) ) {
			cmsss.idwop = 3 ;
      cmsss.dwlim[0] = tmp[0];
      cmsss.dwlim[1] = tmp[1];
		}

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}

	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	return;

} /* end of function */


