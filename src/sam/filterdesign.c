
#include <string.h>

#include "sam.h"
#include "gdm.h"
#include "gem.h"
#include "com.h"
#include "amf.h"
#include "bool.h"


#include "msg.h"
#include "top.h"
#include "bot.h"
#include "cpf.h"

#include "errors.h"

#define	IFILTMOD	6
#define	MXMPTR	10

void /*FUNCTION*/ filterdesign(nerr)
int *nerr;
{
	char kcommand[9], kFilePrefix[MCPFN] ;
	int lfound , lprint = FALSE ;
	int idx, index, memptr[MXMPTR], module, nerrmem, nerrplt, notused;
	int nchar = 0 , nerrwrt = 0 , newnpts , xbeg ;
	float userData[ 9 ] ;
  Token *t;

	/*=====================================================================
	 * PURPOSE:  To control plot production of a filter's digital vs analog
	 *           characteristics for: phase, amplitude and group delay.
	 *           Also includes the impulse response for the filter.
	 * INPUT ARGUMENTS:
	 *      None.  Just the command line parameters (on the command stack)
	 *      for the appropriate filter.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/6
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SAM:     KTPIIR, MTPIIR
	 *    ERR:     MBDSYN, MOUTRG
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  fdlp, fdhp, fdbp, fdbr, fdplot
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *  memptr:   Array of pointers to the data. [i]
	 *  mxmptr:   Size of memptr, (number of traces in the plot). [i]
	 *  ifiltmod: The command module number for filter commands. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 * 910624: Commented out error message printing. Handled in saccommands.
	 * 910301: Original version.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	kFilePrefix[ 0 ] = '\0' ;

	/* - Check for PRINT option */
  if ( lcmore ( nerr ) ) {
    /* -- "PRINT":  print the final product */
    if( lckey( "PRINT#$", 8 ) ) {
      if ( cmgdm.lbegf ) {
        setmsg ( "WARNING" , 2403 ) ;
        outmsg () ;
        clrmsg () ;
      }
      else {
		    char * ptr ;

        lprint = TRUE ;
        
        if((t = arg()) && token_is_string(t)) {
          ptr = t->str;
          arg_next();
        } else {
          *nerr = ERROR_BAD_COMMAND_SYNTAX;
          setmsg("ERROR", *nerr);
          return;
        }

		    if ( strcmp ( ptr , "LP      " ) &&
             strcmp ( ptr , "HP      " ) &&
             strcmp ( ptr , "BP      " ) &&
             strcmp ( ptr , "BR      " ) &&
             strcmp ( ptr , "lp      " ) &&
             strcmp ( ptr , "hp      " ) &&
             strcmp ( ptr , "bp      " ) &&
             strcmp ( ptr , "br      " ) &&
             strcmp ( ptr , "lP      " ) &&
             strcmp ( ptr , "hP      " ) &&
             strcmp ( ptr , "bP      " ) &&
             strcmp ( ptr , "bR      " ) &&
             strcmp ( ptr , "Lp      " ) &&
             strcmp ( ptr , "Hp      " ) &&
             strcmp ( ptr , "Bp      " ) &&
             strcmp ( ptr , "Br      " ) ) {
          
          lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
                   MAXPRNTRNAMELEN+1 , &notused ) ;
          terminate ( kmgem.kptrName ) ;
          if ( !lprint )
            kmgem.kptrName[0] = '\0' ;
		    }
      }
    }
  }
  
	/* - Check for FILE option. */

	if ( lkchar ( "FILE#$" , 7 , MCPFN , kFilePrefix , MCPFN , &nchar ) )
	    terminate ( kFilePrefix ) ;

	/* - Check for possible filter commands. */

	if ( lcmore( nerr ) ){
	    lcchar( MCPW, kcommand,9, &notused );
	    modcase( TRUE, kcommand, MCPW, kcommand );

	    /* -- Validate command and find module and index number. */
	    findcommand( kcommand, &lfound, &module, &index );

	    /* --- Test for filter module number. */
	    if( lfound && (module == IFILTMOD) ){

		/* ---- Produce the data files in memory */
		if( index >= 6 && index <= 9 ){
		    switch( index - 5 ){
			case 1: /* LP - lowpass */
				fdlp( memptr, MXMPTR, userData, nerr );
				break ;
			case 2: /* HP - highpass */
				fdhp( memptr, MXMPTR, userData, nerr );
				break ;
			case 3: /* BP - bandpass */
				fdbp( memptr, MXMPTR, userData, nerr );
				break ;
			case 4: /* BR - bandreject */
				fdbr( memptr, MXMPTR, userData, nerr );
				break ;
		    }
		}

                /* --- invalid module or index number. */
                else {
                    cfmt( "ILLEGAL OPTION:",17 );
                    cresp();
                    goto L_8888;
                }
	    }
	}

	/* --- No command line parameters; do nothing. */
	else{
	    goto L_8888;
	}



	/* - Stop if parsing error, or data file creation error.
	 *   This may be bad.  There could have been a memory allocation
	 *   error, and some memory release may be required.  Add
	 *   appropriate error codes to fdlp, hp, bp, br and check them here to
	 *   avoid a memory problem (wct). */

	if( *nerr != 0 )
	    goto L_8888;

	/* - Display the filter response curves. */

	newnpts = fdplot ( memptr , lprint , &xbeg , &nerrplt ) ;

	/* - Write files if requested */
	if ( nchar > 0 ) {
	    fdWriteFiles ( memptr, kFilePrefix, userData, newnpts, &nerrwrt ) ;
	}

	/* - Release data storage allocation. */

	for( idx = 0; idx < MXMPTR; idx++ ){
	    relamb( cmmem.sacmem, memptr[idx], &nerrmem );
	}

	/* - Return: memory allocation error gets higher priority
	 *   than plot error. */
	if( nerrplt != 0 )
	    *nerr = nerrplt;
	if( nerrwrt != 0 )
	    *nerr = nerrwrt;
	if( nerrmem != 0 )
	    *nerr = nerrmem;

L_8888:
	return;
} /* end of function */





