/** 
 * @file   getclun.c
 * 
 * @brief  Parse "DO"
 * 
 */
#include <stdio.h>
#include <string.h>

#include "mach.h"
#include "cpf.h"
#include "cnd.h"
#include "vars.h"

/** 
 * Parse the action command "DO"
 * 
 * @param nun 
 *    Fortran logical unit number for the file currently being used
 *       to get commands
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   870817:  Original version.
 *
 */
void 
getclun(FILE **nun, 
	int   *nerr) {

	if( strcmp(kmcpf.kvarsname,"macro000") == 0 ){
		*nun = MUNINP;
	}
	else{
		getvFILEptr( kmcpf.kvarsname,9, "fileunit",9, nun, nerr );
	}

	return;
}

