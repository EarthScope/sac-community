/** 
 * @file   qapf.c
 * 
 * @brief  Report the pick parameters
 * 
 */

#include "eam.h"

#include "exm.h"

EAM_EXTERN

/** 
 * Report the current alphanumeric pick file parameters
 * 
 * @date   870728:  Original version.
 *
 */
void
qapf() {
    repav("HYPO pick file$", 16, kmeam.kapfnm, MCPFN + 1);
    return;
}
