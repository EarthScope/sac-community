/** 
 * @file   wrhelp.c
 * 
 * @brief  Write help contents
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ucf.h"
#include "msg.h"
#include "bool.h"

#include "config.h"

#include "string_utils.h"

#include "co.h"
#include "gdm.h"
#include "bot.h"
#include "debug.h"

extern char *pager;

/** 
 * Write the contents of a help package to the message subsystem
 * 
 * @param ktoken 
 *    Name of help package to write
 * @param ktoken_s 
 *    Length of \p ktoken
 * @param imode 
 *    - 1 Write entire help file, pause for each full screen
 *    - 2 Write only command syntax
 * @param lprint 
 *    - TRUE if the command is to be send to the printer (lpr)
 *    - FALSE if the command is only send to the screen
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Succcess
 *
 * @date   911218:  Added call to typmsg per Doug Neuhauser email of 9/12/91.
 * @date   870924:  Changed error return logic.
 * @date   831111:  Removed from machine dependent level.
 * @date   830818:  Used NLSCRN from TERM insert to determine full screen.
 * @date   830104:  Modifications due to changes in HELP system.
 * @date   820823:  Factored from XHELP.
 * @date   811116:  Changed check for form-feed from column 1 to column 2.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   810115:  Major revision for new help package format.
 * @date   800915:  Original version.
 * 
 */
void 
wrhelp(char *ktoken, 
       int   ktoken_s, 
       int   imode, 
       int   lprint, 
       int  *nerr) { 

	char kfilename[ MCPFN + 1 ] ;
	int ncerr;
        FILE *nun;
	int i;
  UNUSED(imode);
	*nerr = 0;

	/* - Activate automatic output message mode. */
	autooutmsg( TRUE );

	/* - Open requested help package. */
	ophelp(ktoken, ktoken_s, &nun, kfilename, nerr);
	if( *nerr != 0 )
		goto L_8888;
	if( nun == NULL )
		goto L_5000;

	/* Get rid of the trailing white-space characters in kfilename */
	for (i = 0; i < MCPFN + 1; i++)
	    if (isspace(kfilename[i]) || kfilename[i] == '\0')
		break;
	if (i < MCPFN + 1)
	    kfilename[i] = '\0';

	/* If this is the PRINTHELP command, print the file and return, else
	   go on and send it to the monitor one line at a time. */
	if ( lprint ) {
	    char kcommand [ MCPFN + 10 ] ;
	    sprintf ( kcommand , "lpr %s" , kfilename ) ;
	    system ( kcommand ) ;
	    goto L_6000 ;
	}

	if (pager) {
	    external_pager(kfilename);
	    goto L_6000;
	} else {
	    if (internal_pager(nun) == 0)
		goto L_6000;
	}

L_5000:
	/* - Process error during read. */
	setmsg( "OUTPUT", 1104 );
	apcmsg( ktoken,ktoken_s );
	outmsg();
        clrmsg();

	if(nun == NULL)
	  return;

L_6000:

	/* - Close help package and return when:
	 *   (1) end-of-file encountered.
	 *   (2) user requests that remainder of file not be printed. */
	zcloses( &nun, &ncerr );

	/* - Deactivate automatic output message mode. */
	autooutmsg( FALSE );

L_8888:
	return;
}

/** 
 * Using internal pager to view a file
 *
 * @param nun
 *	the input file pointer
 *
 * @return
 *	0 upon successful completion, -1 otherwise
 */
int 
internal_pager(FILE *nun) {
    char kmsg[MCMSG+1], kerase[41];
    int nlw = 0, numsave, nlscrn;
    char kresp[9];

    /* - Set the message type */
    typmsg( "OUTPUT" );

    /* - Get screen attributes (number of lines per screen and
     *   text to send to erase screen, if any.) */
    getalphainfo(&nlscrn, kerase, 41);
    if(nlscrn <= 0)
	nlscrn = 23;

    for (;;) {
	if(fgetsp(kmsg, MCMSG + 1, nun)==NULL){
	    if(feof(nun))
		return 0;
	    return -1;
	}
	numsave = strlen(kmsg) - 1;
	if(kmsg[numsave] == '\n')
	    kmsg[numsave] = ' ';

	aplmsg(kmsg,MCMSG + 1);
	nlw = nlw + 1;

	/* -- After a screen full of info, see if user wants to see more. */
	if( nlw > (nlscrn - 2) ){
	    outmsg();
	    clrmsg();
	    setmsg("OUTPUT", 99);
	    zgpmsg("More? $", 8, kresp, 9);

	    upcase(kresp, 1, kresp, 9);

	    if (kresp[0] == 'N' || kresp[0] == 'Q')
		return 0;
	    else
		nlw = 0;
	}
    }
}

/** 
 * Wrapper or portable setenv for those system which it is missing
 *
 * @param name
 *   name of the environment variable
 * @param value
 *   Environment variable value
 * @param overwrite
 *   Replace the value
 *
 */
int
setenv_portable(const char *name,
                const char *value,
                int         overwrite) {
#if defined(HAVE_FUNC_SETENV)

  return setenv(name, value, overwrite);

#elif defined(HAVE_FUNC_PUTENV)
  {
    int len;
    char *string;
    if(getenv(name) == NULL || overwrite != TRUE) {
      len = strlen(name) + strlen(value) + 2;
      string = (char*) malloc(sizeof(char) * len);
      sprintf(string, "%s=%s", name, value);
      string[len] = 0;
      return putenv(string);
    }
    return 0;
  }
#else
   #error "Requirement of either setenv or putenv not met."
#endif /* HAVE_FUNC_SETENV and HAVE_FUNC_PUTENV */

}

/** 
 * Using external pager to view a file
 *
 * @param filename
 *	the input file
 *
 * @return Nothing
 */
void 
external_pager(char *filename) {
    char *syscom;

    syscom = (char *)malloc(strlen(pager) + 1 + strlen(filename) + 1);
    sprintf(syscom, "%s %s", pager, filename);
    setenv_portable("LESS", "isRXM+%", 0);
    if (system(syscom) != 0)
	fprintf(stderr, "Error loading pager %s!\n", pager);
}
