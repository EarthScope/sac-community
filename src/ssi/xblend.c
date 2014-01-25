
#include "ssi.h"
#include "dfm.h"

void 
xrecall (int *nerr) {
        /*=====================================================================
         * PURPOSE:  To execute the action command RECALL.
         *           Files in SAC are copied to SeisMgr 
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
         *    SSI:     LDATA
         *=====================================================================
         * MODIFICATION HISTORY:
	 *	980915:	Original version.
         *=====================================================================*/

    *nerr = 0 ;

    sacToSeisMgr ( 0 , 1 , 0 , nerr ) ;
    rollback ( wfHeader , nerr ) ;
}
