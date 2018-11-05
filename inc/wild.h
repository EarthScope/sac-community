/** 
 * @file   wild.h
 * 
 * @brief  Wildcard characters
 * 
 */

#ifndef _WILD_H_
#define _WILD_H_

#include "strlist.h"

/** 
 * @struct kmwild 
 *    Wildcard Characters
 */
struct t_kmwild {
    char sngl;     /** Single wildcard character */
    char mult;     /** Multiple wildcard character */
    char ccon[3];  /**  */
} ;

/** 
 * @struct cmwild
 *    
 */
struct t_cmwild {
    int igcon;     /**  */
} ;

#define WILD_EXTERN \
    extern struct t_kmwild kmwild; \
    extern struct t_cmwild cmwild;


void getdir(char *kpath, int kpath_s, char *kdirpt, int kdirpt_s, char *kpatpt,
            int kpatpt_s);
int lfilec(char *kentry, int ndflou, char *kdflou, int nerr);
int lwildc(char *flist, int flist_s);
void wildch(int xsngl, int xmult, char *xccon);
string_list *wildfl(char *kdfdir, int kdfdir_s, string_list * list,
                    int *lexpnd);

#endif /* _WILD_H_ */
