/** 
 * @file   reperr.c
 * 
 * @brief  Report current SAC error
 * 
 */

#include <string.h>

#include "exm.h"
#include "bbs.h"
#include "ucf.h"
#include "vars.h"

/** 
 * Report the current SAC error condition. This report is sent
 *   to the global variable file
 * 
 * @param nerr 
 *    Error Number
 *
 * @date   870301:  Modifications due to new global variable structure.
 * @date   860206:  Original version.
 *
 */
void 
reperr(int nerr) {

	/* - Send SAC error status to global variable file. */
	/* - Also send the error number. */
  setbb("sacerror", VAR_STRING, (nerr) ? "TRUE" : "FALSE");
  setbb("numerror", VAR_INTEGER, nerr);
	return;
}

