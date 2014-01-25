
#include <string.h>
#include <ctype.h>

#include "ssi.h"
#include "exm.h"
#include "bool.h"


#include "msg.h"
#include "bot.h"
#include "ucf.h"

/* SeisMgrCode:	facilitates running SAC commands that make use of the
		SeisMgr module.

   Input Variable:  kcommand is a character string containing a command
		    that was entered at the prompt, or read from a macro

   Output Variable: nerr is the error code.

   If kcommand contains a command that uses the SeisMgr module, SeisMgrCode
   runs the command directly and returns 1.  Otherwise it returns 0,
   and SAC processes the command in standard SAC fashion.
*/

int SeisMgrCode ( kcommand , nerr )
char *kcommand ;
int *nerr ;
{
    /* Declare Variables */
    char * kcommandPtr = kcommand ,
	 * endCommand = NULL ,
	 commandName[ 81 ] ;
    int commandLength ;

    /* Get first word in command, and convert to upper case */
    while ( isspace ( *kcommandPtr ) )
	kcommandPtr++ ;
    endCommand = strchr ( kcommandPtr , ' ' ) ;
    if ( endCommand == NULL )
	commandLength = strlen ( kcommandPtr ) ;
    else
	commandLength = endCommand - kcommandPtr ;
    modcase ( TRUE , kcommandPtr , commandLength , commandName ) ;

    /* If it's a SeisMgr command, run it, otherwise return 0. */
    if ( ( !strncmp ( commandName , "READDB" , commandLength ) && 
	   commandLength == 6 ) || ( commandLength == 3 &&
	   !strncmp ( commandName , "RDB" , commandLength ) ) )
    {
	echoAndShave ( kcommand , &kcommandPtr , commandLength ) ;

	/* execute the command */
	xreaddb ( kcommandPtr , nerr ) ;
    } /* end READDB */

    else if ( !strncmp ( commandName , "COMMIT" , commandLength ) 
		&& commandLength == 6 )
    {
	echoAndShave ( kcommand , &kcommandPtr , commandLength ) ;

        /* execute the command */
	xcommit ( /* kcommandPtr ,*/ nerr ) ;
	/* Note:  kcommandPtr will be used if COMMIT gets an option */
    } /* end COMMIT */

    else if ( ( !strncmp ( commandName , "RECALL" , commandLength )
		&& commandLength == 6 ) || ( commandLength == 11 &&
		!strncmp ( commandName , "RECALLTRACE" , commandLength ) ) )
    {
	echoAndShave ( kcommand , &kcommandPtr , commandLength ) ;

	/* execute the command */
	xrecall ( /* kcommandPtr ,*/ nerr ) ;
	/* Note:  kcommandPtr will be used if RECALL gets an option */
    } /* end RECALL */

    else if ( ( !strncmp ( commandName , "ROLLBACK" , commandLength ) &&
	      commandLength == 8 ) || ( commandLength == 2 &&
	      !strncmp ( commandName , "RB" , commandLength ) ) )
    {
	echoAndShave ( kcommand , &kcommandPtr , commandLength ) ;

	/* execute the command */
	xrollback ( /* kcommandPtr ,*/ nerr ) ;
	/* Note:  kcommandPtr will be used if ROLLBACK gets an option */
    } /* end ROLLBACK */

/*    else if ( ( !strncmp ( commandName , "TABLNAME" , commandLength ) &&
              commandLength == 8 ) || ( commandLength == 2 &&
              !strncmp ( commandName , "TN" , commandLength ) ) )
    {
        echoAndShave ( kcommand , &kcommandPtr , commandLength ) ;

        * execute the command *
        xtablname( kcommandPtr , nerr ) ;
    } * end TABLNAME */

    else
	return 0 ;

    /* Take care of loose ends and stuff */
    reperr( *nerr );
    
    if( *nerr != 0 ){
        setmsg( "ERROR", *nerr );
        proerr( nerr );
    }

    if( cmexm.ntraces > 0 )
        tracereport( nerr );

    return 1 ;
}


void echoAndShave ( char * kcommand , char ** kcommandPtr , int commandLength )
{
    /* -- Echo command if requested. */
    if( cmexm.lecho && strcmp(kcommand,"ECHO    ") != 0 )
	wrcom();

    /* shave off the first word */
    *kcommandPtr += commandLength ;
    while ( isspace ( **kcommandPtr ) )
	(*kcommandPtr)++ ;
}
