
#include <string.h>


#include "co.h"

#define DOINITS
#include "sss.h"
#undef DOINITS
#include "tt.h"
#include "hdr.h"
#include "bool.h"

void /*FUNCTION*/ inisss()
{
	int j, j_;

	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMSSS and CMTT.
	 *=====================================================================
	 * PARAMETERS:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *  mach:
	 *  hdr:    fundef  
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *===================================================================== */
	/* PROCEDURE: */
	cmsss.dlytg = 0.;
	cmsss.dlyng = 0.;
	cmsss.wtg = 1.;
	cmsss.lpolg = TRUE;
	cmsss.dstg = cmhdr.fundef;
	cmsss.dlytig = 0.;
	cmsss.dlynig = 0.;
	Wt[1] = 1.;
	Lpol[1] = TRUE;
	cmsss.lnorm = TRUE;
	Dlyt[1] = 0.;
	Dlyn[1] = 0.;
	Dlyvm[1] = 0.;
	cmsss.ltwlim = FALSE;
	cmsss.lsrc = TRUE;
	cmsss.srcfac = RNDOFF;
	cmsss.ndwun = 1;
	strcpy( kmsss.kdwun[0], "KILOMETE" );
	strcpy( kmsss.kdwun[1], "DEGREES " );
	strcpy( kmsss.kdwun[2], "KM      " );
	cmsss.laspect = FALSE;
	cmsss.idwun = 1;
	cmsss.idaop = 1;
	cmsss.dalen = 35.;
	cmsss.dasca = 2.;
	cmsss.idwop = 1;
	cmsss.axdel = 5.;
	cmsss.dwwid = 10.0;
	Dwlim[1] = 0.;
	Dwlim[2] = 0.;
	cmsss.itaop = 1;
	cmsss.talen = 23.;
	cmsss.tasca = 2.;
	cmsss.lroset = FALSE;
	cmsss.iroset = 4;
	cmsss.lrslab = TRUE;
	cmsss.lrscur = FALSE;
	strcpy( kmsss.knmlab, "FILENAME" );
	cmsss.lrswt = TRUE;
	cmsss.lrspol = TRUE;
	cmsss.lrslin = TRUE;
	cmsss.lOriginDefault = TRUE ;		/* formerly llefor maf 961004 */
	cmsss.lPlottingTT = FALSE ;		/* 1 when plotting traveltime curves. maf 961004 */
	cmsss.lorient = TRUE;
	cmsss.xpfrac = 0.1;
	cmsss.xpsize = 1.0;
	cmsss.lpswt = TRUE;
	cmsss.lpspl = TRUE;
	cmsss.lpssum = TRUE;
	cmsss.lpsper = FALSE;
	cmsss.npsper = 3;
	for( j = 1; j <= MVM; j++ ){
		j_ = j - 1;
		Ivm[j] = 1;
		Vapp[j] = 5.6;
		T0vm[j] = 6.0;
		Vappi[j] = 0.;
		T0vmi[j] = 0.;
		Ntvm[j] = 1;
		cmsss.tvm[j_][0] = 33.784;
		cmsss.tvm[j_][1] = 0.;
		Ndvm[j] = 1;
		cmsss.dvm[j_][0] = 200.;
		cmsss.dvm[j_][1] = 0.;
		Lcvapp[j] = FALSE;
		Lct0vm[j] = FALSE;
		Lvm[j] = FALSE;
		}
	cmsss.nvmtp = 3;
	strcpy( kmsss.kvmtp[0], "REFRACTE" );
	strcpy( kmsss.kvmtp[1], "NORMALMO" );
	strcpy( kmsss.kvmtp[2], "NMO     " );
	cmsss.irefr = 1;
	cmsss.inmo = 2;
	cmsss.lnarli = TRUE;
	cmsss.ndxsum = 0;
	cmsss.nlnsum = 0;
	fstrncpy( kmsss.knmsum, MCPFN, "sum", 3 );

	cmsss.lxlabreq = TRUE;
	cmsss.lxlabdef = TRUE;
	cmsss.lylabreq = TRUE;
	cmsss.lylabdef = TRUE;

	cmtt.lrdtt = FALSE;
	cmtt.lpreviousModel = FALSE;	/* .lpreviousModel, npreviousFileNames, */
	cmtt.npreviousFileNames = 0 ;	/* and previousFileNames added.  maf 960829 */
	cmtt.previousFileNames = NULL ;
	cmtt.lttm = FALSE;
	cmtt.nttm = 0;
	cmtt.ittunit = TTDEGREE;  
	cmtt.ttdep = 0.0;
	cmtt.nttrd = 1;
	cmtt.nphases = 0;
	cmtt.nhlines = 1 ;		/* number of header lines to skip.  maf 970808 */
	cmtt.nrdph = 0;
	cmtt.rdvel = 8.0;
	kmtt.kphaseNames = NULL ;	/* for use only with TAUP option. */
	strcpy( kmtt.kttrd[0], "VELOCITY" );
	strcpy( kmtt.kttrd[1], "PHASE   " );

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    961004:  Added stuff for plotting traveltime curves in portrait
	 *		mode and in origin reversed mode.
	 *    920729:  Added travel time curve initializations.
	 *    881114:  Minor cleanup work.
	 *    860304:  Added initialization of PLOTRECORDSECTION variables.
	 *    810414:  Original version.
	 *===================================================================== */

} /* end of function */

