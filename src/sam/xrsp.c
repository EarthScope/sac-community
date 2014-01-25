
#include <string.h>
#include <math.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "ssi.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void /*FUNCTION*/ xrsp(nerr)
int *nerr;
{
  char krspnm[MCPFN+1];
	int irsptp, jdx, jdfl, jdfl_, 
	 junk, ndx1, ndx2, nfreq, nlcdsk, nlcmem, nlen, nrspnm, 
	 nun, lswap[ MDFL ] ;

    char s1[4];
    char *tmp;
    float *Sacmem1, *Sacmem2;
    string_list *list;
    static string_list *last_list = NULL;
    
    if(!last_list) {
        last_list = string_list_init();
    }

    list = NULL;
    for( jdx = 0 ; jdx < MDFL ; jdx++ )
      lswap[ jdx ] = 0 ;

	/*=====================================================================
	 * PURPOSE:  To execute the action command READSP.
	 *           This command reads separate spectral component files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL: SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     IAMPH, IRLIM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     NDFL, KDFL, NCOMP, NDXHDR, NLNDTA, NDXDTA
	 *    HDR:     All.
	 *    MEM:     SACMEM
	 *    SAM:     LRSPE, LRAMPH
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, LCDFL, CFMT, CRESP
	 *             VFLIST, CLEARDFL, ZGTFUN, INDEXB, ZOPEN, RDHDR, ZRABS,
	 *             ALLAMB, GETFIL, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    KRSPNM:  Name of disk file being read.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    921105:  Modification of 920420 din't work. Removed. The filenames
	 *             are modified.  so can't do a lfilesok before this.
	 *    920501:  Added memory delete on READERR NOFILES MEMORY DELETE cond.
	 *    920420:  Added test of file existence (lfilesok) before updating 
	 *             data-set info. Done in andicipation that a MORE option
	 *             may someday be implemented.
	 *    920316:  Added data-set storage logic.
	 *    880425:  Added option to read SPE files and convert to AMPH format.
	 *    871124:  Fixed bug that was not updating header correctly.
	 *    850614:  Major rewrite due to addition of memory manager.
	 *    820621:  Changed to newest set of parsing functions.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810508:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  871124
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

        strcpy(s1,"   ");

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){
	    /* -- "AMPH|RLIM|SPE":  choose format for spectral files. */
	    if( lckey( "SPE#$",6 ) ){
		cmsam.lrspe = TRUE;
		cmsam.lramph = TRUE;
	    }
	    else if( lclog2( "AMPH#$",7, "RLIM#$",7, &cmsam.lramph ) )
		cmsam.lrspe = FALSE;

	    /* -- TRUST:  whether or not to trust matching evids while
         moving data from SAC buffers to CSS buffers. */
      else if( lklog( "TRUST#$",8, &cmdfm.ltrust ) )
        { /* do nothing */ }

	    /* -- List of data files to be read. */
      else if( ( list = lcdfl() ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

    if(!list) {
        list = string_list_init();
        string_list_extend(list, last_list);
    }

	if( cmdfm.nreadflag == RDB )
	    cmdfm.nreadflag = HIGH ;

	/* - EXECUTION PHASE: */

	/* - Define suffixes. */

	if( cmsam.lramph ){
	    strcpy( kmsam.krsps1, ".am     " );
	    strcpy( kmsam.krsps2, ".ph     " );
	    irsptp = *iamph;
	}
	else{
	    strcpy( kmsam.krsps1, ".rl     " );
	    strcpy( kmsam.krsps2, ".im     " );
	    irsptp = *irlim;
	}

	/* -- Clear data sets */
	cleardfl( nerr );

	/* - Copy input data file list to real one. */
    string_list_clear(datafiles);
    string_list_extend(datafiles, list);
    string_list_free(list);
	cmdfm.ndfl = string_list_length(datafiles);

	/* - Read headers (from first file in each pair) into memory. */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	    /* -- Determine character length of input file name. */
        tmp = string_list_get(datafiles, jdfl-1);
	    fstrncpy( krspnm, MCPFN, tmp, strlen(tmp)+1);
	    nrspnm = min( strlen(krspnm), MCPFN - 3 );

	    /* -- Prepare new name */
	    if( !cmsam.lrspe ) {
                strncpy(s1,kmsam.krsps1,3);
		subscpy( krspnm, nrspnm, -1, MCPFN, s1);
	    }

	    /* -- Open file. */
	    zopen_sac( &nun, krspnm,strlen(krspnm)+1, "RODATA",7, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Allocate block for the header in data-set storage. */
	    allamb( &cmmem, SAC_HEADER_WORDS , &Ndxhdr[jdfl], nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    Ndsndx[jdfl] = 1 ;

	    /* -- Read header. */
	    lswap[ jdfl ] = rdhdr( jdfl, &nun, krspnm, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    if( *nevid == -12345 || *norid == -12345 )
		cmdfm.nreadflag = LOW ;


	    /* -- Adjust certain header fields. */
	    nfreq = *npts;
	    *npts = 2*(nfreq - 1);
	    *b = 0.;
	    *e = *delta*(float)( nfreq - 1 );
	    *iftype = irsptp;

	    /* -- Give header back to memory manager. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Close file. */
	    zclose( &nun, nerr );
	    if( *nerr != 0 )
			goto L_8888;
	}

	/* - Read data sections from both files in pair. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    jdfl_ = jdfl - 1;

	    /* -- Get header from memory manager. */
	    getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Set number and size of data components in working storage. */
	    Ncomp[jdfl] = 2;
	    Nlndta[jdfl] = *npts;

	    /* -- Allocate memory blocks in data-set storage. */
	    allamb( &cmmem, Nlndta[jdfl], &cmdfm.ndxdta[jdfl - 1][0], nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    allamb( &cmmem, Nlndta[jdfl], &cmdfm.ndxdta[jdfl - 1][1], nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Open first file. */
        tmp = string_list_get(datafiles, jdfl-1);
	    fstrncpy( krspnm, MCPFN, tmp, strlen(tmp)+1);
	    nrspnm = min(strlen(krspnm), MCPFN - 3 );
	    if( !cmsam.lrspe ){
                strncpy(s1,kmsam.krsps1,3);
		subscpy( krspnm, nrspnm, -1, MCPFN, s1);
	    }
	    zopen_sac( &nun, krspnm,MCPFN+1, "RODATA",7, nerr );
	    if( *nerr != 0 )
			goto L_8888;

	    /* -- Read data. */
	    nlcdsk = SAC_HEADER_WORDS_FILE;
	    nlcmem = cmdfm.ndxdta[jdfl_][0];
	    nfreq = *npts/2 + 1;
	    zrabs( (int *)&nun, (char *)(cmmem.sacmem[nlcmem]), nfreq, (int *)&nlcdsk, (int *)nerr );
            if( lswap[ jdfl ] ){     /* byteswap if necessary. */
                int idx ;
                float *ptr ;

                for( idx = 0, ptr = cmmem.sacmem[nlcmem] ;
                     idx < nfreq ; idx++, ptr++ )
                {
                    byteswap( (void *)ptr, 4 ) ;
                }
            } /* end if( lswap[ jdfl ] ) */
	    if( *nerr != 0 )
		goto L_7777;

	    /* -- Close file. */
	    zclose( &nun, nerr );
	    if( *nerr != 0 )
		goto L_7777;

	    /* -- Fill second half of first data component.
	     *    This is either the real or the amplitude component
	     *    and is therefore symmetric about its midpoint. */
             Sacmem1 = cmmem.sacmem[nlcmem]+1;
             Sacmem2 = cmmem.sacmem[nlcmem]+*npts-1;
	     for( jdx = 1; jdx <= (nfreq - 2); jdx++ ){
                *(Sacmem2--) = *(Sacmem1++);
	     }

	     /* -- If this is a SPE file:
	      *    (1) Take the square root of each data point. This converts
	      *        it from a power estimate to an amplitude estimate.
	      *    (2) Recompute the extrema stored in the header.
	      *    (3) Zero out the second (phase) data component.
	      *    (4) Loop to the next file in the list. */

	     if( cmsam.lrspe ){
                Sacmem1 = cmmem.sacmem[cmdfm.ndxdta[jdfl_][0]];
		for( jdx = 0; jdx <= (*npts - 1); jdx++ ){
                    *Sacmem1 = sqrt(*Sacmem1);
                    Sacmem1++;
		}
		extrma( cmmem.sacmem[cmdfm.ndxdta[jdfl_][0]], 1, *npts, depmin, 
		 depmax, depmen );

                Sacmem1 = cmmem.sacmem[cmdfm.ndxdta[jdfl_][1]];
		for( jdx = 0; jdx <= (*npts - 1); jdx++ ){
                    *(Sacmem1++) = 0.;
		}
		goto L_4800;
	     }

	    /* -- Open second file in pair. */
            strncpy(s1,kmsam.krsps2,3);
	    subscpy( krspnm, nrspnm, -1, MCPFN, s1 );

	    zopen_sac( &nun, krspnm,MCPFN+1, "RODATA",7, nerr );
	    if( *nerr != 0 )
		goto L_7777;

	    /* -- Read data (do not read header from second file.) */
	    nlcmem = cmdfm.ndxdta[jdfl_][1];
	    zrabs( (int *)&nun, (char *)(cmmem.sacmem[nlcmem]), nfreq, (int *)&nlcdsk, (int *)nerr );
            if( lswap[ jdfl ] ){     /* byteswap if necessary. */
                int idx ;
                float *ptr ;

                for( idx = 0, ptr = cmmem.sacmem[nlcmem] ;
                     idx < nfreq ; idx++, ptr++ )
                {
                    byteswap( (void *)ptr, 4 ) ;
                }
            } /* end if( lswap[ jdfl ] ) */
	    if( *nerr != 0 )
		goto L_7777;

	    /* -- Close file. */
	    zclose( &nun, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Fill second half of second component (assymetric this time.) */
            Sacmem1 = cmmem.sacmem[nlcmem]+1;
            Sacmem2 = cmmem.sacmem[nlcmem]+*npts-1;
	    for( jdx = 1; jdx <= (nfreq - 2); jdx++ ){
                *(Sacmem2--) = -*(Sacmem1++);
	    }

	    /* -- Give file back to memory manager. */
L_4800:
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	}

L_7777:
	zclose( &nun, &junk );

L_8888:
    string_list_clear(last_list);
    string_list_extend(last_list, datafiles);
    list = NULL;

	if( *nerr == 0 ) {
	    cmdfm.nfilesFirst = 0 ;
	    cmdfm.lread = TRUE ;
	    sacToSeisMgr ( TRUE , FALSE , TRUE , nerr ) ;
	    cmdfm.lread = FALSE ;
	}
	else
	    cmdfm.ndfl = 0;

	return;
} /* end of function */

