/** 
 * @file   izshft.c
 * 
 * @brief  Shift by bits
 * 
 */
 
/** 
 * Shifts specified integer specified number of bits
 *    right with wrap.  pnumshft>0 implies left shift, pnumshft<0
 *    implies right shift.
 * 
 * @param pint 
 *    short integer to be shifted
 * @param pnumshft 
 *    number of bits to shift by
 * 
 * @return the shifted short integer
 *
 * @bug Only used by gd2/hardwaretext2()
 *
 * @date 07/20/84  Under development--D. Trimmer
 * @date 07/23/84  Tested and debugged
 *
 */
short int 
izshft(short int *pint,
       short int *pnumshft) {
 
	unsigned   wrap;	/*  save shifted bits for wrapping  */
	short int nbits;	/*  length of shifted number in bits */
	short int shift;	/*  actual number of bits to be shifted */
 
	nbits = sizeof(*pint) * 8;
	shift = *pnumshft;
	while (shift < 0)	/* convert right shift into equiv. left shift*/
		shift += nbits;
	shift = shift % nbits;	/* # shifts must be less than integer size */
 
	wrap = ((~((short) 0)) << (nbits-shift))   &  *pint;	/* save wrap */
	wrap = (wrap>>(nbits-shift)) & ((1<<shift)-1);	/* move & mask wrap  */
	return ((short)(wrap |  (*pint << shift)));
}
 
