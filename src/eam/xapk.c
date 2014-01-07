/** 
 * @file   xapk.c
 * 
 * @brief  Execute the command APK
 * 
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "eam.h"
#include "bool.h"
#include "hdr.h"
#include "dfm.h"
#include "amf.h"
#include "gam.h"
#include "ucf.h"


#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "gam.h"
#include "dff.h"

/** 
 * Execute the action command APK to apply an automatic first arrival
 *    picker to data in memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @note Local Variables
 *   - LPKERR  .TRUE. if a pick couldn't be found for all files.
 *   - LPKFND  Array of length MDFL.  LPKFND(J) is .TRUE. if a pick
 *             was found for file J in DFL.
 *   - NDXPK   Relative index in SACMEM array of pick.
 *   - NLNCDA  Length in samples of event.
 *
 * @date   860625:  Added LVALPK option to enable/disable validation phase.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810220:  Deleted some parameters that weren't being used.
 * @date   810120:  Changed to error message retrieval from disk.
 * @date   800930:  Changed name from APKVAR.
 * @date   800530:  Original version.
 *
 */
void 
xapk(int *nerr)
{
	char kmsg[MCMSG+1];
	int lpkerr, lpkfnd[MDFL];
	char kdir, kqual, ktype;
	int i7, jdfl, ncerr, ndxpk, 
	 nexday, nlncda, npkmsc, npksec, npmsec, npsec;
	int *const Lpkfnd = &lpkfnd[0] - 1;
  sac *s;
	*nerr = 0;
    memset(kmsg, 0, MCMSG+1);
    memset(lpkfnd, 0, MDFL);
    nlncda = 0;
	/* PARSING PHASE: */
	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

		/* -- "C1 v":  change value of constant C1 */
		if( lkreal( "C1$",4, &cmeam.c1 ) ){

			/* -- "C2 v":  change value of constant C2 */
			}
		else if( lkreal( "C2$",4, &cmeam.c2 ) ){

			/* -- "C3 v":  change value of constant C3 */
			}
		else if( lkreal( "C3$",4, &cmeam.c3 ) ){

			/* -- "C4 v":  change value of constant C4 */
			}
		else if( lkreal( "C4$",4, &cmeam.c4 ) ){

			/* -- "C5 v":  change value of constant C5 */
			}
		else if( lkreal( "C5$",4, &cmeam.c5 ) ){

			/* -- "C6 v":  change value of constant C6 */
			}
		else if( lkreal( "C6$",4, &cmeam.c6 ) ){

			/* -- "C7 v":  change value of constant C7 */
			}
		else if( lkreal( "C7$",4, &cmeam.c7 ) ){

			/* -- "C8 v":  change value of constant C8 */
			}
		else if( lkreal( "C8$",4, &cmeam.c8 ) ){

			/* -- "I3 v":  change value of constant I3 */
			}
		else if( lkint( "I3$",4, &cmeam.i3 ) ){

			/* -- "I4 v":  change value of constant I4 */
			}
		else if( lkint( "I4$",4, &cmeam.i4 ) ){

			/* -- "D5 v":  change value of constant D5 */
			}
		else if( lkreal( "D5$",4, &cmeam.d5 ) ){

			/* -- "I6 v":  change value of constant I6 */
			}
		else if( lkint( "I6$",4, &cmeam.i6 ) ){

			/* -- "I7 v":  change value of constant I7 */
			}
		else if( lkint( "I7$",4, &i7 ) ){

			/* -- "D8 v":  change value of constant D8 */
			}
		else if( lkreal( "D8$",4, &cmeam.d8 ) ){

			/* -- "D9 v":  change value of constant D9 */
			}
		else if( lkreal( "D9$",4, &cmeam.d9 ) ){

			/* -- "VALIDATION ON/OFF": modify validation option. */
			}
		else if( lklog( "VALIDAT$",9, &cmeam.lvalpk ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */
	/* - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */
	/* - For each file in DFL: */
	lpkerr = FALSE;
	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

		/* -- Get file from memory manager. */
		if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		if( *nerr != 0 )
			goto L_8888;

		/* -- Try to detect a valid pick. */
		pkdet( s->y, s->h->npts, s->h->delta, 1, &ndxpk );

		/* -- If a valid pick was detected: */

		if( ndxpk > 0 ){
			Lpkfnd[jdfl] = TRUE;

			/* --- Characterize the pick as to quality and direction of first motion. */
			if( cmeam.lvalpk ){
				pkchar( s->y, s->h->npts, s->h->delta, ndxpk, &ktype, 
				 &kdir, &kqual );
                                fstrncpy(kmeam.kpwave, 8, (char *)&ktype, 1);
                                fstrncpy(kmeam.kpwave+1, 7, "P", 1);
                                fstrncpy(kmeam.kpwave+2, 6, (char *)&kdir, 1);
                                fstrncpy(kmeam.kpwave+3, 5, (char *)&kqual, 1);
				}
			else{
				strcpy( kmeam.kpwave, kmhdr.kundef );
				}

			/* --- Evaluate the pick, determining duration, maximum amplitudes, etc. */
			if( cmeam.lvalpk )
				pkeval( s->y, s->h->npts, s->h->delta, ndxpk, &nlncda );

			/* --- Store results in SAC header fields. */
			s->h->a = s->h->b + (float)( ndxpk - 1 ) * s->h->delta;
			strcpy( s->h->ka, kmeam.kpwave );
			if( nlncda > 0 )
				s->h->f = s->h->a + s->h->delta*(float)( nlncda );

			/* --- Write results to output and to HYPO pick file if open. */
			inctim( s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, s->h->a,
              &cmeam.nphour, &cmeam.npmin, &npsec, &npmsec, &nexday );
			cmeam.psecs = tosecs( npsec, npmsec );
			incdat( s->h->nzyear, s->h->nzjday, nexday, &cmeam.npyear, &cmeam.npjday );
			kidate( cmeam.npyear, cmeam.npjday, &cmeam.npmon, &cmeam.npday, 
			 &ncerr );
			cmeam.lpphas = TRUE;
			cmeam.lsphas = FALSE;
			cmeam.lampx = FALSE;
			if( nlncda > 0 ){
				cmeam.lfini = TRUE;
				cmeam.fmp = s->h->f - s->h->a;
				}
			else{
				cmeam.lfini = FALSE;
				}
			strcpy( kmeam.kstid, s->h->kstnm );
			whpf1( kmsg,MCMSG+1 );
			setmsg( "OUTPUT", 99 );
			apcmsg( kmsg,MCMSG+1 );
			outmsg();
			clrmsg();
			if( cmeam.lhpfop )
				{
                                fprintf(cmeam.nhpfun,"%s\n",kmsg);
				}

			/* --- Write results to card image pick file if open. */
			if( cmeam.lapfop ){
				inctim( s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, s->h->a, &cmeam.npkhr, 
				 &cmeam.npkmn, &npksec, &npkmsc, &nexday );
				cmeam.pksecs = tosecs( npksec, npkmsc );
				incdat( s->h->nzyear, s->h->nzjday, nexday, &cmeam.npkyr, &cmeam.npkjdy );
				strcpy( kmeam.kpkid, kmeam.kpwave );
				cmeam.pkampl = s->y[ndxpk-1];
				strcpy( kmeam.kpksrc, "A       " );
				strcpy( kmeam.kpkrid, "        " );
				strcpy( kmeam.kpkev, s->h->kevnm );
				strcpy( kmeam.kpkst, s->h->kstnm );
				cmeam.pkcmpa = s->h->cmpaz;
				cmeam.pkcmpi = s->h->cmpinc;
				wapf();
				if( nlncda > 0 ){
					inctim( s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, s->h->f, 
					 &cmeam.npkhr, &cmeam.npkmn, &npksec, &npkmsc, 
					 &nexday );
					cmeam.pksecs = tosecs( npksec, npkmsc );
					incdat( s->h->nzyear, s->h->nzjday, nexday, &cmeam.npkyr, 
					 &cmeam.npkjdy );
					strcpy( kmeam.kpkid, "F       " );
					cmeam.pkampl = s->y[ndxpk-1];
					strcpy( kmeam.kpksrc, "A       " );
					strcpy( kmeam.kpkrid, "        " );
					strcpy( kmeam.kpkev, s->h->kevnm );
					strcpy( kmeam.kpkst, s->h->kstnm );
					cmeam.pkcmpa = s->h->cmpaz;
					cmeam.pkcmpi = s->h->cmpinc;
					wapf();
					}
				}

			/* -- Set flag if no valid pick found for this file. */
			}
		else{
			Lpkfnd[jdfl] = FALSE;
			lpkerr = TRUE;
			}

		}

	/* - Write out message if picks weren't found for all files. */
	if( lpkerr ){
		setmsg( "WARNING", 1910 );
		for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
      if(!(s = sacget(jdfl-1, TRUE, nerr))) {
        goto L_8888;
      }
			if( !Lpkfnd[jdfl] ){
        apcmsg2(s->m->filename, strlen(s->m->filename)+1);
				}
			}
		outmsg();
		}

L_8888:
	return;
}

