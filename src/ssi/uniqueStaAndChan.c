
#include <string.h>

#include "ssi.h"
#include "hdr.h"
#include "bool.h"

#include "select.h"

#include "smDataIO.h"
#include "cssListOps/cssListOps.h"


int
set_default_station_name(int getset) {
  static int default_station_names = OPTION_OFF;
  if(getset != OPTION_GET) {
    default_station_names = getset;
  }
  return default_station_names;
}

/*
	This routine makes sure that station names and components (channels)
	are defined.  If one is undefigned, it is given a unique name.  

	Design Note:  The definision and initialization of the tree
		      variable is distributed into the if blocks because
		      this function will be called a lot, and will usually
		      not be needed.  The code will run a little faster
		      by not defining and initializing the tree in the 
		      vast majority of cases where it will not be used.
*/

int uniqueStaAndChan ( ) 
{
    int returnValue = FALSE ;

    if( set_default_station_name(OPTION_GET) == OPTION_OFF ) {
      return returnValue;
    }

    if ( !strcmp ( kstnm , SAC_CHAR_UNDEFINED ) ) {
	int idx ;
	DBlist tree ;

	/* get tree for default wordset */
	tree = smGetDefaultTree () ;

	strcpy ( kstnm , MakeUniqueSiteName ( tree , "sta" ) ) ;

	if ( strlen ( kstnm ) < 8 ) {
	    for ( idx = strlen ( kstnm ) ; idx < 8 ; idx++ )
		kstnm[ idx ] = ' ' ;
	    kstnm[ 8 ] = '\0' ;
	}

	returnValue = TRUE ;
    }

    if ( !strcmp ( kcmpnm , SAC_CHAR_UNDEFINED ) ) {
	int idx ;
	DBlist tree ;

	/* get tree for default wordset */
	tree = smGetDefaultTree () ;

	strcpy ( kcmpnm , MakeUniqueChanName ( tree , kstnm , "Q" ) ) ;

        if ( strlen ( kcmpnm ) < 8 ) {
            for ( idx = strlen ( kcmpnm ) ; idx < 8 ; idx++ )
                kcmpnm[ idx ] = ' ' ;
            kcmpnm[ 8 ] = '\0' ;
        }

	returnValue = TRUE ;
    }

    return returnValue ;
}
