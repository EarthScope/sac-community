/** 
 * @file   inidfm.c
 * 
 * @brief  Initialize the Data File Management Block
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "dfm.h"
#include "clf.h"
#include "bool.h"

#include "SacHeader.h"

#include "cssListOps/cssArchitecture.h"


#include "co.h"
#include "bot.h"
#include "dff.h"
#include "debug.h"

void
dfm_free() {
  int i;
  if(kmdfm.kauthors) {
    for(i = 0; i < cmdfm.iauthors; i++) {
      FREE(kmdfm.kauthors[i]);
    }
    FREE(kmdfm.kauthors);
  }
  if(datafiles) {
    string_list_free(datafiles);
  }
}

/** 
 * Variable initialization of the Data File Management Block
 * 
 * @date   981102:  Remove ncdscnt.  maf
 * @date   970409:  Allow readcss to get picks from .arrival file.  maf
 * @date   970403:  Allow broad selection on channel.  maf
 * @date   961216:  Changed gain variables to station variables.  rcss is
 *             filtering on station rather than gain.  maf
 * @date   961111:  Added cmdfm.lshift to tell whether rcss should shift
 *             the origin time to zero.  maf
 * @date   920515:  Added initialization of ndsflcnt and nflncds to zero.
 * @date   920410:  Removed initialization of ndsflcnt to 1. Previously
 *             used to avoid counting all the file in getdsflcnt.
 * @date   910828:  Added initialization of ncdscnt and ncdsndx array.
 * @date   910812:  Added initialization of multiple data-set stuff.
 * @date   870915:  Deleted fmtsoc, fmtsac, iinmem, etc. variables.
 * @date   870217:  Added initialization of READALPHA command.
 * @date   860917:  Deleted KZDFL.  Added KSUFFX.
 * @date   850801:  Deleted SOCKITTOME format.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   831020:  Added initialization of SYNCH command.
 * @date   820806:  Added initialization of LOVRRQ.
 * @date   810414:  Original version.
 *
 */
void 
inidfm() {

        int jdx;
    int nerr;

	inihdr();

	cmdfm.lcut = FALSE;

	strcpy( kmdfm.kpick[0], "N       " );
	strcpy( kmdfm.kpick[1], "Z       " );
	strcpy( kmdfm.kpick[2], "G       " );
	strcpy( kmdfm.kpick[3], "B       " );
	strcpy( kmdfm.kpick[4], "E       " );
	strcpy( kmdfm.kpick[5], "O       " );
	strcpy( kmdfm.kpick[6], "A       " );
	strcpy( kmdfm.kpick[7], "T0      " );
	strcpy( kmdfm.kpick[8], "T1      " );
	strcpy( kmdfm.kpick[9], "T2      " );
	strcpy( kmdfm.kpick[10], "T3      " );
	strcpy( kmdfm.kpick[11], "T4      " );
	strcpy( kmdfm.kpick[12], "T5      " );
	strcpy( kmdfm.kpick[13], "T6      " );
	strcpy( kmdfm.kpick[14], "T7      " );
	strcpy( kmdfm.kpick[15], "T8      " );
	strcpy( kmdfm.kpick[16], "T9      " );
	strcpy( kmdfm.kpick[17], "F       " );

        datafiles = string_list_init();

	cmdfm.ipckn = 1;
	cmdfm.ipckz = 2;
	cmdfm.ipckg = 3;
	cmdfm.ipckb = 4;
	cmdfm.ipcke = 5;
	cmdfm.ipcko = 6;
	cmdfm.ipcka = 7;
	cmdfm.ipckt0 = 8;
	cmdfm.ipckt1 = 9;
	cmdfm.ipckt2 = 10;
	cmdfm.ipckt3 = 11;
	cmdfm.ipckt4 = 12;
	cmdfm.ipckt5 = 13;
	cmdfm.ipckt6 = 14;
	cmdfm.ipckt7 = 15;
	cmdfm.ipckt8 = 16;
	cmdfm.ipckt9 = 17;
	cmdfm.ipckf = 18;

	Ipckhd[1] = 0;
	Ipckhd[2] = 0;
	Ipckhd[3] = 0;
	Ipckhd[4] = 6;
	Ipckhd[5] = 7;
	Ipckhd[6] = 8;
	Ipckhd[7] = 9;
	for( jdx = 8; jdx <= 18; jdx++ ){
		Ipckhd[jdx] = jdx + 3;
	}

	fstrncpy( kmdfm.krddir, MCPFN, " ", 1);
	fstrncpy( kmdfm.kwrdir, MCPFN, " ", 1);
	fstrncpy( kmdfm.kdirnm, MCPFN, "FIRST_TIME", 10);
	cmdfm.ncdir = indexb( kmdfm.kdirnm,MCPFN+1 );
	cmdfm.lechof = TRUE;

	fstrncpy( kmdfm.krdcssdir, MCPFN, " ", 1);
	kmdfm.lstation = FALSE;
	kmdfm.lchannel = FALSE;
	kmdfm.lbandw = FALSE;
	kmdfm.lorient = FALSE;
	strcpy( kmdfm.kstation, "*     ");	
	strcpy( kmdfm.kchannel, "*       ");
	strcpy( kmdfm.kbandw,   "*       " );
	strcpy( kmdfm.korient,  "*       " );

	cmdfm.ndfl = 0;
	cmdfm.lovrrq = FALSE;

	/* - Data Set Storage initialization */
	cmdfm.ndsflcnt = 0;
	for( jdx = 1; jdx <= MDFL; jdx++ ){
		Ndsndx[jdx] = 0;
	}


	/* - File name suffix initialization */
	strcpy( kmdfm.ksuffx[0], "01" );
	strcpy( kmdfm.ksuffx[1], "02" );
	strcpy( kmdfm.ksuffx[2], "03" );
	strcpy( kmdfm.ksuffx[3], "04" );
	strcpy( kmdfm.ksuffx[4], "05" );
	strcpy( kmdfm.ksuffx[5], "06" );
	strcpy( kmdfm.ksuffx[6], "07" );
	strcpy( kmdfm.ksuffx[7], "08" );
	strcpy( kmdfm.ksuffx[8], "09" );
	strcpy( kmdfm.ksuffx[9], "10" );
	strcpy( kmdfm.ksuffx[10], "11" );
	strcpy( kmdfm.ksuffx[11], "12" );
	strcpy( kmdfm.ksuffx[12], "13" );
	strcpy( kmdfm.ksuffx[13], "14" );
	strcpy( kmdfm.ksuffx[14], "15" );
	strcpy( kmdfm.ksuffx[15], "16" );
	strcpy( kmdfm.ksuffx[16], "17" );
	strcpy( kmdfm.ksuffx[17], "18" );
	strcpy( kmdfm.ksuffx[18], "19" );
	strcpy( kmdfm.ksuffx[19], "20" );
	strcpy( kmdfm.ksuffx[20], "21" );
	strcpy( kmdfm.ksuffx[21], "22" );
	strcpy( kmdfm.ksuffx[22], "23" );
	strcpy( kmdfm.ksuffx[23], "24" );
	strcpy( kmdfm.ksuffx[24], "25" );
	strcpy( kmdfm.ksuffx[25], "26" );
	strcpy( kmdfm.ksuffx[26], "27" );
	strcpy( kmdfm.ksuffx[27], "28" );
	strcpy( kmdfm.ksuffx[28], "29" );
	strcpy( kmdfm.ksuffx[29], "30" );
	strcpy( kmdfm.ksuffx[30], "31" );
	strcpy( kmdfm.ksuffx[31], "32" );
	strcpy( kmdfm.ksuffx[32], "33" );
	strcpy( kmdfm.ksuffx[33], "34" );
	strcpy( kmdfm.ksuffx[34], "35" );
	strcpy( kmdfm.ksuffx[35], "36" );
	strcpy( kmdfm.ksuffx[36], "37" );
	strcpy( kmdfm.ksuffx[37], "38" );
	strcpy( kmdfm.ksuffx[38], "39" );
	strcpy( kmdfm.ksuffx[39], "40" );
	strcpy( kmdfm.ksuffx[40], "41" );
	strcpy( kmdfm.ksuffx[41], "42" );
	strcpy( kmdfm.ksuffx[42], "43" );
	strcpy( kmdfm.ksuffx[43], "44" );
	strcpy( kmdfm.ksuffx[44], "45" );
	strcpy( kmdfm.ksuffx[45], "46" );
	strcpy( kmdfm.ksuffx[46], "47" );
	strcpy( kmdfm.ksuffx[47], "48" );
	strcpy( kmdfm.ksuffx[48], "49" );
	strcpy( kmdfm.ksuffx[49], "50" );
	strcpy( kmdfm.ksuffx[50], "51" );
	strcpy( kmdfm.ksuffx[51], "52" );
	strcpy( kmdfm.ksuffx[52], "53" );
	strcpy( kmdfm.ksuffx[53], "54" );
	strcpy( kmdfm.ksuffx[54], "55" );
	strcpy( kmdfm.ksuffx[55], "56" );
	strcpy( kmdfm.ksuffx[56], "57" );
	strcpy( kmdfm.ksuffx[57], "58" );
	strcpy( kmdfm.ksuffx[58], "59" );
	strcpy( kmdfm.ksuffx[59], "60" );
	strcpy( kmdfm.ksuffx[60], "61" );
	strcpy( kmdfm.ksuffx[61], "62" );
	strcpy( kmdfm.ksuffx[62], "63" );
	strcpy( kmdfm.ksuffx[63], "64" );
	strcpy( kmdfm.ksuffx[64], "65" );
	strcpy( kmdfm.ksuffx[65], "66" );
	strcpy( kmdfm.ksuffx[66], "67" );
	strcpy( kmdfm.ksuffx[67], "68" );
	strcpy( kmdfm.ksuffx[68], "69" );
	strcpy( kmdfm.ksuffx[69], "70" );
	strcpy( kmdfm.ksuffx[70], "71" );
	strcpy( kmdfm.ksuffx[71], "72" );
	strcpy( kmdfm.ksuffx[72], "73" );
	strcpy( kmdfm.ksuffx[73], "74" );
	strcpy( kmdfm.ksuffx[74], "75" );
	strcpy( kmdfm.ksuffx[75], "76" );
	strcpy( kmdfm.ksuffx[76], "77" );
	strcpy( kmdfm.ksuffx[77], "78" );
	strcpy( kmdfm.ksuffx[78], "79" );
	strcpy( kmdfm.ksuffx[79], "80" );
	strcpy( kmdfm.ksuffx[80], "81" );
	strcpy( kmdfm.ksuffx[81], "82" );
	strcpy( kmdfm.ksuffx[82], "83" );
	strcpy( kmdfm.ksuffx[83], "84" );
	strcpy( kmdfm.ksuffx[84], "85" );
	strcpy( kmdfm.ksuffx[85], "86" );
	strcpy( kmdfm.ksuffx[86], "87" );
	strcpy( kmdfm.ksuffx[87], "88" );
	strcpy( kmdfm.ksuffx[88], "89" );
	strcpy( kmdfm.ksuffx[89], "90" );
	strcpy( kmdfm.ksuffx[90], "91" );
	strcpy( kmdfm.ksuffx[91], "92" );
	strcpy( kmdfm.ksuffx[92], "93" );
	strcpy( kmdfm.ksuffx[93], "94" );
	strcpy( kmdfm.ksuffx[94], "95" );
	strcpy( kmdfm.ksuffx[95], "96" );
	strcpy( kmdfm.ksuffx[96], "97" );
	strcpy( kmdfm.ksuffx[97], "98" );
	strcpy( kmdfm.ksuffx[98], "99" );
	strcpy( kmdfm.ksuffx[99], "00" );

	strcpy( kmdfm.kcuter[0], "FATAL   " );
	strcpy( kmdfm.kcuter[1], "USEBE   " );
	strcpy( kmdfm.kcuter[2], "FILLZ   " );
	cmdfm.icuter = 2;
	cmdfm.nrwfmt = 2;
	strcpy( kmdfm.krwfmt[0], "SAC     " );
	strcpy( kmdfm.krwfmt[1], "ALPHA   " );
	cmdfm.iwfmt = 1;
	Icfmt[1] = 1;
	Icfmt[2] = 1;
	strcpy( kmdfm.kcfmt[0], "%#15.7g" );
	strcpy( kmdfm.kcfmt[1], "%#15.7g" );
	fstrncpy( kmdfm.kcfile[0], MCPFN, "in.saf", 6);
	fstrncpy( kmdfm.kcfile[1], MCPFN, "out.saf", 7);

	strcpy( kmdfm.kecbdf, "WARNING " );
	strcpy( kmdfm.kecmem, "SAVE    " );

	/* - SYNCH command. */
	cmdfm.lround = FALSE;

	/* - READALPHA command. */
	cmdfm.ldfree = TRUE;
	fstrncpy( kmdfm.kdform, MCMSG, "(5G15.7)", 8);
	fstrncpy( kmdfm.kdcont, MCMSG, "Y.", 2);

	/* - DATAGEN command. */
	strcpy( kmdfm.kdgsub[0], "LOCAL   " );
	strcpy( kmdfm.kdgsub[1], "REGIONAL" );
	strcpy( kmdfm.kdgsub[2], "TELESEIS" );
	cmdfm.idgsub = 1;
	fstrncpy( kmdfm.kdgfil, MCMSG, " cdv.z", 6);
	cmdfm.ndgfil = 1;

	/* by default rcss shifts orogin time to zero.  */
	cmdfm.lshift = TRUE ;
	/* by default, don't calibrate */
	cmdfm.lscale = FALSE;

	/* by default choose mag from algorithm. */
	cmdfm.nMagSpec = Any ;	

	/* The following added to facilitate rcss reading picks from
	 *   the .arrival css file. 
	 */
        cmdfm.lpref    = FALSE ;  
	cmdfm.iauthors = 0 ;
	kmdfm.kauthors = NULL ;

	/* get name of pick prefrences file */
    
    zbasename(kmdfm.kprefsFileName, MCPFN+1);
    if(strlen(kmdfm.kprefsFileName) == 0) {
        fprintf(stderr, "ERROR: Environmental variable SACAUX not defined.\n");
        exit(1);
    }
    
    /* append the name of the prefs file. */
    crname(kmdfm.kprefsFileName, MCPFN+1, KDIRDL, 
           "csspickprefs", strlen("csspickprefs")+1, &nerr);
    rstrip(kmdfm.kprefsFileName);
	getprefs ( TRUE , TRUE ) ;

	/* SORT command */
	cmdfm.nSortOrder = 0 ;
	for ( jdx = 0 ; jdx < MAXSORTFIELDS ; jdx++ ) {
	    kmdfm.ksort[ jdx ][ 0 ] = '\0' ;
	    cmdfm.idirection[ jdx ] = Ascending ;
	}

	/* added for array option in readcss */
	cmdfm.larray = FALSE ;

	/* read and write ascii by default */
	cmdfm.lrascii = TRUE ;
	cmdfm.lwascii = TRUE ;

	/* COMMIT and ROLLBACK options. */
	cmdfm.icomORroll = COMMIT ;

	/* BINARY or ASCII options */
	strcpy ( kmdfm.kbinORasc[BINARY] , "BINARY  " ) ;
	strcpy ( kmdfm.kbinORasc[ASCII] ,  "ASCII   " ) ;

	/* COMMIT option on deletechannel */
	cmdfm.lcommit = FALSE ;

        /* rdsegy */
        cmdfm.iztype = IB ;

        /* writetable  */
        cmdfm.liftype = FALSE ;
        cmdfm.lheader = FALSE ;

        /* Fix data transfer from SAC buffers to CSS buffers. */
        cmdfm.ltrust = TRUE ;
        cmdfm.nreadflag = RDB ;
        cmdfm.lread = FALSE ;
	cmdfm.nfilesFirst = 0 ;

	/* TRUE has GSE write CM6 data instead of integer. */
        cmdfm.lcm6 = FALSE ;  

}


