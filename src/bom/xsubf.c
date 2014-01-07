/** 
 * @file   xsubf.c
 * 
 * @brief  Execute SUBF
 * 
 */

#include <string.h>
#include <math.h>

#include "amf.h"
#include "bom.h"
#include "dfm.h"
#include "hdr.h"
#include "dff.h"
#include "cpf.h"
#include "bool.h"
#include "ucf.h"
#include "msg.h"
#include "co.h"

#include "errors.h"


#include "clf.h"

/** 
 * Execute the SUBF command.  This command subtracts a set of files 
 *    from data in memory.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_HEADER_FILE_MISMATCH
 *    - ERROR_OPERATION_ON_UNEVEN_FILE
 *    - ERROR_OPERATION_ON_SPECTRAL_FILE
 *
 * @date   881130:  Fixed bug in begin time error checking.
 * @date   850730:  Changes due to new memory manager.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Original version.
 */
