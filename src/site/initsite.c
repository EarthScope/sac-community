
#include <string.h>

#include "site.h"

void 
initsite() {

	/*=====================================================================
	 * PURPOSE: Variable initialization of site common block.
	 *=====================================================================
	 * MODULE/LEVEL:  site/4
	 *=====================================================================
	 * PARAMETERS:
	 *    MODULESITECOM:  Module number for site (dependent) commands.
	 *                    This value should NOT be changed. [i]
	 *    MSITECOMNAMES:  Maximum number of site dependent names and
	 *                    abbreviations. Change this to increase storage
	 *                    arrays as needed. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR:
	 *    nsitecomnames:  Number of site dependent command names and
	 *                    abbreviations. [i]
	 *    ksitecomnames:  List of names of commands and any abbreviations. 
	 *                    Limited to a maximum of MCPW=8 characters each.
	 *    isitecomindex:  List of index numbers for commands. The command
	 *                    name and its abbreviations (if any) must all
	 *                    have the same index number.  This is the value
	 *                    that is passed to xsitecom and used in the
	 *                    jump table to find the subroutine to execute. [ia]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900804:  Changed from idm to site.
	 *    831107:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900804
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Initialize list of command names and index numbers. */
	/*   (The following values are for testing only. They set up a single
	 *   command called "testsite" and its abbreviation "tsite".) */
	cmsite.nsitecomnames = 2;
	strcpy( kmsite.ksitecomnames[0], "TESTSITE" );
	Isitecomindex[1] = 1;
	strcpy( kmsite.ksitecomnames[1], "TSITE   " );
	Isitecomindex[2] = 1;

	/* - Initialize other global variables added to site common block here: */
	return;

}

