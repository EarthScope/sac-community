/** 
 * @file   inieam.c
 * 
 * @brief  Initialize the common block CMEAM
 * 
 */

#include <string.h>

#include "eam.h"
#include "bool.h"



#include "co.h"

/** 
 * Initialize the Common Block CMEAM
 * 
 * @note Variable Definitions for automatic picking modules
 *  - C1      [0.985]    Recursive high-pass filtering constant.
 *  - C2      [3.0]      Weighting constant, used to weight the first
 *                       difference in the characteristic function.
 *  - C3      [0.6]      Timing constant, used to compute short term
 *                       average of the characteristic function.
 *  - C4      [0.03]     Timing constant, used to compute long term
 *                       average of the characteristic function.
 *  - C5      [5.0]      Threshold constant, used to compute reference level.
 *  - C6      [0.0039]   Timing constant, used to compute running mean
 *                       absolute value of filtered data.
 *  - C7      [100.]     Station is assumed to be dead when ABS(cf)>C7.
 *  - C8      [-0.1]     Coda termination level.
 *  - I3      [3]        Constant used in computing MSSZC, the number of
 *                       successive small counts to be allowed before
 *                       declaring an event over.
 *  - I4      [40]       Test value for MZC, the number of zero cross-
 *                       ings.  Validation phase is over when MZC ge I4.
 *  - I6      [3]        Test value for NLZC, the number of large
 *                       amplitude zero crossings.
 *                       NLZC must be ge I6 for event to be valid.
 *  - D5      [2.]       Minimum duration in seconds for a valid event.
 *  - D8      [3.]       Minimum duration of low level signal
 *                       to declare event over.
 *  - D9      [1.]       Long term average initialization duration.
 *  - LVALPK  [.TRUE.]   Pick validation option.
 *
 * @note Variable Definitions for alphanumeric pick file (APF) modules
 *  - LAPFOP  [.FALSE.]  Set to .TRUE. if a APF is open.
 *  - NAPFUN             Fortran file unit on which APF is open.
 *  - KAPFNM  ['APF']    Name of APF.
 *  - KPKID              Pick id.
 *  - KPKEV              Pick event id [char*16].
 *  - KPKST              Pick station id.
 *  - PKCMPA             Station comp. aziumthal angle [deg. from North].
 *  - PKCMPI             Station comp. incident angle [deg. from vert.].
 *  - PKSECI             Pick time in seconds
 *                       (offset relative to reference time).
 *  - PKAMPL             Pick amplitude.
 *  - DTWF               Time increments for waveform pick [arraylen=5].
 *  - AWF                Amplitudes for waveform pick [arraylen=5].
 *  - KPKSRC  [' ']      Pick source [char*1].
 *  - KPKRID  [' ']      Picker id [char*3].
 *  - KPKFMT             Pick format [internal].
 *  - LPKGMT  [.TRUE.]   Write GMT times to APF if .TRUE.
 *                       Write relative times if .FALSE.
 *  - LPFSTD  [.TRUE.]   Write event name, station name, and
 *                       station component id to APF if .TRUE.
 *                       Write dfl name to APF if .FALSE.
 *  - NPKYR              Pick year [internal].
 *  - NPKJDY             Pick julian day [internal].
 *  - NPKHR              Pick hour [internal].
 *  - NPKMN              Pick minute [internal].
 *  - NPKSC              Pick second [internal].
 *  - NPKMSC             Pick millisecond [internal].
 *
 * @date   860625:  Added LVALPK.
 * @date   810414:  Original version.
 *
 */
void 
inieam()
{

	/* - Initialization for automatic picker. */
	cmeam.c1 = 0.985;
	cmeam.c2 = 3.;
	cmeam.c3 = 0.6;
	cmeam.c4 = 0.03;
	cmeam.c5 = 5.;
	cmeam.c6 = 0.0039;
	cmeam.c7 = 100.;
	cmeam.c8 = -0.1;
	cmeam.i3 = 3;
	cmeam.i4 = 40;
	cmeam.d5 = 2.;
	cmeam.i6 = 3;
	cmeam.d8 = 3.;
	cmeam.d9 = 1.;
	cmeam.lvalpk = TRUE;

	/* - Initialization for HYPO pick file. */
	cmeam.lhpfop = FALSE;
	cmeam.nhpfun = 0;
	fstrncpy( kmeam.khpfnm, MCPFN, "HPF", 3);
	cmeam.lsphas = FALSE;
	cmeam.lampx = FALSE;
	cmeam.lfini = FALSE;
	strcpy( kmeam.kpwave, "        " );
	strcpy( kmeam.kswave, "        " );

	cmeam.lichpf = TRUE;
	Ichpf[1] = 5;
	Ichpf[2] = 0;

	/* - Initialization for alphanumeric pick file (APF). */
	cmeam.lapfop = FALSE;
	fstrncpy( kmeam.kapfnm, MCPFN, "APF", 3);
	strcpy( kmeam.kpkrid, "        " );
	cmeam.lpfstd = TRUE;
	cmeam.lpfgmt = TRUE;
	strcpy( kmeam.kpkid, "        " );

       
	return;

}

