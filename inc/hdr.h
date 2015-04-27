/** 
 * @file   hdr.h
 * 
 * @brief  Internal definition of Current SAC file
 * 
 */

#ifndef _HDR_H_
#define _HDR_H_

/** 
 * @param SAC_HEADER_STRINGS_SIZE_MEMORY
 *    In Memory size of character header struct in four byte increments
 *    @size (8 + 1) * \p SAC_HEADER_STRINGS
 *    The extra 1 is for string termination
 *
 * @date 2009 Feb 15 was MKMHDR
 *
 */
#define SAC_HEADER_STRINGS_SIZE_MEMORY  54
/** 
 * @param SAC_HEADER_STRINGS_SIZE_FILE
 *    Size of file character header data block in four byte increments
 *    @size (8) * \p SAC_HEADER_STRINGS
 *
 * @date 2009 Feb 15 was FILEMKMHDR
 *
 */
#define SAC_HEADER_STRINGS_SIZE_FILE    ( 2 * SAC_HEADER_STRINGS )

/* 
   #define	MCMHDR	(MFHDR + MNHDR + MIHDR + MLHDR)
   #define	MFHDR	70
   #define	MHDR	(MCMHDR + MKMHDR)
   #define MHDRFILE (MCMHDR + FILEMKMHDR)
   #define	MIHDR	20
   #define	MIV	830
   #define	MKHDR	24
   #define FOURBYTEHDRS 164

   #define MKMHDR  54        / * in-memory size of character header struct * /
   #define FILEMKMHDR (2*MKHDR) / * size of file character header data block * /
   #define	MLHDR	5
   #define	MNHDR	15
*/
/** 
 * @note History
 *     The old character header (kmhdr) size (fortran version) was
 *     2*MKHDR 32bit words, due to the fact that each entry was
 *     8 characters (2 32bit words) long.
 *
 * @param MKMHDR  (2*MKHDR)
 *    the new character header (kmhdr) size (c version) in 32bit
 *    words is (9*MKHDR)/4.  Which for MKHDR=24 is 54.  If MKHDR
 *    is changed then this will have to be looked at, because
 *    (9*MKHDR)/4 may have a remainder.
 *
 * @note More History
 *     For purposes of maintaining backward compatibility, the
 *     null terminated strings comprising the character header
 *     data in memory are written into the file concatenated
 *     without the terminating nulls.  Read and write operations
 *     map these strings into/out of the in-memory struct.
 *
 */

#define SAC_VERSION_LOCATION              76
#define SAC_HEADER_MAJOR_VERSION          6

#define SAC_FLOAT_UNDEFINED              (-12345.0)
#define SAC_INT_UNDEFINED                (-12345)
#define SAC_ENUM_UNDEFINED                SAC_INT_UNDEFINED
#define SAC_LOGICAL_UNDEFINED             SAC_INT_UNDEFINED
#define SAC_CHAR_UNDEFINED                "-12345  "    /* Must be 8 characters */
#define SAC_CHAR_UNDEFINED_2              "-12345          "    /* Must be 16 characters */

/** 
 * @param SAC_HEADER_FLOATS
 *    Number of Floating point values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes  
 *
 * @date 2009 Feb 15 was MFHDR
 *
 */
#define SAC_HEADER_FLOATS                 70    /* 4 bytes  (real or float)    */
/** 
 * @param SAC_HEADER_INTEGERS 
 *    Number of Integer values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes  
 *
 * @date 2009 Feb 15 was MNHDR
 *
 */
#define SAC_HEADER_INTEGERS               15

/** 
 * @param SAC_HEADER_ENUMS
 *    Number of Enumerated values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes  
 *
 * @date 2009 Feb 15 was MIHDR
 *
 */
#define SAC_HEADER_ENUMS                  20
/** 
 * @param SAC_HEADER_LOGICALS 
 *    Number of Logical values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes  
 *
 * @date 2009 Feb 15 was MLHDR
 *
 */
#define SAC_HEADER_LOGICALS               5
/** 
 * @param SAC_HEADER_STRINGS
 *    Number of strings in the SAC Header.  The number here is in reality 
 *      one less as the number below as of header version 6.  The second
 *      value, the event name is twice as long as any other characer string
 *     
 * @see SAC_HEADER_STRING_LENGTH
 *  
 * @date 2009 Feb 15 was MKHDR 
 *
 */
#define SAC_HEADER_STRINGS                24    /* 9 bytes  (character or char)
                                                 *   actually 23 + 1 */

/** 
 * @param SAC_HEADER_NUMBERS
 *    Number of numeric values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes  
 *
 * @date 2009 Feb 15 was MCMHDR
 *
 */
#define SAC_HEADER_NUMBERS                ( SAC_HEADER_FLOATS +   \
					    SAC_HEADER_INTEGERS + \
					    SAC_HEADER_ENUMS +    \
					    SAC_HEADER_LOGICALS )
/** 
 * @param SAC_HEADER_SIZEOF_NUMBER
 *    Size of a number stored on disk or in memory for a SAC header
 *    This is equivalent to a int on 32 and 64 bit machines
 *       and a int or long int on 32 bit machines
 * 
 */
#define SAC_HEADER_SIZEOF_NUMBER          4

/** 
 * @param SAC_HEADER_STRING_LENGTH_FILE
 *    Size of a character string stored on disk for a SAC header
 *    Strings are stored without the C string termination character
 * 
 */
#define SAC_HEADER_STRING_LENGTH_FILE     8

/** 
 * @param SAC_HEADER_STRING_LENGTH
 *    Size of a character string stored in memory for a SAC header
 *    The extra character is for the C string termination character
 * 
 */
#define SAC_HEADER_STRING_LENGTH          ( SAC_HEADER_STRING_LENGTH_FILE + 1 )

/** 
 * @param SAC_HEADER_SIZEOF
 *   Size of the SAC Header in a file
 *
 */
#define SAC_HEADER_SIZEOF_FILE            ( SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER + \
                                            SAC_HEADER_STRINGS * (SAC_HEADER_STRING_LENGTH_FILE ) )

/** 
 * @param SAC_HEADER_SIZEOF
 *   Size of the SAC Header in memory 
 *
 */
#define SAC_HEADER_SIZEOF                 ( SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER + \
                                            SAC_HEADER_STRINGS * (SAC_HEADER_STRING_LENGTH ) )

/** 
 * @param SAC_HEADER_WORDS
 *     Number of 4 byte words in the header in memory
 *
 * @date Feb 15 2009 was MHDR
 */
#define SAC_HEADER_WORDS                  ( SAC_HEADER_SIZEOF      / SAC_HEADER_SIZEOF_NUMBER )

/** 
 * @param SAC_HEADER_WORDS_FILE
 *     Number of 4 byte words in the header in a file
 *
 * @date Feb 15 2009 was MHDRFILE
 */
#define SAC_HEADER_WORDS_FILE             ( SAC_HEADER_SIZEOF_FILE / SAC_HEADER_SIZEOF_NUMBER )

/** 
 * @param SAC_HEADER_STRING_SIZE_BYTES_FILE
 *     Size of string header in file measured in bytes
 *     SAC_HEADER_STRINGS * SAC_HEADER_STRINGS_LENGTH_FILE
 */
#define SAC_HEADER_STRINGS_SIZE_BYTES_FILE ( SAC_HEADER_STRINGS * SAC_HEADER_STRING_LENGTH_FILE )

/** 
 * @param SAC_HEADER_NUMBERS_SIZE_BYTES_FILE
 *     Size of numbers header in file measured in bytes
 *     SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER
 */
#define SAC_HEADER_NUMBERS_SIZE_BYTES_FILE ( SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER )

/** 
 * @param SAC_DATA_SIZE 
 *     Size of a data point in bytes
 */
#define SAC_DATA_SIZE    4

/** 
 * @param SAC_FIRST_DATA_POINT_WORD
 *    Location of the first data point within a SAC file.  This assumes the 
 *    data file is packed and \p SAC_HEADER_SIZEOF_NUMBER is half the size
 *    of \p SAC_HEADER_STRING_LENGTH and each of these are correct for 
 *    a 32 or 64 bit machine. 
 * @size 158 
 *
 * @bug This is currently used in dff/rsac1()
 * @bug This value is very fragile
 * 
 * @date Feb 15 2009 was MHDRFILE ( 164 )
 */
#define SAC_FIRST_DATA_POINT_WORD         SAC_HEADER_WORDS

/** 
 * @param SAC_FIRST_COMPONENT 
 *    Location of the first data component    
 */
#define SAC_FIRST_COMPONENT               1
/** 
 * @param SAC_SECOND_COMPONENT 
 *    Location of the second data component    
 */
#define SAC_SECOND_COMPONENT              2

/** 
 * @param SAC_ENUMS
 *    Number of ids for enumerated values 
 *
 * @date 2009 Feb 15 was MIV
 */
#define SAC_ENUMS                         830

#define SAC_INT_DEFINED(x) ( x != SAC_INT_UNDEFINED )
#define SAC_CHAR_DEFINED(x) ( strcasecmp(x, "-12345") != 0 && strcasecmp(x, SAC_CHAR_UNDEFINED) != 0 )

struct t_cmhdr {
    int nvhdrc;                     /** Header Number */
    int linc;                   /* TRUE if INC option is set on lh. */
    int llh;                    /* TRUE during the execution of xlh() */
} cmhdr;

/* 	Note:  in the following list, ninf, nhst, and nsn were 
	changed to norid, nevid, and nwfid respectively.  maf 961031 

	mag, imagtyp, and imagsrc added to provide magnitude, 
	magnitude type (mb, ms, ml, etc.) and magnitude source ( ie
	what institution measured the magnitude).  maf 970205 */

#endif /* _HDR_H_ */
