/** 
 * @file   xsynch.c
 * 
 * @brief  Sort 
 * 
 */

#include "dfm.h"
#include "bool.h"
#include "hdr.h"


#include "co.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"
#include "amf.h"
#include "array.h"
#include "errors.h"

/** 
 * Execute the command SORT to sort the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   831020:  Moved from XSC to internal DFM command.
 * @date   830915:  Merged SYNCH and SYNCH2 using new shell.
 * @date   830124:  Modifications due to new set of parsing/checking functions.
 * @date   810624:  Original version of XSC shell.
 *
 */
void 
xsynch(int *nerr) {

        int i, nb, *ndttmi, *ndttmo;
        float *begi, *bego, dtnew;

	static int lbegin = FALSE ; 
  sac *s;
	*nerr = 0;
  begi   = xarray_new_with_len('f', saclen());
  bego   = xarray_new_with_len('f', saclen());
  ndttmi = xarray_new_with_len('i', saclen() * 6);
  ndttmo = xarray_new_with_len('i', saclen() * 6);

	/* PARSING PHASE */
	/* - Parse each token in command (if any): */
	while ( lcmore( nerr ) ){

	    /* -- "ROUND [ON/OFF]":  change begin time rounding option. */
	    if( lklog( "R$",3, &cmdfm.lround ) )
	    { /* do nothing */ }

	    /* -- "BEGIN [ON/OFF]":  set both begin times to zero if ON. */
	    else if( lklog( "B#EGIN$",8 , &lbegin ) )
	    { /* do nothing */ }

	    else{
	        /* -- Bad syntax. */
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	if( *nerr != 0 )
	    return ;

	/* CHECKING PHASE: */
	/* - Test for a non-null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Make sure each file is an evenly spaced time series file. */
	vfeven( nerr );
	if( *nerr != 0 )
	    return ;

	/* EXECUTION PHASE: */
	/* - Save beginning offset and reference time for each file. */
	for( i = 0; i < saclen(); i++ ){
      if(!(s = sacget(i, TRUE, nerr))) {
        *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
        return;
      }
	    //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

	    begi[i] = s->h->b;
	    copyi( &s->h->nzyear, &ndttmi[i * 6], 6 );
	}

	/* - Calculate new reference times and beginning offsets. */
	synch( (int(*)[6]) ndttmi, begi, saclen(), (int(*)[6])ndttmo, bego ,lbegin );

	/* - For each file in DFL: */
	for( i = 0; i < saclen(); i++ ){
      if(!(s = sacget(i, TRUE, nerr))) {
        return;
      }
	    /* -- Get next file from the memory manager.
	     *    (Header is moved into common blocks CMHDR and KMHDR.) */
	    //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

	    /* -- Update time header fields.
	     *    New beginning offsets are set to exact value or they
	     *    may be rounded to the nearest multiple of DELTA. */
	    dtnew = bego[i] - s->h->b;
	    if( cmdfm.lround ){
		nb = (int)( bego[i]/ s->h->delta + sign( 0.5, s->h->b ) );
		s->h->b = s->h->delta*(float)( nb );
	    }
	    else{
		s->h->b = s->h->b + dtnew;
	    }
	    s->h->e = s->h->b + s->h->delta*(float)( s->h->npts - 1 );
	    if( s->h->a != cmhdr.fundef )
		s->h->a = s->h->a + dtnew;
	    if( s->h->f != cmhdr.fundef )
		s->h->f = s->h->f + dtnew;
	    if( s->h->o != cmhdr.fundef )
		s->h->o = s->h->o + dtnew;
	    if( s->h->t0 != cmhdr.fundef )
		s->h->t0 = s->h->t0 + dtnew;
	    if( s->h->t1 != cmhdr.fundef )
		s->h->t1 = s->h->t1 + dtnew;
	    if( s->h->t2 != cmhdr.fundef )
		s->h->t2 = s->h->t2 + dtnew;
	    if( s->h->t3 != cmhdr.fundef )
		s->h->t3 = s->h->t3 + dtnew;
	    if( s->h->t4 != cmhdr.fundef )
		s->h->t4 = s->h->t4 + dtnew;
	    if( s->h->t5 != cmhdr.fundef )
		s->h->t5 = s->h->t5 + dtnew;
	    if( s->h->t6 != cmhdr.fundef )
		s->h->t6 = s->h->t6 + dtnew;
	    if( s->h->t7 != cmhdr.fundef )
		s->h->t7 = s->h->t7 + dtnew;
	    if( s->h->t8 != cmhdr.fundef )
		s->h->t8 = s->h->t8 + dtnew;
	    if( s->h->t9 != cmhdr.fundef )
		s->h->t9 = s->h->t9 + dtnew;
	    copyi( &ndttmo[i * 6], &s->h->nzyear, 6 );


	}
  xarray_free(begi);
  xarray_free(bego);
  xarray_free(ndttmi);
  xarray_free(ndttmo);
	return;
}

