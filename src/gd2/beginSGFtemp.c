
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gam.h"
#include "gd2.h"
#include "gdm.h"
#include "gem.h"
#include "gpm.h"
#include "bool.h"

void 
beginSGFtemp(int *nerr)
{
        /*=====================================================================
         * PURPOSE: To begin plotting to a list of graphics devices.
         *=====================================================================
         * OUTPUT ARGUMENTS:
         *    nerr:    Error flag. Set to 0 if no error occurred.
         *=====================================================================
         * MODIFICATION HISTORY:
         *    981123:  Original version.
         */
        *nerr = 0;

    if(kmgd2.kfilename[0] == '\0') {
        char *tmp;
        tmp = tmpfile_create("/tmp/sac_temp_XXXXXX.sgf", 4);
        if(tmp) {
            strcpy(kmgd2.kfilename, tmp);
            free(tmp);
            tmp = NULL;
        } else {
            memset(kmgd2.kfilename, 0, MCPFN+1);
        }
    }    
}

void
init_print_device_SGF(print_device_begin_t *begin,
                      print_device_end_t   *end) {
  *begin = beginSGFtemp;
  *end   = endSGFtemp;
}
