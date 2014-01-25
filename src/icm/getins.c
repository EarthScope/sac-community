
#include <string.h>

#include "icm.h"
#include "bool.h"
#include "co.h"
#include "msg.h"
#include "cpf.h"
#include "EVRESPnames.h"

void /*FUNCTION*/ getins(kinstr, kinstr_s, ninstr, ldone, fp, lfp, 
	 ip, lip, kp, kp_s, lkp, nerr)
char *kinstr;   int kinstr_s;
int ninstr;
int *ldone;
float fp[];
int lfp[]; 
int ip[];
int lip[];
char *kp;   int kp_s;
int lkp[];
int *nerr;
{
#define KINSTR(I_,J_)	(kinstr+(I_)*(kinstr_s)+(J_))
#define KP(I_,J_)	(kp+(I_)*(kp_s)+(J_))
	int index, nchar;
	char temp[ MCPFN+1 ] ;

	float *const Fp = &fp[0] - 1;
	int *const Ip = &ip[0] - 1;
	int *const Lfp = &lfp[0] - 1;
	int *const Lip = &lip[0] - 1;
	int *const Lkp = &lkp[0] - 1;
  double tmp;
    memset(temp, 0, sizeof(temp));
	/*=====================================================================
	 * PURPOSE:  To parse an instrument name and set of instrument parameters.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kinstr:  List of instrument names. [ka]
	 *    ninstr:  Length of "kinstr". [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    ldone:   Set to .TRUE. if there are no more tokens in command. [l]
	 *    fp:      Array of floating point parameters. [fa]
	 *    lfp:     Array of logical flags for "fp" array. [la]
	 *             lfp(j)=.TRUE. if fp(j) has been assigned a value.
	 *    ip:      Array of integer parameters. [ia]
	 *    lip:     Array of logical flags for "ip" array. [la]
	 *    kp:      Array of character parameters. [ia]
	 *    lkp:     Array of logical flags for "kp" array. [la]
	 *    nerr:    Error return flag. Set to zero if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  icm/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    mcpw, mcpfn
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    871023:  Complete overhaul to use arrays of parameters.
	 *             Added GAIN and HIGHPASS options for REFTEK instrument.
	 *    870000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  871023
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Parse the name of the of the instrument. */
	if( lclist( kinstr,kinstr_s, ninstr, &index ) ){
	    fstrncpy( KP(0,0), kp_s-1, KINSTR(index - 1,0),
		      strlen(KINSTR(index - 1,0)));
	    Lkp[1] = TRUE;
	}
	else{
	    cfmt( "UNKNOWN INSTRUMENT TYPE:",25 );
	    return ;
	}

	/* - For the remaining tokens in command: */
	/* - Loop until all tokens are parsed or an unknown keyword has been found. */
	while ( lcmore( nerr ) ){

	    /* -- Check for floating point parameters. */
	    if( lkreal( "FREEPERIOD$",12, &tmp ) ){
        Fp[1] = (float) tmp;
        Lfp[1] = TRUE;
	    }
	    else if( lkreal( "MAGNIFICATION$",15, &tmp ) ){
        Fp[2] = (float) tmp;
        Lfp[2] = TRUE;
	    }
	    else if( lkreal( "DAMPING$",9, &tmp ) ){
        Fp[3] = (float) tmp;
        Lfp[3] = TRUE;
	    }
	    else if( lkreal( "CORNER$",8, &tmp ) ){
        Fp[4] = (float) tmp;
        Lfp[4] = TRUE;
	    }
	    else if( lkreal( "GAIN$",6, &tmp ) ){
        Fp[5] = (float) tmp;
        Lfp[5] = TRUE;
	    }
	    else if( lkreal( "HIGHPASS$",10, &tmp ) ){
        Fp[6] = (float) tmp;
        Lfp[6] = TRUE;
	    }

	    /* -- Check for integer parameters. */
	    else if( lkint( "NZEROS$",8, &Ip[1] ) ){
		Lip[1] = TRUE;
	    }

	    /* -- Check for character parameters. */
	    else if( lkchar( "SUBTYPE$",9, MCPFN, KP(1,0),kp_s, &nchar ) ){
		if ( index == 47 ) {	/* if DBASE */
		    setmsg ( "WARNING" , 2117 ) ;
		    outmsg () ;
		    clrmsg () ;
		    *(KP(1, 0)) = '\0' ;
		    continue ;
		}
		Lkp[2] = TRUE;
	    }
	    else if( lkchar( "STATION$",9, MCPFN, temp,MCPFN, &nchar ) ){
	        setStationName(temp, getTransferDirection());
	    }
	    else if( lkchar( "CHANNEL$",9, MCPFN, temp,MCPFN, &nchar ) ){
	        setChannelName(temp, getTransferDirection());
	    }
	    else if( lkchar( "NETWORK$",9, MCPFN, temp,MCPFN, &nchar ) ){
	        setNetworkName(temp, getTransferDirection());
	    }
	    else if( lkchar( "DATE$",6, MCPFN, temp,MCPFN, &nchar ) ){
	        setDate(temp, getTransferDirection());
	    }
	    else if( lkchar( "TIME$",6, MCPFN, temp,MCPFN, &nchar ) ){
	        setTime(temp, getTransferDirection());
	    }
	    else if( lkchar( "LOCID$",7, MCPFN, temp,MCPFN, &nchar ) ){
	        setLocidName(temp, getTransferDirection());
	    }
	    else if( lkchar( "FNAME$",7, MCPFN, temp,MCPFN, &nchar ) ){
		if ( index == 47 ) {    /* if DBASE */
		    setmsg ( "WARNING" , 2117 ) ;
		    outmsg () ;
		    clrmsg () ;
		    temp[ 0 ] = '\0' ;
		    continue ;
		}
	        setFileName(temp, getTransferDirection());
	    }
	    /* -- Return without setting "done" flag if an unknown keyword is found. */
	    else{
		break ;
	    }
	}

	/* - Return with "done" flag set if there are no more tokens in command. */
	if ( lcmore ( nerr ) )
	    *ldone = FALSE ;
	else
	    *ldone = TRUE ;

#undef	KP
#undef	KINSTR
} /* end of function */

