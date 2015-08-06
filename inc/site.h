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
    int nsitecomnames;
    int isitecomindex[MSITECOMNAMES];
} ;

/** 
 * @struct kmsite
 *    Site specific command variables
 * 
 */
struct t_kmsite {
    char ksitecomnames[MSITECOMNAMES][9];
} ;

void initsite(void);
void xsitecom(int index, int *nerr);
void xtestsite(int *nerr);

#define SITE_EXTERN \
    extern struct t_kmsite kmsite; \
    extern struct t_cmsite cmsite;

#endif /* _SITE_H_ */
