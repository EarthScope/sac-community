/** 
 * @file   timecheck.c
 * 
 * @brief  Normalize a time
 * 
 */

#include "ncpf.h"
#include "ucf.h"

/** 
 * Normalize a time 
 * 
 * @param year 
 *    Year
 * @param day 
 *    Day of the year, [ 1 and 365/366 ]
 * @param hour 
 *    Hour [ 0 .. 23 ]
 * @param min 
 *    Minute [ 0 .. 59 ]
 * @param sec 
 *    Seconds [ 0 .. 59 ]
 * @param ms 
 *    Milliseconds [ 0 .. 999 ]
 *
 */
void 
timecheck( int *year, 
           int *day, 
           int *hour, 
           int *min,
           int *sec, 
           int *ms ) {

  /* Milliseconds */
      while( *ms > 999 ) {
         *ms -= 1000 ;
         (*sec)++ ;
      }
      while( *ms < 0 ) {
         *ms += 1000 ;
         (*sec)-- ;
      }

  /* Seconds */
      while( *sec > 59 ) {
         *sec -= 60 ;
         (*min)++ ;
      }
      while( *sec < 0 ) {
         *sec += 60 ;
         (*min)-- ;
      }

  /* Minutes */
      while( *min > 59 ) {
         *min -= 60 ;
         (*hour)++ ;
      }
      while( *min < 0 ) {
         *min += 60 ;
         (*hour)-- ;
      }

  /* Hours */
      while( *hour > 23 ) {
         *hour -= 24 ;
         (*day)++ ;
      }
      while( *hour < 0 ) {
         *hour += 24 ;
         (*day)-- ;
      }

  /* Days and Years */
      while( *day > ( isleap( *year ) ? 366 : 365 ) ) {
         (*year)++ ;
         *day -= isleap( *year ) ? 366 : 365 ;
      }
      while( *day < 1 ) {
         (*year)-- ;
         *day += isleap( *year ) ? 366 : 365 ;
      }
} 
