
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gem.h"
#include "gdm.h"
#include "gd2.h"
#include "bool.h"

#include "co.h"
#include "msg.h"
#include "bot.h"

extern display_t sgf;

void 
beginframe2(int *nerr)
{
	char kfname[MCPFN+1], ktemp[MCPFN+1];
	int lexist;
	int idx, jfnum, nc, nc1;
    char *s1;

    memset(kfname, 0, sizeof(kfname));
    memset(ktemp, 0, sizeof(ktemp));
	/*=====================================================================
	 * PURPOSE:  To begin a new frame on graphics device 2 (SGF).
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  gd2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPFN
	 *    gd2:     nfnum, kfnameb, sizetype, xw
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gd2:     jfun, encodesize
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - move2 uses encodesize to encode plot size. See comments below.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     zgtfun, crname, znfile, getvport, getvspace, getworld
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910807:  Changed nstart,nstop to nc1, nc2 to avoid debugger
	 *             problem with global variable of same name.
	 *    900310:  Changed method of computing plot size.
	 *    870923:  Changed method of generating ".sgf" suffixes.
	 *    861014:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861014
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

        for( idx = 0 ; idx < MCPFN ; idx++ )
            kfname[ idx ] = ' ' ;
        kfname[ MCPFN ] = '\0' ;

	/* - Encode frame number and build frame name. */
        if(cmgem.lSGFtemp) { /* PRINT Option ON */
            memset(kfname, 0, MCPFN+1);
            memcpy(kfname, kmgd2.kfilename, strlen(kmgd2.kfilename)+1);
        } else {
            if(! record_filename(NULL)) {
                
                strncpy((s1=malloc(cmgd2.nfnamb+1)),kmgd2.kfnamb,cmgd2.nfnamb);
                s1[cmgd2.nfnamb] = '\0';
                sprintf(ktemp,"%s%3.3d", s1, cmgd2.nfnum );
                free(s1);
                
                if( cmgd2.nfdir > 0 ){
                    fstrncpy( kfname, MCPFN, kmgd2.kfdir, min(cmgd2.nfdir,MCPFN));
                    crname( kfname,MCPFN+1, KDIRDL, ktemp,MCPFN+1, nerr );
                    if( *nerr != 0 )
                        goto L_8888;
                }
                else{
                    strcpy( kfname, ktemp );
                }
                nc = indexb( kfname,MCPFN+1 );
                subscpy( kfname, nc, -1, MCPFN, ".sgf" );
            } else {
                strcpy(kfname, record_filename(NULL));
            }
            /* save filename in global area */
            strncpy ( kmgd2.kfilename , kfname , MCPFN ) ;
            kmgd2.kfilename[ MCPFN - 1 ] = '\0' ;
            terminate ( kmgd2.kfilename ) ;
        }
	/* - If frame number is 0, search directory for SGFs and set
	 *   frame number to next available number. */

	if( cmgd2.lfnum && cmgd2.nfnum <= 0 && !cmgem.lSGFtemp ){
		nc1 = cmgd2.nfdir + 1 + cmgd2.nfnamb;
		nc1 = nc1 + 2;
		jfnum = 1;
		lexist = TRUE;
L_1000:
		if( !lexist ){
			cmgd2.nfnum = jfnum;
		}
		else if( jfnum < 999 ){
                        sprintf(&kfname[nc1-1],"%3.3d",jfnum);
                        zinquire(kfname,&lexist);
			goto L_1000;
		}
		else{
			*nerr = 2401;
			setmsg( "ERROR", *nerr );
			goto L_8888;
		}
	}

	/* - Open a new file. */

	znfile( &cmgd2.jfun, kfname,MCPFN+1, "DATA",5, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Initialize buffer and disk pointers. */

	cmgd2.jfbpnt = 1;
	cmgd2.jfdpnt = 0;

	/* - Set flag to encode plot size if a fixed or scaled plot size
	 *   has been requested. The actual encoding is done by subroutine
	 *   "move2" the first time it is called.  This coupling is needed
	 *   because the size is related to the initial viewport and/or world
	 *   coordinates which are not known when this subroutine is called. */

	if( strcmp(kmgd2.sizetype,"NORMAL  ") == 0 ){
		cmgd2.encodesize = FALSE;
	}
	else{
		cmgd2.encodesize = TRUE;
	}
        
	/* - If SGF device is active, must also reset color, linestyle, etc.
	 *   because each SGF frame is in a separate disk file. */
        sgf.set_color( cmgdm.icolor );
        sgf.set_line_style( &cmgdm.iline );
        sgf.set_text_size( cmgdm.twidth, cmgdm.thgt );
        

L_8888:
	return;

}

