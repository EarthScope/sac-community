/** 
 * @file   tracereport.c
 * 
 * @brief  Sedn a variable tracing report
 * 
 */

#include <string.h>

#include "exm.h"
#include "msg.h"
#include "bbs.h"
#include "dff.h"
#include "co.h"

/** 
 * Send a variable tracing report to the message subsystem
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   881230:  Original version.
 *
 */
void 
tracereport(int *nerr) {

	char currentvalue[MCMSG+1];
	int jtrace, jtrace_, ngerr;

	*nerr = 0;

	for( jtrace = 1; jtrace <= cmexm.ntraces; jtrace++ ){
		jtrace_ = jtrace - 1;
		if( Lblackboard[jtrace] ){
                  getbbv( (char*)cmexm.ktracename[jtrace_], currentvalue, &ngerr, 16, MCMSG );
                }
		else{
                  gethv( (char*)cmexm.ktracename[jtrace_],17, currentvalue, MCMSG+1, &ngerr );
                }
		if( memcmp(currentvalue,cmexm.ktracevalue[jtrace_],min(strlen(currentvalue),strlen(cmexm.ktracevalue[jtrace_]))) != 0 ){
                  strcpy( cmexm.ktracevalue[jtrace_], currentvalue );
                  setmsg( "OUTPUT", 99 );
                  apcmsg( "TRACE (mod)",12 );
                  apcmsg( (char*)cmexm.ktracename[jtrace_],17 );
                  apcmsg( "=",2 );
                  apcmsg( (char*)cmexm.ktracevalue[jtrace_],MCMSG+1 );
                  outmsg();
                  clrmsg();
                }
        }
        
	return;
}

