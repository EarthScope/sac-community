/** 
 * @file   sddhdr.h
 * 
 * @brief  SDD Header
 * 
 * 
 */

#ifndef _SDDHDR_H_
#define _SDDHDR_H_

#define	MBSHDR	716
#define	MSCOM	10
#define	MSREP	150
#define	MWESHD	(MSREP + 12 + 2)
#define	MWSHDR	179

struct t_kmshdr {
    char kshdr[MBSHDR * 9];
} ;

#define SDD_EXTERN                              \
    extern int *const iscalg;                   \
    extern int *const isclas;                   \
    extern int *const iscom;                    \
    extern int *const isdate;                   \
    extern int *const isdelt;                   \
    extern int *const isfrmt;                   \
    extern int *const ishdr;                    \
    extern int *const isnpts;                   \
    extern int *const isrep;                    \
    extern int *const issdep;                   \
    extern int *const issel;                    \
    extern int *const issla;                    \
    extern int *const isslo;                    \
    extern int *const istime;                   \
    extern char *const kschan;                  \
    extern char *const kschdr;                  \
    extern char *const ksclas;                  \
    extern char *const kscom;                   \
    extern char *const ksevnm;                  \
    extern char *const ksfrmt;                  \
    extern char *const ksstnm;                  \
    extern int *const Iscom;                    \
    extern int *const Ishdr;                    \
    extern int *const Isrep;


#endif /* _SDDHDR_H_ */
