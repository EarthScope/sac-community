/** 
 * @file mach.h
 *
 * @brief Define Constants
 */

#ifndef _MACH_H
#define _MACH_H

#ifdef PI
#undef PI
#endif
#define	FMAXFLOAT	3.40282e38
#define	FMINFLOAT	(-FMAXFLOAT)
#define	KDIRDL	'/'
#define	KSUBDL	'/'
#define	KWCONC	"[]"
#define	KWMULT	'*'
#define	KWSNGL	'?'
#define	MCMSG	1001
#define	MCPFN	128		/* max length of filename maybe. maf 960619 */
#define	MCPW	8
#define	MDFL	1000		/* max number of files in memory? */
/** 
 * @param DATA_FILE_LIST_MAXIMUM
 *    Maximum number of files in the data file list
 */
#define DATA_FILE_LIST_MAXIMUM         MDFL

#define MAXCHARS ( MCPFN * MDFL ) /* max number of chars in list of files in mem. maf 970806 */
/* #define MFILELIST MDFL*MCPFN/2 */
#define	MLARGE	2147483647
#define	MODEFILECASE	0
#define	MUNINP	stdin
#define	MUNOUT	stdout
#define munout  MUNOUT

#define	RNDOFF	1.0e-06
#define	TODEG	57.29577950
#define	TORAD	(1./TODEG)
#define	VLARGE	3.40282e38
#define	VSMALL	1.0e-30


#endif /* _MACH_H */
