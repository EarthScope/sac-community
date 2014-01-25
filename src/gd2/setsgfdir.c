
#include "gd2.h"


#include "co.h"
#include "bot.h"

void /*FUNCTION*/ setsgfdir(dir, dir_s)
char *dir;   int dir_s;
{
	int ndir;



	/*=====================================================================
	 * PURPOSE:  To set the name of the directory in which to store
	 *           subsequent SAC Graphics Files (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    dir:   Name of directory to store SGF's. [c]
	 *           A blank name implies the current directory.
	 *=====================================================================
	 * MODULE/LEVEL:  gd2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:  KDIRDL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gd2:     kfdir, kfdirStore, nfdir, nfdirStore
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - Any errors resulting from a bad directory name or insufficient 
	 *   access rights will be detected and reported by BEGINFRAME when 
	 *   the SGF is created.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900518:  Added logic to not copy the terminating directory
	 *             delimiter to common block variable if present.
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900518
	 *===================================================================== */
	/* PROCEDURE: */
	ndir = indexb( dir,dir_s );
	if( ndir <= 0 ){
		cmgd2.nfdir = cmgd2.nfdirStore = 0;
		fstrncpy( kmgd2.kfdir, MCPFN, " ", 1);
		fstrncpy( kmgd2.kfdirStore, MCPFN, " ", 1);
		}
	else if( dir[ndir - 1] == KDIRDL ){
		cmgd2.nfdir = cmgd2.nfdirStore = ndir - 1;
		fstrncpy( kmgd2.kfdir, MCPFN, dir, cmgd2.nfdir);
		fstrncpy( kmgd2.kfdirStore, MCPFN, dir, cmgd2.nfdir);
		}
	else{
		cmgd2.nfdir = cmgd2.nfdirStore = ndir;
		fstrncpy( kmgd2.kfdir, MCPFN, dir, cmgd2.nfdir);
		fstrncpy( kmgd2.kfdirStore, MCPFN, dir, cmgd2.nfdir);
		}

       
	return;

} /* end of function */

