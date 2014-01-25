/** 
 * @file   sacmsg.c
 * 
 * @brief  Read the SAC message file
 * 
 */

#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "co.h"
#include "bot.h"

#include "string_utils.h"

/** 
 * Read the SAC message file from disk and save
 * 
 * \param *nerr
 *   Error return flag
 *   - 0 on Success
 *   - Non-Zero on Error
 *
 * \return Nothing
 *
 * \see zbasename crname zopens setmsg apimsg apcmsgnum apcmsg zcloses
 * \see t_cmmsg.nfmsg
 * \see t_cmmsg.ifmsg
 * \see t_kmmsg.kfmsg
 * 
 * \date   870923:  Deleted ".saf" suffixes from aux files.
 * \date   870527:  Reworked file reading logic to make it compatible
 *                   with the current version of the MASSCOMP f77 compiler.
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void 
sacmsg(int *nerr)
{
	char kfile[MCPFN+1], kiline[MCMSG+1];
	int  idx ;
	int ioerr, ntused, numsave;
        FILE *nun;

	/* - Build the pathname and open the file containing output messages. */
	ioerr = 0;
        /*memset(kfile,' ',MCPFN);*/
        for( idx = 0 ; idx < MCPFN ; idx++ )
	    kfile[ idx ] = ' ' ;
        kfile[MCPFN] = '\0';

	zbasename( kfile,MCPFN+1 );
	crname( kfile,MCPFN+1, KDIRDL, "messages",9, nerr );
	if( *nerr != 0 )
		goto L_4000;
	zopens( &nun, kfile,MCPFN+1, "ROTEXT",7, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Read each message from disk file into common. */

L_2000:
	if( cmmsg.nfmsg < MFMSG ){
		cmmsg.nfmsg = cmmsg.nfmsg + 1;

                if(fgetsp(kiline,MCMSG,nun)==NULL){
                  if(feof(nun)) goto L_2020;
                  goto L_2010;
		}
                if(kiline[(numsave=strlen(kiline)-1)] == '\n') kiline[numsave] = ' ';

                if(sscanf(kiline,"%4d",  &Ifmsg[cmmsg.nfmsg]) != 1){
                  printf("error reading SAC message file-sacmsg\n");
                  goto L_2010;
	        }
                strcpy( kmmsg.kfmsg[cmmsg.nfmsg - 1],kiline+5);

		goto L_2000;
L_2010:
		*nerr = 100;
		setmsg( "ERROR", *nerr );
		apimsg( ioerr );
		apcmsgnum( 114 );
		apcmsg( kfile,MCPFN+1 );
L_2020:
		cmmsg.nfmsg = cmmsg.nfmsg - 1;
		}
	else{
		*nerr = 919;
		setmsg( "ERROR", *nerr );
		}

L_4000:
	zcloses( &nun, &ntused );

L_8888:
	return;

}

