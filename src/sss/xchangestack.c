#include "sss.h"
#include "dfm.h"
#include "bool.h"

#include "msg.h"
#include "clf.h"
#include "cpf.h"

void xchangestack(int *nerr)
{
	char kfile[MCPFN+1];
	int jdfl, ncfile;
	double delay, tmp;

	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command CHANGESTACK.
	 *          This command changes properties of a file in stack.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001, 5106, 5107.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    mcpfn
	 *    dfm:     ndfl, kdfl
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     dlyn, dlyt, dlyni, dlyti, wt, dst, lpol, del, beginTime,
	 *             endTime
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcint, lcdfl, lkreal, lclog2,
	 *             setmsg, apimsg, apcmsg
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    kfile:   Name of file whose properties are to be changed. [c]
	 *    ncfile:  Number of characters in kfile. [i] {NOT USED}
	 *    jdfl:    Index of file whose properties are to be changed. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960701:  Added beginTime and endTime, maf
	 *    850812:  Major rewrite of subprocess.
	 *    821130:  Changed to new command parsing logic.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850812
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Parse position dependent tokens.
	 *   (This is the name or number of the file whose properties are to be changed.) */

	if( lcint( &jdfl ) ){
		if( jdfl < 1 || jdfl > cmdfm.ndfl ){
			*nerr = 5107;
			setmsg( "ERROR", *nerr );
			apimsg( jdfl );
			goto L_8888;
			}
		}
	else if( lcchar( MCPFN, kfile,MCPFN+1, &ncfile ) ){
        jdfl = 1 + string_list_find(datafiles, kfile, MCPFN+1);
		if( jdfl <= 0 ){
			*nerr = 5106;
			setmsg( "ERROR", *nerr );
			apcmsg( kfile,MCPFN+1 );
			goto L_8888;
			}
		}
	else{
		cfmt( "NEED A FILENAME OR NUMBER",27 );
		cresp();
		}

	/* - Loop on rest of tokens in command:
	 *   (These are keywords which change properties for this file only.) */

	while( lcmore( nerr ) ){

		/* -- "WEIGHT v":  define global weight property. */
		if( lkreal( "WEIGHT$",8, &tmp ) ){
      Wt[jdfl] = (float) tmp;
    }

			/* -- "DELAY v":  define global static delay propertys. */
		else if( lkreal( "DE#LAY$",8, &delay ) ){
			if( lckey( "SECONDS$",9 ) ){
				Dlyt[jdfl] = delay;
				}
			else if( lckey( "POINTS$",8 ) ){
				Dlyn[jdfl] = delay;
				}
			}

			/* -- "INCREMENT v":  define global static delay propertys. */
		else if( lkreal( "INCREMENT$",11, &delay ) ){
			if( lckey( "SECONDS$",9 ) ){
				Dlyti[jdfl] = delay;
				}
			else if( lckey( "POINTS$",8 ) ){
				Dlyni[jdfl] = delay;
				}
			}

			/* -- "NORMAL/REVERSED":  define global polarity property. */
		else if( lclog2( "NORMAL$",8, "REVERSED$",10, &Lpol[jdfl] ) ){
			}

			/* -- "DISTANCE v":  define global distance property. */
		else if( lkreal( "DI#STANCE$",11, &tmp ) ){
      Dst[jdfl] = (float) tmp; 
    }

    /* -- "BEGINTIME v":  define global begin time property. added 960701 maf */
    else if( lkreal( "BE#GINTIME$",12, &tmp ) ){
      Tbegin[jdfl] = (float) tmp;
    }

    /* -- "ENDTIME v":  define global end time property. added 960701 maf */
    else if( lkreal( "END#TIME$",10, &tmp ) ){
      Tend[jdfl] = (float) tmp;
    }

			/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}

		}

	/* - The above loop is over when one of two conditions has been met.:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0.
	 *   (2) All the tokens in the command have been successfully parsed. */

L_8888:
	return;

} /* end of function */

