
#include "xyz.h"

void /*FUNCTION*/ flipdata(olddata, lengthx, lengthy, newdata)
float *olddata;
int lengthx, lengthy;
float *newdata;
{
#define OLDDATA(I_,J_)	(*(olddata+(I_)*(lengthx)+(J_)))
#define NEWDATA(I_,J_)	(*(newdata+(I_)*(lengthy)+(J_)))
	int jx, jx_, jy, jy_;

	/*=====================================================================
	 * PURPOSE:  To flip the data in a two-dimensional array.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    olddata:  Array containing old 2-d data. [fa]
	 *    lengthx:  Length of data in the x direction. [i]
	 *    lengthy:  Length of data in the y direction. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    newdata:  Array containing flip 2-d data. [fa]
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/2
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900308:  Added coding to replace data in memory with spectrogram.
	 *    900129:  Original version by Terri Quinn.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900308
	 *===================================================================== */
	/* - Copy each data point, flipping the indices. */
	for( jx = 1; jx <= lengthx; jx++ ){
		jx_ = jx - 1;
		for( jy = 1; jy <= lengthy; jy++ ){
			jy_ = jy - 1;
			NEWDATA(jx_,jy_) = OLDDATA(jy_,jx_);
			}
		}

       
	return;

#undef	NEWDATA
#undef	OLDDATA
} /* end of function */

