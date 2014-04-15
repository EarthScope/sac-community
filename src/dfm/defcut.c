/** 
 * @file   defcut.c
 * 
 * @brief  Define cut parameters
 * 
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dfm.h"
#include "hdr.h"

#include "errors.h"

#include "msg.h"
#include "clf.h"
#include "bot.h"

//#define __DEBUG__
#include "debug.h"

void cut_define(float b, float delta, double dt, int *n);
void cut_define_check(float start, float stop, int npts, int cuterr, int *nstart, int *nstop, int *nfillb, int *nfille, int *nerr);

/** 
 * Define cut parameters for a given data file
 * 
 * @param kcut
 *    Which parameter
 * @param ocut
 *    Cut times
 * @param idfl
 *    Data file list number
 * @param nerr
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR
 *    - ERROR_UNDEFINED_START_CUT_TIME
 *    - ERROR_UNDEFINED_STOP_CUT_TIME
 *    - ERROR_START_TIME_LESS_THAN_BEGIN
 *    - ERROR_STOP_TIME_GREATER_THAN_END
 *    - ERROR_START_TIME_GREATER_THAN_END
 *    - ERROR_STOP_TIME_LESS_THAN_BEGIN
 *    - ERROR_START_TIME_GREATER_THAN_STOP
 *    - ERROR_CORRECTED_BY_FILL_WITH_ZEROS
 *    - ERROR_CORRECTED_BY_USING_START_TIME
 *    - ERROR_CORRECTED_BY_USING_END_TIME
 *
 * @date   961211, 970114, 970214, and 970304:  
 *             I made incremental changes to harden cut.  It now cuts
 *             precisely on the values entered.  maf
 * @date   880128:  Fixed bug that occurred when the sampling interval
 *             was smaller than the machine roundoff factor.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   840228:  Original version from DEFMEM.
 *
 */
void 
defcut(char   kcut[2][9], 
       double ocut[2],
       int    idfl, 
       int   *nerr) {

	int jdx, nptrd;
	double pick[2], start, stop;
	double *const Pick = &pick[0] - 1;
  char *tmp;
	*nerr = 0;
	/* - Get file name from character list. */
    tmp = string_list_get(datafiles, idfl-1);
	/* - Save total number of points in file. */
	Ntotal[idfl] = *npts;

	/* - Compute start value. */
	if( strcmp(kcut[0],"Z       ") == 0 ){
		Pick[1] = 0.;
	}

	else if ( strcmp(kcut[0],"N       ") != 0 ) {
		jdx = nequal( (char*)kcut[0], (char*)kmdfm.kpick,9, MPICK );
		if( jdx > 0 )
			Pick[1] = Fhdr[Ipckhd[jdx]];

		else{
			*nerr = ERROR_SAC_LOGIC_ERROR;
			setmsg( "ERROR", *nerr );
			apcmsg( "DEFCUT #2",10 );
			return ;
		}
	} else {
        *nerr = ERROR_SAC_LOGIC_ERROR;
        setmsg( "ERROR", *nerr );
        apcmsg( "DEFCUT #2",10 );
        return ;        
    }

	/* - Check to make sure the requested pick field 
	 *   in the header is defined. 
	 */
	if( Pick[1] == cmhdr.fundef ){
		if( cmdfm.icuter == 1 ){
			*nerr = ERROR_UNDEFINED_START_CUT_TIME;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			return ;
		}
		else{
			setmsg( "WARNING", ERROR_UNDEFINED_START_CUT_TIME );
            apcmsg2(tmp, strlen(tmp)+1);
			outmsg();
			setmsg( "OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME );
			outmsg();
			start = *begin;
			Nstart[idfl] = 1;
		}
	}
	else{
		/* start time of data to read */
    start = Pick[1] + ocut[0];
    cut_define(*begin, *delta, start, &Nstart[idfl]);
	}

	/* -  Compute stop value. */
	if( strcmp(kcut[1],"N       ") == 0 ){
		nptrd = ocut[1] + RNDOFF**delta;
		stop = start + (double)( nptrd - 1 )**delta;
		Nstop[idfl] = Nstart[idfl] + nptrd - 1;
		Pick[2] = 0.;
	}
	else{
		jdx = nequal( (char*)kcut[1], (char*)kmdfm.kpick,9, MPICK );
		if(jdx <= 0) {
            *nerr = ERROR_SAC_LOGIC_ERROR;
			setmsg( "ERROR", *nerr );
			apcmsg( "DEFCUT #3",10 );
			return ;
		} else {
      if(strcmp(kcut[1],"Z       ") == 0 ) {
        Pick[2] = 0.0;
      } else {
        Pick[2] = Fhdr[Ipckhd[jdx]];
      }
      stop = Pick[2] + ocut[1];
      cut_define(*begin, *delta, stop, &Nstop[idfl]);
    }
	}

	/* - Make sure stop pick is defined. */
	if( Pick[2] == cmhdr.fundef ){
		if( cmdfm.icuter == 1 ){
			*nerr = ERROR_UNDEFINED_STOP_CUT_TIME;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			return ;
		}
		else{
			setmsg( "WARNING", ERROR_UNDEFINED_STOP_CUT_TIME );
      apcmsg2(tmp, strlen(tmp)+1);
			outmsg();
			setmsg( "OUTPUT", ERROR_CORRECTED_BY_USING_END_TIME );
			outmsg();
			stop = *ennd;
			Nstop[idfl] = *npts;
		}
    }

  /* Check the cut time and adjust Nstart, Nstop, Nfillb, Nfille */
  cut_define_check(start, stop, *npts, cmdfm.icuter,
                   &Nstart[idfl], &Nstop[idfl],
                   &Nfillb[idfl], &Nfille[idfl], nerr);
  /* Error checking */
  if(*nerr) {
    switch(*nerr) {
    case ERROR_START_TIME_GREATER_THAN_STOP:
      error(*nerr, "%s\n\ttime:  %f >= %f\n\tindex: %d >= %d", tmp, start, stop, Nstart[idfl], Nstop[idfl]);
      return ;
      break;
    case ERROR_START_TIME_GREATER_THAN_END:
      error(*nerr, "%s\n\ttime:  %f > %f\n\tindex: %d > %d", tmp, start, *e, Nstart[idfl], *npts);
      return ;
      break;
    case ERROR_STOP_TIME_LESS_THAN_BEGIN:
      error(*nerr, "%s\n\ttime:  %f < %f\n\tindex: %d < %d", tmp, stop, *b, Nstop[idfl], 1);
      return ;
      break;
    case ERROR_START_TIME_LESS_THAN_BEGIN:
      if( cmdfm.icuter == 2 ){
        setmsg( "WARNING", *nerr);
        apcmsg2(tmp, strlen(tmp)+1);
        outmsg();
        setmsg( "OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME );
        outmsg();
        *nerr = SAC_OK;
      } else {
        setmsg( "ERROR", *nerr );
        apcmsg2(tmp, strlen(tmp)+1);
        return ;
      }
      break;
    case ERROR_STOP_TIME_GREATER_THAN_END:
      if( cmdfm.icuter == 2 ){
        setmsg( "WARNING", *nerr );
        apcmsg2(tmp, strlen(tmp)+1);
        outmsg();
        setmsg( "OUTPUT", ERROR_CORRECTED_BY_USING_END_TIME );
        outmsg();
        *nerr = SAC_OK;
      } else {
        setmsg( "ERROR", *nerr );
        apcmsg2(tmp, strlen(tmp)+1);
        return ;
      }
      break;
    case ERROR_CUT_TIMES_BEYOND_DATA_LIMITS:
      /* Begin */
      setmsg("WARNING", ERROR_START_TIME_LESS_THAN_BEGIN);
      apcmsg2(tmp, strlen(tmp)+1);
      outmsg();
      setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME);
      outmsg();
      /* End */
      setmsg("WARNING", ERROR_STOP_TIME_GREATER_THAN_END);
      apcmsg2(tmp, strlen(tmp)+1);
      outmsg();
      setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_END_TIME);
      outmsg();
      *nerr = SAC_OK;
      break;
    }
  }
	/* - Convert these start and stop points to new begin and end times. */
	*begin = *begin + (double)( Nstart[idfl] - 1 )**delta;
	*npts = Nstop[idfl] - Nstart[idfl] + 1;
	*ennd = *begin + (double)( *npts - 1 )**delta;

  DEBUG("nstart[%d]: %d\n", idfl, Nstart[idfl]);
  DEBUG( "nstop[%d]:  %d\n", idfl, Nstop[idfl]);
  DEBUG( "npts[%d]:   %d\n", idfl, Nstop[idfl] - Nstart[idfl] + 1);
  DEBUG( "CUT: %f %f %d %f\n", *begin, *ennd, *npts, *ennd-*begin);
}

