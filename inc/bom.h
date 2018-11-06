/** 
 * @file   bom.h
 * 
 * @brief  Binary Operations
 * 
 */

#ifndef _BOM_H_
#define _BOM_H_

#include "mach.h"
#include "strlist.h"
#include "sacdata.h"

/** 
 * @struct kmbom
 *    Binary Operations Lengths and Indicies
 */
struct t_cmbom {
    int nbfl;
    int ibflc;
    int nlenbf;
    int ndxhbf;
    int ndx1bf;
    int ndx2bf;
    int lnewhdr;                /* optionally let the header information come from
                                   the new file being added or whatever in
                                   addf, subf, mulf, and divf.  */
} ;

/** 
 * @struct kmbom
 *    Binary Operations Characters
 */
struct t_kmbom {
    char kecnpt[9];
    char kecdel[9];
} ;

#define BOM_EXTERN \
    extern struct t_kmbom kmbom;                \
    extern struct t_cmbom cmbom;


void getbfl(string_list * list, int ibfl, int ldta, int *nlen, int *ndx1,
            int *ndx2, int *nerr);
void inibom(void);
void relbfl(int *nerr);
void xaddf(int *nerr);
void xboec(int *nerr);
void xbomc(int index, int *nerr);
void xdivf(int *nerr);
void xmerge(int *nerr);
void xmulf(int *nerr);
void xsubf(int *nerr);

int vbeven();
int isFatal(char *key);
int isWarning(char *key);
int delta_equal(sac * s1, sac * s2);
int npts_equal(sac * s1, sac * s2);
int time_equal(sac * s1, sac * s2);
int station_equal(sac * s1, sac * s2);
int files_similar(string_list * list);
void sac_extrema(sac * s);

#endif /* _BOM_H_ */
