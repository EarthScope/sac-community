/** 
 * @file   xwild.c
 * 
 * @brief  Defined Wild settings
 * 
 */

#include <string.h>

#include "dfm.h"

#include "wild.h"


#include "cpf.h"

/** 
 * Execute the command WILD which defined the wild card keys to be used
 *    in the commands READ, READHDR, and READALPHA.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 *    860922:  Original version.
 *
 */
void 
xwild(int *nerr) {

	char _c0[2];
	int _l0, nret;
	char citr[4];

	*nerr = 0;

	/* PARSING PHASE: */
	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

	  /* -- "SINGLE char": define character to use to match single 
	   *                   characters. 
	   */
	  _c0[0] = kmwild.sngl;
	  _c0[1] = '\0';
	  _l0 = lkchar( "SINGLE$",8, 1, _c0, 2, &nret );
	  kmwild.sngl = _c0[0];
	  if( _l0 ){  }
	  /* -- "MULTIPLE char": define character to use to match 
	   *                     multiple characters. 
	   */
	  else{
	    _c0[0] = kmwild.mult;
	    _c0[1] = '\0';
	    _l0 = lkchar( "MULTIPLE$",10, 1, _c0,2, &nret );
	    kmwild.mult = _c0[0];
	    if( _l0 ){   }
	    /* -- "ITERATION chars": define two characters to use for 
	     *                       iteration. 
	     */
	    else{
	      strcpy(citr,"   ");
	      _l0 = lkchar( "ITERATION$",11, 2, citr, 4, &nret );
	      if( _l0 ){
		if( nret != 2 ){
		  cfmt( "NEED TWO CHARACTERS:",22 );
		  cresp();
		}
	      }
	      /* -- "CONCATENATION char": define two characters to use for 
	       *                          concatentation. 
	       */
	      else if( lkchar( "CONCATENATION$",15, 2, kmwild.ccon,3, &nret)){
		if( nret != 2 ){
		  cfmt( "NEED TWO CHARACTERS:",22 );
		  cresp();
		}
	      }
	      /* -- "ECHO [ON|OFF]": turn echoing of filelists on or off. */
	      else if( lklog( "ECHO$",6, &cmdfm.lechof ) ){   }

	      /* -- Bad syntax. */
	      else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
		
	      }
	    }
	  }
	  goto L_1000;
	  
	}
	
       
	return;
}

