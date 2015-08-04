
#include "gdm.h"

/** 
 * Change the text justification
 *
 * @param khorz
 *    Horizontal Justification
 *     - 'LEFT' for left justification.
 *     - 'CENTER' for centered justification.
 *     - 'RIGHT' for right justification.
 *     -  (Only the first character need be entered.)
 * @param kvert
 *    Vertical Justification
 *     - 'BOTTOM' for bottom justification.
 *     - 'CENTER' for centered justification.
 *     - 'TOP' for TOP justification.
 *     -  (Only the first character need be entered.)
 *
 *    861017:  Original version.
 *
 */
void
settextjust(int horz, int vert) {
    /* - Only check first character, but check for upper and lower case. */
    /* - Errors result in default (LEFT, BOTTOM). */
    if (horz == LEFT) {
        cmgdm.ihjust = 1;
    } else if (horz == CENTER) {
        cmgdm.ihjust = 2;
    } else if (horz == RIGHT) {
        cmgdm.ihjust = 3;
    } else {
        cmgdm.ihjust = 1;
    }

    if (vert == BOTTOM) {
        cmgdm.ivjust = 1;
    } else if (vert == CENTER) {
        cmgdm.ivjust = 2;
    } else if (vert == TOP) {
        cmgdm.ivjust = 3;
    } else {
        cmgdm.ivjust = 1;
    }
}
