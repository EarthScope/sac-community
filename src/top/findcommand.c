
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "top.h"
#include "bool.h"
#include "site.h"
#include "comlists.h"


#include "ucf.h"

void /*FUNCTION*/ findcommand(kcommand, lfind, module, index)
char *kcommand;
int *lfind;
int *module, *index;
{
	int istart, j, j_, jfind, nentries;



	/*=====================================================================
	 * PURPOSE: To find and validate the named SAC command.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kcommand:  Name of SAC command to find and validate. [k]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    lfind:     Set to .TRUE. if the command is legitimate. [l]
	 *    module:    The command module number if legitimate, 0 otherwise. [i]
	 *    index:     The command index number if legitimate, 0 otherwise. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  top/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *  mach:
	 *  comlists:  nextcomnames, kextcomnames, iextcomindex, MODULEEXTCOM,
	 *             icomlist, icomliststart, ncomlistentries,
	 *             kcomnames, icommodule, icomindex
	 *  site       nsitecomnames, ksitecomnames, isitecomindex, MODULESITECOM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:    lbsrch
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900804:  Changed name from vc. Moved determination of command
	 *             name to "saccmd". Added kcommand argument.
	 *             Also added search for external and site commands.
	 *    870730:  Added call to convert token to uppercase.
	 *    861129:  Changed to lcchar so that command names can be longer
	 *             than MCPW characters.
	 *    821129:  Modified to handle subprocesses.
	 *    820503:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900804
	 *===================================================================== */
	/* PROCEDURE: */

	/* - First sequentially search list of "external" commands.
	 *   These commands are ones that have been dynamically loaded.
	 *   If found set module number and command index number and return. */

	for( j = 1; j <= cmcomlists.nextcomnames; j++ ){
		j_ = j - 1;
		if( memcmp(kcommand,kmcomlists.kextcomnames[j_],strlen(kcommand)) == 0 ){
			*lfind = TRUE;
			*module = MODULEEXTCOM;
			*index = Iextcomindex[j];
			goto L_8888;
			}
		}

	/* - Next sequentially search list of "site dependent" commands.
	 *   These commands are ones that have been statically loaded
	 *   in the site dependent module. 
	 *   If found set module number and command index number and return. */

	for( j = 1; j <= cmsite.nsitecomnames; j++ ){
		j_ = j - 1;
		if( memcmp(kcommand,kmsite.ksitecomnames[j_],strlen(kcommand)) == 0 ){
			*lfind = TRUE;
			*module = MODULESITECOM;
			*index = Isitecomindex[j];
			goto L_8888;
			}
		}

	/* - If not found, perform a binary search on the current list of 
	 *   legitimate internal SAC commands. */

	istart = Icomliststart[cmcomlists.icomlist];
	nentries = Ncomlistentries[cmcomlists.icomlist];
	*lfind = lbsrch( kcommand, MCPW, (char*)kmcomlists.kcomnames_full[istart - 1]
	 ,30, nentries, &jfind );

	/* - Return internal module and index numbers if this search was successful. */

	if( *lfind ){
		*module = Icommodule[istart + jfind - 1];
		*index = Icomindex[istart + jfind - 1];
		}
	else{
		*module = 0;
		*index = 0;
		}

L_8888:
	return;

} /* end of function */

