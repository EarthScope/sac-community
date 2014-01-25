/*
 *
 *	g2tofloat - convert g2 format data to host single precision
 *
 * SYNOPSIS
 *
 *	g2tofloat (from, to, num)
 *	unsigned char	*from;		(i) array with gainranged data
 *	float		*to;		(o) degained data
 *	int		num;		(i) number of entries
 *
 * DESCRIPTION
 *	G2 format provides a representation with a manitissa ranging
 *	from -8192 to +8192 (8191 is the BIAS) and a multiplier of 1,
 *	4, 16, or 128 (the multiplier is 128/<gain-code>).
 *	Although the total dynamic range is +- 2**20,
 *	many numbers within this range are not representable.
 *
 *	To avoid the difficulty of having a vax and ieee versions of
 *	the g2 format, we define the g2 format as a byte stream.  Each
 *	value takes two bytes (bytes are 8 bits) with the high order
 *	two bits of byte[0] being the gain code, the low order 6 bits
 *	and the 8 bits of the second byte being the value (the 6 bits
 *	are the high order bits in the 14 bit value).
 *
 *	If value is less than BIAS, it is a negative number, if it is
 *	greater than BIAS it is a positive number.
 *
 *	Bit 15 and 16 (two upper bits) of the first byte in each
 *	sample contains the gain ranging value which determines the gain code:
 *
 *	00 -> gain code	= 128
 *	01 -> gain code 	= 32
 *	10 -> gain code  	= 8
 *	11 -> gain code  	= 1
 *
 *	The formula for g2 to integer is:
 *	(value-BIAS)*(128/<gain code>).
 * NOTES
 *	These routines can be used as inplace conversions.
 *
 *	Although g2 format offers less precision then integer format,
 *	in practice there is no difference since the original data from
 *	NORSAR is recorded in g2 format.  In other words, we have already
 *	lost the precision before the data reaches the processing stage.
 * AUTHOR
 *	Pete Ware
 */

/*
 * 128/gain code results in values to multiply sample value with.
 *
 * The values to multiply with are:
 *	gain bit 00 --> 128/128 =   1 no gain,0 bit shift
 *	gain bit 01 --> 128/32  =   4         2 bit shift
 *	gain bit 10 --> 128/8   =  16         4 bit shift
 *	gain bit 11 --> 128/1   = 128         7 bit shift
 *
 */

static int gain_value[4] = {0, 2, 4, 7}; 

/*
 * Array gain_range contains the maximum+1 value for each
 * gain bit. It is used when converting from i4 to g2 format.
 */

/*static int gain_range[4] = {1 << 13, 1 << 15, 1 << 17, 1 << 30};*/

#define REG	register
#define TOP	0xc0			/* top two bits */
#define BOT	0x3f			/* bottom 6 bits */
#define BIAS	8191			/* offset */
#define BYTES_PER_G2	2		/* bytes in g2 sample.  Changing
					 this is not sufficient to change
					 the size of g2 format data */

/*
 * g2tofloat converts NORESS gain ranged data to host specific float format
 */

void g2tofloat (REG unsigned char *from, REG float *to, REG int num)
/*REG unsigned char	*from;		* array with gain ranged data */
/*REG float		*to;		* degained data */
/*REG int		num;		* number of entries */
{
	REG int	value;

	/*
	 * Start at the end and work backwords.  This allows
	 * inplace conversions, assuming there is enough space
	 * and the user is not doing any tricky buffering.
	 */

	for (to += (num - 1), from += (num-1) * BYTES_PER_G2;num-- ;
	     from -= BYTES_PER_G2, --to)
	{
		value = ((from[0] & BOT) << 8) | from[1];
		*to = (value - BIAS) << gain_value[(TOP & from[0]) >> 6];
	}
}
