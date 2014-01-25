
#include <string.h>

#include "spe.h"
#include "bool.h"


#include "co.h"

void /*FUNCTION*/ inispe()
{
	/*=====================================================================
	 * PURPOSE: Variable initialization of common blocks CMSPE and KMSPE.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/4
	 *=====================================================================
	 * PARAMETERS:
	 *    MLNPE:   Length of prediction error function. [i]
	 *    MWINTP:  Number of window types. [i]
	 *    MPSPTP:  Number of spectral estimate plot types. [i]
	 *    MPREWH:  Maximum number of prewhitening filter coefficients. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    LFILE:   .TRUE. if one evenly spaced data file is in memory. [l]
	 *    NDXDAT:  Index (first word address in SACMEM array) of data. [i]
	 *    NLNDAT:  Length (number of points) of data. [i]
	 *    SAMFRQ:  Sampling freqency of data. [f]
	 *    NDXCOR:  Index of correlation function. [i]
	 *    NDXPE:   Index of prediction error function. [i]
	 *    NDXSPE:  Index of spectral estimate. [i]
	 *    NDXAUX:  Index of auxiliary (scratch) space. [i]
	 *    LCOR:    .TRUE. if correlation function has been calculated. [l]
	 *    IWNCOR:  Type of window used used in correlation. [i]
	 *             (Index into KWINTP array.)
	 *    NUMWIN:  Number of windows used in correlation. [i]
	 *    WINLEN:  Window length in seconds. [f]
	 *    NWINLN:  Window length in samples. [i]
	 *    LSNUMW:  .TRUE. if the number of windows is specified. [l]
	 *             .FALSE. if the number of windows is calculated from
	 *             the data length and window length.
	 *    LPREWH:  .TRUE. if prewhitening of data is on. [l]
	 *    NPRERQ:  Requested number of prewhitening filter coefficients. [i]
	 *    NPREWH:  Number of prewhitening coefficients used. [i]
	 *    CPREWH:  Array used to save prewhitening filter coefficients. [f]
	 *    KSCALE:  Type of scaling used in autocorrelation. [c10]
	 *             ='STOCHASTIC' if data is noise.
	 *             ='TRANSIENT' if data contains a waveform.
	 *    NLNCOR:  Length of autocorrelation function. [i]
	 *    NLNFFT:  Length of FFT used to compute autocorrelation. [i]
	 *    NLNSPE:  Length of spectral estimate. [i]
	 *    LSPE:    .TRUE. if spectral estimate has been calculated. [l]
	 *    KWINTP:  List of allowed window types. [k]
	 *             Used in autocorrelation and PDS calculations.
	 *    LPCSEC:  .TRUE. if partial correlation plot is requested. [l]
	 *    PCSEC:   Width in seconds of partial correlation plot. [f]
	 *    IWNPDS:  Type of window used in PDS calculation. [i]
	 *             (Index into KWINTP array.)
	 *    SECPDS:  Window length in seconds for PDS. [f]
	 *    NLGPDS:  Window length in lags for PDS. [i]
	 *    NLGMEM:  Order of estimate in lags for MEM. [i]
	 *    NLGMLM:  Order of estimate in lags for MLM. [i]
	 *    LRESL:   .TRUE. if a resolution has been calculated. [l]
	 *    RESL:    Resolution in Hz. [f]
	 *    LCL      .TRUE. if a confidence limit has been calculated. [l]
	 *    CLU:     Upper confidence limit. [f]
	 *    CLL:     Lower confidence limit. [f]
	 *    LRQCL:  .TRUE. if confidence limits are to be plotted.. [l]
	 *    LSPEID:  .TRUE. if spectral estimate id is to be plotted. [l]
	 *    KPSPTP:  List of allowed spectral estimate plot types. [k]
	 *    NPSPTP:  Length of KPSPTP.
	 *    IPSPTP:  Type of spectral estimate plot format. [i]
	 *    KPSPLB:  Label to put on spectral estimate plot. [k]
	 *    KNMCOR:  Correlation function file name. [c]
	 *    KNMSPE:   Spectral estimate file name. [c]
	 *    KERMSG:  Error message returned from Harris's subroutines. [c130]
	 *===================================================================== */
	/* PROCEDURE: */
	cmspe.lfile = FALSE;
	cmspe.lcor = FALSE;
	cmspe.winlen = 10.0;
	cmspe.lsnumw = FALSE;
	cmspe.numwin = 10;
	cmspe.lprewh = FALSE;
	cmspe.nprerq = 6;
	strcpy( kmspe.kscale, "STOCHASTIC" );
	cmspe.nlnspe = 1024;
	strcpy( kmspe.kwintp[0], "HAMMING " );
	strcpy( kmspe.kwintp[1], "HANNING " );
	strcpy( kmspe.kwintp[2], "COSINE  " );
	strcpy( kmspe.kwintp[3], "RECTANGL" );
	strcpy( kmspe.kwintp[4], "TRIANGLE" );
	cmspe.iwncor = 1;
	cmspe.iwnpds = 1;
	cmspe.nlgmem = 25;
	cmspe.nlgmlm = 25;
	cmspe.lrqcl = FALSE;
	cmspe.lspeid = TRUE;
	strcpy( kmspe.kpsptp[0], "POWER   " );
	strcpy( kmspe.kpsptp[1], "LOG     " );
	strcpy( kmspe.kpsptp[2], "AMPLITUD" );
	cmspe.npsptp = 3;
	cmspe.ipsptp = 1;
	fstrncpy( kmspe.knmspe, MCPFN, "spe", 3 );
	fstrncpy( kmspe.knmcor, MCPFN, "cor", 3 );

	cmspe.firstPowerOf2 = MINPOW ;

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    810414:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */

} /* end of function */

