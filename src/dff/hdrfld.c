/** 
 * @file   hdrfld.c
 * 
 * @brief  Get type and index number for a header variable
 * 
 */

#include <string.h>

#include "dff.h"
#include "bot.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "bool.h"

/** 
 * Determine the category (type) and the index number of a header variable
 * 
 * @param kname 
 *    Name of the header variable
 * @param kname_s 
 *    Length of \p kname
 * @param icat 
 *    Category on output
 *    - 1 Floating Point or real variable ( 1 .. 70 )
 *    - 2 Integer varaible                ( 1 .. 15 )
 *    - 3 Enumerated variable             ( 1 .. 20 )
 *    - 4 Logical or Boolean variable     ( 1 ..  5 )
 *    - 5 Character String                ( 1 .. 24 )
 *    - 6 Auxillary Fields                ( 1 .. 20 )
 * @param item 
 *    Index number within category
 * @param lfound 
 *    - TRUE if the item was found
 *    - FALSE if the item was not found
 *
 * @date   870514:  Added convertion to upper case.
 * @date   810000:  Original version.
 *
 */
void 
hdrfld(char *kname, 
       int   kname_s, 
       int  *icat, 
       int  *item, 
       int  *lfound) {

	char ktemp[9];
	int nc;
	
        memset(ktemp,' ',8);
        ktemp[8] = '\0';

	/* - Convert input name to uppercase for testing. */
	nc = indexb( kname,kname_s );
	modcase( TRUE, kname, nc, ktemp );
	if( nc < SAC_HEADER_STRING_LENGTH_FILE )
		subscpy( ktemp, nc, SAC_HEADER_STRING_LENGTH_FILE - 1, 8, " " );

	/* - Assume the worst. */
	*lfound = FALSE;
	*icat = 0;

	/* - Check name versus list of variables in each category. */
	/* -- Floating point fields. */
	*item = nequal( ktemp, (char*)kmlhf.kfhdr,9, SAC_HEADER_FLOATS );
	if( *item > 0 ){
		*lfound = TRUE;
		*icat = cmlhf.icatf;
		goto L_8888;
	}

	/* -- Integer fields. */
	*item = nequal( ktemp, (char*)kmlhf.knhdr,9, SAC_HEADER_INTEGERS );
	if( *item > 0 ){
		*lfound = TRUE;
		*icat = cmlhf.icatn;
		goto L_8888;
		}

	/* -- Enumerated fields. */
	*item = nequal( ktemp, (char*)kmlhf.kihdr,9, SAC_HEADER_ENUMS );
	if( *item > 0 ){
		*lfound = TRUE;
		*icat = cmlhf.icati;
		goto L_8888;
		}

	/* -- Logical fields. */
	*item = nequal( ktemp, (char*)kmlhf.klhdr,9, SAC_HEADER_LOGICALS );
	if( *item > 0 ){
		*lfound = TRUE;
		*icat = cmlhf.icatl;
		goto L_8888;
		}

	/* -- Character fields. */
	*item = nequal( ktemp, (char*)kmlhf.kkhdr,9, SAC_HEADER_STRINGS );
	if( *item > 0 ){
		*lfound = TRUE;
		*icat = cmlhf.icatk;
		goto L_8888;
		}

	/* -- Auxiliary fields. */
	*item = nequal( ktemp, (char*)kmlhf.kahdr,9, MAHDR );
	if( *item > 0 ){
		*lfound = TRUE;
		*icat = cmlhf.icata;
		goto L_8888;
		}

L_8888:
	return;

} /* end of function */

void hdrfldf(char      *kname, 
	     int       *icat, 
	     int       *item, 
	     int       *lfound, 
	     int        kname_s) {
  hdrfld(kname, kname_s, icat, item, lfound);
}
void hdrfldf_(char      *kname, 
	      int       *icat, 
	      int       *item, 
	      int       *lfound, 
	      int        kname_s) {
  hdrfld(kname, kname_s, icat, item, lfound);
}
void hdrfldf__(char      *kname, 
	       int       *icat, 
	       int       *item, 
	       int       *lfound, 
	       int        kname_s) {
  hdrfld(kname, kname_s, icat, item, lfound);
}


