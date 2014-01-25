
#include <string.h>

#include "smm.h"
#include "bool.h"

void 
inismm()
{



	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMSMM.
	 *===================================================================== */
	/* PROCEDURE: */
	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  MTW command
	 *    LMTW:     Use measurement time window (mtw) if true. [l]
	 *              Use full data window if false.
	 *    KMTW:     Starting and stopping time markers for mtw. [ka]
	 *    OMTW:     Starting and stopping time offsets for mtw. [fa]
	 *===================================================================== */
	cmsmm.lmtw = FALSE;
	strcpy( kmsmm.kmtw[0], "Z       " );
	strcpy( kmsmm.kmtw[1], "Z       " );
	Omtw[1] = 0.;
	Omtw[2] = 0.;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  MARKTIMES command
	 *    MVEL:     Maximum number of travel time velocities. [ip]
	 *    NVEL:     Current number of travel time velocities. [i]
	 *    VEL:      Travel time velocities. [fa]
	 *    LDISTR:   Use requested distance if true. [l]
	 *              Use header distance if false.
	 *    DISTR:    Requested distance. [f]
	 *    LORIGINR: Use requested origin time if true. [l]
	 *              Use header origin time if false.
	 *    LGMT:     Use requested GMT origin time if true. [l]
	 *              Use requeseted offset origin time if false.
	 *              Only applies if LORIGINR is true.
	 *    ORIGINR:  Requested offset origin time. [f]
	 *    NODTTM:   Requested GMT origin time. [ia]
	 *    KTMARK:   Name of starting time marker to use for results. [k]
	 *===================================================================== */

	cmsmm.nvel = 5;
	Vel[1] = 2.;
	Vel[2] = 3.;
	Vel[3] = 4.;
	Vel[4] = 5.;
	Vel[5] = 6.;
	cmsmm.ldistr = FALSE;
	cmsmm.loriginr = FALSE;
	cmsmm.lgmt = FALSE;
	strcpy( kmsmm.ktmark, "T0      " );

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  MARKVALUE command
	 *    VALUE:    Value to search for. [f]
	 *    LGE:      Search for a data point greater than or equal to VALUE
	 *              if true, less than or equal to VALUE if false. [l]
	 *    KVMARK:   Name of time marker to use for results. [k]
	 *===================================================================== */

	cmsmm.value = 1.0;
	cmsmm.lgedata = TRUE;
	strcpy( kmsmm.kvmark, "T0      " );

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  MARKPTP command
	 *    WINLEN:   Length of moving window in seconds. [f]
	 *    KPMARK:   Name of starting time marker to use for results. [k]
	 *===================================================================== */

	cmsmm.winlen = 5.;
	strcpy( kmsmm.kpmark, "T0      " );

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  RMS command
	 *    LNOISEMTW:  Use noise measurement time window (mtw) if true. [l]
	 *    KNOISEMTW:  Starting and stopping time markers for noise mtw. [ka]
	 *    ONOISEMTW:  Starting and stopping time offsets for noise mtw. [fa]
	 *    IRMSPICK:   Offset into user defined header variables for result. [i]
	 *===================================================================== */

	cmsmm.lnoisemtw = FALSE;
	strcpy( kmsmm.knoisemtw[0], "Z       " );
	strcpy( kmsmm.knoisemtw[1], "Z       " );
	Onoisemtw[1] = 0.;
	Onoisemtw[2] = 0.;
	cmsmm.irmspick = 1;

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    890224:   Added RMS command.
	 *    861128:   Original version.
	 *===================================================================== */

} /* end of function */

