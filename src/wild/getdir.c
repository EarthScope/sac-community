
#include "mach.h"
#include "wild.h"
#include "bool.h"


#include "co.h"
#include "bot.h"

void /*FUNCTION*/ getdir(kpath, kpath_s, kdirpt, kdirpt_s, kpatpt, 
	 kpatpt_s)
char *kpath;   int kpath_s;
char *kdirpt;   int kdirpt_s;
char *kpatpt;   int kpatpt_s;
{
	int ifirst;

	/*=====================================================================
	 * PURPOSE: Breaks a pathname into a directory part and a pattern part.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    KPATH:   Character string for pathname. [c]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    KDIRPT:  Character string containing directory name. [c]
	 *    KPATPT:  Character string containing the pattern part. [c]
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    KDIRDL
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    INDEXA
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    IFIRST:  Index of last occurrence of delimiter in pathname. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870908:  Deleted conversion of filename to lowercase.
	 *    860922:  Original version.
	 *=====================================================================
	 * DOCUMENTED: 860922
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Search pathname backwards for last occurrence of a delimiter */
	ifirst = indexa( kpath,kpath_s, KDIRDL, FALSE, TRUE );

	/* - If there's not one, then return empty string for the directory part */

	if( ifirst == 0 ){

		fstrncpy( kdirpt, kdirpt_s-1, " ", 1 );
		fstrncpy( kpatpt, kpatpt_s-1, kpath, kpath_s - 1);

		}
	else{

		/* - If it's there, break string there */

		fstrncpy( kdirpt, kdirpt_s-1, kpath, ifirst);
		ifirst = ifirst + 1;
		fstrncpy( kpatpt, kpatpt_s-1, kpath+ifirst - 1, kpath_s - ifirst);

		}

       
	return;

} /* end of function */

