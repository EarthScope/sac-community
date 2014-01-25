/** 
 * @file   relbfl.c
 * 
 * @brief  Release the Current file in Binary Operations
 * 
 */

#include "amf.h"
#include "bom.h"
#include "dfm.h"

/** 
 * Release the current Binary Operations file from memory
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 * 
 * @date   880308:  Was not clearing the DFM array variables properly.
 * @date   880306:  Fixed bug that was not releasing header block.
 * @date   850730:  Original version.
 *
 */
void 
relbfl(int *nerr) {
         int jcomp, jcomp_;

	*nerr = 0;

	/* - Release blocks if they are in memory: */
	/* -- Header block. */
	if( cmbom.ndxhbf > 0 )
		relamb( cmmem.sacmem, cmbom.ndxhbf, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* -- First data component. */
	if( cmbom.ndx1bf > 0 )
		relamb( cmmem.sacmem, cmbom.ndx1bf, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* -- Second data component if any. */
	if( cmbom.ndx2bf > 0 )
		relamb( cmmem.sacmem, cmbom.ndx2bf, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Zero DFM pointers. (Binary file is stored at end of data file list.) */

	Ndxhdr[cmdfm.ndfl + 1] = 0;
	Nlndta[cmdfm.ndfl + 1] = 0;
	for( jcomp = 1; jcomp <= Ncomp[cmdfm.ndfl + 1]; jcomp++ ){
		jcomp_ = jcomp - 1;
		cmdfm.ndxdta[cmdfm.ndfl][jcomp_] = 0;
	}
	Ncomp[cmdfm.ndfl + 1] = 0;

	/* - Zero BOM pointers. */

	cmbom.ibflc = 0;
	cmbom.ndxhbf = 0;
	cmbom.ndx1bf = 0;
	cmbom.ndx2bf = 0;

L_8888:
	return;

}

