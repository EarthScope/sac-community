/** 
 * @file   wrcom.c
 * 
 * @brief  Write command to terminal
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "ucf.h"
#include "com.h"
#include "bool.h"


#include "co.h"
#include "bot.h"

#include "token.h"

/** 
 * Write current command to the terminal
 * 
 * @date   871022:  Cleaned up output of strings and long tokens.
 * @date   870513:  Deleted call to wrtxtd.  Writing to terminal directly.
 * @date   870218:  Fixed bug in formatting quoted strings.
 * @date   840206:  Improved method of formatting current command.
 *
 */
void 
wrcom() {

  fprintf(stdout, "%s\n", lexer_input());

  return;
}

