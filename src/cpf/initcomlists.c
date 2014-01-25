/** 
 * @file   initcomlists.c
 * 
 * @brief  Initialize command list blocks
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "comlists.h"
#include "co.h"

#include "errors.h"

#include "string_utils.h"


#include "msg.h"
#include "bot.h"

#define	MCOMLISTS	3

/** 
 * Initialize Command List Blocks.  Read in the commands and their index
 *   and command modules numbers.  Files are names clstd, clsss, clspe
 *   and stored in the SACAUX directory.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR 
 *
 * @note Only called by top/initcommon()
 *
 * @date  920410:  Standardized initialization of kclproj.
 * @date  911107:  Added nerr to call.
 * @date  900804:  Changed from clf to comlist.
 * @date  881122:  Changed command list name of Signal Stacking 
 *                 Subproces to "clsss".
 * @date  870721:  Added reporting of errors occuring in this routine.
 * @date  861201:  Fixed error handing bugs in this routine.
 * @date  860819:  Changed format of command list files.
 * @date  840612:  Changed OPEN statement to call to ZOPEN.
 * @date  831110:  Generalized creation of command list pathnames.
 * @date  810000:  Original version.
 *
 */
void 
initcomlists(int *nerr) {
	char kfile[MCPFN+1];
        char kline[MCMSG+1];
        char *tok;
        int  idx ;
	int i, jcl, jclsub, jclsub_, ncerr, _r;
        FILE *nun;

	static char kclpro[MCOMLISTS][6];
	static int _aini = 1;

	if( _aini ){ /* Do 1 TIME INITIALIZATIONS! */
		{ static char* _itmp0[] = {"clstd","clspe","clsss"};
		for( i = 1, _r = 0; i <= MCOMLISTS; i++ ){
                        fstrncpy( kclpro[i - 1],5,_itmp0[_r],strlen(_itmp0[_r]));
                        _r++ ;
			}
		}
		_aini = 0;
	}

	for( idx = 0 ; idx < MCPFN ; idx++ )
	    kfile[ idx ] = ' ' ;
        kfile[ MCPFN ] = '\0' ;

	*nerr = 0;

	/* - Initialize variables for external command module. */
	cmcomlists.nextcomnames = 0;

	/* - Read in list of standard SAC commands, the module they are in,
	 *   and their index number within that module. */
	jcl = 1;
	for( jclsub = 1; jclsub <= 3; jclsub++ ){
		jclsub_ = jclsub - 1;

		/* -- Create command list pathname and open file. */
		zbasename( kfile,MCPFN+1 );
		crname( kfile,MCPFN+1, KDIRDL, (char*)kclpro[jclsub_]
		 ,6, nerr );

		zopens( &nun, kfile,MCPFN+1, "ROTEXT",7, nerr );
		if( *nerr != 0 )
			goto L_9000;

		/* -- Save starting index number in master list. */
		Icomliststart[jclsub] = jcl;

		/* -- Add command names and locations to master list until EOF. */
L_1000:

                if(fgetsp(kline,MCMSG+1,nun) == NULL) goto L_2000;
         
                if((tok=strtok(kline,".")) == NULL) goto L_9100;
                   Icommodule[jcl] = atol(tok);

                if((tok=strtok(NULL," ")) == NULL) goto L_9100;
                   Icomindex[jcl] = atol(tok);

                if((tok=strtok(NULL," \n")) == NULL) goto L_9100;
                   memset(kmcomlists.kcomnames[jcl-1],' ',8);
                   kmcomlists.kcomnames[jcl-1][8] = '\0';
                   strncpy(kmcomlists.kcomnames[jcl-1],tok,min(8,strlen(tok)));
                   strncpy(kmcomlists.kcomnames_full[jcl-1], tok, min(30,strlen(tok)+1));
                   kmcomlists.kcomnames[jcl-1][min(8,strlen(tok))] = 0;
                   kmcomlists.kcomnames_full[jcl-1][min(30,strlen(tok))] = 0;

		if( jcl < MCOMNAMES ){
			jcl = jcl + 1;
			goto L_1000;
			}
		else{
			*nerr = ERROR_SAC_LOGIC_ERROR;
			setmsg( "ERROR", *nerr );
			apcmsg( "in INITCOMLISTS: table overflow",32 );
			goto L_2000;
			}
L_2000:
		Ncomlistentries[jclsub] = jcl - Icomliststart[jclsub];
		zcloses( &nun, &ncerr );
		}

	/* - Set process command list to standard one. */
	cmcomlists.icomlist = 1;

L_8888:
	if( *nerr != 0 )
		wrtmsg( MUNOUT );
	return;

	/* - Process any errors that may have occurred. */
L_9000:
	*nerr = ERROR_SAC_LOGIC_ERROR;
	setmsg( "ERROR", *nerr );
	apcmsg( "in INITCOMLISTS.",17 );
	aplmsg( "Problems opening Command List:",31 );
	apcmsg( kfile,MCPFN+1 );
	goto L_8888;

L_9100:
	*nerr = ERROR_SAC_LOGIC_ERROR;
	setmsg( "ERROR", *nerr );
	apcmsg( "in INITCOMLISTS.",17 );
	aplmsg( "Problems reading Command List:",31 );
	apcmsg( kfile,MCPFN+1 );
	zcloses( &nun, &ncerr );
	goto L_8888;

}

