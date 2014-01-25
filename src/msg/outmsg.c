/** 
 * @file   outmsg.c
 * 
 * @brief  Write the Current Message
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "bot.h"
#include "gdm.h"
#include "co.h"
#include "top.h"
#include "bool.h"

/** 
 * Write current output message to the approproate device
 *
 * \return Nothing
 *
 * \bug if else for formatted of message number
 * \bug Lots of repeated code, invert the structure and remove code
 *
 * \date   890106:  Added logic for processed (evaluated) command lines.
 * \date   881230:  Added output to multiple file units.
 * \date   870811:  Added logic to compute end of each text line.
 * \date   860213:  Added output of message number for error messages.
 * \date   860130:  Moved prefix logic from SETMSG to here.
 *             This allows the prefix to be changed before writing.
 * \date   830916:  Original version.
 * \date   881230:  Documented/Reviewed
 *
 */
void 
outmsg()
{
	int jlimsg, jlimsg_, junit, junit_, nc;
        char *s1, *message;

	/* - Add a prefix to first line of message if appropriate.
	 *   (There is an ASCII BEL embedded in the error prefix.) */
	nc = max( 1, indexb( (char*)kmmsg.klimsg[0],MCMSG+1 ) );
        s1 = (char *)malloc(nc+1);
        message = (char *)malloc(nc+100+1); 
        strncpy(s1,kmmsg.klimsg[0],nc); s1[nc] ='\0';
	if( cmmsg.itpmsg == MERRORS ){
		if( cmmsg.nummsg >= 1000 ){
			for( junit = 1; junit <= cmmsg.nunits; junit++ ){
				junit_ = junit - 1;
				if( cmmsg.lsend[junit_][cmmsg.itpmsg - 1] ){

                                        sprintf(message,"%s%4d%s%s",
					 " ERROR ", cmmsg.nummsg, ": ", s1);
                                        bell();
                                          fprintf(cmmsg.iunits[junit_],"%s\n", message);
					}
				}
			}
		else{
			for( junit = 1; junit <= cmmsg.nunits; junit++ ){
				junit_ = junit - 1;
				if( cmmsg.lsend[junit_][cmmsg.itpmsg - 1] ){

                                        sprintf(message,"%s %3d%s%s",
					 " ERROR ", cmmsg.nummsg, ": ", s1);

                                        bell();
                                          fprintf(cmmsg.iunits[junit_],"%s\n",  message);
					}
				}
			}
		}
	else if( cmmsg.itpmsg == MWARNINGS ){
		for( junit = 1; junit <= cmmsg.nunits; junit++ ){
			junit_ = junit - 1;
			if( cmmsg.lsend[junit_][cmmsg.itpmsg - 1] ){

                                sprintf(message,"%s%s", " WARNING: ", s1 );
                                if( cmgdm.lgui ){
                                  ;
				}else
                                  fprintf(cmmsg.iunits[junit_],"%s\n", message );
				}
			}
		}
	else if( cmmsg.itpmsg == MPROCESSED ){
		for( junit = 1; junit <= cmmsg.nunits; junit++ ){
			junit_ = junit - 1;
			if( cmmsg.lsend[junit_][cmmsg.itpmsg - 1] ){

                                sprintf(message,"%s%s", " ==> ", s1 );
                                if( cmgdm.lgui ){
                                  ;
				}else
                                  fprintf(cmmsg.iunits[junit_],"%s\n", message );
				}
			}
		}
	else{
		for( junit = 1; junit <= cmmsg.nunits; junit++ ){
			junit_ = junit - 1;
			if( cmmsg.lsend[junit_][cmmsg.itpmsg - 1] ){
                                if( cmgdm.lgui ){
                                  ;
                                }else
                                  fprintf(cmmsg.iunits[junit_]," %s\n", s1);
				}
			}
		}

        free(s1);
        free(message);

	/* - Write remaining lines of current message. */

	for( jlimsg = 2; jlimsg <= cmmsg.nlimsg; jlimsg++ ){
		jlimsg_ = jlimsg - 1;
		nc = max( 1, indexb( (char*)kmmsg.klimsg[jlimsg_],MCMSG+1 ) );
                s1 = (char *)malloc(nc+1);
                strncpy(s1,kmmsg.klimsg[jlimsg_],nc);
                s1[nc] = '\0';
		for( junit = 1; junit <= cmmsg.nunits; junit++ ){
			junit_ = junit - 1;
			if( cmmsg.lsend[junit_][cmmsg.itpmsg - 1] ){
                                if( cmgdm.lgui ){
                                  ;
				}else
                                  fprintf(cmmsg.iunits[junit_]," %s\n", s1);
                                  fflush(cmmsg.iunits[junit_]);
				}
			}
                free(s1);
	}

	return;
}


static int use_bell = TRUE;
void bell_off() { use_bell = FALSE; }
void bell_on()  { use_bell = TRUE; }

void
bell() {
  if(use_bell) {
    fprintf(stdout, "%c", 0x07); /* Annoying Bell */
  }
}




