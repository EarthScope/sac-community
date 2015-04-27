/** 
 * @file   xboec.c
 * 
 * @brief  Execute BOEC
 * 
 */

#include <string.h>

#include "amf.h"
#include "bom.h"
#include "exm.h"
#include "cpf.h"

/** 
 * Execute the parameter setting command "BOEC".  This command controls
 *   certain error conditions that can occur during binary operations.
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   810723:  Original version.
 *
 */
void
xboec(int *nerr) {

    int index;

    *nerr = 0;

  L_1000:
    if (lcmore(nerr)) {

        /* -- "NPTS condition":  
           set control for unequal number of data points. */
        if (lklist("N$", 3, (char *) kmexm.kectp, 9, cmexm.nectp, &index)) {
            strcpy(kmbom.kecnpt, kmexm.kectp[index - 1]);

        }
        /* "DELTA condition":  
           set control for sampling interval mismatch. */
        else if (lklist("D$", 3, (char *) kmexm.kectp, 9, cmexm.nectp, &index)) {
            strcpy(kmbom.kecdel, kmexm.kectp[index - 1]);

        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    return;
}

#include "array.h"

static sac **sac_binary_file_list = NULL;

sac *sacread(char *file);

sac *
bflget(string_list * list, int i) {
    sac *s;

    if (!sac_binary_file_list) {
        sac_binary_file_list = xarray_new('p');
    }

    if (i >= (int) xarray_length(sac_binary_file_list)) {
        /* Read in File */
        if (i >= string_list_length(list)) {
            return NULL;
        }
        if (!(s = sacread(string_list_get(list, i)))) {
            return NULL;
        }
        sac_binary_file_list = xarray_append(sac_binary_file_list, s);
    }
    s = sac_binary_file_list[i];

    return s;
}

void
bflclear() {

    if (sac_binary_file_list) {
        xarray_free(sac_binary_file_list);
    }
    sac_binary_file_list = NULL;
}
