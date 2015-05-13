/** 
 * @file   kadate.c
 * 
 * @brief  Stingify a date
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "ucf.h"
#include "bot.h"

#include "msg.h"
#include "co.h"

/** 
 * Convert a year/day_of_year int a string of form MMM DD (JJJ), YYYY
 * 
 * @param iyear 
 *    Input Year
 * @param ijday 
 *    Input day of the year
 * @param ncdate 
 *    Maximum length of output string, must be at least 18
 * @param kkdate 
 *    Output string
 * @param kkdate_s 
 *    Length of \p kkdate on output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 * 
 * @date   811019:  Rewrote using F77 character string maninpulations.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void
kadate(int iyear, int ijday, int ncdate, char *kkdate, int kkdate_s, int *nerr) {

    int iday, imonth;
    static char kmonth[12][5] = { "JAN ", "FEB ", "MAR ", "APR ", "MAY ",
        "JUN ", "JUL ", "AUG ", "SEP ", "OCT ", "NOV ", "DEC "
    };

    *nerr = 0;

    /* - Make sure output character variable is long enough. */
    if (ncdate < 18) {
        *nerr = 906;
        setmsg("ERROR", *nerr);
        subscpy(kkdate, 0, ncdate - 1, kkdate_s - 1, "********");
        goto L_8888;
    }

    /* - Convert from julian day to month and day. */
    kidate(iyear, ijday, &imonth, &iday, nerr);

    /* - Return if input fields were bad. */
    if (*nerr != 0) {
        subscpy(kkdate, 0, ncdate - 1, kkdate_s - 1, "********");
        goto L_8888;
    }

    snprintf(kkdate, kkdate_s, "%3s%02d (%03d), %4d", kmonth[imonth-1], iday, ijday, iyear);

  L_8888:

    return;
}
