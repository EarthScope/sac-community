
#include <string.h>

#include "icm.h"
#include "bool.h"

void /*FUNCTION*/ iniicm()
{

	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMICM.
	 *=====================================================================
	 * MODIFICATION HISTORY: 
	 *    970113:   Added EVALRESP instrument type. (Doug Dodge)
	 *    900409:   Added LNN instrument type.
	 *    890000:   Added REFTEK instrument type.
	 *    870519:   Added POLEZERO instrument type
	 *    870501:   Made the instrument names more consistent.
	 *    870316:   Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870316
	 *===================================================================== */
	/* PROCEDURE:  */
	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  TRANSFER command
	 *     fpfrom:   Array of floating point parameters for "from" instrument.
	 *               (1) = free period
	 *               (2) = magnification
	 *               (3) = damping factor
	 *               (4) = corner frequency
	 *               (5) = gain
	 *               (6) = highpass corner frequency, 0.0 if none.
	 *     ipfrom:   Array of integer parameters for "from" instrument.
	 *               (1) = number of zeros
	 *     kpfrom:   Array of character parameters for "from" instrument.
	 *               (1) = instrument name
	 *               (2) = instrument subtype name
	 *    fpto, ipto, kpto:  Arrays of parameters for the "to" instrument.
	 *===================================================================== */
	cmicm.lprew = FALSE;
	cmicm.iprew = 6 ;

	cmicm.lfd = FALSE ;	/* TRUE if WHITEN move coefficients into FILTERDESIGN */

	cmicm.lfreql = TRUE;
	Freq[1] = -2.0;
	Freq[2] = -1.0;
	Freq[3] = 1.0e5;
	Freq[4] = 1.0e6;

	cmicm.ninstr = 49;
	strcpy( kmicm.kinstr[0], "ACC" );
	strcpy( kmicm.kinstr[1], "BBDISP" );
	strcpy( kmicm.kinstr[2], "BBVEL" );
	strcpy( kmicm.kinstr[3], "BENBOG" );
	strcpy( kmicm.kinstr[4], "DSS" );
	strcpy( kmicm.kinstr[5], "DWWSSN" );
	strcpy( kmicm.kinstr[6], "EKALP6" );
	strcpy( kmicm.kinstr[7], "EKASP2" );
	strcpy( kmicm.kinstr[8], "ELMAG" );
	strcpy( kmicm.kinstr[9], "IW" );
	strcpy( kmicm.kinstr[10], "GBALP" );
	strcpy( kmicm.kinstr[11], "GBASP" );
	strcpy( kmicm.kinstr[12], "GENERAL" );
	strcpy( kmicm.kinstr[13], "GSREF" );
	strcpy( kmicm.kinstr[14], "HFSLPWB" );
	strcpy( kmicm.kinstr[15], "S750" );
	strcpy( kmicm.kinstr[16], "LLL" );
	strcpy( kmicm.kinstr[17], "LLSN" );
	strcpy( kmicm.kinstr[18], "LRSMLP" );
	strcpy( kmicm.kinstr[19], "LRSMSP" );
	strcpy( kmicm.kinstr[20], "NORESS" );
	strcpy( kmicm.kinstr[21], "NORESSHF" );
	strcpy( kmicm.kinstr[22], "OLDBB" );
	strcpy( kmicm.kinstr[23], "OLDKIR" );
	strcpy( kmicm.kinstr[24], "PORTABLE" );
	strcpy( kmicm.kinstr[25], "PTBLLP" );
	strcpy( kmicm.kinstr[26], "REDKIR" );
	strcpy( kmicm.kinstr[27], "RSTN" );
	strcpy( kmicm.kinstr[28], "SANDIA" );
	strcpy( kmicm.kinstr[29], "SANDIA3" );
	strcpy( kmicm.kinstr[30], "SRO" );
	strcpy( kmicm.kinstr[31], "VEL" );
	strcpy( kmicm.kinstr[32], "WA" );
	strcpy( kmicm.kinstr[33], "WABN" );
	strcpy( kmicm.kinstr[34], "WIECH" );
	strcpy( kmicm.kinstr[35], "WWLPBN" );
	strcpy( kmicm.kinstr[36], "WWSP" );
	strcpy( kmicm.kinstr[37], "WWSPBN" );
	strcpy( kmicm.kinstr[38], "YKALP" );
	strcpy( kmicm.kinstr[39], "YKASP" );
	strcpy( kmicm.kinstr[40], "POLEZERO" );
	strcpy( kmicm.kinstr[41], "NONE" );
	strcpy( kmicm.kinstr[42], "REFTEK" );
	strcpy( kmicm.kinstr[43], "LNN" );
	strcpy( kmicm.kinstr[44], "EVALRESP" );
	strcpy( kmicm.kinstr[45], "FAPFILE" );
	strcpy( kmicm.kinstr[46], "DBASE" );
  strcpy( kmicm.kinstr[47], "NDC" );
  strcpy( kmicm.kinstr[48], "FAP" );
       
	return;

} /* end of function */

