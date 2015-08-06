/** 
 * @file   initok.c
 * 
 * @brief  Initialize token module
 * 
 */

#include "ucf.h"
#include "tok.h"


struct t_kmtok kmtok;
struct t_cmtok cmtok;

/** 
 * Initialize token module
 * 
 * @date   841011:  Original version.
 *
 */
void
initok() {

    cmtok.ntokdl = 0;
    cmtok.nmsgdl = 0;

}
