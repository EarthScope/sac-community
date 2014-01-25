
#include "pl.h"

void /*FUNCTION*/ incat(icurat, iattab, nattab, jattab, inewat)
int icurat, iattab[], nattab, *jattab, *inewat;
{

	int *const Iattab = &iattab[0] - 1;


	/*=====================================================================
	 * PURPOSE: To increment a attribute based upon an attribute table.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ICURAT:  Current attribute.
	 *    IATTAB:  Attribute table.
	 *    NATTAB:  Length of IATTAB.
	 *    JATTAB:  Index in IATTAB pointing to current attribute.
	 *             Set to 0 if you don't know its current value.
	 *             Subroutine will search for current pointer.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    JATTAB:  Index in IATTAB pointing to new attribute.
	 *    INEWAT:  New attribute value.
	 *=====================================================================
	 * MODULE/LEVEL:  SERVICE/4
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Find current attribute value in table if JATTAB is out of range. */
	if( *jattab <= 0 || *jattab > nattab ){
		*jattab = 1;
L_1000:
		if( icurat != Iattab[*jattab] ){
			*jattab = *jattab + 1;
			if( *jattab <= nattab )
				goto L_1000;
			}
		}

	/* - Set JATTAB to 1 if current JATTAB is equal to length of table.
	 *   This could also result from ICURAT could not be found in table. */

	if( *jattab >= nattab ){
		*jattab = 1;

		/* - Otherwise, increment JATTAB by 1 */

		}
	else{
		*jattab = *jattab + 1;
		}

	/* - Return new attribute value. */

	*inewat = Iattab[*jattab];

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    830114:  Modified argument list and logic to allow for
	 *             multiple occurances of an attribute in the list.
	 *    820305:  Original version.
	 *===================================================================== */

} /* end of function */

