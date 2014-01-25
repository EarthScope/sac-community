
#include "ssi.h"
#include "dfm.h"

void 
xrollback (int *nerr) {

        /*=====================================================================
         * PURPOSE:  To execute the action command ROLLBACK.
         *           Files in SAC are replaced with files from SeisMgr 
         *=====================================================================
         * INPUT ARGUMENTS:
         *    KINPUT:  Character string containing options
         *             Note, this command is not parsed by the standard SAC
	 *             parsing mechanism.
         *=====================================================================
         * OUTPUT ARGUMENTS:
         *    NERR:    Error flag. Set to 0 if no error occurred.
         *=====================================================================
         * MODULE/LEVEL: SSI/2
         *=====================================================================
         * GLOBAL INPUT:
         *    SSI:     LFULL 
         *=====================================================================
         * MODIFICATION HISTORY:
	 *	980915:	Original version.
         *=====================================================================*/

    *nerr = 0 ;

    rollback ( allHeader , nerr ) ;

}

