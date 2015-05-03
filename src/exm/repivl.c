/** 
 * @file   repivl.c
 * 
 * @brief  Report an integer array
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "bot.h"
#include "msg.h"
#include "co.h"

/** 
 * Report the value of an integer array
 * 
 * @param ktext 
 *    Variable Name
 * @param ktext_s 
 *    Length of \p ktext
 * @param iv 
 *    Integer array
 * @param nv 
 *    Length of \p iv
 *
 * @date   890104:  Changed from direct terminal output to message subsystem.
 * @date   870728:  Original version.
 *
 */
void
repivl(char *ktext, int *iv, int nv) {

    char kline[MCMSG + 1];
    int j, j1, j2, jlines, nctext, nlines, n;
    char *strtemp;
    memset(kline, 0, sizeof(kline));

    int *const Iv = &iv[0] - 1;

    /* - Write text and values of array to message subsystem.. */
    strcpy(kline, "   ");
    strcat(kline, ktext);

    aplmsg(kline, MCMSG + 1);
    nlines = (nv - 1) / 5 + 1;
    j1 = 1;
    for (jlines = 1; jlines <= nlines; jlines++) {
        j2 = min(j1 + 4, nv);
        strcpy(kline, "           ");
        n = strlen(kline);
        for (j = j1; j <= j2; j++) {
            n += sprintf(&kline[n], "%5d", Iv[j]);
        }
        aplmsg(kline, MCMSG + 1);
        j1 = j1 + 5;
    }

    return;
}
