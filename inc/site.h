/** 
 * @file   site.h
 * 
 * @brief  Site specific command Module
 * 
 */

#ifndef _SITE_H_
#define _SITE_H_

#define	MODULESITECOM   99
#define	MSITECOMNAMES   10

/** 
 * @struct cmsite
 *    Site specific command variables
 * 
 */
struct t_cmsite {
  int  nsitecomnames;
  int  isitecomindex[MSITECOMNAMES];
} cmsite;

/** 
 * @struct kmsite
 *    Site specific command variables
 * 
 */
struct t_kmsite {
  char ksitecomnames[MSITECOMNAMES][9];
} kmsite;


#ifdef DOINITS

   int *const Isitecomindex = &cmsite.isitecomindex[0] - 1;

#else

   extern int *const Isitecomindex;

#endif

void initsite (void);
void xsitecom ( int index, 
                int *nerr);
void xtestsite ( int *nerr);

#endif /* _SITE_H_ */
