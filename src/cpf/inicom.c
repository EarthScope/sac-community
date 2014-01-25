/** 
 * @file   inicom.c
 * 
 * @brief  Initialize Command Block
 * 
 */

#include "cpf.h"
#include "com.h"

/** 
 * Variable initialization of the Command Block
 * 
 * @date   820420:  Added NCERR.
 * @date   810414:  Original version.
 *
 */
void 
inicom() {

	/* cmcom.inumbr = 1; /\* number flag - never changes *\/ */
	/* cmcom.ialpha = 2; /\* alpha flag - never changes *\/ */
	/* cmcom.icont  = 3; /\* Never used *\/ */
	/* cmcom.ncom   = 0; /\* Number of token in command *\/ */
	/* cmcom.jcom   = 0; /\* Current token *\/ */
  /*       kmcom.nkargs = 0; /\* Number of Arguments *\/ */
  /*       kmcom.nkargs_allocated = 0; /\* Number of Arguemnts allocated *\/ */

	cmcom.ncerr  = 0; /* command error */

	return;
}

