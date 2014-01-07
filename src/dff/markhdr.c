/** 
 * @file   markhdr.c
 * 
 * @brief  Mark a header
 * 
 */

#include <string.h>

#include "amf.h"
#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "co.h"
#include "errors.h"
/** 
 * Mark a header value
 * 
 * @param jdflrestore 
 *    File id to restore after setting all files
 * @param jdfl1 
 *    First file 
 * @param jdfl2 
 *    Last file
 * @param kvmknm 
 *    Header field to mark
 * @param vmk 
 *    Header field
 * @param kimk 
 *    Header field string marker
 *
 */
void 
markhdr(int    jdflrestore,
	int    jdfl1, 
	int    jdfl2, 
	char  *kvmknm, 
	double vmk, 
	char  *kimk) {

	int jdfl, nerr;
  sac *s;
	/* - If we are to mark all of the subplots, for each subplot: */
	for( jdfl = jdfl1; jdfl <= jdfl2; jdfl++ ){

		/* -- Get header. */
    if(!(s = sacget(jdfl-1, TRUE, &nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, FALSE, &ntused, &ntused, &ntused, &nerr );

		/* -- Set value of marker. */
		if( kvmknm[0] == 'A' ){
			s->h->a = vmk;
			fstrncpy( s->h->ka, 8, kimk, strlen(kimk));
			}
		else if( kvmknm[0] == 'O' ){
			s->h->o = vmk;
			fstrncpy( s->h->ko, 8, kimk, strlen(kimk));
			}
		else if( kvmknm[0] == 'F' ){
			s->h->f = vmk;
			fstrncpy( s->h->kf, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T0",2) == 0 ){
			s->h->t0 = vmk;
			fstrncpy( s->h->kt0, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T1",2) == 0 ){
			s->h->t1 = vmk;
			fstrncpy( s->h->kt1, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T2",2) == 0 ){
			s->h->t2 = vmk;
			fstrncpy( s->h->kt2, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T3",2) == 0 ){
			s->h->t3 = vmk;
			fstrncpy( s->h->kt3, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T4",2) == 0 ){
			s->h->t4 = vmk;
			fstrncpy( s->h->kt4, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T5",2) == 0 ){
			s->h->t5 = vmk;
			fstrncpy( s->h->kt5, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T6",2) == 0 ){
			s->h->t6 = vmk;
			fstrncpy( s->h->kt6, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T7",2) == 0 ){
			s->h->t7 = vmk;
			fstrncpy( s->h->kt7, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T8",2) == 0 ){
			s->h->t8 = vmk;
			fstrncpy( s->h->kt8, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T9",2) == 0 ){
			s->h->t9 = vmk;
			fstrncpy( s->h->kt9, 8, kimk, strlen(kimk));
			}

		}

L_8888:
	if(jdflrestore > 0) {
    //getfil( jdflrestore, FALSE, &ntused, &ntused, &ntused, &nerr );
    s = sacget(jdflrestore-1, FALSE, &nerr);
  }
	return;

} /* end of function */

