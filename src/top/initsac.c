
#include <stdlib.h>
#include "unistdx.h"
#include <string.h>

#include "top.h"
#include "bool.h"

#include "co.h"
#include "ssi.h"
#include "select.h"
#include "gpm.h"
#include "dfm.h"
#include "gam.h"
#include "gdm.h"
#include "msg.h"
#include "vars.h"
#include "com.h"
#include "dload.h"
#include "amf.h"
#include "exm.h"
#include "sac_history.h"

DFM_EXTERN
GAM_EXTERN
GDM_EXTERN
VARS_EXTERN
COM_EXTERN
EXTCOM_EXTERN

struct t_cmcom cmcom;
struct t_cmvars cmvars;
struct t_cmextcom cmextcom;

/* external pager program we use to view help fils */
char *pager;

void /*FUNCTION*/
initsac() {
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
	 *             setmsg, apcmsg
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

    sacclear();
    /* - Initialization that can't be handled in lower level modules. */

    /* -- SAC error message function. */
    inimsg();

    /* -- Common blocks. */
    initcommon();

    /* -- Graphics Library. */
    begingraphics(&nerr);
    if(nerr) {
        outmsg();
        clrmsg();
    }

    /* -- Get name of default graphics device. */
    zgetgd(kmgam.kgddef, 9);

    /* Initialize Data Base Module */
    inissi();

    /* Read in the Resource Control file (RC) 
     *   to handle options and settings
     *   Status: Unknown as of 101.2
     *   rc();
     */

    /* - Say hello. */

    sac_history_file_set(NULL);

    if (ifirst) {
        ifirst = 0;
        xabout();
    }

    setup_pager();

    return;

}                               /* end of function */

void /*FUNCTION*/
initblkdata() {
    static int _aini = 1;

    if (_aini) {                /* Do 1 TIME INITIALIZATIONS! */
        cmextcom.nfiles = 0;
        cmgdm.lginit = FALSE;
        cmdfm.ndsflcnt = 0;
        cmvars.lvarsinit = FALSE;
        _aini = 0;
    }

    /* - inc/dload */
    /* - inc/gdm */
    /* - inc/dfm */
    /* - inc/cnv */
    /* - inc/vars */

    return;
}                               /* end of function */

void
setup_pager() {
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
                buf = (char *) malloc(strlen(*j) + strlen(*i) + 2);
                sprintf(buf, "%s/%s", *j, *i);
                if (access(buf, F_OK) == 0) {
                    pager = buf;
                    return;
                }
            }
        }
    }
}
