/** 
 * @file   bot.h
 * 
 * @brief  Text Manipulation
 * 
 */

#ifndef _BOT_H_
#define _BOT_H_

void capf ( int *nerr);
void chpf ( int *nerr);
void crname ( char *kname, 
              int kname_s, 
              char kdelim, 
              char *kappnd, 
              int kappnd_s, 
              int *nerr);
void clipdp ( float *xdp, 
              float *ydp, 
              int *ildp, 
              float *xrect, 
              float *yrect, 
              int *ncdp);
int indexa ( char *string, 
             int string_s, 
             int kchar, 
             int lfwd, 
             int locc);
int indexb ( char *string, 
             int string_s);
int indexc ( char *string, 
             int string_s, 
             int kchar);
void modcase(int   upflag, 
	     char *input, 
	     int   nchar, 
	     char *output);
int lequal ( char *ksrch, 
             int ksrch_s, 
             char *klist, 
             int klist_s, 
             int nlist, 
             int *index);
void locdp ( double x, 
             double y, 
             float *xr, 
             float *yr, 
             int *ilocdp);
void locdp ( double x, 
             double y, 
             float *xr, 
             float *yr, 
             int *ilocdp);
int nequal ( char *ksrch, 
             char *klist, 
             int klist_s, 
             int nlist);
int terminate ( char *paddedString);
void upcase ( char *kinput, 
              int nchar, 
              char *koutpt, 
              int koutpt_s);
char * upcase_dup(char *s);
void wrlist ( char *klist, 
              int klist_s, 
              int nlist);

char * strcut(char        *in, 
              unsigned int start, 
              unsigned int end);
char *lstrip(char *s);
char *rstrip(char *s);
#endif /* _BOT_H_ */
