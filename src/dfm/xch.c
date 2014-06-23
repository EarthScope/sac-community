/** 
 * @file   xch.c
 * 
 * @brief  Change a Header variable
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dfm.h"
#include "hdr.h"
#include "lhf.h"
#include "bool.h"
#include "cpf.h"

#include "msg.h"
#include "bot.h"
#include "ucf.h"
#include "dff.h"
#include "co.h"
#include "amf.h"
#include "array.h"
#include "errors.h"

/** 
 * @param MGDTTM 
 *    15 - Size of a time value
 */
#define	MGDTTM	15

/** 
 * Execute the command CHNHDR (CH), which changes values for header
 *    variables in memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   961111:  Changed variables *a, *b, *e, *f, and *o to *arrivl, 
 *                  *begin, *ennd, *fini, and *origin, respectively (see
 *                   hdr.h)  maf.
 * @date   960812:  Added FILE option to allow file specification, maf
 * @date   910808:  Added test for identical station-event location and set
 *                  AZ and BAZ values both to zero. 
 * @date   900517:  Fixed VAX bug in changing longer header character variables.
 * @date   840216:  Added ability to change header times to any GMT time.
 * @date   821004:  Added automatic recalculation of distance, azimuth, etc.
 * @date   820204:  Fixed bug in "ALLT" option.
 * @date   820119:  Fixed bug in changing KHDR values.
 * @date   811120:  Added recalculation of ENND everytime.
 * @date   810630:  Added a special "ALLT' option to change all header times.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void 
xch(int *nerr) {

	char khdrc[SAC_HEADER_STRINGS][SAC_HEADER_STRING_LENGTH], ktemp[19], ktok[9];
	int lallt, lfound, lhdrc[SAC_HEADER_LOGICALS], lnumbr, log;
	int icat[SAC_HEADER_WORDS], icatx, igdttm, ihdrc[SAC_HEADER_ENUMS], item[SAC_HEADER_WORDS];
	int itemx, ival, j, j1, jdfl;
	int nckhdr, ngdttm[MGDTTM][6], nhdrc[SAC_HEADER_INTEGERS];
	int nia, nitem;
	float diff, fhdrc[SAC_HEADER_FLOATS];
	static int icatg = -1;
  double vallt;
	/* 1 if the corresponding file is to be modified */
  int	 *doFile;
	int	 idx ;
  double fnumbr;
  sac *s;
  float *fp;
  int *ip;
	float *const Fhdrc = &fhdrc[0] - 1;
	int *const Icat = &icat[0] - 1;
	int *const Ihdrc = &ihdrc[0] - 1;
	int *const Item = &item[0] - 1;
	int *const Lhdrc = &lhdrc[0] - 1;
	int *const Nhdrc = &nhdrc[0] - 1;

	*nerr = 0;

  doFile = xarray_new_with_len('i', saclen());
  
	/* PARSING PHASE: */
	/* - Initialize several state variables. */
	nitem = 0;
	lallt = FALSE;
	igdttm = 1;

	/* modify all files by default */
	for ( idx = 0 ; idx < saclen() ; idx++ )
		doFile[idx] = TRUE ;
	
	/* - Parse position dependant tokens, ie FILE; maf 960812 */
	if ( lckeyExact ( "FILE", 5 )) {
    /* initialize doFile to FALSE */
    for ( idx = 0 ; idx < saclen() ; idx++ )
      doFile[idx] = FALSE ;
    
    /* set specified values of doFile to TRUE */
    while ( lcint ( &lnumbr ) ) {
      if ( lnumbr <= saclen() && lnumbr >= 1 ) 
        doFile[ lnumbr - 1 ] = TRUE ;
      else { 
        warning(1003, "%d", lnumbr);
        outmsg () ;
      }
    } 
	} 


	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){
	    /* -- Get next token from command. */
    if( lckeyExact( "ALLT$",6 ) ){
      if( lcreal( &vallt ) ){
        lallt = TRUE;
      } else {
        cerr( 1001 );
        goto L_8888;
      }
    } else if( lcchar( ktok,sizeof(ktok)) ) {
      hdrfld( ktok,9, &icatx, &itemx, &lfound );
      /* --- If it is the name of a SAC header field. */
      if(!lfound) {
		    cfmt( "ILLEGAL OPTION:",17 );
		    cresp();
      } else {
		    /* Block changes to certain header fields */
        /* nvhdr, norid, nevid, npts, nsnpts, nwfid */
		    if ( icatx == cmlhf.icatn && itemx >= 7 && itemx <= 12 ) {
          /* Increment token counter. */
          arg_next( );
          setmsg ( "WARNING" , 1389 ) ;
          outmsg () ;
          clrmsg () ;
          continue ;
		    }

		    /* ---- Save category and item number. */
		    nitem = nitem + 1;
		    Icat[nitem] = icatx;
		    Item[nitem] = itemx;
		    /* ---- Depending upon what kind of header item (floating, integer,
		     *      alphanumeric, etc.), verify and save next token. */
		    if( icatx == cmlhf.icatf ){
          if( lcreal( &fnumbr ) ) { 
            Fhdrc[itemx] = (float)fnumbr ;
          }
          else if( lckey( "UNDEF$",7 ) || !arg() ){
            Fhdrc[itemx] = cmhdr.fundef;
          }
          else if( lkia( "GMT$",5, 1, 6, &ngdttm[igdttm - 1][0], &nia ) ){
            Icat[nitem] = icatg;
            Fhdrc[itemx] = (float)( igdttm );
            for( j = nia ; j < 6; j++ ){
              ngdttm[igdttm - 1][j] = 0.;
            }
            if( ngdttm[igdttm - 1][0] <= 99 ) {
              time_t t;
              char *tmptm;
              int century, year;
              
              t = time(NULL);
              tmptm = ctime(&t);
              
              year = atoi ( tmptm + 22 ) ;
              century = atoi ( tmptm + 20 ) - year ;
              
              if ( ngdttm[igdttm - 1][0] > year )
                century -= 100 ;
              
              ngdttm[igdttm - 1][0] = century + ngdttm[igdttm - 1][0];
            }
            igdttm = igdttm + 1;
          }
          else{
            cerr( 1001 );
            goto L_8888;
          }
		    }
		    else if( icatx == cmlhf.icatn ){
          if( lcint( &Nhdrc[itemx] ) )
            { /* do nothing */ }
          else if( lckey( "UNDEF$",7 ) ){
            Nhdrc[itemx] = cmhdr.fundef;
          }
          else{
            cerr( 1001 );
            goto L_8888;
          }
		    }
		    else if( icatx == cmlhf.icati ){
          ival = 0;
          if( lclist( (char*)kmlhf.kiv,9, SAC_ENUMS-1, &ival ) ){
            Ihdrc[itemx] = ival;
          }
          else if( lckey( "UNDEF$",7 ) ){
            Ihdrc[itemx] = cmhdr.iundef;
          }
          else{
            cerr( 1001 );
            goto L_8888;
          }
		    }
		    else if( icatx == cmlhf.icatl ){
          if( lclog2( "TRUE$",6, "FALSE$",7, &log ) ){
            Lhdrc[itemx] = log;
          }
          else if( lclog2( "YES$",5, "NO$",4, &log ) ){
            Lhdrc[itemx] = log;
          }
          else if( lclog2( ".TRUE.$",8, ".FALSE.$",9, &log ) ){
            Lhdrc[itemx] = log;
          }
          else if( lckey( "UNDEF$",7 ) ){
            Lhdrc[itemx] = FALSE;
          }
          else{
            cerr( 1001 );
            goto L_8888;
          }
		    }
		    else if( icatx == cmlhf.icatk ){
          int n = Nkhdr[itemx];
          j1 = itemx;
          memset(khdrc[j1-1], ' ', n*8);
          if( lckey( "UNDEF$",7 ) ){
            /* Disallow undefined kstnm and kcmpnm */
            if ( itemx == 1 || itemx == 21 ) {
              nitem-- ;
              setmsg ( "WARNING" , 1390 ) ;
              outmsg () ;
              clrmsg () ;
              continue ;
            }
            strncpy( khdrc[j1 - 1], SAC_CHAR_UNDEFINED, strlen(SAC_CHAR_UNDEFINED) );
          }
          else{
            strcpy( ktemp, "                  " );
            nckhdr = SAC_HEADER_STRING_LENGTH_FILE * Nkhdr[itemx];
            if( lcchar_base( ktemp, sizeof(ktemp)) ){
              strncpy(khdrc[j1-1], ktemp, strlen(ktemp));
            }
          }
          khdrc[j1-1][n*8] = 0;
		    }
		    else{
          *nerr = 901;
          setmsg( "ERROR", *nerr );
          apcmsg( "CHNHDR",7 );
          goto L_8888;
		    }
      }
    }
    else{
      /* -- Bad syntax. */
      cfmt( "ILLEGAL OPTION:",17 );
      cresp();
    }
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - For each file in data file list: */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
	    if ( doFile [ jdfl - 1 ] ) {	/* file specification. maf 960812 */

        if(!(s = sacget(jdfl-1, FALSE, nerr))) {
          goto L_8888;
        }
        //getfil( jdfl, FALSE, &junk1, &junk2, &junk3, nerr );

		/* -- Update appropriate header fields. */
		for( j = 1; j <= nitem; j++ ){
      switch(Icat[j]) {
      case CAT_FLOAT:
        fp = fhdr(s, Item[j]);
        VALUE(fp) = Fhdrc[Item[j]];
        if ( Item[ j ] >= 6 && Item[ j ] <= 20 )
			    iztypeMessage ( Item[ j ] , s->h->iztype ) ;
        break;
      case -1: // Set Time GMT
        igdttm = (int)( Fhdrc[Item[j]] );
        ddttm( &ngdttm[igdttm - 1][0], &s->h->nzyear, &diff );
        fp = fhdr(s,Item[j]);
        *fp = diff;
        break;
      case CAT_NUMBER:
        ip = nhdr(s, Item[j]);
        VALUE(ip) = Nhdrc[Item[j]];
        break;
      case CAT_ENUM:
        ip = ihdr(s, Item[j]);
        VALUE(ip) = Ihdrc[Item[j]];
        break;
      case CAT_LOGICAL:
        ip = lhdr(s, Item[j]);
        VALUE(ip) = Lhdrc[Item[j]];
        break;
      case CAT_STRING:
        strcpy(khdr(s,Item[j]), khdrc[Item[j]-1]);
        break;
      }
		}

		/* -- Change all time fields if requested. */
		if( lallt ){
		    s->h->b = s->h->b + vallt;
		    s->h->e = s->h->e + vallt;
		    if( s->h->nzyear != cmhdr.nundef )
			idttm( &s->h->nzyear, -vallt, &s->h->nzyear );
		    if( s->h->a != cmhdr.fundef )
			s->h->a = s->h->a + vallt;
		    if( s->h->f != cmhdr.fundef )
			s->h->f = s->h->f + vallt;
		    if( s->h->o != cmhdr.fundef )
			s->h->o = s->h->o + vallt;
		    if( s->h->t0 != cmhdr.fundef )
			s->h->t0 = s->h->t0 + vallt;
		    if( s->h->t1 != cmhdr.fundef )
			s->h->t1 = s->h->t1 + vallt;
		    if( s->h->t2 != cmhdr.fundef )
			s->h->t2 = s->h->t2 + vallt;
		    if( s->h->t3 != cmhdr.fundef )
			s->h->t3 = s->h->t3 + vallt;
		    if( s->h->t4 != cmhdr.fundef )
			s->h->t4 = s->h->t4 + vallt;
		    if( s->h->t5 != cmhdr.fundef )
			s->h->t5 = s->h->t5 + vallt;
		    if( s->h->t6 != cmhdr.fundef )
			s->h->t6 = s->h->t6 + vallt;
		    if( s->h->t7 != cmhdr.fundef )
			s->h->t7 = s->h->t7 + vallt;
		    if( s->h->t8 != cmhdr.fundef )
			s->h->t8 = s->h->t8 + vallt;
		    if( s->h->t9 != cmhdr.fundef )
			s->h->t9 = s->h->t9 + vallt;
		}

		/* -- Recompute ending time if appropriate. */
		if( s->h->leven )
      s->h->e = CALC_E(s);

		/* -- Recompute distance, azimuth, etc. if proper header fields are present. */
    update_distaz(s);

	    } /* end if ( doFile [ jdfl - 1 ] ). maf 960812 */
	} /* end for loop between files */

 L_8888:
  xarray_free(doFile);
	return;
}

