/** 
 * @file   lckeyExact.c
 * 
 * @brief  Parse a key
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bot.h"
#include "bool.h"


#include "co.h"
#include "msg.h"

#define	MCHECK	136

/** 
 * Search for a key command construct
 * 
 * @param kkey 
 *    Key to search for
 * @param kkey_s 
 *    Length of \p kkey
 * 
 * @return 
 *    - TRUE if the key was found
 *    - FALSE if the key was not found
 *
 * @date   970702:  Original version, based on lckey.c.  maf
 *
 */
int
lckeyExact(char *kkey, 
	   int   kkey_s) {

	char kcheck[MCHECK + 1], ktoken[MCHECK + 1];
	int lckeyExact_v, lnoabb;
	int idx, ncabb, ncheck, nckey, ncsym;
  Token *t;

	/* - Determine length of input key (i.e., find trailing dollar sign.) */
	nckey = indexc( kkey,kkey_s, '$' );

	for( idx = 0 ; idx < 136 ; idx++ ) {
	    ktoken[ idx ] = ' ' ;
	    kcheck[ idx ] = ' ' ;
	}
	ktoken[ 136 ] = '\0' ;
	kcheck[ 136 ] = '\0' ;

	/* - If trailing dollar sign is missing find 
	 *    last non-blank character. */
	if( nckey <= 0 )
	    nckey = indexb( kkey,kkey_s );

	/* - If character length of key is still 0, set function value to .TRUE.
	 *   and return immediately.  Do not increment command pointer. */
	if( nckey == 0 ){
	    lckeyExact_v = TRUE;
	    goto L_8888;
	}

	/* - Copy key to local variable, 
	 *   deleting special characters if present. */
	lnoabb = kkey[0] == '&';
	ncabb = indexa( kkey,kkey_s, '#', TRUE, TRUE );
	if( lnoabb ){
            fstrncpy(kcheck,136,kkey+1,kkey_s-2);
	    nckey = nckey - 1;
	}
	else if( ncabb > 0 ){
            fstrncpy(kcheck,136,kkey,ncabb-1);
            memcpy(kcheck+ncabb-1,kkey+ncabb,kkey_s - (ncabb + 1));
	    nckey = nckey - 1;
	}
	else{
            fstrncpy(kcheck,136,kkey,strlen(kkey));
	}

	/* - Determine length of current command symbol and 
	 *   increment in command counter. Copy current command 
	 *   symbol to local storage. */

	/* -- Long command symbol case: */
	/* if( kmcom.kcom[cmcom.jcom - 1][0] == '\'' ){ */
	/*     jsym = cmcom.jcom + 2; */
	/*     ncsym = (int)( Flnum[cmcom.jcom + 1] + 0.1 ); */

	/*     /\* -- Check ktoken size, truncate and issue  */
	/*      *    warning if token is too long. *\/ */
	/*     if( ncsym > MCHECK ){ */
	/* 	ncsym = MCHECK; */
	/* 	setmsg( "WARNING", 921 ); */
	/* 	wrtmsg( stdout ); */
	/* 	clrmsg(); */
	/*     } */

	/*     /\* -- Not sure where else this logic is required...  */
	/*      *    so leave it here *\/ */
	/*     jcopy = (ncsym - 1)/MCPW + 1; */
	/*     jc = 1; */

	/*     for( j = 1; j <= jcopy; j++ ){ */
	/* 	strtemp = malloc(MCPW+1); */
	/* 	strncpy(strtemp,kmcom.kcom[jsym + j - 2],MCPW); */
	/* 	strtemp[MCPW] = '\0'; */
	/* 	subscpy( ktoken, jc - 1, jc + MCPW - 2, 136, strtemp); */
	/* 	free(strtemp); */
	/* 	jc = jc + MCPW; */
	/*     } */
	/*     jicom = jcopy + 2; */
	/* } */

	/* -- Normal command symbol case: */
	/* else{ */
	/*     jsym = cmcom.jcom; */
	/*     ncsym = indexb( (char*)kmcom.kcom[cmcom.jcom - 1],9 ); */
  /*           fstrncpy(ktoken,136,kmcom.kcom[jsym - 1],strlen(kmcom.kcom[jsym - 1])); */
	/*     jicom = 1; */
	/* } */

  if(!(t = arg()) || !token_is_string(t)) {
    return FALSE;
  }

  ncsym = strlen(t->str);

	/* - Determine number of characters to check. */
	if ( nckey != ncsym )
	    return FALSE ;

	ncheck = nckey;

        ncheck = min(ncheck,MCHECK);
	if( ncabb > 0 )
	    ncheck = max( ncheck, ncabb - 1 );

	/* - Convert current command token upper case. */
	modcase( TRUE, ktoken, ncheck, ktoken );

  if(strncasecmp(t->str, kcheck, ncheck) == 0) {
    arg_next();
    return TRUE;
  }
  return FALSE;

L_8888:

	return( lckeyExact_v );

}

