#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gcm.h"
#include "gd2.h"


#include "cpf.h"

void /*FUNCTION*/ xsgf(nerr)
int *nerr;
{
	char ktext[MCPFN+1];
	int ifnum, ntext;
	double value;

    memset(ktext, 0, sizeof(ktext));
	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command SGF.
	 *           This command sets attributes of the SAC Graphics File.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL: gcm/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPFN, VSMALL, VLARGE
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     lcmore, lckey, lkchar, lkrrc, cfmt, cresp,
	 *             setsgfprefix, setsgfnumber, setsgfdir, setsgfsize
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900310:  Added SIZE options.
	 *    861112:  Major rewrite due to new graphics library.
	 *    841016:  Original version (based on obsolete XHCD.)
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900310
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "PREFIX text":  set frame name PREFix. */
		if( lkchar( "PREFIX$",8, MCPFN, ktext,MCPFN+1, &ntext ) ){
			setsgfprefix( ktext );
		}

		/* -- "ID text": an old name for the PREFIX option. */
		else if( lkchar( "ID$",4, MCPFN, ktext,MCPFN+1, &ntext ) ){
			setsgfprefix( ktext );
		}

		/* -- "NUMBER n":  set frame number attribute. */
		else if( lkint( "NUMBER$",8, &ifnum ) ){
			setsgfnumber( ifnum );
		}

		/* -- "FRAME n":  an old name for the NUMBER option. */
		else if( lkint( "FRAME$",7, &ifnum ) ){
			setsgfnumber( ifnum );
		}

		/* -- "DIRECTORY CURRENT|text": set directory name to store SGF files. */
		else if( lkchar( "DIRECTO$",9, MCPFN, ktext,MCPFN+1, &ntext ) ){
			if( memcmp(ktext,"CURRENT",7) == 0 ){
				setsgfdir( " ",2 );
			}
			else{
				setsgfdir( ktext,MCPFN+1 );
			}
		}

		/* -- "SIZE NORMAL|FIXED v|SCALED v": set SGF plot size option. */
		else if( lckey( "SIZE$",6 ) ){
			if( lckey( "NORMAL$",8 ) ){
				setsgfsize( "NORMAL", 1.0 );
			}
			else if( lkrrc( "FIXED$",7, VSMALL, VLARGE, &value ) ){
				setsgfsize( "FIXED", (float)value );
			}
			else if( lkrrc( "SCALED$",8, VSMALL, VLARGE, &value ) ){
				setsgfsize( "SCALED", (float)value );
			}
			else{
				cfmt( "ILLEGAL SIZE OPTION:",22 );
				cresp();
			}
		}

		/* -- "OVERWRITE ON|OFF": ON doesn't increment file names.  */
		else if(lklog("OVER#WRITE$",12,&cmgd2.lover))
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
		goto L_1000;

	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

} /* end of function */

