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
		int iBegin , iStart ;

		/* start time of data to read */
		start = Pick[1] + ocut[0] ;
		iStart = lround(  start / *delta ) ;
		iBegin = lround( *begin / *delta ) ;

		Nstart[idfl] = iStart - iBegin + 1 ;
    DEBUG("START/B: %f (%d) %d [%f/%f] %f %d\n", start, iStart, Nstart[idfl],Pick[1],ocut[0], start/ *delta, *npts);
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
            int iBegin , iStop ;
            stop = Pick[2] + ocut[1] ;
            iStop = lround(  stop / *delta ) ;
            iBegin = lround( *begin / *delta ) ;
            
            Nstop[idfl] = iStop - iBegin + 1 ;
            DEBUG("STOP/E: %f (%d) %d [%f/%f] %f %d\n", stop, iStop, Nstop[idfl], Pick[2],ocut[1], stop / *delta, *npts);
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

	/* - Check that start value less than stop value. */
	if( start >= stop ){
		*nerr = ERROR_START_TIME_GREATER_THAN_STOP;
    error(*nerr, "%s\n\ttime:  %f >= %f\n\tindex: %d >= %d",
          tmp, start, stop,
          Nstart[idfl], Nstop[idfl]);
		return ;
	}

	/* - Handle cases where the requested data window is not entirely
	 *   within the range of the data file. */

	/* -- Entire data window after file end. */
	if( Nstart[idfl] > *npts ){
		if( cmdfm.icuter == 3 ){
			Nfillb[idfl] = 0;
			Nfille[idfl] = Nstop[idfl] - Nstart[idfl] + 1;
		}
		else{
			*nerr = ERROR_START_TIME_GREATER_THAN_END;
      error(*nerr, "%s\n\ttime:  %f > %f\n\tindex: %d > %d", 
            tmp, start, *e,
            Nstart[idfl], *npts);
		}
		return ;
	}

	/* -- Entire data window before file begin. */
	if( Nstop[idfl] < 1 ){
		if( cmdfm.icuter == 3 ){
			Nfillb[idfl] = Nstop[idfl] - Nstart[idfl] + 1;
			Nfille[idfl] = 0;
		}
		else{
			*nerr = ERROR_STOP_TIME_LESS_THAN_BEGIN;
      error(*nerr, "%s\n\ttime:  %f < %f\n\tindex: %d < %d",
            tmp, stop, *b, 
            Nstop[idfl], 1);
      free(tmp);
		}
		return ;
	}

	/* - Start of data window before file begin. */
	if( Nstart[idfl] < 1 ){
		if( cmdfm.icuter == 3 )
			Nfillb[idfl] = 1 - Nstart[idfl];

		else if( cmdfm.icuter == 2 ){
			setmsg( "WARNING", ERROR_START_TIME_LESS_THAN_BEGIN );
            apcmsg2(tmp, strlen(tmp)+1);
			outmsg();
			setmsg( "OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME );
			outmsg();
			/* start = *begin; */
			Nstart[idfl] = 1;
			Nfillb[idfl] = 0;
		}
		else{
			*nerr = ERROR_START_TIME_LESS_THAN_BEGIN;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			return ;
		}
	}
	else
		Nfillb[idfl] = 0;


	/* -- Stop of data window is after file end. */
	if( Nstop[idfl] > *npts ){
		if( cmdfm.icuter == 3 )
			Nfille[idfl] = Nstop[idfl] - *npts;

		else if( cmdfm.icuter == 2 ){
			setmsg( "WARNING", 1325 );
            apcmsg2(tmp, strlen(tmp)+1);
			outmsg();
			setmsg( "OUTPUT", 1331 );
			outmsg();
			/* stop = *ennd; */
			Nstop[idfl] = *npts;
			Nfille[idfl] = 0;
		}
		else{
			*nerr = ERROR_STOP_TIME_LESS_THAN_END;
			setmsg( "ERROR", *nerr );
			return ;
		}
	}
	else
		Nfille[idfl] = 0;


	/* - Convert these start and stop points to new begin and end times. */

	*begin = *begin + (double)( Nstart[idfl] - 1 )**delta;
	*npts = Nstop[idfl] - Nstart[idfl] + 1;
	*ennd = *begin + (double)( *npts - 1 )**delta;

  DEBUG("nstart[%d]: %d\n", idfl, Nstart[idfl]);
  DEBUG( "nstop[%d]:  %d\n", idfl, Nstop[idfl]);
  DEBUG( "npts[%d]:   %d\n", idfl, Nstop[idfl] - Nstart[idfl] + 1);
  DEBUG( "CUT: %f %f %d %f\n", *begin, *ennd, *npts, *ennd-*begin);
}

