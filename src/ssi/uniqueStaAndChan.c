
#include <string.h>

#include "amf.h"
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
    sac *s;
    s = sacget_current();
    if( set_default_station_name(OPTION_GET) == OPTION_OFF ) {
      return returnValue;
    }
    if ( !strcmp ( s->h->kstnm , SAC_CHAR_UNDEFINED ) ) {
	int idx ;
	DBlist tree ;

	/* get tree for default wordset */
	tree = smGetDefaultTree () ;

	strcpy ( s->h->kstnm , MakeUniqueSiteName ( tree , "sta" ) ) ;

	if ( strlen ( s->h->kstnm ) < 8 ) {
	    for ( idx = strlen ( s->h->kstnm ) ; idx < 8 ; idx++ )
		s->h->kstnm[ idx ] = ' ' ;
	    s->h->kstnm[ 8 ] = '\0' ;
	}

	returnValue = TRUE ;
    }

    if ( !strcmp ( s->h->kcmpnm , SAC_CHAR_UNDEFINED ) ) {
	int idx ;
	DBlist tree ;

	/* get tree for default wordset */
	tree = smGetDefaultTree () ;

	strcpy ( s->h->kcmpnm , MakeUniqueChanName ( tree , s->h->kstnm , "Q" ) ) ;

        if ( strlen ( s->h->kcmpnm ) < 8 ) {
            for ( idx = strlen ( s->h->kcmpnm ) ; idx < 8 ; idx++ )
                s->h->kcmpnm[ idx ] = ' ' ;
            s->h->kcmpnm[ 8 ] = '\0' ;
        }

	returnValue = TRUE ;
    }

    return returnValue ;
}
