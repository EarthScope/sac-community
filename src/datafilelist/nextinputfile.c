/** 
 * @file   nextinputfile.c
 * 
 * @brief  Get the next entry in the data file list
 * 
 */

#include <string.h>
#include <fern/array.h>

#include "datafilelist.h"
#include "bool.h"
#include "co.h"

#include "dff.h"

struct t_kmdatafilelist kmdatafilelist;
struct t_cmdatafilelist cmdatafilelist;

static int *iselect = NULL;/** Current entries which are selected  */

/** 
 * @file   selectinputfi.c
 * 
 * @brief  Select the active entries in the input data file list
 * 
 */

/** 
 * Select the active entres in the input data file list for use in
 *    subsequent action commands
 * 
 * @param list 
 *    List of active entries in the input data file list
 *    Numbers in \p list refer to the order of the entries in
 *    the data file list.  An entry in \p list becomes active 
 *    for subsequent commands
 * @param nlist 
 *    Length of \p list
 *
 * @date   900409:  Original version.
 *
 */
void
selectinputfiles(int *list, int nlist) {
    int j;

    int *const List = &list[0] - 1;

    if (iselect) {
        xarray_free(iselect);
    }
    iselect = xarray_new_with_len('i', nlist + 1);
    /* - Save list in common block. */
    cmdatafilelist.nselect = nlist;

    for (j = 1; j <= nlist; j++) {
        iselect[j] = List[j];
    }

    return;
}

/** 
 * Get the next entry in the input data file list
 * 
 * @param ientry 
 *    - 0 to initialize
 *    - Do not change between calls
 * 
 * @return 
 *    - TRUE if there are more entries in the data file list
 *    - FALSE if there are no more entries in the data file list
 *
 * @date   900409:  Original version.
 *
 */
int
nextinputfile(int *ientry) {

    int nextinputfile_v;

    /* - If in "ALL" mode: */
    if (strcmp(kmdatafilelist.kselectmode, "ALL     ") == 0) {

        /* -- Initialize entry pointer and number of files */
        if (*ientry <= 0) {
            *ientry = 0;
            getnfiles(&cmdatafilelist.nentries);
        }

        /* -- Increment entry number */
        if (*ientry < cmdatafilelist.nentries) {
            *ientry = *ientry + 1;
            nextinputfile_v = TRUE;
        } else {
            /* -- Zero entry number and return with FALSE 
             *    value if no more entries. */
            *ientry = 0;
            nextinputfile_v = FALSE;
        }
    } else {
        /* - If in "SELECT" mode: */

        /* -- Initialize select pointer and if necessary. */
        if (*ientry <= 0)
            cmdatafilelist.jselect = 0;

        /* -- Increment select pointer if there are more selections.
         *    Set entry number to selected entry. */
        if (cmdatafilelist.jselect < cmdatafilelist.nselect) {
            cmdatafilelist.jselect = cmdatafilelist.jselect + 1;
            *ientry = iselect[cmdatafilelist.jselect];
            nextinputfile_v = TRUE;
        } else {
            /* -- Zero entry number and return with FALSE 
             *    value if no more selections. */
            *ientry = 0;
            nextinputfile_v = FALSE;
        }
    }

    return (nextinputfile_v);

}
