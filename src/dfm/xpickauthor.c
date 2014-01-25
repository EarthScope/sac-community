/** 
 * @file   xpickauthor.c
 * 
 * @brief  Pick an author
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"

#include "errors.h"


#include "msg.h"
#include "bot.h"
#include "cpf.h"

/** 
 * Execute the command PICKAUTHOR which controls the picks read by readcss
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OUT_OF_MEMORY
 *
 * @date    970409:  Original version.  maf
 *
 */
void 
xpickauthor(int *nerr) {

    char prefsFileName [ MCPFN + 1 ] ;
    int nchar , idx , length , nAuthors = 0 ;

    *nerr = 0;

    /* - Look for order dependance key, FILE 
     * also if no tokens are present, assume FILE be default 
     */
    if ( !lcmore ( nerr ) || lckey ( "FILE$" , 6 ) ) {
	/* see if a filename was given. */
	if ( lcchar ( MCPFN , prefsFileName , MCPFN , &nchar ) ) {
	    prefsFileName [ nchar ] = '\0' ;
	    strcpy ( kmdfm.kprefsFileName , prefsFileName ) ;
	}

	/* get list of authors from user-defined file. */
	getprefs ( TRUE , FALSE ) ;
	return ;
    }

    if ( lckey ( "PHASE$" , 7 ) ) {
	/* see if a filename was given. */
        if ( lcchar ( MCPFN , prefsFileName , MCPFN , &nchar ) ) {
	    prefsFileName [ nchar ] = '\0' ;
	    strcpy ( kmdfm.kprefsFileName , prefsFileName ) ;
	}

	/* get authors and phases from the user-defined file. */
	getprefs ( TRUE , TRUE ) ;
	return ;
    }

    /* free up kmdfm.kauthors */
    if ( kmdfm.kauthors != NULL ) {
	for ( idx = 0 ; idx < cmdfm.iauthors ; idx++ )
	    free ( kmdfm.kauthors[idx] ) ;
	free ( kmdfm.kauthors ) ;
	cmdfm.iauthors = 0 ;
    }

    /* - Loop on each token in command: */
    while ( lcmore ( nerr ) ) {
	if ( nAuthors % 10 == 0 ) {
	  /* stop every 10 authors and get space */
	    char **temp = NULL ;

	    temp = ( char ** ) realloc ( (void *) kmdfm.kauthors ,
	      ( nAuthors + 10 ) * sizeof ( char * ) ) ;
	    if ( temp == NULL ) {
	      goto L_8888 ;
	    } else {
	      kmdfm.kauthors = temp ;
	    }
	} 

	/* allocate a string for the author name. */
	kmdfm.kauthors[nAuthors] = (char *) malloc ( 16 * sizeof ( char ) ) ;
	if ( kmdfm.kauthors[nAuthors] == NULL ) {
	    goto L_8888 ;
	} 

	/* copy the author name. */
	lcchar ( 15 , kmdfm.kauthors[nAuthors] , 15 , &length ) ;
	kmdfm.kauthors[nAuthors][length] = '\0' ;

	/* convert to lower case for case insensitive comparisons. */
	modcase ( FALSE , kmdfm.kauthors[nAuthors] ,
		  length, kmdfm.kauthors[nAuthors] ) ;

	nAuthors++ ; 

    } 

    /* save number of authors. */
    cmdfm.iauthors = nAuthors ;

    return ;

L_8888:
    *nerr = ERROR_OUT_OF_MEMORY ;
    setmsg ( "ERROR" , *nerr ) ;
    outmsg () ;
    clrmsg () ;

    if ( kmdfm.kauthors != NULL ) {
      for ( idx = 0 ; idx < nAuthors ; idx ++ )
	free ( kmdfm.kauthors[idx] ) ;
      free ( kmdfm.kauthors ) ;
    } 
}
