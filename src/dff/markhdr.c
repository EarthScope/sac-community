/** 
 * @file   markhdr.c
 * 
 * @brief  Mark a header
 * 
 */

#include <string.h>

#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "co.h"

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

	int jdfl, nerr, ntused;

	/* - If we are to mark all of the subplots, for each subplot: */
	for( jdfl = jdfl1; jdfl <= jdfl2; jdfl++ ){

		/* -- Get header. */
		getfil( jdfl, FALSE, &ntused, &ntused, &ntused, &nerr );
		if( nerr != 0 )
			goto L_8888;

		/* -- Set value of marker. */
		if( kvmknm[0] == 'A' ){
			*a = vmk;
			fstrncpy( ka, 8, kimk, strlen(kimk));
			}
		else if( kvmknm[0] == 'O' ){
			*o = vmk;
			fstrncpy( ko, 8, kimk, strlen(kimk));
			}
		else if( kvmknm[0] == 'F' ){
			*f = vmk;
			fstrncpy( kf, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T0",2) == 0 ){
			*t0 = vmk;
			fstrncpy( kt0, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T1",2) == 0 ){
			*t1 = vmk;
			fstrncpy( kt1, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T2",2) == 0 ){
			*t2 = vmk;
			fstrncpy( kt2, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T3",2) == 0 ){
			*t3 = vmk;
			fstrncpy( kt3, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T4",2) == 0 ){
			*t4 = vmk;
			fstrncpy( kt4, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T5",2) == 0 ){
			*t5 = vmk;
			fstrncpy( kt5, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T6",2) == 0 ){
			*t6 = vmk;
			fstrncpy( kt6, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T7",2) == 0 ){
			*t7 = vmk;
			fstrncpy( kt7, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T8",2) == 0 ){
			*t8 = vmk;
			fstrncpy( kt8, 8, kimk, strlen(kimk));
			}
		else if( memcmp(kvmknm,"T9",2) == 0 ){
			*t9 = vmk;
			fstrncpy( kt9, 8, kimk, strlen(kimk));
			}

		/* -- Give header back to memory manager. */
		putfil( jdfl, &nerr );
		if( nerr != 0 )
			goto L_8888;

		}

L_8888:
	if(jdflrestore > 0) getfil( jdflrestore, FALSE, &ntused, &ntused, &ntused, &nerr );
	return;

} /* end of function */

