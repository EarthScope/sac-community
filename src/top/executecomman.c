
#include "top.h"
#include "comlists.h"
#include "site.h"


#include "gcm.h"
#include "uom.h"
#include "gem.h"
#include "spe.h"
#include "dfm.h"
#include "bom.h"
#include "nnm.h"
#include "cnd.h"
#include "sam.h"
#include "exm.h"
#include "msg.h"
#include "scm.h"
#include "xyz.h"
#include "icm.h"
#include "sss.h"
#include "co.h"
#include "mat.h"
#include "gam.h"
#include "eam.h"
#include "smm.h"
#include "fks.h"
#include "coda.h"

void /*FUNCTION*/ executecommand(module, index, nerr)
int module, index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a SAC command given its module and index numbers.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    module:  Functional module index number. [i]
	 *    index:   Command index number. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return number.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  top/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    comlists:  MODULEEXTCOM, kextcomnames
	 *    site:      MODULESITECOM
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    010627:  Put mat module back for linux.  pg
	 *    970502:  Remove mat module for non-ANSI compilors.  maf
	 *    900804:  Added "External Command Module". Changed Installation 
	 *             Dependent Module to "Site Command Module".
	 *    900305:  Added XYZ (3-d) Data Processing Module.
	 *    890306:  Added Neural Network Module.
	 *    870901:  Added Conditional Execution Module. (Mandy Goldner)
	 *    870513:  Deleted Output File Module.
	 *             Added Graphics Control Module.
	 *             Changed Data File Module to module number 2.
	 *    870316:  Added Instrument Correction Module.
	 *    861128:  Added Signal Measurement Module.
	 *    831107:  Added ability to define Installation Dependent Commands.
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900804
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Jump to correct module number. */

	switch( module ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		case 7: goto L_700;
		case 8: goto L_800;
		case 9: goto L_900;
		case 10: goto L_1000;
		case 11: goto L_1100;
		case 12: goto L_1200;
		case 13: goto L_1300;
		case 14: goto L_1400;
		case 15: goto L_1500;
		case 16: goto L_1600;
		case 17: goto L_1700;
		case 18: goto L_1800;

#ifdef __STDC__
		case 19: goto L_1900;
#endif
		case 20: goto L_2000;
		}

	

	if( module == MODULEEXTCOM ){      /* - "External Command Module" */
		zexecute( index, nerr );
		goto L_8888;
		}

	

	if( module == MODULESITECOM ){     /* - "Installation (Site) Dependent Module" */
		xsitecom( index, nerr );
		goto L_8888;
		}

	

	*nerr = 901;			   /* - Return if bad value for module number. */
	setmsg( "ERROR", *nerr );
	apcmsg( "in EXECUTECOMMAND",18 );
	goto L_8888;

	

L_100:
	xexmc( index, nerr );   /* - Module EXM:  Executive Module */
	goto L_8888;

	

L_200:
	xdfmc( index, nerr );   /* - Module DFM:  Data File Module */
	goto L_8888;

	

L_300:
	xgcmc( index, nerr );   /* - Module GCM:  Graphics Control Module */
	goto L_8888;

	

L_400:
	xgemc( index, nerr );   /* - Module GEM:  Graphic Environment Module */
	goto L_8888;

	

L_500:
	xgamc( index, nerr );   /* - Module GAM:  Graphic Action Module */
	goto L_8888;

	

L_600:
	xsamc( index, nerr );   /* - Module SAM:  Spectral Analysis Module */
	goto L_8888;

	

L_700:
	xuomc( index, nerr );   /* - Module UOM:  Unary Operations Module */
	goto L_8888;

	

L_800:
	xbomc( index, nerr );   /* - Module BOM:  Binary Operations Module */
	goto L_8888;

	

L_900:
	xeamc( index, nerr );   /* - Module EAM:  Event Analysis Module */
	goto L_8888;

	

L_1000:
	xscmc( index, nerr );   /* - Module SCM: Signal Correction Module */
	goto L_8888;

	

L_1100:
	xspec( index, nerr );   /* - Module SPE:  Spectral Estimation Subprocess */
	goto L_8888;

	

L_1200:
	xsssc( index, nerr );   /* - Module SSS:  Signal Stacking Subprocess */
	goto L_8888;

	

L_1300:
	xsmmc( index, nerr );   /* - Module SMM:  Signal Measurement Module */
	goto L_8888;

	

L_1400:
	xicmc( index, nerr );   /* - Module ICM:  Instrument Correction Module */
	goto L_8888;

	

L_1500:
	xcndc( index, nerr );   /* - Module CND:  Conditional Execution Module */
	goto L_8888;

	

L_1600:
	xnnmc( index, nerr );   /* - Module NNM:  Neural Network Module */
	goto L_8888;

	

L_1700:
	xxyzc( index, nerr );  /* - Module "xyz":  XYZ (3-d) Data Processing Module */
	goto L_8888;

	

L_1800:
	xfksc( index, nerr ); /* - Module "fks":  frequency-wavenumber (k) spectral analysis */
	goto L_8888;
	

L_1900:
	xmatc( index, nerr ); /* - Module "mat":  matlab analysis routines */
	goto L_8888;
	
L_2000:
	xcoda( index, nerr ); /* - Module "coda": Kevin Mayeda's coda magnitude */
	goto L_8888;

L_8888:
	return;

} /* end of function */

