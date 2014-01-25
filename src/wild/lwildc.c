
#include "wild.h"
#include "bool.h"


#include "bot.h"

int /*FUNCTION*/ lwildc(flist, flist_s)
char *flist;   int flist_s;
{
	int lwildc_v;

	/*=====================================================================
	 * PURPOSE:  To check the input string for any wild card specification.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    FLIST:   File pathname, possibly containing wild cards.  [c]
	 *=====================================================================
	 * FUNCTION VALUE:
	 *    LWILDC:  Set to .TRUE. if wild card specification is found.
	 *=====================================================================
	 * MODULE/LEVEL:  WILD/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    KDIRDL
	 *    WILD:    SNGL, MULT, CITR, CCON
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870619:  Deleted iteration option.
	 *    860922:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  860922
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set function value to .FALSE., then look for wild card characters
	 * */
	lwildc_v = FALSE;
	if( ((indexa( flist,flist_s, kmwild.sngl, TRUE, TRUE ) != 0 || 
	 indexa( flist,flist_s, kmwild.mult, TRUE, TRUE ) != 0) || indexa( flist
	 ,flist_s, kmwild.ccon[0], TRUE, TRUE ) != 0) || indexa( flist
	 ,flist_s, kmwild.ccon[1], TRUE, TRUE ) != 0 )
		lwildc_v = TRUE;

	return( lwildc_v );

} /* end of function */

