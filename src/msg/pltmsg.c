/** 
 * @file   pltmsg.c
 * 
 * @brief  Write current message to the active graphics device
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "gem.h"
#include "bot.h"
#include "pl.h"


GEM_EXTERN

MSG_EXTERN

/** 
 * Write current output message to the active graphics device
 *
 * \param *xloc
 *    X plot location for beginning of text
 * \param *yloc
 *    Y plot location for beginning of text
 *
 * \return Nothing
 *
 * \bug Message types referred to as actual numbers, not names
 * \bug Could probably get away with a sprint here
 *
 * \see pltext
 *
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void
pltmsg(float *xloc, float *yloc) {
    int j, j_;
    float ytemp;
    char cattemp[1024];

    /* - Add a prefix to first line of message if appropriate.
     *   (There is an ASCII BEL embedded in the error prefix.) */
    if (cmmsg.itpmsg == 1) {
        snprintf(cattemp, sizeof(cattemp), "\aERROR: %s", kmmsg.klimsg[0]);
    } else if (cmmsg.itpmsg == 2) {
        snprintf(cattemp, sizeof(cattemp), "WARNING: %s", kmmsg.klimsg[0]);
    } else {
        snprintf(cattemp, sizeof(cattemp), "%s", kmmsg.klimsg[0]);
    }
    rstrip(cattemp);
    pltext(cattemp, *xloc, *yloc);
    /* - Write remaining lines of current message. */

    ytemp = *yloc;
    for (j = 2; j <= cmmsg.nlimsg; j++) {
        j_ = j - 1;
        ytemp = ytemp - cmgem.chht;
        snprintf(cattemp, sizeof(cattemp), "%s", kmmsg.klimsg[j_]);
        rstrip(cattemp);
        pltext(cattemp, *xloc, ytemp);
    }

    return;
}
