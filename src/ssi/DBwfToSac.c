
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

    /* - For each data component: */
    for( jcomp = 0, pArray = seis->i ; jcomp < Ncomp[idfl]; jcomp++, pArray = seis->r ){

      /* -- Define initial memory location. */
      nlcmem = cmdfm.ndxdta[idfl - 1][jcomp];

      cut(pArray, Nstart[idfl], Nstop[idfl], Nfillb[idfl], Nfille[idfl], cmmem.sacmem[nlcmem]);
      if ( cmdfm.lscale && *scale != SAC_FLOAT_UNDEFINED && *scale != 1.0 ) {
        for(idx = 0; idx < (Nstop[idfl] - Nstart[idfl] + 1); idx++) {
          cmmem.sacmem[nlcmem][idx] *= *scale;
        }
        *scale = 1.0;
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
