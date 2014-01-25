/** 
 * @file   getxw.c
 * 
 * @brief  Get x data point from y value
 * 
 */

#include "ucf.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"

/** 
 * Get x data point from current data file given y value
 * 
 * @param ywloc 
 *    Input Y value in world coordinates
 * @param xwloc 
 *    Output x value in world coordinates
 *
 * @date   961219:  Original version; copied from getyw.c,  maf.
 * @date   850617:  getyw.c initial
 *
 */
void getxw(double ywloc, 
           float *xwloc) {

	int index, ioffst;

	/* - Compute index offset into current array. */
	ioffst = (ywloc - *b + 0.5**delta)/ *delta;

	/* - Compute starting location of current data file. */
	index = cmdfm.ndxdta[cmdfm.idflc - 1][0];

	/* - Return corresponding x world coordinate. */
	*xwloc = *(cmmem.sacmem[index]+ioffst);

	return;
}

