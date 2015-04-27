/** 
 * @file   lfilesok.c
 * 
 * @brief  Test for the existance of data files 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "config.h"

#include "amf.h"
#include "co.h"
#include "clf.h"
#include "dff.h"
#include "ncpf.h"

#ifdef HAVE_LIBRPC
#include <rpc/rpc.h>
#endif /* HAVE_LIBRPC */

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"

#include "debug.h"

/** 
 * Test for the existence of data files to be read
 * 
 * @param kfiles 
 *    List of files to be read
 * @param kfiles_s 
 *    Length of \p kfiles
 * @param nfiles 
 *    Number of files in \p kfiles
 * @param kdir 
 *    Default directory to read from
 * @param kdir_s 
 *    Length of \p kdir
 * @param lmore 
 *    - TRUE if the files are to be appended to the current list
 *    - FALSE if the list is to be truncated first
 * @param lheader 
 *    - TRUE if reading the header
 *    - FALSE of just opening the file
 * @param lxdr 
 *    - TRUE - If the list of files are xdr format
 *    - FALSE - If the list of files are not xdr format
 * @param nerr 
 *    - Error Return Flag
 *    - 0 on Success
 *    - 
 * 
 * @return 
 *    - TRUE if a file in the list could be opened and the header read. 
 *    - FALSE if no files could be opened for reading.
 *
 * @bug This calls zopen_sac(), which opens the file. Does it need to do this
 *       All it needs to do is check for the files existence.
 *
 * @date   981103:  Removed call to mkdsflsactive() by removing cmdfm.ldschg
 *                  which is never TRUE since removing the TO option from
 *                  the READ command.  maf
 * @date   920618:  Changed kfile to be substr of kfiles rather than kdflrq.
 *                  Problem came up with READ DIR bug reported by Owens.
 * @date   920427:  Original version.
 *
 */
int
lfilesok(string_list * files, char *kdir, int kdir_s, int lmore, int lheader,
         int lxdr, int *nerr) {

    int i;
    char *file, *kopen;
    int lfilesok_v;

    UNUSED(kdir_s);
    UNUSED(lxdr);
    UNUSED(lheader);
    kopen = NULL;
    file = NULL;

    struct stat st;

    *nerr = 0;

    lfilesok_v = FALSE;

    /* - For each file in the list. */
    for (i = 0; i < string_list_length(files); i++) {
        /* -- Get a file name. */
        file = string_list_get(files, i);

        if (kdir) {
            asprintf(&kopen, "%s/%s", kdir, file);
        } else {
            kopen = strdup(file);
        }
        if (stat(kopen, &st) == 0) {
            lfilesok_v = TRUE;
        }
        FREE(kopen);
    }

    if (lfilesok_v) {
        if (!lmore) {
            /* -- When not using READ MORE: 
             * --- Clear working-storage pointers, data-set storage pointers
             *     and deallocate memory for the current data set. 
             */
            sacclear();
        }
    }

    return (lfilesok_v);
}
