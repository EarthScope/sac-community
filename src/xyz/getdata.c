
#include <stdio.h>

#include "xyz.h"
#include "amf.h"
#include "bool.h"

#include "spectrogram.h"
#include "specdata.h"


#include "dff.h"
#include "debug.h"

/* 
 * 
 *  Name:     GETDATA
 *  
 * 
 *  Summmary:    Get data for distribution calculation
 * 
 *  
 * 
 *  Usage/Calling sequence:
 *  
 *     ierr = GETDATA ( nfiles,delta,windwsize,
 *                      windwovrl,windwfunc,buffersize,filelength,
 *                      bufindex,signals )
 * 
 *  Arguments:
 * 
 *     nfiles      =:    length of filelist(integer)
 *     delta       =:    time between data in files(real).
 *     windwsize   =:    size of window for FFT calc, integer
 *     windwovrl   =:    window to window data overlap, integer
 *     windwfunc   =:    function to apply to window, char*8
 *     buffersize  =:    buffersize
 *     filelength  =:    length of each file(integer)
 *     bufindex    =:    SACMEM index for buffer
 *     signals      :=   real array containing data for each station
 *                       length, (<=lfft)
 * 
 *  
 *   Returns:
 *  
 *       GETDATA = 0 no errors but more data, continue
 *       GETDATA = 1 error
 *       GETDATA = 2 no error and no more data
 * 
 *   Notes:
 * 
 *    
 *        By:    T.M.Quinn
 *        On:    1/16/90
 *  
 *        Updates:
 *  
 *             By: 
 *             On:  
 *             Subj:
 *  
 *             By:
 *             On:
 *             Subj:
 *  
 * 
 * 
 * 
 *   bufferfull     Is the input buffer full ?
 *   buffersize     Size of input buffer, calculated in calcfftsize
 *   filesread      Has the current file been read completely ?
 *   ifile          How many files have been opened
 *   lbuff          Ending point of data in buffer
 *   ptrbuffer      Pointer in buffer to next data to parse out 
 *   ptrfiles       Pointer in file where new data exists
 *   nodata         Is there any data left in files not yet used ?
 * 
 *   */


int 
getdata(int nfiles,
	double delta,
	int windwsize,
	int windwovrl,
	char *windwfunc,
	int buffersize,
	int filelength[],
	int bufindex,
	float signals[])
{
	int err, error, getdata_v, i, index, j, lread;
	float dum1, dum2;

	int *const Filelength = &filelength[0] - 1;
	float *const Signals = &signals[0] - 1;
  
  UNUSED(windwfunc);
  UNUSED(delta);
	/*    * Include files: */
	/*     * Local Variables: */
	/*     * Externals: */
	/*     * Code Implementation: */
	getdata_v = 1;

	/*          Initializations */
	error = 0;
	for( i = 1; i <= MAXLFFT; i++ ){
		Signals[i] = 0.;
		}

	if( error != 0 ){
		}
	else{

		/*          Check if all data used or last window of data not a full
		 *                window's length */
		if( (filesinfo.nodata) && (filesinfo.lbuff - filesinfo.ptrbuffer < 
		 windwsize) ){
			getdata_v = 2;

			}
		else{

			/*          Check if enough data left in buffer for a window's worth
			 *          If not need to read in more data from files */
			if( filesinfo.lbuff - filesinfo.ptrbuffer >= windwsize ){

				}
			else{

				/*          Move data at end of buffer to front to provide continuous windowing if not first time through */
				if( !filesinfo.first ){
					for( i = 1; i <= windwovrl; i++ ){
						*(cmmem.sacmem[bufindex]-1+i) =
                                                      *(cmmem.sacmem[bufindex]-1+buffersize-windwovrl+i);
						}
					filesinfo.lbuff = windwovrl;
					}

				/*          Reset buffer pointer */
				filesinfo.ptrbuffer = 0;
				filesinfo.bufferfull = FALSE;

L_1:
				;
				if( (!(filesinfo.bufferfull) && (!(filesinfo.nodata)
				 )) && (error == 0) ){

					/*          If file completely read... */
					if( filesinfo.filesread ){
						/*          open a new file */
						filesinfo.ifile = filesinfo.ifile + 1;
						filesinfo.filesread = FALSE;
						}

					if( error == 0 ){

						/*          Calculate how much data to read from each station file */
						if( (Filelength[filesinfo.ifile] - filesinfo.ptrfiles) > 
						 (buffersize - filesinfo.lbuff) ){
							lread = buffersize - filesinfo.lbuff;
							filesinfo.bufferfull = TRUE;
							}
						else if( (Filelength[filesinfo.ifile] - filesinfo.ptrfiles) == 
						 (buffersize - filesinfo.lbuff) ){
							lread = buffersize - filesinfo.lbuff;
							filesinfo.bufferfull = TRUE;
							filesinfo.filesread = TRUE;
							}
						else{
							lread = Filelength[filesinfo.ifile] - 
							 filesinfo.ptrfiles;
							filesinfo.filesread = TRUE;
							}

						/*          Get data from SAC file */
						getfil( filesinfo.ifile, TRUE, (int*)&dum1, 
						 &index, (int*)&dum2, &err );
						if( err != 0 ){
							error = 1;
							}
						else{
							for( i = 1; i <= lread; i++ ){
								*(cmmem.sacmem[bufindex]+i-1+filesinfo.lbuff) =
                                                                       *(cmmem.sacmem[index]+i-1+filesinfo.ptrfiles);
								}
							putfil( filesinfo.ifile, &err );
							if( err != 0 )
								error = 1;
							}

						/*          Set lbuff - length of new data in buffer */
						filesinfo.lbuff = filesinfo.lbuff + lread;

						filesinfo.ptrfiles = filesinfo.ptrfiles + 
						 lread;

						if( filesinfo.filesread ){
							/*          Reset files pointer */
							filesinfo.ptrfiles = 0;
							if( filesinfo.ifile == nfiles ){
								filesinfo.nodata = TRUE;
								}
							}
						}
					goto L_1;
					}

				}

			if( error != 0 ){
				fprintf( stdout, "Error getting data from file                (getdata).\n" );
				}
			else{


				/*          Load data to return */
				for( j = 1; j <= windwsize; j++ ){
					Signals[j] = *(cmmem.sacmem[bufindex]+filesinfo.ptrbuffer+j-1);
					}

				/*          Set buffer pointer to last data passed */
				filesinfo.ptrbuffer = filesinfo.ptrbuffer + windwsize - 
				 windwovrl;

				/*          Apply any window weighting function to data */
/*				if( zwindow( (float(*)[4096])signals, windwfunc, windwsize, 
				 1, (float(*)[4096])signals ) != 0 ){
					fprintf( stdout, "Error applying window weighting function(getdata).\n" );

					}
				else{
*/
					filesinfo.first = FALSE;
					getdata_v = 0;
/*				      }      */
				}
			}
		}


	return( getdata_v );
} /* end of function */



