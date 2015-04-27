
#include <string.h>

#include "ncpf.h"
#include "cpf.h"

void /*FUNCTION*/
inicpf() {

        /*=====================================================================
	 * PURPOSE: Variable initialization of common blocks cmcpf and kmcpf.
	 *=====================================================================
	 * PARAMETERS:
	 *    MSIZE:      Size of vars section used to store macro arguments. [i]
	 *    MMACLEVEL:  Maximum number of levels of macro nesting. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    knoval:  Character string used to designate that a particular
	 *             macro keyword has no current value. [c8]
	 *    kbb:     Character used to designate a blackboard variable. [c1]
	 *    khdr:    Character used to designate a header variable. [c1]
	 *    karg:    Character used to designate a macro argument. [c1]
	 *    kfill:   Character used to fill common block to a word boundary.
	 *    nmaclevel:  Current number of levels of macro nesting. [i]
	 *    knames:  Names of macro files open at each level of nesting. [ca]
	 *    imacstatus:  Current macro status. [i]
	 *                 = 0 ok
	 *                 = 1 execution error has occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920325:  Added getxtime function.
	 *    890818:  Added initialization for REPLY function.
	 *    881228:  Added inline function initialization.
	 *    870722:  Added macro status information initialization.
	 *    870410:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870410
	 *===================================================================== */
    /* PROCEDURE: */
    strcpy(kmcpf.knoval, "NO VALUE");

    setmacrolevel(0);
    setmacrostatus("OK", 3);

    return;

}                               /* end of function */
