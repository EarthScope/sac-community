/** 
 * @file   xdfmc.c
 * 
 * @brief  Execute a Data file list Command
 * 
 */

#include <stdio.h>

#include "dfm.h"
#include "bool.h"

#include "errors.h"


#include "msg.h"
#include "ssi.h"
#include "cpf.h"

/** 
 * Execute a Data File List Command given its index number
 * 
 * @param index 
 *    Index number of the command
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR
 *
 * @date   981030:  Removed dataset commands, added READGSE, READSUDS and SORT
 * @date   970409:  Added PICKAUTHOR and PICIPHASE for flexibility in 
 *                  reading picks with readcss. maf 970409
 * @date   970203:  Added DELETECHANNEL to remove a file from memory.  maf
 * @date   911107:  Added GROUP command to group current data-sets to new ds.
 * @date   910916:  Added LDS command for listing defined and current data-sets.
 * @date   910827:  Added COPY command for copying a data-set
 * @date   910827:  Removed include files hdr and mem.
 * @date   910826:  Added RENAME and DELETE commands for multiple data-sets.
 * @date   910823:  Added CDS - Current Data Set.
 * @date   910430:  Added READCSS command.
 * @date   900905:  Added READSDD and WRITESDD command.
 * @date   870615:  Added DATAGEN command.
 * @date   870209:  Added COPYHDR command.
 * @date   870203:  Deleted DESAMP and MEMORY commands.
 *                  Changed SMOOTH to an action command and moved to SCM.
 *                  Renumbered some of the remaining commands.
 * @date   860917:  Added READALPHA and WILD commands.
 * @date   860317:  Changed command numbering for this module.
 * @date   831020:  Added SYNCH command.
 * @date   820801:  Original version.
 *
 */
void 
xdfmc(int  index, 
      int *nerr) {

	int lsdd;

	*nerr = 0;

	/* - Jump to correct command based upon its index number. */
	switch( index ){
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
		case 20: goto L_2000;
		case 21: goto L_2100;
		case 23: goto L_2300;
		case 24: goto L_2400;
		case 25: goto L_2500;
	        case 26: goto L_2600;
		case 27: goto L_2700;
		case 28: goto L_2800;
		case 29: goto L_2900;
		case 30: goto L_3000;
		case 31: goto L_3100;
		case 32: goto L_3200;
		case 33: goto L_3300;
		}


	/* - Error return if bad index value. */

	*nerr = ERROR_SAC_LOGIC_ERROR;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XDFMC",9 );
	goto L_8888;

L_100:
	/* - Command 01: READ */
	xr( nerr );
	goto L_8888;

L_200:
	/* - Command 02: READERR */
	xrerr( nerr );
	goto L_8888;

L_300:
	/* - Command 03: WRITE */
	lsdd = FALSE;
	xw( lsdd, nerr );
	goto L_8888;

L_400:
	/* - Command 04: CONVERT */
	xconv( nerr );
	goto L_8888;

L_500:
	/* - Command 05: CUT */
	xcrtw( &cmdfm.lcut, (char*)kmdfm.kcut,9, cmdfm.ocut, nerr );
	goto L_8888;

L_600:
	/* - Command 06: CUTERR */
	xcuter( nerr );
	goto L_8888;

L_700:
	/* - Command 07: LISTHDR */
	xlh( nerr );
	goto L_8888;

L_800:
	/* - Command 08: CHNHDR */
	xch( nerr );
	goto L_8888;

L_900:
	/* - Command 09: READHDR */
	xrh( nerr );
	goto L_8888;

L_1000:
	/* - Command 10: WRITEHDR */
	xwh( nerr );
	goto L_8888;

L_1100:
	/* - Command 11: SYNCHRONIZE */
	xsynch( nerr );
	goto L_8888;

L_1200:
	/* - Command 12: WILD */
	xwild( nerr );
	goto L_8888;

L_3300:
    fprintf(stderr, "Command READALPHA is deprecated, please use READTABLE\n");
L_1300:
	/* - Command 13: READALPHA */
    
	xrtab( FALSE, nerr );
	goto L_8888;

L_1400:
	/* - Command 14: COPYHDR */
	xcopyhdr( nerr );
	goto L_8888;

L_1500:
	/* - Command 15: DATAGEN */
	xdatagen( nerr );
	goto L_8888;

L_1600:
	/* - Command 16: READSDD */
	xrsdd( nerr );
	goto L_8888;

L_1700:
	/* - Command 17: WRITESDD */
	lsdd = TRUE;
	xw( lsdd, nerr );
	goto L_8888;

L_1800:
	/* - Command 18: READCSS */
	xrcss( nerr );
	goto L_8888;


L_2000:
	/* - Command 20: CUTIM (located in ssi module, included
	     here because ssi has alternate command parsing which
	     won't work for this command. */
	xcutim ( nerr ) ;
	goto L_8888;

	/* - Command 21: WRITETABLE */
L_2100:
	xwtab( nerr ) ;
	goto L_8888;


L_2300:
	xpickprefs( nerr ) ;
	goto L_8888;

L_2400:
	/* - Command 24: write GSE file */
	xwgse ( nerr ) ;
	goto L_8888;

L_2500:
	/* - Command 25: write to CSS flat files or binary files */
	xwcss( nerr );
	goto L_8888;

L_2600:
	/* removed to compile under sunos without libgdi */
	/*	xrcssdb( nerr ); */ 
	goto L_8888;

L_2700:
	/* - Command 27: DELETECHANNEL removes one or more files.  maf 970203 */
	xdeletechannel( nerr ) ;
	goto L_8888 ;

L_2800:
	/* - Command 28: PICKAUTHOR allows user to specify authors in 
	     for readcss to read picks.  Authors are specified in 
	     priority order. maf 970409 */
	xpickauthor ( nerr ) ;
	goto L_8888;
 
L_2900:
	/* - Command 29: PICKPHASE allows user to specify phases and 
	     authors for specific pick slots (t0, t1, etc.). maf 970409 */
	xpickphase ( nerr ) ;
	goto L_8888;

L_3000:
	/* - Command 30: SORT sorts files in memory on two 
	     numeric header variables.  maf 980812 */
	xsort ( nerr ) ;
	goto L_8888;

L_3100:
	/* - Command 31: READSUDS reads PC-SUDS files into memory */ 
	xrsuds ( nerr ) ;
	goto L_8888;

L_3200:
	/* - Command 31: READGSE reads GSE files into memory */ 
	xrgse ( nerr ) ;
	goto L_8888;

L_8888:
	return;
}

