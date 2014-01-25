/** 
 * @file   xrerr.c
 * 
 * @brief  Read Error Control
 * 
 */

#include <string.h>

#include "dfm.h"
#include "exm.h"


#include "cpf.h"

/** 
 * Execute the command READERR which controls behavior for read errors 
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920501:  Added KECMEM, save or delete contents in memory.
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   820113:  Merged old ERRCON command into this command.
 *
 */
void 
xrerr(int *nerr) {

	int index;

	*nerr = 0;

L_1000:
	if( lcmore( nerr ) ){

	  /* -- "BADFILE FATAL|WARNING|IGNORE":  error control for missing
	   *                                     or unreadable data file. */
	  if( lklist( "BADFILE$",9, (char*)kmexm.kectp, 9, 
		      cmexm.nectp, &index ) ){
	    strcpy( kmdfm.kecbdf, kmexm.kectp[index - 1] );
	    
	  }
	  /* -- "NOFILES FATAL|WARNING|IGNORE":  
	   *     error control for null data file list. 
	   */
	  else if( lklist( "NOFILES$",9, (char*)kmexm.kectp,9, 
			   cmexm.nectp, &index ) ){
	    strcpy( kmexm.kecnof, kmexm.kectp[index - 1] );
	    
	  }
	  /* -- "MEMORY SAVE|DELETE":  error control for memory contents. */
	  else if( lklist( "MEMORY$",8, (char*)kmexm.kectp,9, 
			   cmexm.nectp, &index ) ){
	    strcpy( kmdfm.kecmem, kmexm.kectp[index - 1] );
	    
	  }
	  /* -- Bad syntax. */
	  else {
	    cfmt( "ILLEGAL OPTION:",17 );
	    cresp();
	  }
	  goto L_1000;
	}
       
	return;
}

