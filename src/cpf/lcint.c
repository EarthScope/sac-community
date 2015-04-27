/** 
 * @file   lcint.c
 * 
 * @brief  Parse an integer
 * 
 */

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse an integer variable command construct
 * 
 * @param intv 
 *    Integer on output
 * 
 * @return 
 *    - TRUE if the integer was found
 *    - FALSE if the integer was not found
 *
 * @bug Check to make sure the conversion from float to int is done 
 *      correctly
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820415:  Added information hiding logic.
 * @date   810207:  Original version.
 *
 */
int
lcint(int *intv) {
    Token *t;

    if ((t = arg()) && token_is_int(t)) {
        *intv = token_as_int(t);
        arg_next();
        return TRUE;
    }
    return FALSE;
}
