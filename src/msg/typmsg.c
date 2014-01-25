/** 
 * @file   typmsg.c
 * 
 * @brief  Set the Mesage Type
 * 
 */
#include <string.h>

#include "msg.h"
#include "bool.h"
#include "bot.h"

/** 
 * Set the mesage type
 *
 * \param *ktype
 *   Type of message 
 *    Only the first letter need be entered.
 *      - 'ERROR'     for an fatal error message condition.
 *      - 'WARNING'   for a warning message condition.
 *      - 'OUTPUT'    for an output message condition.
 *      - 'COMMANDS'  for commands typed at the terminal.
 *      - 'MACROS'    for commands executed from a command file.
 *      - 'PROCESSED' for processed (evaluated) commands.
 *
 * \return Nothing
 *
 * \see modcase
 * \see t_kmmmsg.ktpmsg
 * \see t_cmmsg.itpmsg
 *
 * \date   881230:  Deleted some obsolete coding.
 * \date   860130:  Original version.
 * \date   881230:  Documented/Reviewed
 */
void 
typmsg(char *ktype)
{
	char ktpinp[9];
	int j, j_;
    size_t len;
	/* - Determine what type of message condition to raise.
	 *   If the type is not a valid one, 
	 raise an informational condition. */
    len = (strlen(ktype) < MCPW) ? strlen(ktype) : MCPW;
	modcase( TRUE, ktype, len, ktpinp );
	for( j = 1; j <= MTPMSG; j++ ){
		j_ = j - 1;
		if( ktpinp[0] == kmmsg.ktpmsg[j_][0] )
			goto L_1100;
		}
	j = 3;

L_1100:
	cmmsg.itpmsg = j;

	return;
}

