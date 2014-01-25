/** 
 * @file   xenddo.c
 * 
 * @brief  Parse "ENDDO"
 * 
 */

#include <stdio.h>

#include "cnd.h"
#include "vars.h"
#include "ucf.h"


#include "co.h"

#include "cpf.h"

extern Token *do_token[100];

/** 
 * Parse the acton command "ENDDO"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @note Global Variables
 *   - ndolevel: Decremented by one.
 *
 * @date   870817:  Original version.
 *
 */
void 
xenddo(int *nerr) {

  FILE *nun;
  
  *nerr = 0;
  if( cnd.ndolevel > 0 ){
    getclun( &nun, nerr );
    
    if( Ndotype[cnd.ndolevel] == 1 ){
      /* -- End of WHILE, backup to WHILE statement */
      backspace(nun,Ndolines[cnd.ndolevel] + 1);
      cnd.ndolevel = cnd.ndolevel - 1;
    } else {
      /* -- End of DO, get next loop variable */
      if( ldolist( nerr ) ){
        backspace(nun, Ndolines[cnd.ndolevel]);
      } else {
        /*    No more variables in this do list */
        /*
        deletev( (char*)kcnd.kdovar[cnd.ndolevel - 1], MCPFN+1, 
                 (char*)kcnd.kdolist[cnd.ndolevel - 1],MCPFN+1, 
                 nerr );
        */
        if(do_token[cnd.ndolevel-1]) {
          token_free(do_token[cnd.ndolevel-1]);
          do_token[cnd.ndolevel-1] = NULL;
        }
        cnd.ndolevel = cnd.ndolevel - 1;
      }
    }
  } else {
    /* - Raise error condition if not in an do condition. */
    *nerr = 1;
  }
  return;
}

