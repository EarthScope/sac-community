/** 
 * @file   qpicks.c
 * 
 * @brief  Report PICKS parameters
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "eam.h"
#include "gam.h"
#include "exm.h"
#include "msg.h"

/** 
 * Report the current values of the PICKS parameters
 * 
 * @date   820921:  Original version (from QDISPL).
 *
 */
void
qpicks() {

    int j, j_;

    mprint("   PICK display is %s", ON_OFF(cmgam.ldsppk));
    repav("Type of each pick display$", 27, "        ", 9);
    for (j = 1; j <= MPKNAM; j++) {
        char cattemp[6];
        cattemp[0] = 0;
        j_ = j - 1;
        strcpy(cattemp, "  ");
        strncat(cattemp, kmgam.kpknam[j_], 2);
        strcat(cattemp, "$");
        repav(cattemp, 2 + 2 + 1 + 1,
              (char *) kmgam.kpktyp[cmgam.ipktyp[j - 1] - 1], 9);
    }
    mprint("   WIDTH of pick lines is " REPORT_FLOAT, cmgam.pkwdth);
    mprint("   HEIGHT of pick lines is " REPORT_FLOAT, cmgam.pkhgth);

    return;
}
