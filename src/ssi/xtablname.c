
#include <string.h>

#include "config.h"


#include "ssi.h"
#include "bool.h"
#include "bot.h"

#include "dbselect/dbDefaults.h"

#include "debug.h"

void xtablname( kinput , nerr )
char * kinput ;
int * nerr ;
{
#ifdef DATABASE_ENABLED
    /* Declare variables */
    char koption[MCMSG+1] , * ptrKoption ;


        /*=====================================================================
         * PURPOSE:  To execute the parameter setting command TABLNAME.
         *           This command sets the names of the tables in a CSS
	 *           formated database.
         *=====================================================================
         * INPUT ARGUMENTS:
         *    KINPUT:  Character string containing options telling which
         *             tables to name what name.  Note, this command is
	 *             not parsed by the standard SAC parsing mechanism.
         *=====================================================================
         * OUTPUT ARGUMENTS:
         *    NERR:    Error flag. Set to 0 if no error occurred.
         *=====================================================================
         * MODULE/LEVEL: SSI/2
         *=====================================================================
         * GLOBAL INPUT:
         *=====================================================================
         * MODIFICATION HISTORY:
	 *	000426:	Original version.
         *=====================================================================*/

    *nerr = 0 ;


    /* Parse the options:  wfdisc, wftag, origin, event, arrival, assoc, site,
                           sitechan, instrument, and sensor. */
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
	if ( strcmp ( ptrKoption , "WFDISC" ) == 0 )
	{
	    ptrKoption = strtok ( NULL , " \t" ) ;
	    if( ptrKoption )
		dbSetWfdiscTableName( ptrKoption ) ;
	    break ;
	} /* end WFDISC */

        if ( strcmp ( ptrKoption , "WFTAG" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetWftagTableName( ptrKoption ) ;
            break ;
        } /* end WFTAG */

        if ( strcmp ( ptrKoption , "ORIGIN" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetOriginTableName( ptrKoption ) ;
            break ;
        } /* end ORIGIN */

        if ( strcmp ( ptrKoption , "EVENT" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetEventTableName( ptrKoption ) ;
            break ;
        } /* end EVENT */

        if ( strcmp ( ptrKoption , "ARRIVAL" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetArrivalTableName( ptrKoption ) ;
            break ;
        } /* end ARRIVAL */

        if ( strcmp ( ptrKoption , "ASSOC" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetAssocTableName( ptrKoption ) ;
            break ;
        } /* end ASSOC */

        if ( strcmp ( ptrKoption , "SITE" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetSiteTableName( ptrKoption ) ;
            break ;
        } /* end SITE */

        if ( strcmp ( ptrKoption , "SITECHAN" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetSitechanTableName( ptrKoption ) ;
            break ;
        } /* end SITECHAN */

        if ( strcmp ( ptrKoption , "INSTRUMENT" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetInstrumentTableName( ptrKoption ) ;
            break ;
        } /* end INSTRUMENT */

        if ( strcmp ( ptrKoption , "SENSOR" ) == 0 )
        {
            ptrKoption = strtok ( NULL , " \t" ) ;
            if( ptrKoption )
                dbSetSensorTableName( ptrKoption ) ;
            break ;
        } /* end SENSOR */


	/* Prepare for the next pass through the loop. */
	ptrKoption = strtok ( NULL , " \t" ) ;
    } /* end while ( ptrKoption ) */
#else /* DATABASE_ENABLED */
  UNUSED(kinput);
  UNUSED(nerr);
#endif
} /* end xtablname () */

