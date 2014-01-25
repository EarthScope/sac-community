/** 
 * @file   locdp.c
 * 
 * @brief  Determine the location of a point to a rectangle
 * 
 */


#define	IABOVE	8
#define	IBELOW	4
#define	ILEFT	1
#define	IRIGHT	2

/** 
 * Determine the location of a point relative to a rectangle
 * 
 * @param x 
 *    X data point
 * @param y 
 *    Y data point
 * @param xr 
 *    Array of x rectangle values, length = 2
 *    Left Edge First
 * @param yr 
 *    Array of y rectangle valyes, length = 2
 *    Bottom Edge First
 * @param ilocdp 
 *    Location of the data point relative to the rectangle
 *    -  0 Inside
 *    -  1 Left 
 *    -  2 Right
 *    -  3 Below
 *    -  4 Above
 *    -  5 Below and Left
 *    -  6 Below and Right
 *    -  7 Undefined
 *    -  8 Above
 *    -  9 Above and Left
 *    - 10 Above and Right
 *
 *             The four rightmost bits mean the following if set:
 *             Bit 4: data point is above top of rectangle.
 *             Bit 3: data point is below bottom of rectangle.
 *             Bit 2: data point is to right of rectangle.
 *             Bit 1: data point is to left of rectangle.
 *             There are therefore nine possible values for ILOC:
 *             = Bits 1001 ( 9) if above and to the left of rectangle.
 *             = Bits 1000 ( 8) if above rectangle.
 *             = Bits 1010 (10) if above and to the right of rectangle.
 *             = Bits 0001 ( 1) if to the left of rectangle.
 *             = Bits 0000 ( 0) if inside rectangle.
 *             = Bits 0010 ( 2) if to the right of rectangle.
 *             = Bits 0101 ( 5) if below and to the left of rectangle.
 *             = Bits 0100 ( 4) if below rectangle.
 *             = Bits 0110 ( 6) if below and to the right of rectangle.
 *
 * @note Global Coupling
 * - Values for parameters IABOVE, IBELOW, ILEFT, and IRIGHT must be
 *   the same in the clipping subroutines: CLIP, BLANK, and LOCDP.
 *   The meaning of these parameters is given below.
 *
 * @note Local Variables
 *   - IABOVE:  Constant to indicate above rectangle [= bits 1000 (8)].
 *   - IBELOW:  Constant to indicate below rectangle [= bits 0100 (4)].
 *   - ILEFT:   Constant to indicate left of rectangle [= bits 0010 (2)].
 *   - IRIGHT:  Constant to indicate right of rectangle [= bits 0001 (1)].
 *
 * @note References
 * - Newman, William M. and Robert F. Sproull, PRINCIPLES OF INTERACTIVE
 *   COMPUTER GRAPHICS, McGraw-Hill, 1939, pp 123-124.
 *
 * @date   811222:  Original version.
 *
 */
void 
locdp(double x, 
      double y, 
      float *xr, 
      float *yr, 
      int   *ilocdp)
{

	float *const Xr = &xr[0] - 1;
	float *const Yr = &yr[0] - 1;

	/* - Assume data point is inside rectangle. */
	*ilocdp = 0;

	/* - Check to see if data point is above or below rectangle. */

	if( y > Yr[2] ){
		*ilocdp = IABOVE;
	}
	else if( y < Yr[1] ){
		*ilocdp = IBELOW;
	}

	/* - Check to see if data point is to left or right of rectangle. */

	if( x > Xr[2] ){
		*ilocdp = *ilocdp + IRIGHT;
	}
	else if( x < Xr[1] ){
		*ilocdp = *ilocdp + ILEFT;
	}
	
	return;
}

