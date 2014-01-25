
#include <string.h>

#include "ssi.h"
#include "dfm.h"
#include "bool.h"

#include "smDataIO.h"


#include "bot.h"
#include "cpf.h"

void xreaddb ( kinput , nerr )
char * kinput ;
int * nerr ;
{
    /* Declare variables */
    char koption[MCMSG+1] , * ptrKoption , * ptrKinput = kinput ;
    int numberOfLines ;
    int lmore = OverWrite ;
    int takeEvid = TRUE ;

    DBlist tree ;
    int Verbose = 0;

        /*=====================================================================
         * PURPOSE:  To execute the action command READDB.
         *           This command reads data from a CSS formatted Oracle 
	 *           database into SAC's memory.
         *           File info is kept in SeisMgr storage and in memory
         *           resident SAC files.  
         *=====================================================================
         * INPUT ARGUMENTS:
         *    KINPUT:  Character string containing primary options and
         *             database options for the command.  Note, this
	 *             command is not parsed by the standard SAC parsing
	 *             mechanism.
         *=====================================================================
         * OUTPUT ARGUMENTS:
         *    NERR:    Error flag. Set to 0 if no error occurred.
         *=====================================================================
         * MODULE/LEVEL: DBM/2
         *=====================================================================
         * GLOBAL INPUT:
         *    DFM:     LSHIFT, LSCALE, NMAGSPEC, NDFL
         *=====================================================================
         * MODIFICATION HISTORY:
	 *	971210:	Original version.
         *=====================================================================*/

    *nerr = 0 ;


    /* Look for the primary options:  MORE, MAGNITUDE, SHIFT, and SCALE */
    strcpy ( koption , kinput ) ;
    ptrKoption = strtok ( koption , " \t" ) ;

    while ( ptrKoption != NULL )
    {
	int tokenLen ;

	tokenLen = strlen ( ptrKoption ) ;

	/* Make all the characters upper-case for the purpose of comparison */
	modcase ( TRUE , ptrKoption , tokenLen , ptrKoption ) ;

	/* Compare current token to acceptable options.  If there's a match,
	   process it. */
	if ( strcmp ( ptrKoption , "MORE" ) == 0 )
	{
	    lmore = Add ;
	} /* end MORE */

	else if ( strcmp ( ptrKoption , "SHIFT" ) == 0 )
	{
	    int lshift ;

	    /* look at the next token to see if it says OFF, ON or neither 
		(return 0, 1, or 2 respectively ) */
	    lshift = OnOrOff ( ptrKoption ) ;
	    cmdfm.lshift = lshift ? TRUE : FALSE ;

	    /* if lshift is less than 2, move beyond the dumb token */
	    if ( lshift < 2 ) 
            strtok ( NULL , " \t" ) ;
	} /* end SHIFT */

	else if ( strcmp ( ptrKoption , "SCALE" ) == 0 )
	{
            int lscale ;

            /* look at the next token to see if it says OFF, ON or neither
                (return 0, 1, or 2 respectively ) */
            lscale = OnOrOff ( ptrKoption ) ;
            cmdfm.lscale = lscale ? TRUE : FALSE ;

	    /* warn user of mutual exclusion between scale and transfer */
/*	    if ( cmdfm.lscale ) {
		setmsg ( "WARNING" , 2119 ) ;
		outmsg () ;
		clrmsg () ;
	    }
*/
            /* if lscale is less than 2, move beyond the dumb token */
            if ( lscale < 2 )
                strtok ( NULL , " \t" ) ;
	} /* end SCALE */

        else if ( strcmp ( ptrKoption , "TRUST" ) == 0 )
	{
	    int ltrust ;

            /* look at the next token to see if it says OFF, ON or neither
                (return 0, 1, or 2 respectively ) */
            ltrust = OnOrOff ( ptrKoption ) ;
            cmdfm.ltrust = ltrust ? TRUE : FALSE ;

            /* if ltrust is less than 2, move beyond the dumb token */
            if ( ltrust < 2 )
                strtok ( NULL , " \t" ) ;
        } /* end TRUST */

	else if ( strncmp ( ptrKoption , "MAGNITUDE" , 3 > tokenLen ? 3 : tokenLen ) == 0 )
	{
            char * kmag = ptrKoption + 1 ;
            kmag += strlen ( ptrKoption ) ;
            while ( *kmag == ' ' || *kmag == '\t' )
                kmag++ ;

            modcase ( TRUE , kmag , 3 , kmag ) ;

	    /* Default (will use algorithm). */
            if ( strncmp ( kmag , "DEF" , 3 ) == 0 )
                cmdfm.nMagSpec = Any ;
	    /* MB, get Bodywave magnitude. */
            else if ( strncmp ( kmag , "MB " , 3 ) == 0 ||
		      strncmp ( kmag , "MB\t" , 3 ) == 0 )
                cmdfm.nMagSpec = MbMag ;
	    /* MS, get Surfacewave magnitude. */
            else if ( strncmp ( kmag , "MS " , 3 ) == 0 ||
		      strncmp ( kmag , "MS\t" , 3 ) == 0 )
	    {
                cmdfm.nMagSpec = MsMag ;
	    }
	    /* ML, get Local magnitude. */
            else if ( strncmp ( kmag , "ML " , 3 ) == 0 ||
		      strncmp ( kmag , "ML\t" , 3 ) == 0 )
	    {
                cmdfm.nMagSpec = MlMag ;
	    }
            else
            {
		cfmt( "ILLEGAL PARAM VALUE:",22 );
		cresp () ;
		return ;
	    }

	    /* move beyond the dumb token */
	    strtok ( NULL , " \t" ) ;
	} /* end MAGNITUDE */

	else if ( !strcmp ( ptrKoption , "COMMIT" ) ) 
	    cmdfm.icomORroll = COMMIT ;
	else if ( !strcmp ( ptrKoption , "RECALLTRACE" ) )
	    cmdfm.icomORroll = RECALL ;
	else if ( !strcmp ( ptrKoption , "RECALL" ) )
	    cmdfm.icomORroll = RECALL ;
	else if ( !strcmp ( ptrKoption , "ROLLBACK" ) )
	    cmdfm.icomORroll = ROLLBACK ;

	else	/* the current and remaining tokens get passed to smLoadOracleData */
	{
	    ptrKinput = kinput + ( ptrKoption - koption ) ;
	    break ;
	}

	/* Prepare for the next pass through the loop. */
	ptrKoption = strtok ( NULL , " \t" ) ;
    } /* end while ( ptrKoption ) */

    /* if ptrKoption is NULL, there are no tokens to pass to smLoadOracleData */
    if ( ptrKoption == NULL )
	*ptrKinput = '\0' ;

    /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
    if ( lmore == Add ) {
	alignFiles ( nerr ) ;
	if ( *nerr )
	    return ;
	cmdfm.nfilesFirst = cmdfm.ndfl ;
    } /* end if */
    else{
	cmdfm.nreadflag = RDB ;
	cmdfm.nfilesFirst = 0 ;
    }

    if( cmdfm.nreadflag == RDB )
        takeEvid = TRUE ;
    else if( cmdfm.nreadflag == HIGH && cmdfm.ltrust == TRUE )
        takeEvid = TRUE ;
    else
        takeEvid = FALSE ;

    /* smLoadOracleData() produces a static array of data in CSSstrucs */
    numberOfLines = smLoadOracleData( takeEvid, ptrKinput , CreateMode , lmore , 0 ) ;

    if ( numberOfLines < 1 ) {
	printf ( "MESSAGE: No Lines Returned\n" ) ;
	return ;
    }

    /* Set tree to the workset that was just created.  For the purposes of 
	the prototype I'm writing on 11/4/97, that is the default workset. */
    tree = ( smGetDefaultWorkset () )->tree ;

    if ( !tree )
    {
	printf ( "Error: can't get a valid tree.\n" ) ;
	return ;
	/* this will have to be rewritten with real error handling. */
    }

    cmdfm.lread = TRUE ;
    SeisMgrToSac ( tree , 
                   lmore == Add ? 0 : 1 , 
                   nerr, 
                   Verbose , 
                   TRUE,
                   FALSE ) ;
    cmdfm.lread = FALSE ;
} /* end xreaddb () */



