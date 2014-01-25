/** 
 * @file   setprompt.c
 * 
 * @brief  Set the prompt
 * 
 */

#include "exm.h"
#include "co.h"
#include "bot.h"

/** 
 * Set the prompt 
 * 
 * @param prompt 
 *    New prompt
 * @param prompt_s 
 *    Length of \p prompt
 *
 * @date   861203:  Original version.
 *
 */
void 
setprompt(char *prompt, 
          int   prompt_s) {

	int nc;

	/* - Determine number of characters in prompt, excluding trailing blanks. */
	nc = min( indexb( prompt,prompt_s ), 10 );

	/* - Append a " $" to end of prompt and store in EXM common block.
	 *   (The space is output, the dollar sign terminates the string.) */

        fstrncpy( kmexm.kprmt, 12, prompt, nc);
        fstrncpy( kmexm.kprmt+nc, 12-nc, " $", 2);

	return;
}

