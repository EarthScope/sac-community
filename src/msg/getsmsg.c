/** 
 * @file   getsmsg.c
 * 
 * @brief  Get a message from message file
 * 
 */

#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "co.h"
#include "debug.h"

#include "vars/chash.h"

extern dict *msg_dict;

/** 
 * Get a message from message file on disk
 *
 * \param number
 *    Message number idntifying message to get
 * \param *kmsg
 *    Message from the disk file to get
 * \param kmsg_s
 *    Length of string \p kmsg
 * 
 * \return Nothing
 *
 * \bug If no error code found, it should indicate that it
 *       needs to be fixed by stating an undefined error code
 *       was requested.
 *
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void
getsmsg(int number, char *kmsg, int kmsg_s) {

    char str[16];
    UNUSED(kmsg_s);
 	/* - Loop through list of message numbers, looking for a match. */
    sprintf(str, "%d", number);
    char *v = (char *) dict_get(msg_dict, str);
    if(v) {
        strcpy(kmsg, v);
        return;
    }

    /* - If no match is found, simply encode the error number. */
    sprintf(kmsg, "%s%5d", "Number", number);

    return;
}
