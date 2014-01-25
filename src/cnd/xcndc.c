/** 
 * @file   xcndc.c
 * 
 * @brief  Execute a CND Command
 * 
 */

#include <stdio.h>
#include <string.h>

#include "cnd.h"
#include "msg.h"
#include "cpf.h"
#include "errors.h"

/** 
 * Execute a CND (Conditional) Command given its index number.
 *    This module contains conditional execution commands.
 * 
 * @param index 
 *    Index Number of the command to execute
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR
 *
 * @date   870901:  Original version developed by Mandy Goldner.
 *
 */
void 
xcndc(int  index, 
      int *nerr) {

  int level;
  char macro_name[1024];
	*nerr = 0;

  memset(&macro_name[0], 0, sizeof(macro_name));
  getmacroinfo(&level, &macro_name[0], 1024);
  if(level < 1) {
    *nerr = 1017;
    error(*nerr, "'%s' can only be used within a macro", arg_begin()->str);
    return;
  }
	/* - Jump to correct command based upon its index number. */
	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		case 7: goto L_700;
		case 8: goto L_800;
		}

	/* - Error return if bad index value. */

	*nerr = ERROR_SAC_LOGIC_ERROR;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XCNDC",9 );
	goto L_8888;

	
L_100: /* - Command 01: IF */
	xif( nerr );
	goto L_8888;

L_200: /* - Command 02: ELSEIF */
	xelseif( nerr );
	goto L_8888;

L_300: /* - Command 03: ELSE */
	xelse( nerr );
	goto L_8888;

L_400: /* - Command 04: ENDIF */
	xendif( nerr );
	goto L_8888;

L_500: /* - Command 05: DO */
	xdo( nerr );
	goto L_8888;

L_600: /* - Command 06: WHILE */
	xwhile( nerr );
	goto L_8888;

L_700: /* - Command 07: ENDDO */
	xenddo( nerr );
	goto L_8888;

L_800: /* - Command 08: BREAK */
	xbreak( nerr );
	goto L_8888;

L_8888:
	return;
} 

