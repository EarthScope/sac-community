/** 
 * @file   lcdfl.c
 * 
 * @brief  Parse a data file list
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

#include "clf.h"
#include "ucf.h"
#include "wild.h"
#include "debug.h"

/** 
 * Parse a data file list command construct
 * 
 * @param kdflist 
 *    Character list with data files found
 * @param kdflist_s 
 *    Length of \p kdflist
 * @param indfl 
 *    Entries in \p kdflist on output
 * 
 * @return 
 *    - TRUE if the list was found
 *    - FALSE if the list was not found
 *
 * @bug Wrapper around lcchar and putcl with a base name generator in between
 *
 * @date  970129:  Add parameter (0) to cnvati.  0 means that if a string
 *                 of digits is too long, let it slide by.  maf 
 * @date  911107:  Changed calling parameter names to avoid debugger problem.
 * @date  880915:  Deleted check for list beginning with a number.
 * @date  820423:  Adjustments due to new command parsing system.
 * @date  800823:  Modified the form of the base name option.
 *                 Added 32 chracter tree names by using ZFILNM [PRIME].
 * @date  810120:  Changed to output message retrieval from disk.
 * @date  810208:  Changed from DEFDFL to logical function LCDFL.
 */
string_list *
lcdfl() {

	char kname[MCPFN+1];
	int n1, n2, ncname, nerr;
  Token *t;

	nerr = 0;

    string_list *list;

    list = NULL;
    memset(kname, ' ', MCPFN);
	kname[ MCPFN ] = '\0' ;

	/* - Forms of names that are currently supported are:
	 *   - NAME   ... normal file name
	 *   - 'LONGTREENAME'   ...   Prime tree name
	 *   - BASE N   ...   N names of form BASE01, BASE02 ...
	 *   - BASE M N   ... N-M+1 names starting with BASExx where xx=M */

	/* - Initialize the character list, counter, and kname. */

	memset ( kname , ' ' , MCPFN ) ;
	kname [ MCPFN ] = '\0';


	/* - Loop until tokens are exhausted. */
	while( arg() ) {

    memset(kname, ' ', sizeof(kname));
    /* -- Use LCCHAR to get next filename. */
    if( lcchar( MCPFN, kname,MCPFN+1, &ncname ) ){
		/*  --- Generate file names from base name if 
		 *      next symbol a number. 
		 */
      if((t = arg()) && token_is_int(t)) {
        n1 = token_as_int(t);
        arg_next();
        if((t = arg()) && token_is_int(t)) {
          n2 = token_as_int(t);
          arg_next();
        } else {
          n2 = n1;
          n1 = 1;
        }

        if(!list) {
          list = string_list_init();
        }
		    basenm( kname,MCPFN+1, n1, n2, list, &nerr );
        string_list_print(list);
		    if( nerr != 0 )
          goto L_8888;
		}

		/* --- Otherwise this is a simple filename. */
		else{
            DEBUG("kname: '%s'\n", kname);
            if(!list) {
                list = string_list_init();
            }
            string_list_put(list, kname, MCPFN+1);
		}
	    }

	    /* -- Raise error condition due to an unexpected token. */
	    else{
            DEBUG("Unexpected token\n");
			nerr = 1001;
			goto L_8888;
    }
	} /* end while */

L_8888:
    return list;
}

string_list *
lcdfl_wild() {
  char readdir[MCPFN];
  string_list *list, *new_list;
  int lexpnd;

  new_list = NULL;
  // Define default read dir to Empty
  memset(readdir, ' ', MCPFN);
  readdir[MCPFN] = 0;
  // Read data file list
  list = lcdfl();
  // Expand wildcards
  new_list = wildfl(readdir, MCPFN+1, list, &lexpnd);
  // Free unexpanded list
  string_list_free(list);
  list = NULL;
  // Return new list
  return new_list;
}

