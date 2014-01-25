/** 
 * @file   getwfdiscs.c
 * 
 * @brief  
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "clf.h"
#include "msg.h"
#include "bool.h"

#include "errors.h"

int
string_ends_with(char *str, char *suffix) {
    size_t n, m;
    n = strlen(str);
    m = strlen(suffix);
    if(n >= m) {
        return(strncmp(str + n - m, suffix, m) == 0);
    }
    return FALSE;
}

/** 
 * Find the wfdisc filenames in the input file list and return them
 *    along with the number found.  Remove the wfdisc filenames from 
 *    the input list
 * 
 * @param wflistout 
 *    wfdisc filenames returned
 * @param nwfout 
 *    Number of files in \p wflistout
 * @param filesin 
 *    Input files list
 * @param nfilesin 
 *    Total number of input files in \p filesin
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OUT_OF_MEMORY
 *    - ERROR_WFDISC_FILE_REQUIRES_A_PERIOD
 *    - ERROR_NO_WFDISC_FILE_SPECIFIED
 *
 */
string_list *
getwfdiscs(string_list *list,
           int         *nerr) {

    int   i;
    char *file;
    string_list *wfdiscs, *files;
    int   lnoDot;

    *nerr = 0;
    lnoDot = FALSE;
    wfdiscs = string_list_init();
    files   = string_list_init();
    
    for(i = 0; i < string_list_length(list); i++) {
        file = string_list_get(list, i);
        if( string_ends_with(file, ".wfdisc") ) {
            string_list_put(wfdiscs, file, strlen(file));
        }else{
            /* if the user denoted the '.' in ".wfdisc" with a wildcard,
             * it's a problem, alert the user. */
            if(string_ends_with(file, "*wfdisc") ||
               string_ends_with(file, "?wfdisc")) {
                warning(ERROR_WFDISC_FILE_REQUIRES_A_PERIOD,
                        "\n          The token %s is not treaded as a .wfdisc file", file);
                lnoDot = TRUE ;
            }
            string_list_put(files, file, strlen(file));
        }
    }
    
    if( string_list_length(wfdiscs) <= 0) {
        if ( lnoDot ) {
            *nerr = ERROR_WFDISC_FILE_REQUIRES_A_PERIOD ;
        } else {		
            *nerr = ERROR_NO_WFDISC_FILE_SPECIFIED;
        }
    }
    string_list_clear(list);
    string_list_extend(list, files);
    string_list_free(files);
    files = NULL;
    return wfdiscs;
}
