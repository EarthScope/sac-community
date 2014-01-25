/** 
 * @file   crsac.c
 * 
 * @brief  Create a new file in SAC memory
 * 
 */

#include "dfm.h"
#include "amf.h"
#include "hdr.h"


#include "ucf.h"
#include "dff.h"

/** 
 * Create a new file in SAC memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param ncmp 
 *    Number of data components
 * @param nlen 
 *    Length of each data component
 * @param ndxh 
 *    Index in sacmem array of header
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index in sacmem array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    
 * @date   850731:  Changes due to new memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   840120:  Cleaned up and documented.
 * @date   821222:  Added zeroing of data arrays.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void 
crsac(int  idfl, 
      int  ncmp, 
      int  nlen, 
      int *ndxh, 
      int *ndx1, 
      int *ndx2, 
      int *nerr) {

	int jcomp, jcomp_, jrel, jrel_, nrerr;

	*nerr = 0;

	/* - Allocate memory for header. */
	allamb( &cmmem, SAC_HEADER_WORDS, &Ndxhdr[idfl], nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - For each data component: */
	Nlndta[idfl] = nlen;
	Ncomp[idfl] = ncmp;
	for( jcomp = 1; jcomp <= Ncomp[idfl]; jcomp++ ){
		jcomp_ = jcomp - 1;

		/* -- Allocate memory block. */
		allamb( &cmmem, 
			Nlndta[idfl], 
			&cmdfm.ndxdta[idfl - 1][jcomp_], 
			nerr );

		/* -- If error occurred, 
		 * release other blocks before returning. 
		 */
		if( *nerr != 0 ){
			relamb( cmmem.sacmem, Ndxhdr[idfl], &nrerr );
			for( jrel = 1; jrel <= (jcomp - 1); jrel++ ){
				jrel_ = jrel - 1;
				relamb( cmmem.sacmem, 
					cmdfm.ndxdta[idfl - 1][jrel_], 
				 &nrerr );
				cmdfm.ndxdta[idfl - 1][jrel_] = 0;
			}
			goto L_8888;
		}
	}

	/* - Set return arguments if no error occurred. */
	*ndxh = Ndxhdr[idfl];
	*ndx1 = cmdfm.ndxdta[idfl - 1][0];
	*ndx2 = cmdfm.ndxdta[idfl - 1][1];

	/* - Initialize header and data components to default values. */
	newhdr();
	*npts = nlen;
	fill( cmmem.sacmem[*ndx1], *npts, 0. );
	*leven = ncmp == 1;
	if( !*leven )
		fill( cmmem.sacmem[*ndx2], *npts, 0. );

	/* - Give file to data manager. */
	putfil( idfl, nerr );

L_8888:
	return;
}

