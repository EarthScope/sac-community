/** 
 * @file   xdivf.c
 * 
 * @brief  Execute DIVF
 * 
 */

#include <string.h>
#include <math.h>

#include "amf.h"
#include "bom.h"
#include "dfm.h"
#include "hdr.h"
#include "cpf.h"
#include "dff.h"
#include "bool.h"
#include "msg.h"
#include "ucf.h"
#include "co.h"

#include "errors.h"


#include "clf.h"

/** 
 * Execute the action command "DIVF". This command divides a set of 
 *     files into data in memory.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OPERATION_ON_SPECTRAL_FILE
 *    - ERROR_OPERATION_ON_UNEVEN_FILE
 *    - ERROR_HEADER_FILE_MISMATCH
 *
 * @date   881130:  Fixed bug in begin time error checking.
 * @date   850730:  Changes due to new memory manager.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Original version.
 *
 */
