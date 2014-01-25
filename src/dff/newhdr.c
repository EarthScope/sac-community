/** 
 * @file   newhdr.c
 * 
 * @brief  Prepare a new header
 * 
 */

#include <string.h>

#include "dff.h"
#include "msg.h"
#include "hdr.h"
#include "bool.h"

/** 
 * Perpare a new default header
 * 
 * @date   961031:  ninf and nhst were changed to norid and nevid for
 *                  compatability with the CSS format.  maf 961031
 * @date   870902:  Added calls to INILHF and INIMSG as part of initialization.
 * @date   821001:  Added initialization of LCALDA.
 * @date   811118:  Replaced FMTSAC with literal "2."
 *                  Replaced DFM insert with HDR insert.
 *                  Deleted call to INIHDR.
 *
 */
void 
newhdr() {
	int jhdr ;

	ka[8]     = '\0';
	kcmpnm[8] = '\0';
	kdatrd[8] = '\0';
	kevnm[17] = '\0';
	kf[8]     = '\0';
	khole[8]  = '\0';
	kinst[8]  = '\0';
	knetwk[8] = '\0';
	ko[8]     = '\0';
	kstnm[8]  = '\0';
	kt0[8]    = '\0';
	kt1[8]    = '\0';
	kt2[8]    = '\0';
	kt3[8]    = '\0';
	kt4[8]    = '\0';
	kt5[8]    = '\0';
	kt6[8]    = '\0';
	kt7[8]    = '\0';
	kt8[8]    = '\0';
	kt9[8]    = '\0';
	kuser0[8] = '\0';
	kuser1[8] = '\0';
	kuser2[8] = '\0';

	/* - Initialize some common blocks if not already done. */
	if( cmhdr.fundef != -12345. ){
	    inihdr();
	    inilhf();
	    inimsg();
	}

	/* - Set ALL header fields to their "undefined" values. */
	/* -- Floating fields: */
	for( jhdr = 1; jhdr <= SAC_HEADER_FLOATS; jhdr++ ){
	    Fhdr[jhdr] = cmhdr.fundef;
	}

	/* -- Integer fields: */
	for( jhdr = 1; jhdr <= SAC_HEADER_INTEGERS; jhdr++ ){
	    Nhdr[jhdr] = cmhdr.nundef;
	}

	/* -- Fixed value fields: */
	for( jhdr = 1; jhdr <= SAC_HEADER_ENUMS; jhdr++ ){
	    Ihdr[jhdr] = cmhdr.iundef;
	}

	/* -- Character variable fields: */
        strcpy( kmhdr.khdr[1 - 1], kmhdr.kundef );
        strcpy( kmhdr.khdr[2 - 1], kmhdr.kundef_2 ); /* kevnm occupies 2nd and 3rd field */
    	for( jhdr = 4; jhdr <= SAC_HEADER_STRINGS; jhdr++ ){
	    strcpy( kmhdr.khdr[jhdr - 1], kmhdr.kundef );
	}

	/* -- Logical fields: */
	for( jhdr = 1; jhdr <= SAC_HEADER_LOGICALS; jhdr++ ){
	    Lhdr[jhdr] = FALSE;
	}

	/* - Now set specific fields to their default values. */
	*nvhdr  = cmhdr.nvhdrc;
	*leven  = TRUE;
	*lovrok = TRUE;
	*lcalda = TRUE;
	*iftype = *itime;

	return;

}



/* Wrapper to make the function more convenient for FORTRAN programmers. */
void newhdr_ () {  newhdr (); }
void newhdr__() {  newhdr (); }
