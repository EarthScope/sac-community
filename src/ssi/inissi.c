
#include "ssi.h"
#include "bool.h"
#include "smDataIO.h"
#include "cssListOps/dblErrors.h"
#include "dbselect/dbDefaults.h"

int linkedToOracleLibs ;  /* True if oracle libraries are linked to sac */
int triedAndFailed ;      /* True if sac tried to link to libs and failed */

        /*=====================================================================
         * PURPOSE: Initialization of the data base module.
         *=====================================================================
         * PARAMETERS:
         *=====================================================================
         * VARIABLE DEFINITIONS:
         *=====================================================================
         * MODIFICATION HISTORY:
	 *      980127: added global variable: linkedToOracleLibs.  maf
	 *	971208:	Original version.
         *===================================================================== */

void inissi () 
{
    char * worksetName ;

    /* set flag */
    linkedToOracleLibs = FALSE ;
    triedAndFailed = FALSE ;

    /* Initialize SeisMgr error handler */
    dblClearErrorList () ;

    /* initialize query */
    dbSetQueryDefaults () ;

    /* delete files from SeisMgr (if this is a INICM command) */
    worksetName = smGetDefaultWorksetName () ;
    if ( worksetName )
	smDeleteWorksetByName ( worksetName ) ;
} /* end inissi */

