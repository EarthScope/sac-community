
#include "co.h"
#include "gdm.h"

#include "debug.h"

GDM_EXTERN

/** 
 * Get current graphics text justification
 *
 * @param khorz
 *    Horizontal text justificiation
 *    - 'LEFT' for left justification.
 *    - 'CENTER' for centered justification.
 *    - 'RIGHT' for right justification.
 * @param khorz_s
 *    Length of \p khorz and \p kvert
 * @param kvert
 *    Vertical text justificiation
 *    - 'BOTTOM' for bottom justification.
 *    - 'CENTER' for centered justification.
 *    - 'TOP' for TOP justification.
 *
 * @date   861017:  Original version.
 *
 */
void
gettextjust(int *horz, int *vert) {
    if (cmgdm.ihjust == 1) {
        *horz = LEFT;
    } else if (cmgdm.ihjust == 2) {
        *horz = CENTER;
    } else {
        *horz = RIGHT;
    }

    if (cmgdm.ivjust == 1) {
        *vert = BOTTOM;
    } else if (cmgdm.ivjust == 2) {
        *vert = CENTER;
    } else {
        *vert = TOP;
    }

}
