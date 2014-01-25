/** 
 * @file   xgetbb.c
 * 
 * @brief  GETBB Command
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "bbs.h"
#include "msg.h"
#include "cpf.h"
#include "bool.h"
#include "bot.h"
#include "dfm.h"

#include "co.h"
#include "clf.h"
#include "vars.h"
#include "errors.h"
#include "debug.h"

static void
newline(FILE *nun) {
  if( nun == MUNOUT ){
    outmsg();
  } else {
    wrtmsg( nun );
  }
}


void
show_var(var *v, char *name, FILE *fp) {
  if( cmexm.lnames ){
    out("%s = ", name);
  }
  switch(v->type) {
  case VAR_VALUE:   out("%g", v->value); break;
  case VAR_STRING:  out("'%s'",  v->str); break;
  case VAR_INTEGER: out("%d",  v->ival); break;
  }
  if( cmexm.lnewline ) {
    newline( fp );
    clrmsg();
    setmsg( "OUTPUT", 99 );
  }
}

static int 
string_cmp(const void *a, const void *b) {
  const char **ia = (const char **)a;
  const char **ib = (const char **)b;
  return strcasecmp(*ia, *ib);
}

/** 
 * Execute the getbb command which gets the blackboard variables
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   890104:  Changed from terminal output to message subsystem.
 * @date   880901:  Added TO, NAMES, and NEWLINE options.
 * @date   870917:  Added ALL option.
 * @date   870514:  Original version.
 *
 */
void 
xgetbb(int *nerr) {

	char kbbvalue[MCMSG+1];
	int ic1, ic2, nc, ncbb ;
  var *v;
  char *name;

	*nerr = 0;

    memset(kmexm.knmbbwrite,0, sizeof(kmexm.knmbbwrite));
    memset(kbbvalue, 0, sizeof(kbbvalue));

	while ( lcmore( nerr ) ){

	    /* -- "ALL": report all of the currently defined blackboard variables. */
    if( lckey( "ALL#$",6 ) ){
      cmexm.lbball = TRUE;
    }
    
    /* -- "TO TERMINAL|filename": define where the output is to be sent. */
    else if( lckey( "TO#$",5 ) ){
      if( lckey( "TERM#INAL$",11 ) ){
		    cmexm.nunbbwrite = MUNOUT;
      }
      else if( lcchar( MCPFN, kmexm.knmbbwrite,MCPFN+1, &nc ) ){
		    cmexm.nunbbwrite = (FILE *)NULL;
		    if( *nerr != 0 )
          goto L_8888;
      }
      else{
		    cfmt( "ILLEGAL OPTION:",17 );
		    cresp();
      }
    }
    
	    /* -- "NAMES ON|OFF": option to include the
         bb variable name with the value. */
    else if( lklog( "NAMES#$",8, &cmexm.lnames ) ) { }
    
	    /* -- "NEWLINE ON|OFF": option to append newline
         after each bb variable value. */
    else if( lklog( "NEWLINE#$",10, &cmexm.lnewline ) ) { }
    
    /* -- The rest of the tokens should be names of a blackboard variables.
     *    First unset ALL flag and initialize list of bb variables.
     *    Then set up an inner parsing loop to collect all of the 
     *    variable names. */
    else{
      cmexm.lbball = FALSE;
      memset ( kmexm.kbbcl , ' ' , MCMSG );
      
      if(!lccl(kmexm.kbbcl, MCMSG+1, &ncbb)) {
        cfmt( "ILLEGAL OPTION:",17 );
        cresp();
      }
    }
	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Open disk file if necessary. */

	if( cmexm.nunbbwrite != MUNOUT ){
    znfiles( &cmexm.nunbbwrite, kmexm.knmbbwrite,MCPFN+1, "TEXT",5,  nerr );
    if( *nerr != 0 )
      goto L_8888;
    if ( fseek ( cmexm.nunbbwrite , 0L , SEEK_END ) != 0 )
      fprintf ( stdout , "fseek returned error-xgetbb\n" ) ;
	}

	/* - Sequentially access blackboard if ALL was requested. */

	setmsg( "OUTPUT", 99 );
	if( cmexm.lbball ){
    int i = 0;
    char **keys = sac_vars_keys(kmbbs.knmbbs);
    while(keys && keys[i]) { i++; }
    if(i > 0) {
      qsort(keys, i, sizeof(char*), string_cmp);
      i = 0;
      while ( keys && keys[i] ){
        if(!(v = sac_vars_get_var(kmbbs.knmbbs, keys[i]))) {
          error(ERROR_FINDING_VARIABLE, "%s", keys[i]);
          outmsg();
          clrmsg();
          i++;
          continue;
        }
        show_var(v, keys[i], cmexm.nunbbwrite);
        i++;
      }
      if( !cmexm.lnewline ) {
        newline( cmexm.nunbbwrite );
      }
      i = 0;
      while(keys && keys[i]) {
        FREE(keys[i]);
        i++;
      }
      FREE(keys);
    }
	}

	/* - Otherwise, get value for each item in request list. */
	else {
    ic1 = 0;    
    while ( lnxtcl( kmexm.kbbcl,MCMSG+1, &ic1, &ic2 ) ){
      name = strcut(kmexm.kbbcl, ic1, ic2);
      if(!(v = getbb(name))) {
        error(ERROR_FINDING_VARIABLE, "%s", name);
        outmsg();
        clrmsg();
        continue;
      }
      show_var(v, name, cmexm.nunbbwrite);
      free(name);
    }
    if( !cmexm.lnewline ) {
      newline( cmexm.nunbbwrite );
    }
	}

	clrmsg();
	if( cmexm.nunbbwrite != MUNOUT ){
	    zcloses( &cmexm.nunbbwrite, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

L_8888:
	return;

} /* end of function */

