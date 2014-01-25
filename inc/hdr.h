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
#define SAC_CHAR_UNDEFINED                "-12345  " /* Must be 8 characters */
#define SAC_CHAR_UNDEFINED_2              "-12345          " /* Must be 16 characters */

/** 
 * @param SAC_HEADER_FLOATS
 *    Number of Floating point values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes  
 *
 * @date 2009 Feb 15 was MFHDR
 *
 */
#define SAC_HEADER_FLOATS                 70  /* 4 bytes  (real or float)    */
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
#define SAC_HEADER_STRINGS                24  /* 9 bytes  (character or char)
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
  float fhdr[SAC_HEADER_FLOATS];    /*  70  Floating point values */
  int   nhdr[SAC_HEADER_INTEGERS];  /*  15  Numerical values      */
  int   ihdr[SAC_HEADER_ENUMS];     /*  20  Integer values        */
  int   lhdr[SAC_HEADER_LOGICALS];  /*   5  Logical values        */
  int   niv[SAC_ENUMS];             /* 830  Possible enumeration values */
  float fundef;                     /** Floating point undefined */
  int   iundef;                     /** Integer undefined */
  int   nundef;                     /** Number undefined */
  int   nvhdrc;                     /** Header Number */
  float exthdr[20];                 /** Not Used  */
  int   linc;                       /* TRUE if INC option is set on lh. */
  int   llh;	                    /* TRUE during the execution of xlh() */
} cmhdr;

struct t_kmhdr {
  /** Character Strings */
  char khdr[SAC_HEADER_STRINGS][SAC_HEADER_STRING_LENGTH];
  /** Character string undefined */
  char kundef[SAC_HEADER_STRING_LENGTH];
  char kundef_2[SAC_HEADER_STRING_LENGTH*2]; 
} kmhdr;


/* 	Note:  in the following list, ninf, nhst, and nsn were 
	changed to norid, nevid, and nwfid respectively.  maf 961031 

	mag, imagtyp, and imagsrc added to provide magnitude, 
	magnitude type (mb, ms, ml, etc.) and magnitude source ( ie
	what institution measured the magnitude).  maf 970205 */

#ifdef DOINITS
	float *const a          = (float*)(cmhdr.fhdr +  32/4);
	float *const arrivl     = (float*)(cmhdr.fhdr +  32/4);
	float *const az         = (float*)(cmhdr.fhdr + 204/4);
	float *const b          = (float*)(cmhdr.fhdr +  20/4);
	float *const baz        = (float*)(cmhdr.fhdr + 208/4);
	float *const begin      = (float*)(cmhdr.fhdr +  20/4);
	float *const cmpaz      = (float*)(cmhdr.fhdr + 228/4);
	float *const cmpinc     = (float*)(cmhdr.fhdr + 232/4);
        float *const delta      = (float*)(cmhdr.fhdr );
	float *const depmax     = (float*)(cmhdr.fhdr +   8/4);
	float *const depmen     = (float*)(cmhdr.fhdr + 224/4);
	float *const depmin     = (float*)(cmhdr.fhdr +   4/4);
	float *const depmn      = (float*)(cmhdr.fhdr +   4/4);
	float *const depmx      = (float*)(cmhdr.fhdr +   8/4);
	float *const dist       = (float*)(cmhdr.fhdr + 200/4);
	float *const e          = (float*)(cmhdr.fhdr +  24/4);
	float *const ennd       = (float*)(cmhdr.fhdr +  24/4);
	float *const evdp       = (float*)(cmhdr.fhdr + 152/4);
	float *const evel       = (float*)(cmhdr.fhdr + 148/4);
	float *const evla       = (float*)(cmhdr.fhdr + 140/4);
	float *const evlo       = (float*)(cmhdr.fhdr + 144/4);
	float *const f          = (float*)(cmhdr.fhdr +  80/4);
	float *const mag        = (float*)(cmhdr.fhdr + 156/4);
	float *const fhdr64     = (float*)(cmhdr.fhdr + 252/4);
	float *const fhdr65     = (float*)(cmhdr.fhdr + 256/4);
	float *const fhdr66     = (float*)(cmhdr.fhdr + 260/4);
	float *const fhdr67     = (float*)(cmhdr.fhdr + 264/4);
	float *const fhdr68     = (float*)(cmhdr.fhdr + 268/4);
	float *const fhdr69     = (float*)(cmhdr.fhdr + 272/4);
	float *const fhdr70     = (float*)(cmhdr.fhdr + 276/4);
	float *const fini       = (float*)(cmhdr.fhdr +  80/4);
	float *const fmean      = (float*)(cmhdr.fhdr + 224/4);
	float *const fmt        = (float*)(cmhdr.fhdr +  36/4);
	float *const gcarc      = (float*)(cmhdr.fhdr + 212/4);

	int   *const ia         = (int*)(  cmhdr.niv  +  44/4);
	int   *const iacc       = (int*)(  cmhdr.niv  +  28/4);
	int   *const iamph      = (int*)(  cmhdr.niv  +   8/4);
	int   *const ib         = (int*)(  cmhdr.niv  +  32/4);
	int   *const ichem      = (int*)(  cmhdr.niv  + 168/4);
	int   *const iday       = (int*)(  cmhdr.niv  +  36/4);
	int   *const idep       = (int*)(  cmhdr.ihdr +   4/4);
	int   *const idisp      = (int*)(  cmhdr.niv  +  20/4);
	int   *const idown      = (int*)(  cmhdr.niv  + 116/4);
	int   *const idrop      = (int*)(  cmhdr.niv  + 184/4);
	int   *const ieast      = (int*)(  cmhdr.niv  + 108/4);
	int   *const ievreg     = (int*)(  cmhdr.ihdr +  24/4);
	int   *const ievtyp     = (int*)(  cmhdr.ihdr +  28/4);
	int   *const iftype     = (int*)cmhdr.ihdr;
	int   *const iglch      = (int*)(  cmhdr.niv  + 180/4);
	int   *const igood      = (int*)(  cmhdr.niv  + 176/4);
	int   *const imagtyp    = (int*)(  cmhdr.ihdr +  40/4);
	int   *const imagsrc    = (int*)(  cmhdr.ihdr +  44/4);
	int   *const ihdr13     = (int*)(  cmhdr.ihdr +  48/4);
	int   *const ihdr14     = (int*)(  cmhdr.ihdr +  52/4);
	int   *const ihdr15     = (int*)(  cmhdr.ihdr +  56/4);
	int   *const ihdr16     = (int*)(  cmhdr.ihdr +  60/4);
	int   *const ihdr17     = (int*)(  cmhdr.ihdr +  64/4);
	int   *const ihdr18     = (int*)(  cmhdr.ihdr +  68/4);
	int   *const ihdr19     = (int*)(  cmhdr.ihdr +  72/4);
	int   *const ihdr20     = (int*)(  cmhdr.ihdr +  76/4);
	int   *const ihdr4      = (int*)(  cmhdr.ihdr +  12/4);
	int   *const ihglp      = (int*)(  cmhdr.niv  + 136/4);
	int   *const ihorza     = (int*)(  cmhdr.niv  + 112/4);
	int   *const iinst      = (int*)(  cmhdr.ihdr +  16/4);
	int   *const illlbb     = (int*)(  cmhdr.niv  + 124/4);
	int   *const ilowsn     = (int*)(  cmhdr.niv  + 188/4);

        int     *const imb      = (int*)(  cmhdr.niv  + 204/4);
        int     *const ims      = (int*)(  cmhdr.niv  + 208/4);
        int     *const iml      = (int*)(  cmhdr.niv  + 212/4);
        int     *const imw      = (int*)(  cmhdr.niv  + 216/4);
        int     *const imd      = (int*)(  cmhdr.niv  + 220/4);
        int     *const imx      = (int*)(  cmhdr.niv  + 224/4);
        int     *const ineic    = (int*)(  cmhdr.niv  + 228/4);
        int     *const ipdeq    = (int*)(  cmhdr.niv  + 232/4);
	int     *const ipdew    = (int*)(  cmhdr.niv  + 236/4);
	int     *const ipde     = (int*)(  cmhdr.niv  + 240/4);
        int     *const iisc     = (int*)(  cmhdr.niv  + 244/4);
        int     *const ireb     = (int*)(  cmhdr.niv  + 248/4);
        int     *const iusgs    = (int*)(  cmhdr.niv  + 252/4);
        int     *const ibrk     = (int*)(  cmhdr.niv  + 256/4);
        int     *const icaltech = (int*)(  cmhdr.niv  + 260/4);
        int     *const illnl    = (int*)(  cmhdr.niv  + 264/4);
        int     *const ievloc   = (int*)(  cmhdr.niv  + 268/4);
        int     *const ijsop    = (int*)(  cmhdr.niv  + 272/4);
        int     *const iuser    = (int*)(  cmhdr.niv  + 276/4);
        int     *const iunknown = (int*)(  cmhdr.niv  + 280/4);

	int     *const iqb      = (int*)(  cmhdr.niv  + 284/4);
	int     *const iqb1     = (int*)(  cmhdr.niv  + 288/4);
	int     *const iqb2     = (int*)(  cmhdr.niv  + 292/4);
        int     *const iqbx     = (int*)(  cmhdr.niv  + 296/4);
        int     *const iqmt     = (int*)(  cmhdr.niv  + 300/4);
        int     *const ieq      = (int*)(  cmhdr.niv  + 304/4);
        int     *const ieq1     = (int*)(  cmhdr.niv  + 308/4);
        int     *const ieq2     = (int*)(  cmhdr.niv  + 312/4);
        int     *const ime      = (int*)(  cmhdr.niv  + 316/4);
        int     *const iex      = (int*)(  cmhdr.niv  + 320/4);
        int     *const inu      = (int*)(  cmhdr.niv  + 324/4);
        int     *const inc      = (int*)(  cmhdr.niv  + 328/4);
        int     *const io_      = (int*)(  cmhdr.niv  + 332/4);
        int     *const il       = (int*)(  cmhdr.niv  + 336/4);
        int     *const ir       = (int*)(  cmhdr.niv  + 340/4);
        int     *const it       = (int*)(  cmhdr.niv  + 344/4);
        int     *const iu       = (int*)(  cmhdr.niv  + 348/4);

        int     *const ieq3     = (int*)(  cmhdr.niv  + 352/4);
        int     *const ieq0     = (int*)(  cmhdr.niv  + 356/4);
        int     *const iex0     = (int*)(  cmhdr.niv  + 360/4);
        int     *const iqc      = (int*)(  cmhdr.niv  + 364/4);
        int     *const iqb0     = (int*)(  cmhdr.niv  + 368/4);
        int     *const igey     = (int*)(  cmhdr.niv  + 372/4);
        int     *const ilit     = (int*)(  cmhdr.niv  + 376/4);
        int     *const imet     = (int*)(  cmhdr.niv  + 380/4);
        int     *const iodor    = (int*)(  cmhdr.niv  + 384/4);

	int     *const inorth   = (int*)(  cmhdr.niv  + 104/4);
	int     *const inucl    = (int*)(  cmhdr.niv  + 144/4);
	int     *const io       = (int*)(  cmhdr.niv  +  40/4);
	int     *const iother   = (int*)(  cmhdr.niv  + 172/4);
	int     *const ipostn   = (int*)(  cmhdr.niv  + 152/4);
	int     *const ipostq   = (int*)(  cmhdr.niv  + 164/4);
	int     *const ipren    = (int*)(  cmhdr.niv  + 148/4);
	int     *const ipreq    = (int*)(  cmhdr.niv  + 160/4);
	int     *const iquake   = (int*)(  cmhdr.niv  + 156/4);
	int     *const iqual    = (int*)(  cmhdr.ihdr +  32/4);
	int     *const iradev   = (int*)(  cmhdr.niv  +  96/4);
	int     *const iradnv   = (int*)(  cmhdr.niv  +  88/4);
	int     *const irldta   = (int*)(  cmhdr.niv  + 192/4);
	int     *const irlim    = (int*)(  cmhdr.niv  +   4/4);
	int     *const isro     = (int*)(  cmhdr.niv  + 140/4);
	int     *const istreg   = (int*)(  cmhdr.ihdr +  20/4);
	int     *const isynth   = (int*)(  cmhdr.ihdr +  36/4);
	int     *const it0      = (int*)(  cmhdr.niv  +  48/4);
	int     *const it1      = (int*)(  cmhdr.niv  +  52/4);
	int     *const it2      = (int*)(  cmhdr.niv  +  56/4);
	int     *const it3      = (int*)(  cmhdr.niv  +  60/4);
	int     *const it4      = (int*)(  cmhdr.niv  +  64/4);
	int     *const it5      = (int*)(  cmhdr.niv  +  68/4);
	int     *const it6      = (int*)(  cmhdr.niv  +  72/4);
	int     *const it7      = (int*)(  cmhdr.niv  +  76/4);
	int     *const it8      = (int*)(  cmhdr.niv  +  80/4);
	int     *const it9      = (int*)(  cmhdr.niv  +  84/4);
	int     *const itanev   = (int*)(  cmhdr.niv  + 100/4);
	int     *const itannv   = (int*)(  cmhdr.niv  +  92/4);
	int     *const itime    = (int*)cmhdr.niv;
	int     *const iunkn    = (int*)(  cmhdr.niv  +  16/4);
	int     *const iup      = (int*)(  cmhdr.niv  + 120/4);
	int     *const ivel     = (int*)(  cmhdr.niv  +  24/4);
	int     *const ivolts   = (int*)(  cmhdr.niv  + 196/4);
	int     *const iwwsn1   = (int*)(  cmhdr.niv  + 128/4);
	int     *const iwwsn2   = (int*)(  cmhdr.niv  + 132/4);
	int     *const ixy      = (int*)(  cmhdr.niv  +  12/4);
	int     *const ixyz     = (int*)(  cmhdr.niv  + 200/4);
	int     *const iztype   = (int*)(  cmhdr.ihdr +   8/4);

	char *const kstnm       = (char*)kmhdr.khdr;
	char *const kevnm       = (char*)(kmhdr.khdr  +   9/9);
	char *const khole       = (char*)(kmhdr.khdr  +  27/9);
	char *const ko          = (char*)(kmhdr.khdr  +  36/9);
	char *const ka          = (char*)(kmhdr.khdr  +  45/9);
	char *const kt0         = (char*)(kmhdr.khdr  +  54/9);
	char *const kt1         = (char*)(kmhdr.khdr  +  63/9);
	char *const kt2         = (char*)(kmhdr.khdr  +  72/9);
	char *const kt3         = (char*)(kmhdr.khdr  +  81/9);
	char *const kt4         = (char*)(kmhdr.khdr  +  90/9);
	char *const kt5         = (char*)(kmhdr.khdr  +  99/9);
	char *const kt6         = (char*)(kmhdr.khdr  + 108/9);
	char *const kt7         = (char*)(kmhdr.khdr  + 117/9);
	char *const kt8         = (char*)(kmhdr.khdr  + 126/9);
	char *const kt9         = (char*)(kmhdr.khdr  + 135/9);
	char *const kf          = (char*)(kmhdr.khdr  + 144/9);
	char *const kuser0      = (char*)(kmhdr.khdr  + 153/9);
	char *const kuser1      = (char*)(kmhdr.khdr  + 162/9);
	char *const kuser2      = (char*)(kmhdr.khdr  + 171/9);
	char *const kcmpnm      = (char*)(kmhdr.khdr  + 180/9);
	char *const knetwk      = (char*)(kmhdr.khdr  + 189/9);
	char *const kdatrd      = (char*)(kmhdr.khdr  + 198/9);
	char *const kinst       = (char*)(kmhdr.khdr  + 207/9);

	int  *const lcalda      = (int*)(  cmhdr.lhdr +  12/4);
	int  *const leven       = (int*)cmhdr.lhdr;
	int  *const lhdr5       = (int*)(  cmhdr.lhdr +  16/4);
	int  *const lovrok      = (int*)(  cmhdr.lhdr +   8/4);
	int  *const lpspol      = (int*)(  cmhdr.lhdr +   4/4);
    int  *const nevid       = (int*)(  cmhdr.nhdr +  32/4);
	int  *const nhdr15      = (int*)(  cmhdr.nhdr +  56/4);
	int  *const norid       = (int*)(  cmhdr.nhdr +  28/4);
	int  *const npts        = (int*)(  cmhdr.nhdr +  36/4);
	int  *const nsnpts      = (int*)(  cmhdr.nhdr +  40/4);
	int  *const nvhdr       = (int*)(  cmhdr.nhdr +  24/4);
    int  *const nwfid       = (int*)(  cmhdr.nhdr +  44/4);
	int  *const nxsize      = (int*)(  cmhdr.nhdr +  48/4);
	int  *const nysize      = (int*)(  cmhdr.nhdr +  52/4);
	int  *const nzdttm      = (int*)cmhdr.nhdr;
	int  *const nzhour      = (int*)(  cmhdr.nhdr +   8/4);
	int  *const nzjday      = (int*)(  cmhdr.nhdr +   4/4);
	int  *const nzmin       = (int*)(  cmhdr.nhdr +  12/4);
	int  *const nzmsec      = (int*)(  cmhdr.nhdr +  20/4);
	int  *const nzsec       = (int*)(  cmhdr.nhdr +  16/4);
	int  *const nzyear      = (int*)cmhdr.nhdr;
	float *const o          = (float*)(cmhdr.fhdr +  28/4);
	float *const odelta     = (float*)(cmhdr.fhdr +  16/4);
	float *const origin     = (float*)(cmhdr.fhdr +  28/4);
	float *const resp0      = (float*)(cmhdr.fhdr +  84/4);
	float *const resp1      = (float*)(cmhdr.fhdr +  88/4);
	float *const resp2      = (float*)(cmhdr.fhdr +  92/4);
	float *const resp3      = (float*)(cmhdr.fhdr +  96/4);
	float *const resp4      = (float*)(cmhdr.fhdr + 100/4);
	float *const resp5      = (float*)(cmhdr.fhdr + 104/4);
	float *const resp6      = (float*)(cmhdr.fhdr + 108/4);
	float *const resp7      = (float*)(cmhdr.fhdr + 112/4);
	float *const resp8      = (float*)(cmhdr.fhdr + 116/4);
	float *const resp9      = (float*)(cmhdr.fhdr + 120/4);
	float *const sb         = (float*)(cmhdr.fhdr + 216/4);
	float *const scale      = (float*)(cmhdr.fhdr +  12/4);
	float *const sdelta     = (float*)(cmhdr.fhdr + 220/4);
	float *const stdp       = (float*)(cmhdr.fhdr + 136/4);
	float *const stel       = (float*)(cmhdr.fhdr + 132/4);
	float *const stla       = (float*)(cmhdr.fhdr + 124/4);
	float *const stlo       = (float*)(cmhdr.fhdr + 128/4);
	float *const t0         = (float*)(cmhdr.fhdr +  40/4);
	float *const t1         = (float*)(cmhdr.fhdr +  44/4);
	float *const t2         = (float*)(cmhdr.fhdr +  48/4);
	float *const t3         = (float*)(cmhdr.fhdr +  52/4);
	float *const t4         = (float*)(cmhdr.fhdr +  56/4);
	float *const t5         = (float*)(cmhdr.fhdr +  60/4);
	float *const t6         = (float*)(cmhdr.fhdr +  64/4);
	float *const t7         = (float*)(cmhdr.fhdr +  68/4);
	float *const t8         = (float*)(cmhdr.fhdr +  72/4);
	float *const t9         = (float*)(cmhdr.fhdr +  76/4);
	float *const time0      = (float*)(cmhdr.fhdr +  40/4);
	float *const time1      = (float*)(cmhdr.fhdr +  44/4);
	float *const time2      = (float*)(cmhdr.fhdr +  48/4);
	float *const time3      = (float*)(cmhdr.fhdr +  52/4);
	float *const time4      = (float*)(cmhdr.fhdr +  56/4);
	float *const time5      = (float*)(cmhdr.fhdr +  60/4);
	float *const time6      = (float*)(cmhdr.fhdr +  64/4);
	float *const time7      = (float*)(cmhdr.fhdr +  68/4);
	float *const time8      = (float*)(cmhdr.fhdr +  72/4);
	float *const time9      = (float*)(cmhdr.fhdr +  76/4);
	float *const user0      = (float*)(cmhdr.fhdr + 160/4);
	float *const user1      = (float*)(cmhdr.fhdr + 164/4);
	float *const user2      = (float*)(cmhdr.fhdr + 168/4);
	float *const user3      = (float*)(cmhdr.fhdr + 172/4);
	float *const user4      = (float*)(cmhdr.fhdr + 176/4);
	float *const user5      = (float*)(cmhdr.fhdr + 180/4);
	float *const user6      = (float*)(cmhdr.fhdr + 184/4);
	float *const user7      = (float*)(cmhdr.fhdr + 188/4);
	float *const user8      = (float*)(cmhdr.fhdr + 192/4);
	float *const user9      = (float*)(cmhdr.fhdr + 196/4);
	float *const xmaximum   = (float*)(cmhdr.fhdr + 240/4);
	float *const xminimum   = (float*)(cmhdr.fhdr + 236/4);
	float *const ymaximum   = (float*)(cmhdr.fhdr + 248/4);
	float *const yminimum   = (float*)(cmhdr.fhdr + 244/4);

        /* OFFSET Vectors w/subscript range: 1 to dimension */
	float *const Exthdr     = &cmhdr.exthdr[0] - 1;
	float *const Fhdr       = &cmhdr.fhdr[0] - 1;
	int   *const Ihdr       = &cmhdr.ihdr[0] - 1;
	int   *const Lhdr       = &cmhdr.lhdr[0] - 1;
	int   *const Nhdr       = &cmhdr.nhdr[0] - 1;
	int   *const Niv        = &cmhdr.niv[0] - 1;

        /* int *const Nzdttm = &nzdttm[0] - 1; */
	int   *const Nzdttm     = (int*)&cmhdr.nhdr[0] - 1;

#else

extern float *const a;
extern float *const arrivl;
extern float *const az;
extern float *const b;
extern float *const baz;
extern float *const begin;
extern float *const cmpaz;
extern float *const cmpinc;
extern float *const delta;
extern float *const depmax;
extern float *const depmen;
extern float *const depmin;
extern float *const depmn;
extern float *const depmx;
extern float *const dist;
extern float *const e;
extern float *const ennd;
extern float *const evdp;
extern float *const evel;
extern float *const evla;
extern float *const evlo;
extern float *const f;
extern float *const mag;	/* magnitude.  maf 970205 */
extern float *const fhdr64;
extern float *const fhdr65;
extern float *const fhdr66;
extern float *const fhdr67;
extern float *const fhdr68;
extern float *const fhdr69;
extern float *const fhdr70;
extern float *const fini;
extern float *const fmean;
extern float *const fmt;
extern float *const gcarc;
extern int   *const ia;
extern int   *const iacc;
extern int   *const iamph;
extern int   *const ib;
extern int   *const ichem;
extern int   *const iday;
extern int   *const idep;
extern int   *const idisp;
extern int   *const idown;
extern int   *const idrop;
extern int   *const ieast;
extern int   *const ievreg;
extern int   *const ievtyp;
extern int   *const iftype;
extern int   *const iglch;
extern int   *const igood;
extern int   *const imagtyp;		/* magnitude type. maf 970205 */
extern int   *const imagsrc;		/* magnitude source. maf 970205 */
extern int   *const ihdr13;
extern int   *const ihdr14;
extern int   *const ihdr15;
extern int   *const ihdr16;
extern int   *const ihdr17;
extern int   *const ihdr18;
extern int   *const ihdr19;
extern int   *const ihdr20;
extern int   *const ihdr4;
extern int   *const ihglp;
extern int   *const ihorza;
extern int   *const iinst;
extern int   *const illlbb;
extern int   *const ilowsn;
extern int   *const imb;		/* these 20 added to support */
extern int   *const ims;		/* magnitude. maf 970205 */
extern int   *const iml;
extern int   *const imw;
extern int   *const imd;
extern int   *const imx;
extern int   *const ineic;
extern int   *const ipdeq;
extern int   *const ipdew;
extern int   *const ipde;
extern int   *const iisc;
extern int   *const ireb;
extern int   *const iusgs;
extern int   *const ibrk;
extern int   *const icaltech;
extern int   *const illnl;
extern int   *const ievloc;
extern int   *const ijsop;
extern int   *const iuser;
extern int   *const iunknown;
extern int   *const inorth;
extern int   *const inucl;
extern int   *const io;
extern int   *const iother;
extern int   *const ipostn;
extern int   *const ipostq;
extern int   *const ipren;
extern int   *const ipreq;
extern int   *const iquake;
extern int   *const iqual;
extern int   *const iradev;
extern int   *const iradnv;
extern int   *const irldta;
extern int   *const irlim;
extern int   *const isro;
extern int   *const istreg;
extern int   *const isynth;
extern int   *const it0;
extern int   *const it1;
extern int   *const it2;
extern int   *const it3;
extern int   *const it4;
extern int   *const it5;
extern int   *const it6;
extern int   *const it7;
extern int   *const it8;
extern int   *const it9;
extern int   *const itanev;
extern int   *const itannv;
extern int   *const itime;
extern int   *const iunkn;
extern int   *const iup;
extern int   *const ivel;
extern int   *const ivolts;
extern int   *const iwwsn1;
extern int   *const iwwsn2;
extern int   *const ixy;
extern int   *const ixyz;
extern int   *const iztype;
extern int   *const iqb;		/* These 17 for */
extern int   *const iqb1;		/* ievtyp.  maf 970325 */
extern int   *const iqb2;
extern int   *const iqbx;
extern int   *const iqmt;
extern int   *const ieq;
extern int   *const ieq1;
extern int   *const ieq2;
extern int   *const ime;
extern int   *const iex;
extern int   *const inu;
extern int   *const inc;
extern int   *const io_;
extern int   *const il;
extern int   *const ir;
extern int   *const it;
extern int   *const iu;
extern int   *const ieq3;           /* These 9 for ievtype to keep */
extern int   *const ieq0;           /* up with database.  maf 970325 */
extern int   *const iex0;
extern int   *const iqc;
extern int   *const iqb0;
extern int   *const igey;
extern int   *const ilit;
extern int   *const imet;
extern int   *const iodor;

extern char  *const kstnm;
extern char  *const kevnm;
extern char  *const khole;
extern char  *const ko;
extern char  *const ka;
extern char  *const kt0;
extern char  *const kt1;
extern char  *const kt2;
extern char  *const kt3;
extern char  *const kt4;
extern char  *const kt5;
extern char  *const kt6;
extern char  *const kt7;
extern char  *const kt8;
extern char  *const kt9;
extern char  *const kf;
extern char  *const kuser0;
extern char  *const kuser1;
extern char  *const kuser2;
extern char  *const kcmpnm;
extern char  *const knetwk;
extern char  *const kdatrd;
extern char  *const kinst;

extern int   *const lcalda;
extern int   *const leven;
extern int   *const lhdr5;
extern int   *const lovrok;
extern int   *const lpspol;
extern int   *const nevid;
extern int   *const nhdr15;
extern int   *const norid;
extern int   *const npts;
extern int   *const nsnpts;
extern int   *const nvhdr;
extern int   *const nwfid;
extern int   *const nxsize;
extern int   *const nysize;
extern int   *const nzdttm;
extern int   *const nzhour;
extern int   *const nzjday;
extern int   *const nzmin;
extern int   *const nzmsec;
extern int   *const nzsec;
extern int   *const nzyear;
extern float *const o;
extern float *const odelta;
extern float *const origin;
extern float *const resp0;
extern float *const resp1;
extern float *const resp2;
extern float *const resp3;
extern float *const resp4;
extern float *const resp5;
extern float *const resp6;
extern float *const resp7;
extern float *const resp8;
extern float *const resp9;
extern float *const sb;
extern float *const scale;
extern float *const sdelta;
extern float *const stdp;
extern float *const stel;
extern float *const stla;
extern float *const stlo;
extern float *const t0;
extern float *const t1;
extern float *const t2;
extern float *const t3;
extern float *const t4;
extern float *const t5;
extern float *const t6;
extern float *const t7;
extern float *const t8;
extern float *const t9;
extern float *const time0;
extern float *const time1;
extern float *const time2;
extern float *const time3;
extern float *const time4;
extern float *const time5;
extern float *const time6;
extern float *const time7;
extern float *const time8;
extern float *const time9;
extern float *const user0;
extern float *const user1;
extern float *const user2;
extern float *const user3;
extern float *const user4;
extern float *const user5;
extern float *const user6;
extern float *const user7;
extern float *const user8;
extern float *const user9;
extern float *const xmaximum;
extern float *const xminimum;
extern float *const ymaximum;
extern float *const yminimum;
extern float *const Exthdr;
extern float *const Fhdr;
extern int   *const Ihdr;
extern int   *const Lhdr;
extern int   *const Nhdr;
extern int   *const Niv;
extern int   *const Nzdttm;
#endif

#endif /* _HDR_H_ */

