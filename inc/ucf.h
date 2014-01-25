/** 
 * @file   ucf.h
 * 
 * @brief  Utilities
 * 
 */

#ifndef _UCF_H_
#define _UCF_H_

#include <stdio.h>

#include "clf.h"

void basenm ( char *kbase, 
              int kbase_s, 
              int n1, 
              int n2, 
              string_list *list,
              int *nerr);
int byteswap ( void *swappee, 
               int Nbytes);
double cdouble ( char *s, 
                 int *nerr);
void copy_float(float  *dest,
                float  *src,
                int     n);
void cnvatf ( char *kfloat, 
              int kfloat_s, 
              float *floatn, 
              int lstrict, 
              int *nerr);
void cnvati ( char *kintgr, 
              int kintgr_s, 
              int *intgr, 
              int lstrict, 
              int *nerr);
void cnvfta ( double float_, 
              int nchar, 
              int nsig, 
              char *kfloat, 
              int kfloat_s);
void cnvita ( int intgr, 
              char *kintgr, 
              int kintgr_s);
void copy ( int *srce, 
            int *sink, 
            int ncopy);
void copydouble ( double *source, 
                  int length, 
                  double *sink);
void copyi ( int *isrce, 
             int *isink, 
             int ncopy);
void copykc ( char *kin, 
              int kin_s, 
              int ncopy, 
              char *kout);
void ddttm ( int *ndttm1, 
             int *ndttm2, 
             float *diff);
void delims ( char *ktd, 
              int ktd_s, 
              int ntd, 
              char *kmd, 
              int kmd_s, 
              int nmd);
void distaz ( double the, 
              double phe, 
              float *ths, 
              float *phs, 
              int ns, 
              float *dist, 
              float *az, 
              float *baz, 
              float *xdeg, 
              int *nerr);
void evallogical ( char *string, 
                   int string_s, 
                   char *result, 
                   int result_s);
void extrma ( float *array, 
              int incrmt, 
              int number, 
              float *aminm, 
              float *amaxm, 
              float *amean);
void fill ( float array[], 
            int number, 
            double value);
void getxw ( double ywloc, 
             float *xwloc);
void getyw ( double xwloc, 
             float *ywloc);
void idttm ( int *ndttm1, 
             double secs, 
             int *ndttm2);
void idttmf ( int *ndttm1, 
              int nsec, 
              int nmsec, 
              int *ndttm2);
void incdat ( int nyrold, 
              int njdold, 
              int njdinc, 
              int *nyrnew, 
              int *njdnew);
void inctim ( int nhrold, 
              int nmnold, 
              int nscold, 
              int nmsold, 
              double secinc, 
              int *nhrnew, 
              int *nmnnew, 
              int *nscnew, 
              int *nmsnew, 
              int *nexday);
void inctimf ( int nhrold, 
               int nmnold, 
               int nscold, 
               int nmsold, 
               int nscinc, 
               int nmsinc, 
               int *nhrnew, 
               int *nmnnew, 
               int *nscnew, 
               int *nmsnew, 
               int *nexday);
void initok (void);
void kadate ( int iyear, 
              int ijday, 
              int ncdate, 
              char *kkdate, 
              int kkdate_s, 
              int *nerr);
void kadttm ( int *idttm, 
              char *kdttm, 
              int kdttm_s, 
              int *nerr);
void katime ( int ihour, 
              int imin, 
              int isec, 
              int imsec, 
              int nctime, 
              char *kktime, 
              int kktime_s, 
              int *nerr);
void kidate ( int iyear, 
              int ijday, 
              int *imonth, 
              int *iday, 
              int *nerr);
void kijdat ( int iyear, 
              int imonth, 
              int iday, 
              int *ijday, 
              int *nerr);
int lbsrch ( char *ksrch, 
             int ncsrch, 
             char *klist, 
             int klist_s, 
             int nlist, 
             int *index);
int ldttm ( int *ndttm);
int linrng ( double value, 
             double valmin, 
             double valmax);
void ljust ( char *ksym, 
             int ksym_s);
int nccomp ( char *ktoken, 
             char *klist, 
             int klist_s, 
             int nlist, 
             int nchar);
int next2 ( int num);
void ophelp ( char *kitem, 
              int kitem_s, 
              FILE **nun, 
              char *khfile, 
              int *nerr);
void poptok ( char *ktext, 
              int nctext, 
              int *icpntr, 
              int *icloc1, 
              int *icloc2, 
              int *itype);
void sorti ( int *iain, 
             int niain, 
             int lincr, 
             int *iaout);
void srtndx ( float *value, 
              int num, 
              int *index, 
              int *nerr);
void timecheck ( int *year, 
                 int *day, 
                 int *hour, 
                 int *min, 
                 int *sec, 
                 int *ms);
void tokdel ( char *ktd, 
              int ktd_s, 
              int ntd, 
              char *kmd, 
              int kmd_s, 
              int nmd);
double tosecs ( int nsec, 
                int nmsec);
void wapf (void);
void wrcom (void);
void wrhelp ( char *ktoken, 
              int ktoken_s, 
              int imode, 
              int lprint, 
              int *nerr);
int internal_pager ( FILE *nun);
void external_pager ( char *filename);
void wrindx ( char *klist, 
              int klist_s, 
              int *ilist, 
              int nlist);

#endif /* _UCF_H_ */
