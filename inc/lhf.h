/** 
 * @file   lhf.h
 * 
 * @brief  List Header Functions
 * 
 * 
 */

#ifndef _LHF_H_
#define _LHF_H_

#include "hdr.h"

#define	MAHDR	20
#define	MLHLST	100
#define	MPKRPT	18      /** Number of Header names for "PICKS" */
#define	MRPT	MSTRPT
#define	MRPTTP	10      /** Number of Header Report Types */
#define	MSPRPT	20
#define	MSTRPT	94      /** Number of Header names for "DEFAULT" */

/** 
 * @param SAC_HEADER_NAMES_DEFAULT_LENGTH
 *    Number of Header names to output for "DEFAULT"
 *
 * @date 1996/12/12   81 became 91 as new fields were added. maf 
 * @date 1997/02/05   91 became 94 as mag, imagtyp, and imagsrc 
 *                    (magnitude, magnitude type, and magnitude source) 
 *                    were added. maf 970205
 * @date 2009 Feb 15 Changed from MSTRPT
 */
#define SAC_HEADER_NAMES_DEFAULT_LENGTH  94

/** 
 * @param SAC_HEADER_NAMES_PICKS_LENGTH
 *
 * @date 2009 Feb 15 Changed from MPKRPT
 *
 */
#define SAC_HEADER_NAMES_PICKS_LENGTH    18

#define	MTM	13

struct t_cmlhf {
  int nstrpt;         /** Number of Header names for "DEFAULT"   */
  int npkrpt;         /** Number of Header names for "PICKS"     */
  int nsprpt;         /** Number of Header names for "SPECIAL"   */
  int nrpt;           /** Current number of output header values */
  int nrpttp;         /** Number of Header Report Types          */
  int irpttp;         /** Current Report Type                    */
  int lstall;         /** Print out all Header variables         */
  int nlhlst;         /** Number of "selected" files for listhdr */
  int ilhlst[MLHLST]; /** List of "selected" files for listhdr   */
  int nlhcol;         /** Number of columns                      */
  int itmkrf;         /** First Index in header (characters) for Time Picks */
  int itmfnm[MTM];    /** Indicies in header (floats) for Time Picks */
  int icatf;          /** Floating point category */
  int icatn;          /** Integer category        */
  int icati;          /** Enum category           */
  int icatl;          /** Logical category        */
  int icatk;          /** Character category      */
  int icata;          /** Auxillary category      */
  int nlhdr;          /** Unused */
  int nkhdr[SAC_HEADER_STRINGS];  /** Length of Character strings*/
} cmlhf;

struct t_kmlhf {
  char kstrpt[MSTRPT][9];   /** Header names for "DEFAULT"  */
  char kpkrpt[MPKRPT][9];   /** Header names for "PICKS"    */
  char ksprpt[MSPRPT][9];   /** Header names for "SPECIAL"  */
  char krpt[MRPT][9];       /** Header names for actual report    */
  char krpttp[MRPTTP][9];   /** Number of Header Report Types     */
  char kfhdr[SAC_HEADER_FLOATS][9];     /** Ids for header values, floats     */
  char knhdr[SAC_HEADER_ENUMS][9];     /** Ids for header values, integers   */
  char kihdr[SAC_HEADER_ENUMS][9];     /** Ids for header values, enums      */
  char klhdr[SAC_HEADER_LOGICALS][9];     /** Ids for header values, logicals   */
  char kkhdr[SAC_HEADER_STRINGS][9];     /** Ids for header values, characters */
  char kahdr[MAHDR][9];     /** Ids for auxillary header values   */
  char kiv[SAC_ENUMS][9];   /** Ids for enumerated values         */
  char kdiv[SAC_ENUMS][33]; /** Description for enumerated values */
} kmlhf;


#ifdef DOINITS

int *const Ilhlst = &cmlhf.ilhlst[0] - 1;
int *const Itmfnm = &cmlhf.itmfnm[0] - 1;
int *const Nkhdr = &cmlhf.nkhdr[0] - 1;

#else

extern int *const Ilhlst;
extern int *const Itmfnm;
extern int *const Nkhdr;

#endif

#endif /* _LHF_H_ */
