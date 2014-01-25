

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "top.h"
#include "bool.h"

#include "co.h"
#include "ssi.h"
#include "select.h"
#include "gpm.h"

#define DOINITS
#include "mach.h"
#include "bbs.h"
#include "bom.h"
#include "cnd.h"
#include "cnv.h"
#include "com.h"
#include "comlists.h"
#include "contouring.h"
#include "cpf.h"
#include "datafilelist.h"
#include "dfir.h"
#include "dfm.h"
#include "dload.h"
#include "eam.h"
#include "exm.h"
#include "fir.h"
#include "fks.h"
#include "gam.h"
#include "gd2.h"
#include "gdm.h"
#include "gtm.h"
#include "gem.h"
#include "hdr.h"
#include "icm.h"
#include "lhf.h"
#include "amf.h" /* Includes cmmem */
#include "msg.h"
#include "nnm.h"
#include "nvars.h"
#include "sam.h"
#include "scm.h"
#include "sddhdr.h"
#include "site.h"
#include "smm.h"
#include "snf.h"
#include "spe.h"
#include "specdata.h"
#include "spectrogram.h"
#include "tok.h"
#include "tt.h"
#include "uom.h"
#include "vars.h"
#include "wild.h"
#undef DOINITS

/* external pager program we use to view help fils */
char *pager;

void /*FUNCTION*/ initsac()
{
	int nerr;
        static int ifirst = 1;

	/*=====================================================================
	 * PURPOSE:  To initialize (or reinitialize) SAC.
	 *=====================================================================
	 * MODULE/LEVEL:  exm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MUNOUT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    exm:     ktime, kdate, kmach
	 *    gam:     kgddef
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  inimsg, initcommon, begingraphics, flash, zgetgd,
	 *             setmsg, apcmsg, aplmsg
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920330:  Block data initialization implemented during proting to 
	 *             IBM RISC 6000.
	 *    890105:  Changed from terminal output to message subsystem.
	 *    881122:  Added initialization of default graphics device.
	 *    860327:  Moved call to DELIMS from here to INICM.
	 *    860218:  Added call to INIMSG.
	 *    830818:  Added call to ZGTERM.
	 *    821004:  Added initialization of graphics library.
	 *    810429:  Original version from top of MAINLP.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850222
	 *===================================================================== */
	/* PROCEDURE: */
/*
#ifndef POSIX
        i = ieee_flags("set","direction","nearest",&dummy);
#endif
*/
	/* -- BLOCK DATA initialization of variables in common blocks. */
	initblkdata();

	/* - Initialization that can't be handled in lower level modules. */

	/* -- SAC error message function. */
	inimsg();

	/* -- Common blocks. */
	initcommon();

	/* -- Graphics Library. */
	begingraphics( &nerr );

	/* -- Get name of default graphics device. */
	zgetgd( kmgam.kgddef,9 );

	/* Initialize Data Base Module */
	inissi () ;

        /* Read in the Resource Control file (RC) 
         *   to handle options and settings
         *   Status: Unknown as of 101.2
         *   rc();
         */

	/* - Say hello. */

        sac_history_file_set(NULL);

        if (ifirst){
            ifirst = 0;
	    xabout () ;
        }

	setup_pager();

       
	return;

} /* end of function */

void /*FUNCTION*/ initblkdata()
{
        static int _aini = 1;


        if( _aini ){ /* Do 1 TIME INITIALIZATIONS! */
                cmextcom.nfiles = 0;
                cmgdm.lginit = FALSE;
		cmdfm.ndsflcnt = 0;
                cmicnv.icnver = 0;
                cmvars.lvarsinit = FALSE;
                cmgetvlist.nlevelsgt = 0;
                cmcopyvlist.nlevelscp = 0;
                cmprintvlist.nlevelspr = 0;
                strcpy( kmvars.varsidcode, "VARS" );
                _aini = 0;
        }

        /* - inc/dload */
        /* - inc/gdm */
        /* - inc/dfm */
        /* - inc/cnv */
        /* - inc/vars */

        return ;
} /* end of function */

void setup_pager()
{
    char *pager_search_list[] = { "less", "more", NULL };
    char *pager_dir_list[] = { "/usr/bin", "/bin", NULL };
    char *buf = NULL;

    pager = getenv("PAGER");
    if (pager && pager[0] == '\0')
	pager = NULL;

    if (pager == NULL) {
        char **i, **j;

	pager = NULL;
	for (i = pager_search_list; *i; i++) {
          for (j = pager_dir_list; *j; j++) {
		if (buf)
		    free(buf);
		buf = (char *)malloc(strlen(*j) + strlen(*i) + 2);
		sprintf(buf, "%s/%s", *j, *i);
		if (access(buf, X_OK) == 0) {
		    pager = buf;
		    return;
		}
	    }
	}
    }
}
