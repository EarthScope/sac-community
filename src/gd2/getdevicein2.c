
#include <string.h>

#include "gd2.h"


#include "co.h"

void /*FUNCTION*/ getdeviceinfo2(kdname, kdname_s, idtype)
char *kdname;   int kdname_s;
int *idtype;
{



	/*=====================================================================
	 * PURPOSE:  To inquire about certain attributes of graphics device 2.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    KDNAME:  The name of the graphics device. [c]
	 *    IDTYPE:  The type of the graphics device. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     INAME2, ITYPE2
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    862020:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  862020
	 *===================================================================== */
	/* PROCEDURE: */
	fstrncpy( kdname, kdname_s-1, kmgd2.kname2, strlen(kmgd2.kname2));
	*idtype = cmgd2.itype2;

       
	return;

} /* end of function */

