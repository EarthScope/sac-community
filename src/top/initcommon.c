
#include <stdlib.h>

#include "top.h"
#include "amf.h"


#include "bom.h"
#include "sam.h"
#include "vars.h"
#include "scm.h"
#include "eam.h"
#include "ncpf.h"
#include "pl.h"
#include "ucf.h"
#include "uom.h"
#include "gdm.h"
#include "icm.h"
#include "exm.h"
#include "spe.h"
#include "sss.h"
#include "contouring.h"
#include "dfm.h"
#include "bbs.h"
#include "cpf.h"
#include "smm.h"
#include "wild.h"
#include "site.h"
#include "xyz.h"
#include "gam.h"
#include "fks.h"
#include "dff.h"

void /*FUNCTION*/ initcommon()
{
	int nerr;

	/*=====================================================================
	 * PURPOSE: Variable initialization of ALL common blocks.
	 *=====================================================================
	 * MODULE/LEVEL:  EXM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    KWSNGL, KWMULT, KWCONC
	 *    MEM:     MMEM, MEPSL
	 *    CSF:     KTOKDL, NTOKDL, KMSGDL, NMSGDL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    MEM:     SACMEM
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920414:  Moved block data initialization to initsac as a new
	 *             procedure, initblkdata, while portint to IBM RISC 6000.
	 *    920410:  Added nlevelsgt data initialization after taking it out
	 *             of getvlist and getvlist2, moving common to inc/vars.
	 *
	 *    920309:  Added include dload and initialization of global nfiles.
	 *    920330:  Added block data initialization of common variables.
	 *             Added include gem, dfm, cnv, vars. Added initialization 
	 *             of ndsflcnt - files in data-set storage.
	 *    911107:  Added nerr check and exti on initcomlists.
	 *    900405:  Added call to INITCONTATTR.
	 *    900305:  Added call to INIXYZ.
	 *    880520:  Fixed bug in call to endgraphics.
	 *    870527:  Deleted call to INIWVT.
	 *    870513:  Deleted call to INIOFM.
	 *    870317:  Added call to INIICM.
	 *    870301:  Added call to create global variable store.
	 *    860407:  Added call to INIAM.
	 *    860327:  Added call to INIWVT.
	 *             Moved call to DELIMS from INISAC to here.
	 *    860218:  Added call to WILDCH.
	 *    830309:  Added call to end graphics library.
	 *    810514:  Added call to INISNF
	 *    810414:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900305
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Terminate graphics library. */
	endgraphics( &nerr );

	/* - SAC's common blocks. */

	inibom();
	inicom();
	inicpf();
	inidfm();
	inieam();
	iniexm();
	inifks();
	inigem();
	inigam();
	inihdr();
	inilhf();
	inisam();
	iniscm();
	inispe();
	initok();
	inisss();
	iniuom();
	initcomlists( &nerr );
	if( nerr == 901 )
		exit(1);
	initsite();
	inismm();
	inibbs();
	iniicm();
	inivars();
	inixyz();
	initcontattr();

	/* - SACMEM array manager. */

	iniam( &cmmem );

	/* - Blackboard store. */

	createbbs( &nerr );
	reperr(nerr);
	sac_report_files_in_memory(&nerr);

	/* - Initialize wildcard patterns with system values. */

	wildch( KWSNGL, KWMULT, KWCONC );

       
	return;

} /* end of function */

