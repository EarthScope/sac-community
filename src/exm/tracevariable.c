/** 
 * @file   tracevariable.c
 * 
 * @brief  Control tracing of a variable
 * 
 */

#include <string.h>

#include "exm.h"
#include "msg.h"
#include "dff.h"
#include "bbs.h"


#include "co.h"

/** 
 * Control the tracing of a blackboard or header variable
 * 
 * @param activate 
 *    - TRUE to activate tracing of a variabel
 *    - FALSE to deactivate tracing of a variable
 * @param blackboard 
 *    - TRUE of the varaible is a blackboard variable
 *    - FALSE of the varaible is a header variable
 * @param variable 
 *    Variable name
 *       If this is the name of a header variable then
 *       it must be of the form: "file,name" where "file"
 *       is the data file list name or number and "name"
 *       is the SAC header variable name. 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   881230:  Original version.
 *
 */
void 
tracevariable(int   activate, 
              int   blackboard, 
              char *variable, 
              int  *nerr) {

	int j, j_, jtrace, jtrace_, ngerr, ntracesav;

	*nerr = 0;

	ntracesav = cmexm.ntraces;
	if( activate ){
		jtrace = 1;
L_100:
		if( memcmp(variable,cmexm.ktracename[jtrace - 1],
                    min(strlen(variable),strlen(cmexm.ktracename[jtrace - 1]))) == 0 ){
			}
		else if( jtrace < cmexm.ntraces ){
			jtrace = jtrace + 1;
			goto L_100;
			}
		else if( cmexm.ntraces < MTRACES ){
			cmexm.ntraces = cmexm.ntraces + 1;
			jtrace = cmexm.ntraces;
			fstrncpy( cmexm.ktracename[jtrace - 1], 16,  variable, strlen(variable));
			Lblackboard[jtrace] = blackboard;
			if( Lblackboard[jtrace] ){
				getbbv( (char*)cmexm.ktracename[jtrace - 1], (char*)cmexm.ktracevalue[jtrace - 1]
				 , &ngerr, 16, MCMSG );
				}
			else{
				gethv( (char*)cmexm.ktracename[jtrace - 1],17, (char*)cmexm.ktracevalue[jtrace - 1]
				 ,MCMSG+1, &ngerr );
				}
			setmsg( "OUTPUT", 99 );
			apcmsg( "TRACE  (on)",12 );
			apcmsg( (char*)cmexm.ktracename[jtrace - 1],17 );
			apcmsg( "=",2 );
			apcmsg( (char*)cmexm.ktracevalue[jtrace - 1],MCMSG+1 );
			outmsg();
			clrmsg();
			}
		else{
			*nerr = 1119;
			setmsg( "ERROR", *nerr );
			apimsg( MTRACES );
			goto L_8888;
			}
		}
	else{
		for( jtrace = 1; jtrace <= cmexm.ntraces; jtrace++ ){
			jtrace_ = jtrace - 1;
			if( memcmp(variable,cmexm.ktracename[jtrace_],
                            min(strlen(variable),strlen(cmexm.ktracename[jtrace_]))) == 0 ){
				if( Lblackboard[jtrace] ){
					getbbv( (char*)cmexm.ktracename[jtrace_], (char*)cmexm.ktracevalue[jtrace_]
					 , &ngerr, 16, MCMSG );
					}
				else{
					gethv( (char*)cmexm.ktracename[jtrace_],17, (char*)cmexm.ktracevalue[jtrace_]
					 ,MCMSG+1, &ngerr );
					}
				setmsg( "OUTPUT", 99 );
				apcmsg( "TRACE (off)",12 );
				apcmsg( (char*)cmexm.ktracename[jtrace_],17 );
				apcmsg( "=",2 );
				apcmsg( (char*)cmexm.ktracevalue[jtrace_],MCMSG+1 );
				outmsg();
				clrmsg();
				ntracesav = cmexm.ntraces - 1;
				for( j = jtrace; j <= ntracesav; j++ ){
					j_ = j - 1;
					Lblackboard[j] = Lblackboard[j + 1];
					strcpy( cmexm.ktracename[j_], cmexm.ktracename[j_ + 1]
					  );
					strcpy( cmexm.ktracevalue[j_], cmexm.ktracevalue[j_ + 1]
					  );
					}
				goto L_8888;
				}
			}
		}

L_8888:
	cmexm.ntraces = ntracesav;
	return;

} /* end of function */

