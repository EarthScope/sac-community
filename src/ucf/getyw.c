/** 
 * @file   getyw.c
 * 
 * @brief  Get y data point from x point
 * 
 */

#include "ucf.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"

/** 
 * Get y data point from current data file given x value
 * 
 * @param xwloc 
 *    Input X value in world coordinates
 * @param ywloc 
 *    Output Y value in world coordinates
 *
 * @date   850617:  Minor change due to addition of new memory manager.
 * @date   820317:  Original version.
 *
 */
void
getyw(double xwloc, 
      float *ywloc) {

	int index, ioffst;

	/* - Compute index offset into current array. */
	ioffst = (xwloc - *b + 0.5**delta)/ *delta;

	/* - Compute starting location of current data file. */
	index = cmdfm.ndxdta[cmdfm.idflc - 1][0];

	/* - Return corresponding y world coordinate. */
	*ywloc = *(cmmem.sacmem[index]+ioffst);

	return;
}

