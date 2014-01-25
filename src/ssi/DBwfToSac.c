
#include "ssi.h"
#include "hdr.h"
#include "dfm.h"
#include "amf.h"


#include "ucf.h"
#include "dff.h"

void DBwfToSac ( idfl , seis , nerr )
int idfl , * nerr ;
struct trace *seis ;
{
    /* Declare Variables. */
    int idx, jcomp, nlcdsk = 0, nlcmem, numrd, offset;
    float unused, *pArray;

    /*=====================================================================
     * PURPOSE:  Copy a waveform from SeisMgr to SAC
     *=====================================================================
     * INPUT ARGUMENTS:
     *    idfl   file number of first file being handled currently
     *    seis:  SeisMgr struct containing a seismogram.
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *=====================================================================
     * MODIFICATION HISTORY:
     *    971202:  Original version.  maf
     *===================================================================== */

    *nerr = 0 ;

    /* - Define number of points to read and initial disk location. */
    numrd = Nstop[idfl] - Nstart[idfl] + 1 - Nfillb[idfl] - Nfille[idfl];

    /* - For each data component: */
    for( jcomp = 0, pArray = seis->i ; jcomp < Ncomp[idfl]; jcomp++, pArray = seis->r ){
	offset = 0;

	/* -- Define initial memory location. */
	nlcmem = cmdfm.ndxdta[idfl - 1][jcomp];

	/* -- Fill beginning with zeros if requested.  Update memory location. */
	if( Nfillb[idfl] > 0 ){
	    fill( cmmem.sacmem[nlcmem], Nfillb[idfl], 0. );
	    offset += Nfillb[idfl];
	}

	/* -- Update disk location and read data. */
	if( numrd > 0 ){
	    nlcdsk = Nstart[idfl] - 1 + Nfillb[idfl];

	    if ( cmdfm.lscale && *scale != SAC_FLOAT_UNDEFINED && *scale != 1.0 ) {
		for ( idx = 0 ; idx < numrd ; idx++ ) {
		    cmmem.sacmem[nlcmem][offset+idx] = pArray[nlcdsk+idx] * *scale ;
		}
		*scale = 1.0 ;
	    }
	    else {
		for ( idx = 0 ; idx < numrd ; idx++ ) {
		    cmmem.sacmem[nlcmem][offset+idx] = pArray[nlcdsk+idx] ;
		}
	    }

	    offset += numrd;
	}

	/* -- Fill end with zeros if requested. */
	if( Nfille[idfl] > 0 ){
	    fill( cmmem.sacmem[nlcmem]+offset, Nfille[idfl], 0. );
	    /* offset += Nfille[idfl]; */
	}

    }

    /* - Compute some header values. */
    *npts = Nlndta[idfl];
    extrma( cmmem.sacmem[cmdfm.ndxdta[idfl - 1][0]], 1, *npts, depmin, depmax, depmen );
    if( *leven ){
                *ennd = *begin + (float)( *npts - 1 )**delta;
    }
    else{
                extrma( cmmem.sacmem[cmdfm.ndxdta[idfl - 1][1]], 1, *npts, begin,
                 ennd, &unused );
    }

    /* - Move header back to working memory. */

    putfil( idfl, nerr );

       
    return;

} /* end DBwfToSac */
