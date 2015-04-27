/** 
 * @file   zgwindowsize.c
 * 
 * @brief  Get the window size
 * 
 */

#include "co.h"

#include "config.h"

#ifndef WIN_APP
#if GWINSZ_IN_SYS_IOCTL
# include <sys/ioctl.h>
#else
# include <termios.h>
#endif

/* Hmmmrph */
#include <sys/ioctl.h>
#endif
/** 
 * Get the current terminal window size
 * 
 * @param number_rows 
 *    Number of rows
 * @param number_columns 
 *    Numer of columns
 * @param error_flag 
 *    Error Return Flag
 *    - 0 on Success
 *    - 1 on Error
 *
 * @bug Function name ends in an underscore, fortran cruft.
 *
 */
void
zgwindowsize_(int *number_rows, int *number_columns, int *error_flag) {
#ifndef WIN_APP
    struct winsize ws;

    if ((*error_flag = ioctl(fileno(stdin), TIOCGWINSZ, &ws)) == 0) {
        *number_rows = (int) ws.ws_row;
        *number_columns = (int) ws.ws_col;
    } else
        *error_flag = 1;
    return;
#else
    *number_rows = 1000000;
    *number_columns = 80;
    *error_flag = 0;
#endif
}
