
#include <string.h>

#include "ssi.h"
#include "bool.h"

/* return 0 if next token is OFF,
   return 1 if next token is ON,
   return 2 if next token is neither (implies ON, but doesn't occupy the space ) */
int OnOrOff ( inString )
char * inString ;
{
    int OnOrOff_v ;
    char * dumb = inString + 1 ;
    dumb += strlen ( inString ) ;
    while ( *dumb == ' ' || *dumb == '\t' ) dumb++ ;

    if ( strncmp ( dumb , "ON" , 2 ) == 0 ||
         strncmp ( dumb , "on" , 2 ) == 0 ||
         strncmp ( dumb , "On" , 2 ) == 0 ||
         strncmp ( dumb , "oN" , 2 ) == 0 )
        OnOrOff_v = TRUE ;

    else if ( strncmp ( dumb , "OF" , 2 ) == 0 ||
              strncmp ( dumb , "of" , 2 ) == 0 ||
              strncmp ( dumb , "Of" , 2 ) == 0 ||
              strncmp ( dumb , "oF" , 2 ) == 0 )
        OnOrOff_v = FALSE ;

    else
        OnOrOff_v = TRUE + 1 ;

    return OnOrOff_v ;

} /* end OnOrOff () */


