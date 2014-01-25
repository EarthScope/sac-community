/** 
 * @file   inihdr.c
 * 
 * @brief  Initialize the Header Block
 * 
 */

#include <string.h>

#include "dff.h"
#include "hdr.h"
#include "bool.h"

/** 
 * Initialize the Header Block
 * 
 * @date   961212:  Added linc and llh as part off adding the INCLUSIVE 
 *             option to the LISTHDR command.  maf
 * @date   910820:  Added include file dfm and subscripted nlnhdr.
 * @date   810414:  Original version.
 *
 */
void 
inihdr() {

	int j;

	cmhdr.nvhdrc = SAC_HEADER_MAJOR_VERSION;

	cmhdr.fundef = SAC_FLOAT_UNDEFINED;
	cmhdr.iundef = SAC_ENUM_UNDEFINED;
	cmhdr.nundef = SAC_INT_UNDEFINED;
	strcpy( kmhdr.kundef, SAC_CHAR_UNDEFINED );
        strcpy( kmhdr.kundef_2, SAC_CHAR_UNDEFINED_2 );

	for( j = 1; j <= SAC_ENUMS; j++ ){
	  Niv[j] = j;
	}

	/* lh starts without the INC option.  maf 961212 */
	cmhdr.linc = FALSE ;
	/* not currently executing xlh().  maf 961212 */
	cmhdr.llh  = FALSE ;	

	return;
}


void inihdr_()  { inihdr() ; }
void inihdr__() { inihdr() ; }
