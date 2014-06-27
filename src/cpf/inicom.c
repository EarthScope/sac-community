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

	cmcom.ncerr  = 0; /* command error */

	return;
}

