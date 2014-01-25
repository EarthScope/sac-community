/** 
 * @file   zclosec.c
 * 
 * @brief  Close a file
 * 
 */
#include <unistd.h>

#include "co.h"

/** 
 * Close a 'C' file descriptor
 * 
 * @param pfd 
 *   C file descriptor, is negative.
 *
 * @note SAC carries around FORTRAN logical unit numbers, but it
 *       doesn't currently have hooks for "C" file descriptors.
 *       Therefore, the "C" descriptors are carried in the same
 *       variable as the logical unit numbers.  The file descriptors
 *       are negated to distinguish them from logical unit numbers.
 *
 * @date 12/16/85  Tested--D. Trimmer
 * @date 01/15/88  Changed name from zclosc to zclosec--J. Tull 
 *
 * @bug Only called by co/zclose()
 *
 */
void
zclosec(int *pfd) {
  close(-(*pfd));
  return;
}
