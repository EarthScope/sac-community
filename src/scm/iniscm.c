
#include <string.h>

#include "scm.h"
#include "bool.h"

void /*FUNCTION*/ iniscm()
{
	int j;

	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMSCM.
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/4
	 *===================================================================== */
	/* PROCEDURE: */
	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: ROTATE command.
	 *   KROTTP:  Type of rotation requested. [k]
	 *             = 'HDRGCP' for rotation to great circle path.
	 *             = 'USRAZ' for rotation to user specified azimuth.
	 *             = 'USRANG' for rotation through a specified angle.
	 *   USRAZ:    User specified azimuth. [f]
	 *   USRANG:   User specified angle. [f]
	 *   LNPREQ:   Normal output signal polarity requested if true. [l]
	 *             Normal polarity: radial X tangential = vertical.
	 *===================================================================== */
	strcpy( kmscm.krottp, "HDRGCP  " );
	cmscm.usraz = 0.;
	cmscm.usrang = 0.;
	cmscm.lnpreq = TRUE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: RQ command.
	 *   RQQCON:   Dimensionless seismic quality factor. [f]
	 *   RQRCON:   Source to station path length in km. [f]
	 *   RQCCON:   Group velocity in km/sec. [f]
	 *===================================================================== */

	cmscm.rqqcon = 1.;
	cmscm.rqrcon = 0.;
	cmscm.rqccon = 1.;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: INTERPOLATE command.
	 *    DTNEW:   New sampling rate. [f]
	 *    EPS:     Interpolation water level. [f]
	 *    LBREQ:   .TRUE. if a specific begin time was requested. [l]
	 *             .FALSE. if the file begin time is to be used.
	 *    BREQ:    Requested begin time if LBREQ is .TRUE. [f]
	 *    LNREQ:   .TRUE. if a specific number of points was requested. [l]
	 *             .FALSE. if the number of points in file is to be used.
	 *    NREQ:    Requested number of points if LNREQ is .TRUE. [i]
	 *===================================================================== */

	cmscm.dtnew = 0.0;
	cmscm.eps = 0.0001;
	cmscm.lbreq = FALSE;
	cmscm.breq = 0.;
	cmscm.lnreq = FALSE;
	cmscm.nreq = 1;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: QUANTIZE command.
	 *    NQGAIN:  Number of quantization levels. [f]
	 *    IQGAIN:  List of allowed gains. [ia]
	 *    QLEVEL:  Quantization level for lowest gain. [f]
	 *    NQMANT:  Number of bits in mantissa. [i]
	 *===================================================================== */

	Iqgain[1] = 128;
	Iqgain[2] = 32;
	Iqgain[3] = 8;
	for( j = 4; j <= (MQGAIN + 1); j++ ){
		Iqgain[j] = 1;
		}
	cmscm.qlevel = 0.00001;
	cmscm.nqmant = 14;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: STRETCH command.
	 *    NSTRFC:   Stretch (upsampling) factor. [i]
	 *    LSTRFI:   Set to true if interpolating filter is to be applied.
	 *===================================================================== */

	cmscm.nstrfc = 2;
	cmscm.lstrfi = TRUE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: SMOOTH command.
	 *    LMEAN:    Set to .TRUE. for mean smoothing,
	 *              set to .FALSE. for median smoothing. [l]
	 *    NHALF:    Half-width of smoothing region. [i]
	 *===================================================================== */

	cmscm.lmean = TRUE;
	cmscm.nhalf = 1;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: RGLITCHES command.
	 *    MRGLTP:  Maximum number of glitch replacement techniques. [ip]
	 *    KRGLTP:  Names of glitch replacement techniques. [k]
	 *    IRGLTP:  Requested glitch replacement technique. [i]
	 *    MRGLMT:  Maximum number of glitch removal method. [ip]
	 *    KRGLMT:  Names of glitch removal method. [k]
	 *    IRGLMT:  Requested glitch removal method. [i]
	 *    THOLD:   Glitch threshold level. [f]
	 *===================================================================== */

	strcpy( kmscm.krgltp[0], "LINEAR  " );
	strcpy( kmscm.krgltp[1], "ZERO    " );
	cmscm.irgltp = 1;
	strcpy( kmscm.krglmt[0], "ABSOLUTE" );
	strcpy( kmscm.krglmt[1], "POWER   " );
	strcpy( kmscm.krglmt[2], "RUNAVG  " );
	cmscm.irglmt = 2;
	cmscm.thold = 1.0e10;
	cmscm.lrglwin = FALSE;
	strcpy( kmscm.krglwin[0], "B       " );
	strcpy( kmscm.krglwin[1], "E       " );
	Orglwin[1] = 0.;
	Orglwin[2] = 0.;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: DECIMATE command.
	 *    NDECFC:   Decimation factor. [i]
	 *    LDECFI:   Set to true if interpolating filter is to be applied.
	 *===================================================================== */

	cmscm.ndecfc = 2;
	cmscm.ldecfi = TRUE;

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    100706:  Changed dtnew from 0.025 to 0.0.  jas/vt
         *    920218:  Added METHOD to RGLITCHES command.
	 *    870206:  Added RGLITCHES command.
	 *    870202:  Changed RESAMPLE to STRETCH.
	 *             Added SMOOTH command.
	 *    861129:  Added INTERPOLATE, QUANTIZE, and RESAMPLE commands.
	 *    810416:  Original version.
	 *===================================================================== */

} /* end of function */

