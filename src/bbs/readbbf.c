/** 
 * @file   readbbf.c
 * 
 * @brief  Read a Blackboard variable file
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "amf.h"
#include "bbs.h"
#include "vars.h"
#include "dff.h"
#include "co.h"

/** 
 * Read a Blackboard variable file into a program
 * 
 * @param kname 
 *    Name of the file to read
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 * @param kname_s 
 *   Length of \p kname
 *
 * @date   890309:  Changed from call to readvfile to getvlist.
 * @date   871012:  Original version.
 * @date   871012:  Documented / Reviewed
 *
 */
void 
readbbf(char *kname, 
	int  *nerr, 
	int   kname_s) {

	char *kname_c;
  int notused;
	kname_c = fstrdup(kname, kname_s);

	/* - Initialize blackboard store and sacmem array if needed. */
	if( strcmp(kmbbs.kbbsinit,"INITDONE") != 0 ){
		initializevars();
		inibbs();
		inivars () ;
		strcpy ( kmvars.varsidcode , "VARS" ) ;
	}

	/* - Read file from disk using the blackboard reserved name. */

	strcpy( kmbbs.knmbbs, kname_c);

  readvfile(kmbbs.knmbbs, MCPFN+1, &notused, nerr);

	free(kname_c);

	return;
}


/* Added for FORTRAN friendliness */
void readbbf_ (char      *kname, 
	       int       *nerr, 
	       int        kname_s) {
  readbbf(kname, nerr, kname_s) ;
}
void readbbf__ (char      *kname, 
		int       *nerr, 
		int        kname_s) {
  readbbf(kname, nerr, kname_s) ;
}
