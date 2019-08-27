/** 
 * @file   formmarker.c
 * 
 * @brief  Format a time marker
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "bool.h"
#include "hdr.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"
#include "co.h"
#include "debug.h"

/** 
 * Format a time marker
 * 
 * @param time 
 *    Time of the marker
 * @param type 
 *    Type of marker
 * @param type_s 
 *     Length of \p type
 * @param output 
 *     Formatted output
 * @param output_s 
 *     Length of \p output
 * @param lok 
 *     - TRUE if value is defined
 *     - FALSE if value is not defined
 *
 * @date   870305:  Original version.
 *
 */
void
formmarker(double time, char *type, int type_s, char *output, int output_s,
           int *lok) {

    char tmp[10];
    UNUSED(type_s);
    if (time != SAC_FLOAT_UNDEFINED) {
        sprintf(output, float_format_with_default("%16.5g"), time);
        ljust(output, output_s);
        *lok = TRUE;
        if (strcmp(type, SAC_CHAR_UNDEFINED) != 0) {
            strcpy(tmp, type);
            rstrip(tmp);
            sprintf(output, "%s (%s)",output, tmp);
        }
    } else {
        strcpy(output, "UNDEFINED");
        *lok = FALSE;
    }

    return;
}
