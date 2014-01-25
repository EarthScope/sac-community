/** 
 * @file   xdo.c
 * 
 * @brief  Parse "DO"
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "cnd.h"
#include "cpf.h"
#include "bot.h"
#include "bool.h"
#include "vars.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "wild.h"
#include "clf.h"
#include "errors.h"

#include "token.h"
#include "clf.h"

Token *do_token[100];


/** 
 * Report a DO Loop Error
 *
 * @param id
 *    Reference id
 * @param ktoken
 *    Token passed in
 * @param nerr
 *    Error Return Flag
 *    Always set to ERROR_SYNTAX_ERROR_IN_DO_STATEMENT
 *
 */
void
xdo_error(int *nerr) {
  
  cfmt("SYNTAX ERROR IN DO", 19); 

  *nerr = ERROR_BAD_COMMAND_SYNTAX;
}


/** 
 * @param MCL
 *    Maximum length of a character list in a "DO" statement
 */
#define	MCL	1500

#define DO_VAR_LIST_TOKEN "LIST"
#define DO_VAR_LIST_TOKEN_LENGTH strlen(DO_VAR_LIST_TOKEN)

#define DO_VAR_WILD_TOKEN "WILD"
#define DO_VAR_WILD_TOKEN_LENGTH strlen(DO_VAR_WILD_TOKEN)

#define DO_VAR_FROM_TOKEN "FROM"
#define DO_VAR_FROM_TOKEN_LENGTH strlen(DO_VAR_FROM_TOKEN)

#define DO_VAR_TO_TOKEN "TO"
#define DO_VAR_TO_TOKEN_LENGTH strlen(DO_VAR_TO_TOKEN)

#define DO_VAR_BY_TOKEN "BY"
#define DO_VAR_BY_TOKEN_LENGTH strlen(DO_VAR_BY_TOKEN)

/** 
 * Parse the action command "DO"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - ERROR_SYNTAX_ERROR_IN_DO_STATEMENT
 *   - ERROR_DO_LOOP_EXCEEDED_MAX_LENGTH
 *
 * @note Global Coupling
 *   - cnd:    ndolevel: Incremented by one.
 *   - cnd:    lifresp:  Set to value of if test.
 *
 * @date   880915:  Fixed bug in getting filelists.
 * @date   871014:  Enlarged size of character lists, added WILD option,
 *             and added much more error checking.
 * @date   870817:  Original version.
 *
 */
void 
xdo(int *nerr) {

	char kcl[MCL+1], kclin[MCL+1], kdirin[MCPFN+1], kmacroname[MCPFN+1];
	int lexpand;
	int idx, inc, index1, index2, nc, nclin, nverr, do_count;

  string_list *files;
  string_list *input_files;


	/* - Initialize and do some error checking. */
    nclin = 0;
	*nerr = 0;
	cnd.ndolevel = cnd.ndolevel + 1;
  memset(kclin, 0, MCL+1);
  memset(kcnd.kdovar[cnd.ndolevel - 1],(int)' ',MCPFN);
  kcnd.kdovar[cnd.ndolevel - 1][MCPFN] = '\0';
  memcpy(kcnd.kdovar[cnd.ndolevel - 1], kmcpf.kvarsname,strlen(kmcpf.kvarsname));
  kcnd.kdovar[cnd.ndolevel-1][strlen(kmcpf.kvarsname)] = 0;

	Ndotype[cnd.ndolevel] = 0;
    
	/* - Get length of do loop */
	getdolen( &Ndolines[cnd.ndolevel], nerr );
	if( *nerr != 0 ){
      getvvstring( kmcpf.kvarsname,9, "macroname",10, &nc, 
                   kmacroname ,MCPFN+1, &nverr );
      out("in macro file %s ", kmacroname);
      outmsg () ;
      clrmsg () ;
      *nerr = 0 ;
      goto L_8888;
	}

	/* - Get loop variable name */

	if( !lcchar(MCPFN+1, (char*)kcnd.kdoname[cnd.ndolevel - 1],MCPFN+1, &nc) ) {
    xdo_error(nerr);
    goto L_8888;
	}

  memset(kcl,(int)' ',MCL);
  kcl[MCL] = '\0';
  
  memcpy(kcl,kcnd.kdoname[cnd.ndolevel - 1], strlen(kcnd.kdoname[cnd.ndolevel - 1]));
    
	idx = indexb( kcl,MCL+1 );
	subscpy( kcl, idx, idx + 3, MCL, "list" );
	strscpy( kcnd.kdolist[cnd.ndolevel - 1], kcl, MCPFN );
  kcnd.kdolist[cnd.ndolevel-1][idx+4] = 0;

	/* - Process do loop
	 *   Syntax: DO var = start , stop [, inc]  
	 *           DO var FROM start TO stop [BY inc] */
  if(lckey("FROM", 5) || lcequals() ) {
      inc = 1;
      /* Get a number */
      if( !lcint(&index1) ) {
        xdo_error(nerr);
        goto L_8888;
      }
      if( !lckey("TO", 3) && ! lccomma() ) {
        xdo_error(nerr);
        goto L_8888;
      }
      /* Get a number */
      if( !lcint(&index2) ) {
        xdo_error(nerr);
        goto L_8888;
      }
      if( lckey("BY", 3) || lccomma() ) {
        if( !lcint(&inc) ) {
          xdo_error(nerr);
          goto L_8888;
        }
      }
      do_count = 0;
      
      if(index1 < index2){
        if( inc <= 0 ){ 
          do_count = 0;
        } else {
          for(idx = index1; idx <= index2; idx += inc) {
            do_count++;
          }
        }
      } else if(index1 > index2) {
        if( inc >= 0 ){ 
          do_count = 0;
        } else {
          for(idx = index1; idx >= index2; idx += inc) {
            do_count++;
          }
        }
      } else { /* index1 == index2 */
        do_count = 1;
      }
      Token *last = NULL;
      for( idx = index1; do_count > 0; idx += inc, do_count-- ){
        Token *t = token_new_int(idx);
        if(last) {
          last->next = t;
        } else {
          do_token[cnd.ndolevel-1] = t;
        }
        last = t;
      }
      goto HELL;
	} else if( lckey("LIST", 5) ) {
    /* - Process Do List option.
     *   Syntax:  DO var LIST value1 value2 value3 ... */
    do_token[cnd.ndolevel-1] = token_to_token_list( arg() );
    goto HELL;

	} else if( lckey("WILD", 5) ) { 
    //strncasecmp(ktoken, DO_VAR_WILD_TOKEN,  DO_VAR_WILD_TOKEN_LENGTH) == 0 ) {
      /* - Process Do Wild option.
       *   Syntax:  DO var WILD [DIR dir] wild1 wild2 wild3 ... */
      memset(kdirin,(int)' ',MCPFN);
      kdirin[MCPFN] = '\0';
      
      while ( lcmore( nerr ) ){
        if( lkchar( "DIR#$",6, MCPFN, kdirin,MCPFN+1, &nc ) ){
          if( kdirin[nc - 1] != KDIRDL )
            kdirin[nc] = KDIRDL;
        } else if( lccl( kclin,MCL+1, &nclin ) ) {  }
      }
      
      /* Expand the wildcards in the input
       *   kcl holds the output, but it could be truncated, so we ignore it and use filelist 
       */
      input_files = string_list_from_cfl( kclin, MCL+1 );
      files = wildfl( kdirin, MCPFN+1, input_files, &lexpand );
      {
        Token *last, *t;
        int i;
        last = NULL;
        for(i = 0; i < string_list_length(files); i++) {
          t = token_new_string(string_list_get(files, i));
          if(last) {
            last->next = t; 
          } else {
            do_token[cnd.ndolevel-1] = t;
          }
          last = t;
        }
      }
      goto HELL;
	} else {
      /* - Syntax error. */
      xdo_error(nerr);
      goto L_8888;
	}
  
 HELL:
	Idoin1[cnd.ndolevel] = 0;
	Idoin2[cnd.ndolevel] = 0;
	if( !ldolist( nerr ) ){
      skipdo( nerr );
      if( *nerr != 0 )
		goto L_8888;
	}

 L_8888:
	if( *nerr != 0 )
      cnd.ndolevel = cnd.ndolevel - 1;

	return;
    
}

