
#include "sss.h"
#include "dfm.h"

void /*FUNCTION*/ xincrementsta(nerr)
int *nerr;
{
	int jdfl, jvm;



	/*=====================================================================
	 * PURPOSE:  To execute the INCREMENTSTACK command.  This command 
	 *           increments certain signal stack parameters.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl
	 *    sss:     dlyti, dlyni, vappi, t0vmi
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     dlyt, dlyn, vapp, t0vm
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    821201:  Minor cleanup and documentation.
	 *    810120:  Changed to output message retrieval from disk.
	 *    790831:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850819
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Increment static delays. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		Dlyt[jdfl] = Dlyt[jdfl] + Dlyti[jdfl];
		Dlyn[jdfl] = Dlyn[jdfl] + Dlyni[jdfl];
		}

	/* - Increment velocity model parameters. */

	for( jvm = 1; jvm <= MVM; jvm++ ){
		Vapp[jvm] = Vapp[jvm] + Vappi[jvm];
		T0vm[jvm] = T0vm[jvm] + T0vmi[jvm];
		}

       
	return;

} /* end of function */

