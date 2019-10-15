/** 
 * @file   bot.h
 * 
 * @brief  Text Manipulation
 * 
 */

#ifndef _BOT_H_
#define _BOT_H_

#include <stdlib.h>

void capf(int *nerr);
void chpf(int *nerr);
void crname(char *kname, int kname_s, char kdelim, char *kappnd, int kappnd_s,
            int *nerr);
void clipdp(double *xdp, double *ydp, int *ildp, double *xrect, double *yrect,
            int *ncdp);
int indexa(char *string, int string_s, int kchar, int lfwd, int locc);
int indexb(char *string, int string_s);
int indexc(char *string, int string_s, int kchar);
void modcase(int upflag, char *input, int nchar, char *output);
int lequal(char *ksrch, int ksrch_s, char *klist, int klist_s, int nlist,
           int *index);
void locdp(double x, double y, double *xr, double *yr, int *ilocdp);
int nequal(char *ksrch, char *klist, int klist_s, int nlist);
int terminate(char *paddedString);
void upcase(char *kinput, int nchar, char *koutpt, int koutpt_s);
char *upcase_dup(char *s);
void wrlist(char *klist, int klist_s, int nlist);

char *strcut(char *in, unsigned int start, unsigned int end);
char *lstrip(char *s);
char *rstrip(char *s);
int snprintfcat(char *buf, size_t size, const char* format, ...);
char * rstrip_char(char *s, char c);
char * lstrip_char(char *s, char c);


#endif /* _BOT_H_ */
