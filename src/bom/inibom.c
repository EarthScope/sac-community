/** 
 * @file   inibom.c
 * 
 * @brief  Initialize the Binary Operations
 * 
 */

#include <string.h>

#include "bom.h"
#include "bool.h"

/** 
 * Initialize the Common Block for Binary Operations
 * 
 * @date   850730:  Added initialization for IBFLC, NDXHBF, NDX1BF, NDX2BF.
 * @date   810811:  Added initializaton for KECDEL.
 * @date   810415:  Original version.
 *
 */
void 
inibom() {

	cmbom.nbfl = 0;

	strcpy( kmbom.kecnpt, "FATAL   " );
	strcpy( kmbom.kecdel, "FATAL   " );

	cmbom.ibflc = 0;
	cmbom.ndxhbf = 0;
	cmbom.ndx1bf = 0;
	cmbom.ndx2bf = 0;

	/* by default, take header info from original file. maf 990526 */
	cmbom.lnewhdr = FALSE ;

	return;
}
