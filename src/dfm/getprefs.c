/** 
 * @file   getprefs.c
 * 
 * @brief  Read Pick Preferences file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dfm.h"
#include "bool.h"

#include "errors.h"

#include "string_utils.h"


#include "msg.h"
#include "bot.h"

/** 
 * Read pick preference file.
 * 
 * @param lauth 
 *    - TRUE  Read Author Lit
 *    - FALSE Do not read Author List
 * @param lphase 
 *    - TRUE Read Header Info
 *    - FALSE Do not read Header Info
 *
 *    970409:  Original version.  maf
 *
 */
void 
getprefs (int lauth, 
	  int lphase) {

  char  prefsLine [ 120 ] ;
  int   nAuthors = 0; 	        /* number of authors. */
  int   idx ;
  FILE *pFile ;		

    /* Open the file to count the number of authors in list. */
    pFile = fopen ( kmdfm.kprefsFileName , "r" ) ;
    if ( pFile == NULL ) {
        setmsg ( "WARNING" , ERROR_FILE_DOES_NOT_EXIST ) ;
        apcmsg ( kmdfm.kprefsFileName , strlen ( kmdfm.kprefsFileName ) ) ;
        apcmsg ( ".  readcss will not read picks." , 32 ) ;
        outmsg () ;
        clrmsg () ;
        return ;
    }

    while ( fgetsp ( prefsLine , 120 , pFile ) && !isspace ( prefsLine[ 0 ] ) )
        nAuthors++ ;

    if(nAuthors == 0) {
        fprintf(stderr, "Pick Preferences File %s is empty\n", kmdfm.kprefsFileName);
        fclose(pFile);
        return;
    }

    /* Reading Authors */
    if ( lauth ) {
	fclose ( pFile );

	/* free up space from previous authors ( if any ) */
	if ( kmdfm.kauthors != NULL ) {
	    for ( idx = 0 ; idx < cmdfm.iauthors ; idx++ )
		free( kmdfm.kauthors[idx] ) ;
	    free ( kmdfm.kauthors ) ;
	    cmdfm.iauthors = 0 ;
	} 

	/* allocate space for author names. */
	kmdfm.kauthors = (char **) calloc ( nAuthors , sizeof( char * ) ) ;
	if ( kmdfm.kauthors == NULL ) {
	    setmsg ( "ERROR" , ERROR_OUT_OF_MEMORY ) ;
	    apcmsg ( "  readcss will not read picks." , 31 ) ;
	    outmsg () ;
	    clrmsg () ;
	    return ;
	}
	for ( idx = 0 ; idx < nAuthors ; idx++ ) {
	    kmdfm.kauthors[ idx ] = ( char * ) calloc ( 16 , sizeof ( char ) ) ;
	    if ( kmdfm.kauthors[ idx ] == NULL ) {
		setmsg ( "ERROR" , ERROR_OUT_OF_MEMORY ) ;
		apcmsg ( "  readcss will not read picks." , 31 ) ;
		outmsg () ;
		clrmsg () ;
		nAuthors = idx ;
		goto L_8888 ;
	    }
	}

	/* Open the file to read it. */
	pFile = fopen ( kmdfm.kprefsFileName , "r" ) ;

	/* Read author names */
	for ( idx = 0 ; idx < nAuthors ; idx++ )
	{
	    char * firstWhiteSpace = NULL ;

	    fgetsp ( prefsLine , 120 , pFile ) ;
	    firstWhiteSpace = strpbrk ( prefsLine , " \n\t\v\f" ) ;
	    if ( firstWhiteSpace == NULL )
		prefsLine[15] = '\0' ;
	    else
		* firstWhiteSpace = '\0' ;

	    /* convert to lowercase for case insensitive comparisons. */
	    modcase ( FALSE , prefsLine , strlen( prefsLine ) , prefsLine ) ;

	    strcpy ( kmdfm.kauthors[idx] , prefsLine ) ;
	}

	/* Read blank line delimiter */
	fgetsp ( prefsLine , 120 , pFile ) ;
    } 

    /* if reading header information */
    if ( lphase ) {
	/* Read default phases and authors for individual pick header vars. */
	for ( idx = 0 ; idx < 10 ; idx++ )
	{
	    int check ;

	    check = fscanf ( pFile , "t%*d\t%s\t%s\n" , kmdfm.ktPh[idx] , 
							kmdfm.ktAu[idx] ) ;
	    if ( check != 2 || strlen ( kmdfm.ktPh[idx] ) > 8 || 
			       strlen ( kmdfm.ktAu[idx] ) > 15 ) {
		setmsg ( "WARNING" , ERROR_BADLY_FORMATTED_CSSPICKSPREFS ) ;
		apcmsg ( "  readcss will not read picks." , 31 ) ;
		outmsg () ;
		clrmsg () ;
		return ;
	    }

	    /* convert to lower case for case insensitive comparisons. */
	    modcase ( FALSE , kmdfm.ktAu[idx] , strlen( kmdfm.ktAu[idx] ) , 
			      kmdfm.ktAu[idx] ) ;
	}
    }

    if ( lauth )
	cmdfm.iauthors = nAuthors ;

    return ;

L_8888:
    /* on error free up allocated memory */
    for ( idx = 0 ; idx < nAuthors ; idx ++ ) 
	free ( kmdfm.kauthors[idx] ) ;

    free ( kmdfm.kauthors ) ;

    return ;
}
