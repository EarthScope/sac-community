/** 
 * @file   writebbf.c
 * 
 * @brief  Write a Blackboard File
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "bbs.h"
#include "dff.h"
#include "vars.h"

/** 
 * Write a Blackboard variable file from a program
 * 
 * @param kname 
 *    Name of file to write
 *    Set to blanks to use previous blackkboard file
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 * @param kname_s 
 *   Length of \p kname
 *
 * @date   871012:  Original version.
 * @date   871012:  Documented / Reveiwed
 *
 */
void 
writebbf(char *kname, 
	 int  *nerr, 
	 int   kname_s) {

	char *kname_c;

	kname_c = fstrdup(kname, kname_s);

	/* - Write file to disk using the blackboard reserved name. */
	writevfile( kmbbs.knmbbs,MCPFN+1, kname_c, nerr );

	free(kname_c);

	return;
}


/* Added for FORTRAN friendliness */
void writebbf_ (char      *kname, 
		int       *nerr, 
		int        kname_s) {
  writebbf(kname, nerr, kname_s) ;
}
void writebbf__ (char      *kname, 
		 int       *nerr, 
		 int        kname_s) {
  writebbf(kname, nerr, kname_s) ;
}
