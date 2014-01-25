/** 
 * @file   cnd.h
 * 
 * @brief  Conditionals
 * 
 */

#ifndef _CND_H_
#define _CND_H_

#include <stdio.h>
#include "mach.h"

/** 
 * @param MDOLEVEL
 *     Maximum Do Level
 */
#define	MDOLEVEL	10

/** 
 * @param MDOLEVEL
 *     Maximum If Level
 */
#define	MIFLEVEL	10

struct t_cnd {
  int niflevel;
  int lifresp[MIFLEVEL];
  int ndolevel;
  int ndolines[MDOLEVEL];
  int ndotype[MDOLEVEL];
  int idoin1[MDOLEVEL];
  int idoin2[MDOLEVEL];
} cnd;

struct t_kcnd {
  char kdovar[MDOLEVEL][MCPFN+1];
  char kdolist[MDOLEVEL][MCPFN+1];
  char kdoname[MDOLEVEL][MCPFN+1];
} kcnd;

void   getclun  (FILE **nun, 
		 int   *nerr);
void   getdolen (int *nlines, 
		 int *nerr);
int    ldolist  (int *nerr);
void   skipdo   (int *nerr);
void   skipif   (int *nerr);
void   xbreak   (int *nerr);
void   xcndc    (int  index, 
		 int *nerr);
void   xdo      (int *nerr);
void   xelse    (int *nerr);
void   xelseif  (int *nerr);
void   xenddo   (int *nerr);
void   xendif   (int *nerr);
void   xif      (int *nerr);
void   xwhile   (int *nerr);


#ifdef DOINITS
int *const Idoin1 = &cnd.idoin1[0] - 1;
int *const Idoin2 = &cnd.idoin2[0] - 1;
int *const Lifresp = &cnd.lifresp[0] - 1;
int *const Ndolines = &cnd.ndolines[0] - 1;
int *const Ndotype = &cnd.ndotype[0] - 1;
#else
extern int *const Idoin1;
extern int *const Idoin2;
extern int *const Lifresp;
extern int *const Ndolines;
extern int *const Ndotype;
#endif

#endif /* _CND_H_ */
