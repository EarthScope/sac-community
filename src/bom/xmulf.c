/** 
 * @file   xmulf.c
 * 
 * @brief  Execute MULF
 * 
 */

#include <string.h>
#include <math.h>

#include "amf.h"
#include "bom.h"
#include "bool.h"
#include "dfm.h"
#include "hdr.h"
#include "cpf.h"
#include "dff.h"
#include "co.h"
#include "ucf.h"

#include "errors.h"


#include "msg.h"
#include "clf.h"
#include "exm.h"

/** 
 * Execute the MULF command. This command multiplies a set of files 
 *    by data in memory.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_HEADER_FILE_MISMATCH
 *    - ERROR_OPERATION_ON_UNEVEN_FILE
 *    - ERROR_OPERATION_ON_SPECTRAL_FILE
 *
 * @date   920331:  Added msg about BINOPEER cmd when header delta differs.
 * @date   881130:  Fixed bug in begin time error checking.
 * @date   850730:  Changes due to new memory manager.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Original version.
 *
 */
