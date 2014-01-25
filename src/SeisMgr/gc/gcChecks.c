#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "timefuncs.h"
#include "sm_gc.h"

#define FUTURE 9999364


/* Routines that check if a given record from a given table is connected
   to a wfdisc in the tree.  Return TRUE if the record does connect */


/* If wfid in wftag matchs a wfid in any wfdisc, return TRUE.
   If it does not match any of them, return FALSE */

int gcCheckWftag ( struct wftag * wt , struct wfdiscList * wfL )
{
	int ID ;

	for ( ; wfL ; wfL = wfL->next ) {
	    ID = wfL->element->wfid ;
	    if ( wt->wfid == ID ) 
		return TRUE ;
	}

	return FALSE ;
}


/* If sta and chan from sensor both match respective fields in any wfdisc,
   and if begin time of wfdisc >= begin of sensor, and if endtime of wfdisc
   <= endtime of sensor, return TRUE.  Else return FALSE */

int gcCheckSensor ( struct sensor * se , struct wfdiscList * wfL )
{
	char * ST , * CH ;
	double TI , ET ;

	for ( ; wfL ; wfL = wfL->next ) {
	    ST = wfL->element->sta ;
	    CH = wfL->element->chan ;
	    TI = wfL->element->time ;
	    ET = wfL->element->endtime ;

	    if ( !strcmp ( ST , se->sta ) && !strcmp ( CH , se->chan ) &&
		 TI >= se->time && ET <= se->endtime )
		return TRUE ;
	}

	return FALSE ;
}


/* If sta and chan from sitechan both match respective fields in any wfdisc,
   and if the begin time of wfdisc >= begining of the ondate in sitechan, and
   if endtime of wfdisc <= the end of the offdate in sitechan, return TRUE. 
   Else return FALSE */

int gcCheckSitechan ( struct sitechan * sc , struct wfdiscList * wfL )
{
        char * ST , * CH ; 
        double TI , ET ;

        for ( ; wfL ; wfL = wfL->next ) {
            ST = wfL->element->sta ;
            CH = wfL->element->chan ;
            TI = wfL->element->time ;
            ET = wfL->element->endtime ;
        
            if ( !strcmp ( ST , sc->sta ) && !strcmp ( CH , sc->chan ) &&
                 TI >= julianToEpoch ( sc->ondate , TRUE ) &&
		( sc->offdate < 0 || ET <= julianToEpoch ( sc->offdate < 0 ? FUTURE : sc->offdate , FALSE ) ) )
                return TRUE ;
        }

        return FALSE ;
}


/* If sta from site matches sta in any wfdisc,
   and if the begin time of wfdisc >= begining of the ondate in site, and
   if endtime of wfdisc <= the end of the offdate in site, return TRUE. 
   Else return FALSE */

int gcCheckSite ( struct site * si , struct wfdiscList * wfL )
{
        char * ST ;
        double TI , ET ;

        for ( ; wfL ; wfL = wfL->next ) {
            ST = wfL->element->sta ;
            TI = wfL->element->time ;
            ET = wfL->element->endtime ;

            if ( !strcmp ( ST , si->sta ) && 
                 TI >= julianToEpoch ( si->ondate , TRUE ) &&
                ( si->offdate < 0 || ET <= julianToEpoch ( si->offdate < 0 ? FUTURE : si->offdate , FALSE ) ) )
                return TRUE ;
        }

        return FALSE ;
}




/* If inid in instrument matchs an inid in any sensor, return TRUE.
   If it does not match any of them, return FALSE */

int gcCheckInstrument ( struct instrument * in , struct sensorList * sL )
{
        int ID ;

        for ( ; sL ; sL = sL->next ) {
            ID = sL->element->inid ;
            if ( in->inid == ID )
                return TRUE ;
        }

        return FALSE ;
}



char * lower ( char * instring )
{
	int idx , ndx ;
	static char outstring[ 1001 ] ;

	ndx = strlen ( instring ) ;

	if ( ndx > 1000 ) {
	    printf ( "ERROR in lower, string too long.\n" ) ;
	    outstring[ 0 ] = '\0' ;
	    return outstring ;
	}

	for ( idx = 0 ; idx < ndx ; idx++ )
	    outstring[ idx ] = tolower ( instring[ idx ] ) ;

	outstring[ idx ] = '\0' ;

	return outstring ;
}



/* If evid in event matchs an evid in any wftag where tagname = "evid",
   return TRUE.  If it does not match any of them, return FALSE */

int gcCheckEvent ( struct event * ev , struct wftagList * wtL )
{
	char *tagname ;

        for ( ; wtL ; wtL = wtL->next ) {
	    tagname = lower( wtL->element->tagname ) ;

            if( strcmp( tagname, "evid" ) )
                continue ;

            if ( ev->evid == wtL->element->tagid )
                return TRUE ;
        }

        return FALSE ;
}



/* If orid in origin matchs a prefor in an event with equal evid, return TRUE.
   If it does not match any of them, return FALSE */

int gcCheckOrigin ( struct origin * orig , struct eventList * evL )
{
        for ( ; evL ; evL = evL->next ) {
	    if( evL->element->evid == orig->evid ) {
		if( evL->element->prefor == orig->orid ) {
		    return TRUE ;
		}
	    }
	}

        return FALSE ;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 *		NOTE:  cssStrucs, and cssListStrucs do not	       *

 *		contain the network table, so the following	       *

 *		routine is neither possible, nor necessary.	       *

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* If net in network matchs a net in any affiliation, return TRUE.
   If it does not match any of them, return FALSE */
/*
int gcCheckNetwork ( struct network * nw , struct affiliationList * afL )
{
        char * net ;

        for ( ; afL ; afL = afL->next ) {
            net = afL->element->net ;
            if ( !strcmp ( nw->net , net ) )
                return TRUE ;
        }

        return FALSE ;
}

*/
