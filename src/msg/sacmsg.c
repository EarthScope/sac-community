/** 
 * @file   sacmsg.c
 * 
 * @brief  Read the SAC message file
 * 
 */

#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "co.h"
#include "bot.h"

#include "string_utils.h"

#include "vars/chash.h"

dict *msg_dict = NULL;

/** 
 * Read the SAC message file from disk and save
 * 
 * \param *nerr
 *   Error return flag
 *   - 0 on Success
 *   - Non-Zero on Error
 *
 * \return Nothing
 *
 * \see zbasename crname zopens setmsg apimsg apcmsgnum apcmsg zcloses
 *
 * \date   870923:  Deleted ".saf" suffixes from aux files.
 * \date   870527:  Reworked file reading logic to make it compatible
 *                   with the current version of the MASSCOMP f77 compiler.
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void
sacmsg(int *nerr) {
    char kfile[MCPFN + 1], kiline[MCMSG + 1];
    char *value;
    int idx;
    int ntused, numsave;
    FILE *nun;
    int n;

    msg_dict = dict_new_with_length(500);

    /* - Build the pathname and open the file containing output messages. */

    /*memset(kfile,' ',MCPFN); */
    for (idx = 0; idx < MCPFN; idx++)
        kfile[idx] = ' ';
    kfile[MCPFN] = '\0';

    zbasename(kfile, MCPFN + 1);
    crname(kfile, MCPFN + 1, KDIRDL, "messages", 9, nerr);
    if (*nerr != 0)
        goto L_4000;
    zopens(&nun, kfile, MCPFN + 1, nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Read each message from disk file into common. */

    while(fgetsp(kiline,MCMSG,nun) != NULL){
        if(kiline[(numsave=strlen(kiline)-1)] == '\n') kiline[numsave] = ' ';

        if(sscanf(kiline,"%4d",  &n) != 1){
            fprintf(stderr, "ERROR 100: decoding line: '%s'\n", kiline);
            *nerr = 100;
            break;
        }

        value = rstrip(strdup(kiline+5));
        sac_msg_add(n, value);
    }

    if(!feof(nun) || *nerr ) {
        fprintf(stderr, "ERROR: 100: Error reading in SAC Messages\n");
        *nerr = 100;
    }

 L_4000:
    zcloses(&nun, &ntused);

  L_8888:
    return;

}
