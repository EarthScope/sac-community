/** 
 * @file   lkt.c
 * 
 * @brief  Parse a keyed t? phase author
 * 
 */

#include <string.h>
#include <ctype.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"
#include "dfm.h"
#include "bot.h"
#include "co.h"

#include "errors.h"


#include "msg.h"
#include "ucf.h"

#define ERROR_RETURN(x) do { \
    *nerr = x;               \
    setmsg("ERROR", *nerr);  \
    return;                  \
  } while(0);

/* define max length of phase and author names. */
#define  MPH 8
#define  MAU 15

/** 
 * Parse a keyed t? phase author command constuct for xpickphase
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_NOT_FOUND_IN_RANGE_T0_T9
 *
 * @date   970409:  Original version.
 *
 */
void 
lkt(int *nerr) {

	int lfound ;
	int idx ;
	char tee[3] ;
	char kchar [ MAU + 1 ] ;
  Token *t;

	*nerr = 0 ;
	strcpy ( tee , "t0" ) ;

	/* loop between t? keys. */
	while ( lcmore ( nerr ) ) {
    /* - Check for key. */
    for ( idx = '0' ; idx <= '9' ; idx++ ) {
      tee[1] = idx ;
      modcase ( TRUE , tee , 2 , tee ) ;
      lfound = lckey( tee , 3 );
      if ( lfound )
		    break ;
    } /* end for ( idx ) */
    
	  /* if key not found, it's an error. */
    if ( !lfound ) {
      ERROR_RETURN( ERROR_NOT_FOUND_IN_RANGE_T0_T9 );
    }
    
    /* reset idx to index global array's later. */
    idx -= '0' ;

    /* - Look for phase, skip if t[0-9] */ 
    if((t = arg()) && token_is_string(t)) {
      if(((t->str[0] == 't' || t->str[0] == 'T') && isdigit(t->str[1])) ||
         strlen(t->str) == 0) {
        continue;
      }
      strcpy ( kmdfm.ktPh[idx] , t->str ) ;
      arg_next();
    } else {
      ERROR_RETURN( ERROR_BAD_COMMAND_SYNTAX );      
    }
    
    /* Look for authror, skip if t[0-9] */
    if((t = arg()) && token_is_string(t)) {
      if(((t->str[0] == 't' || t->str[0] == 'T') && isdigit(t->str[1])) ||
         strlen(t->str) == 0) {
        continue;
      }
      modcase ( FALSE , t->str , strlen ( t->str ) , kchar ) ;
      strcpy ( kmdfm.ktAu[idx] , kchar ) ;
      arg_next();            
    } else {
      ERROR_RETURN( ERROR_BAD_COMMAND_SYNTAX );
    }

	} /* end while ( lcmore ( nerr ) ) */

}

