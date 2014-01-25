/** 
 * @file   iztypeMessage.c
 * 
 * @brief  Get a message type
 * 
 */

#include <string.h>

#include "dfm.h"

#include "SacHeader.h"

#include "errors.h"


#include "msg.h"

/** 
 * Check a time change to make sure it is not a reference time.
 *   If the command CHANGEHDR is changing the header field \p item which
 *       happens to be the reference time \p iztype,
 *       eg. origin when iztype == IO,
 *       then warn the user.
 * 
 * @param item 
 *    Field which is being changed
 * @param iztype 
 *    Current reference time
 *
 * Errors
 *    - ERROR_REFERENCE_TIME_NOT_ZERO
 *
 */
void 
iztypeMessage(const int item, 
	            int iztype) {

	char field[ 3 ] ;

	field[ 0 ] = '\0' ;

	switch ( item ) {
	    case 6:	/* B */
		if ( iztype == IB )
		    strcpy ( field , "B" ) ;
		break ;
	    case 8:	/* O */
		if ( iztype == IO )
		    strcpy ( field , "O" ) ;
		break ;
	    case 9:	/* A */
		if ( iztype == IA )
		    strcpy ( field , "A" ) ;
		break ;
	    case 11:	/* T0 */
		if ( iztype == IT0 )
		    strcpy ( field , "T0" ) ;
		break ;
	    case 12:	/* T1*/
		if ( iztype == IT1 )
		    strcpy ( field , "T1" ) ;
		break ;
	    case 13:	/* T2 */
		if ( iztype == IT2 )
		    strcpy ( field , "T2" ) ;
		break ;
	    case 14:	/* T3 */
		if ( iztype == IT3 )
		    strcpy ( field , "T3" ) ;
		break ;
	    case 15:	/* T4 */
		if ( iztype == IT4 )
		    strcpy ( field , "T4" ) ;
		break ;
	    case 16:	/* T5 */
		if ( iztype == IT5 )
		    strcpy ( field , "T5" ) ;
		break ;
	    case 17:	/* T6 */
		if ( iztype == IT6 )
		    strcpy ( field , "T6" ) ;
		break ;
	    case 18:	/* T7 */
		if ( iztype == IT7 )
		    strcpy ( field , "T7" ) ;
		break ;
	    case 19:	/* T8 */
		if ( iztype == IT8 )
		    strcpy ( field , "T8" ) ;
		break ;
	    case 20:	/* T9 */
		if ( iztype == IT9 )
		    strcpy ( field , "T9" ) ;
		break ;
	} /* end switch */


	if ( field[ 0 ] ) {
	    setmsg ( "WARNING" , ERROR_REFERENCE_TIME_NOT_ZERO ) ;
	    apcmsg ( field , strlen ( field ) + 1 ) ;
	    outmsg () ;
	    clrmsg () ;
	}
}
