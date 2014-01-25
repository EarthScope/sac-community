#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gem.h"
#include "gam.h"
#include "bool.h"


#include "co.h"

void /*FUNCTION*/ inigam()
{
	int j, j_;

	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMGAM.
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *   LPPKPP:  Plot NPPKPP files per plot frame if .TRUE.
	 *            Plot all files on one frame if .FALSE.
	 *   NPPKPP:  Number of files per plot frame if LPPKPP is .TRUE.
	 *   LPPKUT:  Display picked times in UT/GMT if .TRUE.
	 *            Display times relative to file zero if .FALSE.
	 *   LPPKRL:  Plot reference lines whenever "Z" cursor response
	 *            is invoked. Lines are drawn at +/- "VPPKRL".
	 *   VPPKRL:  Amplitude of reference lines.
	 *   LMKALL:  Mark all files in current frame if .TRUE.
	 *            Mark only file being pointed at if .FALSE.
	 *===================================================================== */
	/* PROCEDURE: */
	strcpy( kmgam.kgddef, "        " );
	cmgam.lppkpp = FALSE;
	cmgam.lppkut = TRUE;
	cmgam.lppkrl = FALSE;
	cmgam.vppkrl = 0.1;
	cmgam.lmkall = FALSE;
	cmgam.lsavelocs = FALSE;
	cmgam.nppkpp=1;                /* default number of plots set to 1 DD 3-12-97 */

	cmgam.lp2abs = TRUE;

	cmgam.lwaitr = TRUE;
	cmgam.lwaite = FALSE;

	cmgam.lbdrrq = TRUE;
	kmgam.kopqui = 'Q';
	kmgam.kopdel = 'D';
	kmgam.kopmac = 'M';
	kmgam.kopcmt = '*';
	kmgam.kopbe = '[';
	kmgam.kopee = ']';
    memset(kmgam.kopetx, 0, 81);
	cmgam.nop1 = 8;
	Kop1[1] = 'G';
	Kop1[2] = 'O';
	Kop1[3] = 'L';
	Kop1[4] = 'A';
	Kop1[5] = 'R';
	Kop1[6] = 'C';
	Kop1[7] = 'N';
	Kop1[8] = 'B';
	cmgam.nop2 = 0;
	cmgam.nope = 62;
	cmgam.nopei = 61;
	strcpy( kmgam.kope[0], "HL" );
	strcpy( kmgam.kope[1], "HC" );
	strcpy( kmgam.kope[2], "HR" );
	strcpy( kmgam.kope[3], "VB" );
	strcpy( kmgam.kope[4], "VC" );
	strcpy( kmgam.kope[5], "VT" );
	strcpy( kmgam.kope[6], "ST" );
	strcpy( kmgam.kope[7], "SS" );
	strcpy( kmgam.kope[8], "SM" );
	strcpy( kmgam.kope[9], "SL" );
	strcpy( kmgam.kope[10], "Q1" );
	strcpy( kmgam.kope[11], "Q2" );
	strcpy( kmgam.kope[12], "Q3" );
	strcpy( kmgam.kope[13], "Q4" );
	strcpy( kmgam.kope[14], "F1" );
	strcpy( kmgam.kope[15], "F2" );
	strcpy( kmgam.kope[16], "F3" );
	strcpy( kmgam.kope[17], "F4" );
	strcpy( kmgam.kope[18], "F5" );
	strcpy( kmgam.kope[19], "F6" );
	strcpy( kmgam.kope[20], "F7" );
	strcpy( kmgam.kope[21], "F8" );
	strcpy( kmgam.kope[22], "AT" );
	strcpy( kmgam.kope[23], "AS" );
	strcpy( kmgam.kope[24], "AM" );
	strcpy( kmgam.kope[25], "AL" );
	strcpy( kmgam.kope[26], "AF" );
	strcpy( kmgam.kope[27], "AU" );
	strcpy( kmgam.kope[28], "AV" );
	strcpy( kmgam.kope[29], "AI" );
	strcpy( kmgam.kope[30], "L1" );
	strcpy( kmgam.kope[31], "L2" );
	strcpy( kmgam.kope[32], "L3" );
	strcpy( kmgam.kope[33], "L4" );
	strcpy( kmgam.kope[34], "W1" );
	strcpy( kmgam.kope[35], "W2" );
	strcpy( kmgam.kope[36], "W3" );
	strcpy( kmgam.kope[37], "W4" );
	strcpy( kmgam.kope[38], "N2" );
	strcpy( kmgam.kope[39], "N3" );
	strcpy( kmgam.kope[40], "N4" );
	strcpy( kmgam.kope[41], "N5" );
	strcpy( kmgam.kope[42], "N6" );
	strcpy( kmgam.kope[43], "N7" );
	strcpy( kmgam.kope[44], "N8" );
	strcpy( kmgam.kope[45], "N9" );
	strcpy( kmgam.kope[46], "P1" );
	strcpy( kmgam.kope[47], "P2" );
	strcpy( kmgam.kope[48], "P3" );
	strcpy( kmgam.kope[49], "P4" );
	strcpy( kmgam.kope[50], "CN" );
	strcpy( kmgam.kope[51], "CR" );
	strcpy( kmgam.kope[52], "CG" );
	strcpy( kmgam.kope[53], "CY" );
	strcpy( kmgam.kope[54], "CB" );
	strcpy( kmgam.kope[55], "CM" );
	strcpy( kmgam.kope[56], "CC" );
	strcpy( kmgam.kope[57], "CW" );
	strcpy( kmgam.kope[58], "QH" );
	strcpy( kmgam.kope[59], "QS" );
	strcpy( kmgam.kope[60], "BH" );
	strcpy( kmgam.kope[61], "BV" );
	cmgam.nopt = 2;
	Kopt[1] = 'T';
	Kopt[2] = 'U';
	cmgam.nopn = 1;
	Kopn[1] = 'S';

	cmgam.icsize = 1;
	cmgam.nhtick = 9;
	cmgam.nvtick = 9;

	cmgam.pcalen = 0.015;
	cmgam.pcamul = 2.;
	cmgam.pcaang = 0.25;
	cmgam.lpcavi = TRUE;
	cmgam.lpcafi = FALSE;

	cmgam.pcdegi = 2.;
	cmgam.npcpsi = 4;

	cmgam.scamac = 1.;
	cmgam.rotmac = 0.;
	strcpy( kmgam.kpcmsu, ".pcm" );
        fstrncpy( kmgam.kpcmac, MCPFN, "out", 3);
        fstrncpy( kmgam.kpcmac+3, MCPFN-3, kmgam.kpcmsu, strlen(kmgam.kpcmsu));
        
	cmgam.lrplrq = FALSE;
	cmgam.lpcfil = TRUE;
	strcpy( kmgam.kpcfsu, ".pcf" );
        fstrncpy( kmgam.kpcfil, MCPFN, "out", 3);
        fstrncpy( kmgam.kpcfil+3, MCPFN-3, kmgam.kpcfsu, strlen(kmgam.kpcfsu));

	/* - Initialization for FILEID command. */

	cmgam.lfidrq = TRUE;
	cmgam.lfinorq = FALSE;	/* 1 or 0 to display or not the file number. maf 970204 */
	cmgam.nfidtp = 4;
	strcpy( kmgam.kfidtp[0], "DEFAULT " );
	strcpy( kmgam.kfidtp[1], "STD     " );
	strcpy( kmgam.kfidtp[2], "NAME    " );
	strcpy( kmgam.kfidtp[3], "LIST    " );
	strcpy( kmgam.kfidtp[4], "UNUSED  " );
	cmgam.ifidtp = 1;
	cmgam.nfidst = 4;
	strcpy( kmgam.kfidst[0], "KEVNM   " );
	strcpy( kmgam.kfidst[1], "KSTCMP  " );
	strcpy( kmgam.kfidst[2], "KZDATE  " );
	strcpy( kmgam.kfidst[3], "KZTIME  " );
	strcpy( kmgam.kfidst[4], "UNUSED  " );
	strcpy( kmgam.kfidst[5], "UNUSED  " );
	strcpy( kmgam.kfidst[6], "UNUSED  " );
	strcpy( kmgam.kfidst[7], "UNUSED  " );
	strcpy( kmgam.kfidst[8], "UNUSED  " );
	strcpy( kmgam.kfidst[9], "UNUSED  " );
	for( j = 1; j <= cmgam.nfidst; j++ ){
		j_ = j - 1;
		strcpy( kmgam.kfidnm[j_], kmgam.kfidst[j_] );
		}
	cmgam.nfidnm = cmgam.nfidst;
	cmgam.nfidlc = 4;
	strcpy( kmgam.kfidlc[0], "UR      " );
	strcpy( kmgam.kfidlc[1], "UL      " );
	strcpy( kmgam.kfidlc[2], "LR      " );
	strcpy( kmgam.kfidlc[3], "LL      " );
	strcpy( kmgam.kfidlc[4], "UNUSED  " );
	cmgam.iur = 1;
	cmgam.iul = 2;
	cmgam.ilr = 3;
	cmgam.ill = 4;
	cmgam.ifidlc = cmgam.iur;
	cmgam.nfidfm = 3;
	strcpy( kmgam.kfidfm[0], "EQUALS  " );
	strcpy( kmgam.kfidfm[1], "COLONS  " );
	strcpy( kmgam.kfidfm[2], "NONAMES " );
	strcpy( kmgam.kfidfm[3], "UNUSED  " );
	strcpy( kmgam.kfidfm[4], "UNUSED  " );
	cmgam.ifidfm = 3;
	cmgam.tsfid = cmgem.tsdef;

	/* - Initialization for PICKS command. */

	cmgam.ldsppk = TRUE;
	strcpy( kmgam.kpknam[0], "O       " );
	strcpy( kmgam.kpknam[1], "A       " );
	strcpy( kmgam.kpknam[2], "T0      " );
	strcpy( kmgam.kpknam[3], "T1      " );
	strcpy( kmgam.kpknam[4], "T2      " );
	strcpy( kmgam.kpknam[5], "T3      " );
	strcpy( kmgam.kpknam[6], "T4      " );
	strcpy( kmgam.kpknam[7], "T5      " );
	strcpy( kmgam.kpknam[8], "T6      " );
	strcpy( kmgam.kpknam[9], "T7      " );
	strcpy( kmgam.kpknam[10], "T8      " );
	strcpy( kmgam.kpknam[11], "T9      " );
	strcpy( kmgam.kpknam[12], "F       " );
	strcpy( kmgam.kpktyp[0], "VERTICAL" );
	strcpy( kmgam.kpktyp[1], "HORIZONT" );
	strcpy( kmgam.kpktyp[2], "CROSS   " );
	Ipktyp[1] = 1;
	Ipktyp[2] = 1;
	Ipktyp[3] = 1;
	Ipktyp[4] = 1;
	Ipktyp[5] = 1;
	Ipktyp[6] = 1;
	Ipktyp[7] = 1;
	Ipktyp[8] = 1;
	Ipktyp[9] = 1;
	Ipktyp[10] = 1;
	Ipktyp[11] = 1;
	Ipktyp[12] = 1;
	Ipktyp[13] = 1;
	cmgam.pkwdth = 0.1;
	cmgam.pkhgth = 0.1;
	cmgam.tspk = cmgem.tsdef;

	/* - Initialization for XLIM command. */

	cmgam.lrtwxl = FALSE;
	strcpy( kmgam.krtwxl[0], "B       " );
	strcpy( kmgam.krtwxl[1], "E       " );
	Ortwxl[1] = 0.;
	Ortwxl[2] = 0.;

	/* - Initialization for YLIM command. */

	cmgam.nylim = 1;
	for( j = 1; j <= MYLIM; j++ ){
		j_ = j - 1;
		strcpy( kmgam.kylims[j_], "OFF     " );
		}

       

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    970204:  Added cmgam.lfinorq to flag whether or not to display
	 *             file numbers in plots.  maf
	 *    910608:  Uninitialized kgddef from TERMINAL to blanks so that
	 *             a prompt will be issued from begindevices when the
	 *             environmental SACGRAPHICSDEVICE is undefined.
	 *             Initializing kgddef to TERMINAL causes sac to try
	 *             and begin graphics to TERM, which is usually "sun"
	 *             or "sun-cmd". (wct)
	 *    850307:  Added initialization for YLIM command.
	 *    830623:  Added enviroment options with arbitrary integer argument.
	 *    810414:  Original version.
	 *===================================================================== */

} /* end of function */

