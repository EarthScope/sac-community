/** 
 * @file   lcrtw.c
 * 
 * @brief  Parse a reference time window
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"
#include "dfm.h"


#include "co.h"
#include "debug.h"

/** 
 * Parse a reference time window command construct
 * 
 * @param lrtw 
 *    - TRUE if the reference time window was turned on
 *    - FALSE if the reference time window was turned off
 * @param krtw 
 *    Reference Time Window
 *    First is starting time, second is stopping time 
 * @param krtw_s 
 *    Length of \p krtw
 * @param ortw 
 *    Reference Time window offset times
 *    First is starting time, second is stopping time 
 * 
 * @return 
 *    - TRUE if the time was found
 *    - FALSE if the time was not found
 *
 * @date   860319:  Fixed bug that prevented other command options
 *             from being parsed correctly if they followed a RTW.
 * @date   820623:  Changed form of output arguments.
 * @date   820610:  Rewrote to use standard parsing functions.
 * @date   820312:  Factored test for key to LCKEY function.
 * @date   810206:  Original version.
 *
 */
int 
lcrtw(int    *lrtw, 
      char   *krtw, 
      int     krtw_s, 
      double *ortw) {
  
#define KRTW(I_,J_)	(krtw+(I_)*(krtw_s)+(J_))

	char krtwb[9], krtwe[9];
	int lcrtw_v;
	int index, ipart, nerr;
	double ortwb, ortwe, realv;

	double *const Ortw = &ortw[0] - 1;

	/* - Assume RTW will not be found. */
	lcrtw_v = FALSE;

	/* - Turn RTW option on or off. */
	if( lclog( lrtw ) ){
	    lcrtw_v = TRUE;
	}

	/* - "SIGNAL" sets RTW to 1 second before first arrival
	 *   to 1 second after fini. */
	else if( lckey( "S$",3 ) ){
	    *lrtw = TRUE;
	    fstrncpy( KRTW(0,0),krtw_s-1,"A",1);
	    Ortw[1] = -1.;
	    fstrncpy( KRTW(1,0),krtw_s-1,"F",1);
	    Ortw[2] = 1.;
	    lcrtw_v = TRUE;
	}

	/* - General form of RTW:
	 *
	 *     BEG_REF BEG_OFF END_REF END_OFF
	 *
	 *   where the REF times refer to reference or marker times in the file
	 *   such as the file begin time (B) or the first arrival time (A),
	 *   or an user defined time (Tn) and OFF is a positive or
	 *   negative offset in seconds which is added to the reference time.
	 * */
	else{

	    /* -- Initialize local variables. */

	    ipart = 1;
	    strcpy( krtwb, "Z       " );	/* B for begin */
	    ortwb = 0.;
	    strcpy( krtwe, krtwb );
	    ortwe = 0.;

	    /* -- Loop on tokens. */
L_2000:
	    if( lcmore( &nerr ) ){
        /* --- Set beginning or ending reference parameter. */
        if( lclist( (char*)kmdfm.kpick,9, MPICK, &index ) ){
          if( ipart == 1 ){
            if( index == cmdfm.ipckn ){
              cerr( 1001 );
              goto L_8888;
            }
            strcpy( krtwb, kmdfm.kpick[index - 1] );
            strcpy( krtwe, krtwb );
            ipart = 2;
            goto L_2000;
          }
          else if( ipart <= 3 ){
            strcpy( krtwe, kmdfm.kpick[index - 1] );
            ipart = 4;
            goto L_2000;
          }
          else{
            arg_prev();
          }
        }
        
        /* --- Set beginning or ending offset parameter. */
        else if( lcreal( &realv ) ){
          if( ipart <= 2 ){
            ortwb = realv;
            ipart = 3;
            goto L_2000;
          }
          else if( ipart <= 4 ){
            ortwe = realv;
            ipart = 5;
          }
          else{
            arg_prev();
          }
        }
	    }

	    /* -- Set output parameters if a "relative time window" 
	     *    has been defined. */
	    lcrtw_v = ipart > 1;
	    if( lcrtw_v ){
        fstrncpy( KRTW(0,0),krtw_s-1,krtwb,strlen(krtwb));
        fstrncpy( KRTW(1,0),krtw_s-1,krtwe,strlen(krtwe));
        Ortw[1] = ortwb;
        Ortw[2] = ortwe;
        *lrtw = TRUE;
	    }
      DEBUG("lcrtw done %lf %lf\n", Ortw[1], Ortw[2]);
	}

L_8888:
	return( lcrtw_v );

#undef	KRTW

} 

