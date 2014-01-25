
#include <string.h>

#include "sam.h"
#include "bool.h"


#include "co.h"

void /*FUNCTION*/ inisam()
{
	char krtbwi[9], krtewi[9];

	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMSAM.
	 *=====================================================================
	 * PARAMETERS:
	 *   MFFT    [65536]    Maximum length of Fourier transform.
	 *   MFIR    [255]      Maximum number of coefficients in FIR filters.
	 *                      Set in included file 'fir'.
	 *   MRIR    [17]       Length of RIR data arrays.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: FFT and IFFT commands.
	 *   IFWD    [-1]       Exponential sign when applying forward transform.
	 *   IBWD    [+1]       Exponential sign when applying inverse transform.
	 *   LRLIM   [.TRUE.]   Spectral output of forward transform is real-
	 *                      imaginary if .TRUE., amplitude-phase if .FALSE.
	 *   LWMEAN  [.FALSE.]  Mean is not removed from signal before
	 *                      performing forward transform if .TRUE.
	 *   LSPFWD  [.TRUE.]   Forward transform in single precision if .TRUE.
	 *   LSPBWD  [.TRUE.]   Inverse transform in single precision if .TRUE.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: IIR filter commands.
	 *   ITPLP   [1]        Type or class of lowpass filter.
	 *   NPOLLP  [2]        Number of poles for lowpass filter.
	 *   NPASLP  [1]        Number of passes to make with lowpass filter.
	 *   CFLP    [0.4]      Cutoff frequency for lowpass filter (Hz).
	 *   TBWLP   [0.3]      Transition bandwidth for Cheb. lowpass filter.
	 *   ATNLP   [30.0]     Attenuation factor for Cheb. lowpass filter.
	 *   ITPHP   [1]        Type or class of highpass filter.
	 *   NPOLHP  [2]        Number of poles for highpass filter.
	 *   NPASHP  [1]        Number of passes to make with highpass filter.
	 *   CFHP    [0.2]      Cutoff frequency for highpass filter (Hz).
	 *   TBWHP   [0.3]      Transition bandwidth for Cheb. highpass filter.
	 *   ATNHP   [30.0]     Attenuation factor for Cheb. highpass filter.
	 *   ITPBP   [1]        Type or class of bandpass filter.
	 *   NPOLBP  [2]        Number of poles for bandpass filter.
	 *   NPASBP  [1]        Number of passes to make with bandpass filter.
	 *   CFBP1   [0.1]      Low  cutoff frequency of bandpass filter (Hz).
	 *   CFBP2   [0.1]      High cutoff frequency of bandpass filter (Hz).
	 *   TBWBP   [0.3]      Transition bandwidth for Cheb. bandpass filter.
	 *   ATNBP   [30.0]     Attenuation factor for Cheb. bandpass filter.
	 *   ITPBR   [1]        Type or class of bandreject filter.
	 *   NPOLBR  [2]        Number of poles for bandreject filter.
	 *   NPASBR  [1]        Number of passes to make with bandreject filter.
	 *   CFBR1   [0.1]      Low  cutoff frequency of bandreject filter (Hz).
	 *   CFBR2   [0.4]      High cutoff frequency of bandreject filter (Hz).
	 *   TBWBR   [0.3]      Transition bandwidth for Cheb. bandreject filter.
	 *   ATNBR   [30.0]     Attenuation factor for Cheb. bandreject filter.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: FIR filter commands.
	 *   KNMFIR  ['FIR']    Name of FIR filter disk file [T: char*32]
	 *   LRQREC  [.FALSE.]  .TRUE. if FIR filter is be applied recursively.
	 *   KIDFIR             FIR filter identification [T: char*80].
	 *   NCFIR              Number of FIR coefficients [R: 1-MFIR].
	 *   CFIR               Array for storage of FIR coefficients [L: MFIR].
	 *   DTFIR              Design sampling rate of FIR filter.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: Wiener filter command.
	 *   NCWIEN  [30]       Number of coefficients in Wiener filter [R: 2-100]
	 *   LRTWWI  [.TRUE.]   Relative time window flag for Wiener filter.
	 *   KRTBWI  ['B']      Beginning reference time key for adaptive window.
	 *   RTOBWI  [0.]       Beginning reference time offset for adaptive window.
	 *   KRTEWI  ['B']      Ending reference time key for adaptive window.
	 *   RTOEWI  [10.]      Ending reference time offset for adaptive window.
	 *   WIENMU  [0.]       Wiener filter step size coefficient.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: PSP, RSP, and WSP commands.
	 *   KSPTPL  [*]        List of allowed spectral types.
	 *                      Used for plotting and writing spectral components.
	 *   NSPTPL  [8]        Number of elements in KSPTPL.
	 *   LRSPE              Set to .TRUE. if SPE files are read using RSP.
	 *   LRAMPH             Set to .TRUE. if AMPH files are read using RSP.
	 *   KWSPTP  ['ASIS']   Type of spectral components to write.
	 *                      Value is limited to an element of KSPTPL.
	 *   KPSPTP  ['ASIS']   Type of spectral components to plot.
	 *                      Value is limited to an element of KSPTPL.
	 *   NPSPPP  [1]        Number of spectral components per plot.
	 *   LWSPOV  [.TRUE.]   Use same names (with suffixes) when writing as
	 *                      read data file list if .TRUE.
	 *                      Use KWSPFL as names when .FALSE.
	 *   KWSPFL             Names to use when writing spectral components.
	 *   NWSPFL  [0]        Number of elements in KWSPFL.
	 *   LWAMPH  [.FALSE.]  Write requires amplitude-phase files if .TRUE.
	 *   LWRLIM  [.FALSE.]  Write requires real-imaginary files if .TRUE.
	 *   LWSPC1  [.TRUE.]   Will write first spectral component if .TRUE.
	 *   LWSPC2  [.TRUE.]   Will write second spectral component if .TRUE.
	 *   KWSPS1             Suffix used in writing first component.
	 *   KWSPS2             Suffix used in writing second component.
	 *   LPAMPH  [.FALSE.]  Plot requires amplitude-phase files if .TRUE.
	 *   LPRLIM  [.FALSE.]  Plot requires real-imaginary files if .TRUE.
	 *   LPSPC1  [.TRUE.]   Will plot first spectral component if .TRUE.
	 *   LPSPC2  [.TRUE.]   Will plot second spectral component if .TRUE.
	 *   KPSPL1             Label used in plotting first component [char*16].
	 *   KPSPL2             Label used in plotting second component [char*16].
	 *    IXSPIN:  X axis interpolation mode for PSP. [i]
	 *             = 0 use linear interpolation.
	 *             = 1 use logarithmic interpolation.
	 *    IYSPIN:  Y axis interpolation mode for PSP. [i]
	 *             = 0 use linear interpolation.
	 *             = 1 use logarithmic interpolation.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: FILTERDESIGN command. */
	/*                      Used for plotting and writing spectral components.
	 *   NDATPTS            Number of points used in signal data sets.
	 *                      This is actually a parameter.
	 *   NIMPPTS            Number of points used in impulse resposne data set.
	 *                      This is actually a parameter.
	 *   FDDELTA            Sampling interval for the data set. User selectable
	 *                      for the filterdesign command. Actual filtering
	 *                      commands get the delta from the SAC file header.
	 *   KTPIIR             Array of types or classes of IIR filters:
	 *                      (1)='BU' for Butterworth filter.
	 *                      (2)='BE' for Bessel filter.
	 *                      (3)='C1' for Chebyshev Type I filter.
	 *                      (4)='C2' for Chebyshev Type II filter.
	 *   KPROTYP            Array of prototypes for the filters:
	 *                      (1)='AM'  for analog amplitude.
	 *                      (2)='PH'  for analog phase.
	 *                      (3)='GD'  for analog group delay.
	 *                      (4)='DAM' for digital amplitude.
	 *                      (5)='DPH' for digital phase.
	 *                      (6)='DGD' for digital group delay.
	 *===================================================================== */
	/* PROCEDURE: */
	cmsam.ifwd = -1;
	cmsam.ibwd = 1;
	cmsam.lrlim = FALSE;
	cmsam.lwmean = TRUE;
	cmsam.lspfwd = TRUE;
	cmsam.lspbwd = TRUE;

	cmsam.fddelta = 0.025;

	strcpy( kmsam.ktpiir[0], "BUTTER  " );
	strcpy( kmsam.ktpiir[1], "BESSEL  " );
	strcpy( kmsam.ktpiir[2], "C1      " );
	strcpy( kmsam.ktpiir[3], "C2      " );

	strcpy( kmsam.kprotyp[0], "AM " );
	strcpy( kmsam.kprotyp[1], "PH " );
	strcpy( kmsam.kprotyp[2], "GD " );
	strcpy( kmsam.kprotyp[3], "DAM" );
	strcpy( kmsam.kprotyp[4], "DPH" );
	strcpy( kmsam.kprotyp[5], "DGD" );

	cmsam.itplp = 1;
	cmsam.npollp = 2;
	cmsam.npaslp = 1;
	cmsam.cflp = 0.4;
	cmsam.tbwlp = 0.3;
	cmsam.atnlp = 30.0;
	cmsam.itphp = 1;
	cmsam.npolhp = 2;
	cmsam.npashp = 1;
	cmsam.cfhp = 0.2;
	cmsam.tbwhp = 0.3;
	cmsam.atnhp = 30.0;
	cmsam.itpbp = 1;
	cmsam.npolbp = 2;
	cmsam.npasbp = 1;
	cmsam.cfbp1 = 0.1;
	cmsam.cfbp2 = 0.4;
	cmsam.tbwbp = 0.3;
	cmsam.atnbp = 30.0;
	cmsam.itpbr = 1;
	cmsam.npolbr = 2;
	cmsam.npasbr = 1;
	cmsam.cfbr1 = 0.1;
	cmsam.cfbr2 = 0.4;
	cmsam.tbwbr = 0.3;
	cmsam.atnbr = 30.0;
	cmsam.lmu = FALSE ;
	cmsam.lepsilon = FALSE ;
	

	cmsam.lrqrec = FALSE;
	fstrncpy( cmsam.knmfir, MCPFN, "FIR", 3 );
	cmsam.ncwien = 30;
	cmsam.wienwb = 0.;
	cmsam.wienwe = 0.;
	cmsam.wienmu = 0.;
	cmsam.lrtwwi = TRUE;
	strcpy( krtbwi, "B       " );
	strcpy( krtewi, "B       " );

	cmsam.nsptpl = 8;
	strcpy( kmsam.ksptpl[0], "ASIS" );
	strcpy( kmsam.ksptpl[1], "RLIM" );
	strcpy( kmsam.ksptpl[2], "AMPH" );
	strcpy( kmsam.ksptpl[3], "RL" );
	strcpy( kmsam.ksptpl[4], "IM" );
	strcpy( kmsam.ksptpl[5], "AM" );
	strcpy( kmsam.ksptpl[6], "PH" );
	strcpy( kmsam.ksptpl[7], "POWER" );
	cmsam.lrspe = FALSE;
	cmsam.lramph = TRUE;
	strcpy( kmsam.kwsptp, "ASIS    " );
	cmsam.lwspov = TRUE;
	cmsam.nwspfl = 0;
	cmsam.lwamph = FALSE;
	cmsam.lwrlim = FALSE;
	cmsam.lwspc1 = TRUE;
	cmsam.lwspc2 = TRUE;
	strcpy( kmsam.kpsptp, "ASIS    " );
	cmsam.lpamph = FALSE;
	cmsam.lprlim = FALSE;
	cmsam.lpspc1 = TRUE;
	cmsam.lpspc2 = TRUE;
	cmsam.ixspin = 1;
	cmsam.iyspin = 1;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: TAPER command.
	 *    MTAPTP:  Maximum number of taper types. [p]
	 *    KTAPTP:  List of taper types. [k]
	 *    NTAPTP:  Current number of taper types. [i]
	 *    ITAPTP:  Index to current taper type. [i]
	 *    WIDTAP:  Fractional width of taper at each end. [f]
	 *===================================================================== */

	strcpy( kmsam.ktaptp[0], "COSINE  " );
	strcpy( kmsam.ktaptp[1], "HANNING " );
	strcpy( kmsam.ktaptp[2], "HAMMING " );
	cmsam.ntaptp = 3;
	cmsam.itaptp = 2;
	cmsam.widtap = 0.05;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: UNWRAP command.
	 *===================================================================== */

	cmsam.lunwfz = FALSE;
	cmsam.vunwit = 1.5;
	cmsam.vunwct = 0.5;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: KHRONHITE command.
	 *    CUTKHR:  Cutoff frequency for khronhite filter. [f]
	 *===================================================================== */

	cmsam.cutkhr = 2.0;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR: CROSS command.
	 *    NWIN:    Number of (potentially overlapping) windows. [i]
	 *    LWINLN:  Set to .TRUE. if a fixed window length is desired. [l]
	 *    WINLN:   Fixed window length in seconds. [f]
	 *    IMAST:   DFL number of "master event." [i]
	 *    MWINTP:  Maximum number of window types. [ip]
	 *    KWINTP:  List of allowed window types. [ka]
	 *    IWINTP:  Desired window type. [i]
	 *===================================================================== */

	cmsam.nwin = 1;
	cmsam.lwinln = FALSE;
	cmsam.winln = 0.;
	cmsam.imast = 1;
	strcpy( kmsam.kwintp[0], "HAMMING " );
	strcpy( kmsam.kwintp[1], "HANNING " );
	strcpy( kmsam.kwintp[2], "COSINE  " );
	strcpy( kmsam.kwintp[3], "RECTANGL" );
	strcpy( kmsam.kwintp[4], "TRIANGLE" );
	cmsam.iwintp = 4;

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    960723:  Added cmsam.lmu and cmsam.lepsilon
	 *    901115:  Added fddelta as a variable to cmsam for filterdesign.
	 *    901009:  Added kprotyp prototype array for filterdesign command.
	 *    870212:  Added initialization of CROSS and KHRONHITE command.
	 *    850322:  Added initialization of LRAMPH.
	 *    850119:  Modified initial values for TAPER command.
	 *    831208:  Changed initial value of LRLIM.
	 *    830108:  Added UNWRAP command initialization.
	 *    820622:  Changed initialization of IIR filter types.
	 *    810721:  Added initialization for TAPER command.
	 *    810413:  Original version.
	 *===================================================================== */

} /* end of function */

