
#define DOINITS
#include "xyz.h"
#undef DOINITS

#include "bool.h"

void /*FUNCTION*/ inixyz()
{



	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMXYZ.
	 *=====================================================================
	 * PARAMETER DEFINITIONS:
	 *    MZLLIST:  Maximum number of z levels. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    lcleanup:   If .TRUE. cleanup when quitting by deleting image
	 *                windows and destroying temporary files.
	 *    zllist:     List of z levels. [fa]
	 *    nzllist:    Number of z levels. [i]
	 *    lzllist:    If .TRUE., zllist contains the list of levels. [l]
	 *                If .FALSE., generate zllist from zlmin, zlmax, etc.
	 *    lzlmin:     If .TRUE., minimum z level is zlmin. [l]
	 *                If .FALSE., minimum z level is derived from data.
	 *    zlmin:      Minimum fixed z level. [f]
	 *    lzlmin:     If .TRUE., maximum z level is zlmax. [l]
	 *                If .FALSE., maximum z level is derived from data.
	 *    zlmin:      Maximum fixed z level. [f]
	 *    lzlines:    If .TRUE., use list of linestyles in izlines. [l]
	 *                If .FALSE., generate linestyles from zregions list.
	 *    izlines:    List of contouring linestyles. [ia]
	 *    nzlines:    Number of contouring linestyles. [i]
	 *    zregions:   List of zlevel/linestyle regions. [fa]
	 *                This list alternates between a value and a line-
	 *                style number (in float format.) Contours lines
	 *                between two zregions use the linestyle specified
	 *                between them in the list. The list should start and
	 *                end with a linestyle which applies to contours
	 *                below and above the first and last zregions.
	 *    nzregions:  Length of nzregions. [i]
	 *    laspect:    If .TRUE. aspect ratio of the data is maintained in
	 *                contour plot by selecting largest viewport with the
	 *                correct aspect ratio within the requested viewport.
	 *                If .FALSE. the entire requested viewport is used.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900503:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900503
	 *===================================================================== */
	/* PROCEDURE: */
	cmxyz.lcleanup = FALSE;

	cmxyz.lzllist = FALSE;
	cmxyz.nzllist = 20;
	cmxyz.lzlmin = FALSE;
	cmxyz.zlmin = 1.0;
	cmxyz.lzlmax = FALSE;
	cmxyz.zlmax = 1.0;
	cmxyz.lzlinc = FALSE;
	cmxyz.zlinc = 0.05;

	cmxyz.lzlines = TRUE;
	Izlines[1] = 1;
	cmxyz.nzlines = 1;

	cmxyz.laspect = FALSE;

       
	return;

} /* end of function */

