/** 
 * @file   datafilelist.h
 * 
 * @brief  Data file list Functions
 * 
 */

#ifndef _DFL_H_
#define _DFL_H_


#include "mach.h"

struct t_kmdatafilelist {
  char kselectmode[9]; /** Current data file list mode. could use a enum */
} kmdatafilelist;

struct t_cmdatafilelist {
  int nentries;       /** Total Entries in the data file list */
  int iselect[MDFL];  /** Current entries which are selected  */
  int nselect;        /** Number of entries selected          */
  int jselect;        /** Current index within iselect        */
} cmdatafilelist;

int       nextinputfile      (int   *ientry);
void      selectinputfiles   (int   *list, 
			      int    nlist);
void      setinputmode       (char  *mode);


#ifdef DOINITS

   int *const Iselect = &cmdatafilelist.iselect[0] - 1;

#else

   extern int *const Iselect;

#endif

#endif /* _DFL_H_ */

