/** 
 * @file   wrci.c
 * 
 * @brief  Write a SAC card image data file
 * 
 */

#include <stdio.h>

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"


#include "co.h"
#include "dff.h"

/** 
 * Write a SAC card image data file
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of file to write
 * @param kname_s 
 *    Length of \p kname
 * @param kfmt 
 *    Format for floating point numbers
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   130105:  kevnm bug fix from Kasahara
 * @date   830607:  Fixed bug causing extra data point to be written.
 * @date   810728:  Added argument specifying data format.
 * @date   800109:  Original version.
 *
 */
void 
wrci(int   idfl, 
     char *kname, 
     int   kname_s, 
     char *kfmt, 
     int  *nerr) {

        int ilhdr[SAC_HEADER_LOGICALS], jdx, jj, jjj, ncards, nderr, ndx1; 
	int ndx2, nlcmem, nlen, nremdr;
        FILE *nun;

        float *Sacmem;

	int *const Ilhdr = &ilhdr[0] - 1;

	*nerr = 0;

	/* - Create file. */
	zdest( kname,kname_s, &nderr );
	znfiles( &nun, kname,kname_s, "TEXT",5, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Get file from memory manager. */
	getfil( idfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

	/* - Write header. */
	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_FLOATS/5); jj++ ){
	    for( jjj = jdx; jjj <= (jdx + 4); jjj++ ){
                        fprintf(nun,kfmt,Fhdr[jjj]);
	    }
	    fprintf(nun,"\n");
	    jdx = jdx + 5;
	}

	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_INTEGERS/5); jj++ ){
	    for( jjj = jdx; jjj <= (jdx + 4); jjj++ ){
		fprintf(nun,"%10d",Nhdr[jjj]);
	    }
	    fprintf(nun,"\n");
	    jdx = jdx + 5;
	}

	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_ENUMS/5); jj++ ){
	    for( jjj = jdx; jjj <= (jdx + 4); jjj++ ){
		fprintf(nun,"%10d",Ihdr[jjj]);
	    }
	    fprintf(nun,"\n");
	    jdx = jdx + 5;
	}

	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_LOGICALS/5); jj++ ){
	    for( jjj = 1; jjj <= 5; jjj++ ){
		if( Lhdr[jdx] ){
		    Ilhdr[jjj] = 1;
		}
		else{
		    Ilhdr[jjj] = 0;
		}
		jdx = jdx + 1;
	    }
	    for( jjj = 1; jjj <= 5; jjj++ ){
		fprintf(nun,"%10d",Ilhdr[jjj]);
	    }
	    fprintf(nun,"\n");
	}

	/* write out the character header values */
        fprintf(nun,"%8s",kmhdr.khdr[0]);
        fprintf(nun,"%-16s\n",kmhdr.khdr[1]);

	for( jj = 4; jj <= SAC_HEADER_STRINGS; jj += 3 ){
	    for( jjj = jj; jjj <= (jj + 2); jjj++ ){
		fprintf(nun,"%8s", kmhdr.khdr[jjj - 1] );
	    }
	    fprintf( nun, "\n" );
	}

	/* - Write first data component. */
	nlcmem = ndx1;
	ncards = *npts/5;
	nremdr = *npts - 5*ncards;
        Sacmem = cmmem.sacmem[ndx1];
	for( jj = 1; jj <= ncards; jj++ ){
	    for( jjj = nlcmem; jjj <= (nlcmem + 4); jjj++ ){
		fprintf(nun,kfmt,*(Sacmem++));
	    }
	    fprintf( nun, "\n" );
	    nlcmem = nlcmem + 5;
	}
	if( nremdr > 0 ){
	    Sacmem = cmmem.sacmem[ndx1]+(nlcmem-ndx1);
	    for( jjj = nlcmem; jjj <= (nlcmem + nremdr - 1); jjj++ ){
		fprintf(nun,kfmt,*(Sacmem++));
	    }
	    fprintf( nun, "\n" );
	    /* nlcmem = nlcmem + nremdr; */
	}

	/* - Write second data component if present. */

	if( Ncomp[idfl] == 2 ){
	    nlcmem = ndx2;
	    Sacmem = cmmem.sacmem[ndx2];
	    for( jj = 1; jj <= ncards; jj++ ){
		for( jjj = nlcmem; jjj <= (nlcmem + 4); jjj++ ){
		    fprintf(nun,kfmt,*(Sacmem++));
		}
		fprintf( nun, "\n" );
		nlcmem = nlcmem + 5;
	    }
	    if( nremdr > 0 ){
		Sacmem = cmmem.sacmem[ndx2]+(nlcmem-ndx2);
		for( jjj = nlcmem; jjj <= (nlcmem + nremdr - 1); jjj++ ){
		    fprintf(nun,kfmt,*(Sacmem++));
		}
		fprintf( nun, "\n" );
		/* nlcmem = nlcmem + nremdr; */
	    }
	}

	/* - Close file and return. */
	zcloses( &nun, nerr );

L_8888:
	return;
}

