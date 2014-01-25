/** 
 * @file   iniexm.c
 * 
 * @brief  Execution Module Initialization
 * 
 */

#include <string.h>

#include "exm.h"
#include "bool.h"


#include "co.h"

/** 
 * Initialize the execution module
 * 
 */
void 
iniexm() {
	char k1301[9];
	int jtranscript, jtranscript_, jtype, jtype_;


	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMEXM.
	 *=====================================================================
	 * MODULE/LEVEL:  EXM/4
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: control parameters.
	 *   LINSYS  [.FALSE.]  .TRUE. if this is an "operating system" version.
	 *   LPROD   [.FALSE.]  .TRUE. if in production mode.
	 *   LCOMF   [.FALSE.]  .TRUE. if running from a command file.
	 *   KPRMT   ['SAC>']   Terminal prompt when waiting for input.
	 *   K1301   ['FATAL']  Mode to handle error 1301 if it occurs.
	 *   LCOMCR  [.FALSE.]  .TRUE. if command correction mode is on.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: FUNCGEN command.
	 *   NFGTP   [10]       Number of functions available.
	 *   KFGTP              Names of available functions:
	 *                      ='IMPULSE','STEP','BOX','TRIANGLE','SINE'
	 *                      ,'LINE','QUADRATI','CUBIC','RANDOM','DATA'.
	 *   IFGTP   [1]        Index of current function.
	 *   NFGPTS  [100]      Number of points in generated function.
	 *   FGBEG   [0.]       Beginning value of independent variable.
	 *   FGDEL   [1.]       Sampling interval used when generating function.
	 *   FGSICO             Coefficients used to generate sine function:
	 *                         y(t) = SINE{2*pi*[f*t+(a/360.)]}
	 *      (1)  [1.]       Frequency in Hz ("f" in above equation.)
	 *      (2)  [0.]       Phase angle in degrees ("a" above.)
	 *
	 *   FGLICO             Coefficients used to generate linear function:
	 *                         y(t) = a*t+b
	 *      (1)  [1.]       Linear coefficient ("a" above.)
	 *      (2)  [1.]       Constant coefficient ("b" above.)
	 *
	 *   FGQUCO             Coefficients used to generate quadratic function:
	 *                         y(t) = a*x**2 + b*x + c
	 *      (1)  [1.]       Quadratic coefficient ("a" above.)
	 *      (2)  [1.]       Linear coefficient ("b" above.)
	 *      (3)  [1.]       Constant coefficient ("c" above.)
	 *
	 *   FGCUCO             Coefficients used to generate cubic function:
	 *                         y(t) = a*x**3 + b*x**2 + c*x + d
	 *      (1)  [1.]       Cubic coefficient ("a" above.)
	 *      (2)  [1.]       Quadratic coefficient ("b" above.)
	 *      (3)  [1.]       Linear coefficient ("c" above.)
	 *      (4)  [1.]       Constant coefficient ("d" above.)
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR: GETBB command.
	 *    lbball:      Set to .TRUE. if all currently defined bb variables
	 *                 are to be reported.  Set to .FALSE. if only a select
	 *                 number of variables are to be reported.
	 *    lnames:      Set to .TRUE. if the bb variable name and an equals
	 *                 sign is to placed before each bb value.
	 *    lnewline:    Set to .TRUE. if a newline is to placed
	 *                 after each bb variable value.
	 *    nunbbwrite:  File unit to report results on. [i]
	 *    knmbbwrite:  Name of file to report results to if not terminal. [c]
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920528:  Added WIDTH to REPORT command.
	 *    920501:  Added option to SAVE or DELETE memory on nofiles read error.
	 *    920403:  Added DATASET to REPORT command.
	 *    890907:  Added UNIT option to FUNCGEN.
	 *    881230:  Deleted initialization of variable lnerr.
	 *    880901:  Added enhancements for GETBB command.
	 *    870817:  Added initialization for GETBB command.
	 *    870728:  Added five more items to the REPORT command.
	 *    860924:  Added initialization of PAUSE command.
	 *    850729:  Added MM (memory manager) report.
	 *    840620:  Added initialization of EVALUATE command.
	 *    840120:  Added initialization for RANDOM function.
	 *    831208:  Changed initial value of LCOMCR.
	 *    820818:  Added REPORT and ERROR_CONTROL command initialization.
	 *    810414:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	cmexm.linsys = FALSE;
	cmexm.lprod = FALSE;
	cmexm.lcomf = FALSE;
	strcpy( kmexm.kprmt, "SAC> $      " );
	strcpy( k1301, "FATAL   " );
	cmexm.lcomcr = FALSE;

	cmexm.nfgtp = 12;
	strcpy( kmexm.kfgtp[0], "IMPULSE " );
	strcpy( kmexm.kfgtp[1], "STEP    " );
	strcpy( kmexm.kfgtp[2], "BOXCAR  " );
	strcpy( kmexm.kfgtp[3], "TRIANGLE" );
	strcpy( kmexm.kfgtp[4], "SINE    " );
	strcpy( kmexm.kfgtp[5], "LINE    " );
	strcpy( kmexm.kfgtp[6], "QUADRATI" );
	strcpy( kmexm.kfgtp[7], "CUBIC   " );
	strcpy( kmexm.kfgtp[8], "RANDOM  " );
	strcpy( kmexm.kfgtp[9], "SEISMOGR" );
	strcpy( kmexm.kfgtp[10], "UNIT    " );
	strcpy( kmexm.kfgtp[11], "IMPSTRIN" );
	cmexm.ifgtp = 1;
	cmexm.nfgpts = 100;
	cmexm.fgbeg = 0.;
	cmexm.fgdel = 1.;
	Fgsico[1] = 0.05;
	Fgsico[2] = 0.;
	Fglico[1] = 1.;
	Fglico[2] = 1.;
	Fgquco[1] = 1.;
	Fgquco[2] = 1.;
	Fgquco[3] = 1.;
	Fgcuco[1] = 1.;
	Fgcuco[2] = 1.;
	Fgcuco[3] = 1.;
	Fgcuco[4] = 1.;
	Fgraco[1] = 1.;
	Fgraco[2] = 12357.;

	cmexm.nreptp = 18;
	strcpy( kmexm.kreptp[0], "HPF     " );
	strcpy( kmexm.kreptp[1], "APF     " );
	strcpy( kmexm.kreptp[2], "COLOR   " );
	strcpy( kmexm.kreptp[3], "FILEID  " );
	strcpy( kmexm.kreptp[4], "PICKS   " );
	strcpy( kmexm.kreptp[5], "TITLE   " );
	strcpy( kmexm.kreptp[6], "XLABEL  " );
	strcpy( kmexm.kreptp[7], "YLABEL  " );
	strcpy( kmexm.kreptp[8], "CUT     " );
	strcpy( kmexm.kreptp[9], "XLIM    " );
	strcpy( kmexm.kreptp[10], "MEMORY  " );
	strcpy( kmexm.kreptp[11], "DEVICES " );
	strcpy( kmexm.kreptp[12], "LINE    " );
	strcpy( kmexm.kreptp[13], "SYMBOL  " );
	strcpy( kmexm.kreptp[14], "GTEXT   " );
	strcpy( kmexm.kreptp[15], "YLIM    " );
	strcpy( kmexm.kreptp[16], "MTW     " );
	strcpy( kmexm.kreptp[17], "WIDTH   " );
	cmexm.nrep = 0;

	cmexm.nectp = 5;
	strcpy( kmexm.kectp[0], "FATAL   " );
	strcpy( kmexm.kectp[1], "WARNING " );
	strcpy( kmexm.kectp[2], "IGNORE  " );
	strcpy( kmexm.kectp[3], "SAVE    " );
	strcpy( kmexm.kectp[4], "DELETE  " );

	fstrncpy( kmexm.kpause, MCMSG, "Pausing $", 9);
	cmexm.lperio = FALSE;
	cmexm.nperio = 10000;
	cmexm.lecho = FALSE;

	cmexm.nfeval = 20;
	strcpy( kmexm.kfeval[0], "+       " );
	strcpy( kmexm.kfeval[1], "-       " );
	strcpy( kmexm.kfeval[2], "*       " );
	strcpy( kmexm.kfeval[3], "/       " );
	strcpy( kmexm.kfeval[4], "**      " );
	strcpy( kmexm.kfeval[5], "SQRT    " );
	strcpy( kmexm.kfeval[6], "EXP     " );
	strcpy( kmexm.kfeval[7], "ALOG    " );
	strcpy( kmexm.kfeval[8], "ALOG10  " );
	strcpy( kmexm.kfeval[9], "SIN     " );
	strcpy( kmexm.kfeval[10], "ASIN    " );
	strcpy( kmexm.kfeval[11], "COS     " );
	strcpy( kmexm.kfeval[12], "ACOS    " );
	strcpy( kmexm.kfeval[13], "TAN     " );
	strcpy( kmexm.kfeval[14], "ATAN    " );
	strcpy( kmexm.kfeval[15], "ADD     " );
	strcpy( kmexm.kfeval[16], "SUBTRACT" );
	strcpy( kmexm.kfeval[17], "MULTIPLY" );
	strcpy( kmexm.kfeval[18], "DIVIDE  " );
	strcpy( kmexm.kfeval[19], "POWER   " );

	cmexm.nleval = 6;
	strcpy( kmexm.kleval[0], "LT      " );
	strcpy( kmexm.kleval[1], "LE      " );
	strcpy( kmexm.kleval[2], "GT      " );
	strcpy( kmexm.kleval[3], "GE      " );
	strcpy( kmexm.kleval[4], "EQ      " );
	strcpy( kmexm.kleval[5], "NE      " );

	cmexm.lfloat = TRUE;
	cmexm.lfeval = TRUE;
	strcpy( kmexm.kevaln, "TERM    " );
	cmexm.neval = 1;
	Ifeval[1] = 3;
	cmexm.feval[0] = 1.;
	cmexm.feval[1] = 1.;

	cmexm.nmcdir = 0;

	cmexm.lbball = TRUE;
	cmexm.nunbbwrite = MUNOUT;
	cmexm.lnames = TRUE;
	cmexm.lnewline = TRUE;
	fstrncpy( kmexm.knmbbwrite, MCPFN, "getbbout", 8);

	cmexm.ntranscripts = 0;
	for( jtranscript = 1; jtranscript <= MTRANSCRIPTS; jtranscript++ ){
		jtranscript_ = jtranscript - 1;
		cmexm.nuntranscript[jtranscript_] = 0;
		for( jtype = 1; jtype <= MTYPES; jtype++ ){
			jtype_ = jtype - 1;
			cmexm.lsendtranscript[jtranscript_][jtype_] = TRUE;
			}
		}
	fstrncpy( kmexm.knametranscript[0], MCPFN, "transcript", 10);

	cmexm.itranscript = 1;
	cmexm.imodetranscript = 1;

	cmexm.ntraces = 0;

	strcpy( kmexm.ktextwait, "ON      " );

       
	return;

} /* end of function */

