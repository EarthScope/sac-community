/** 
 * @file   co.h
 * 
 * @brief  Command and File Operations
 * 
 */

#ifndef _CO_H_
#define _CO_H_

#include <stdio.h>
#include <math.h>
#include "clf.h"

#include "config.h"
/** 
 * Complex functions are found in complex.h
 * Select functions are found in select.h
 *
 */

double fmin(double a, double b);
double fmax(double a, double b);
long int lround(double z);


int backspace ( FILE *stream, 
                int n);
void ext_init (void);
char *fstrncpy ( char *to, 
                 int tolen, 
                 char *from, 
                 int fromlen);
int getfline ( FILE *pfd, 
               char *pch, 
               int maxlen);
int getline_sac ( FILE *pfd, 
              char *pch, 
              int maxlen);
short int izshft ( short int *pint, 
                   short int *pnumshft);
int min ( int a, 
	  int b);
int max ( int a, 
          int b);
int isign ( int a, 
            int b);
double sign ( double a, 
              double b);
double powi ( double b, 
              int x);
int ipow ( int b, 
           int x);

char *strscpy ( char *to, 
                char *from, 
                int l);
char *subscpy ( char *to, 
                int start, 
                int end, 
                int len, 
                char *from);
void tokenize ( char ***argv, 
                int *argc, 
                char *linein, 
                int *nerr);
void zauxfile ( char *ksub, 
                int ksub_s, 
                char *kfile, 
                int kfile_s, 
                int *nerr);
void zbasename ( char *name, 
                 int name_len);
char *sacaux();
void zclose ( int *nfu, 
              int *nerr);
void zclosec ( int *pfd);
void zcloses ( FILE **nfu, 
               int *nerr);
void zdest ( char *kname, 
             int kname_s, 
             int *nerr);
void zdestf ( char *kbase, 
              int kbase_s, 
              int *nerr);
void zexecute ( int index, 
                int *nerr);
string_list *
zfiles ( char *kdirin, 
         char *kpatrn, 
         int *nErr);
void zgetc ( int *array, 
             char *str, 
             int pnumc);
void zgetgd ( char *name, 
              int name_len);
void zgimsg ( int argc, 
              char **argv, 
              char *mess, 
              int messlen);
void  zgpmsg ( char *prmt, 
             int  prmtlen, 
             char *msg, 
             int  msglen);
void  zgtmsg ( char *prmt, 
               int  prmtlen, 
               char *msg, 
               int  msglen);
void zgwindowsize_ ( int *number_rows, 
                     int *number_columns, 
                     int *error_flag);
void zinquire ( char *kname, 
                int *lexist);
void zload ( char *kfile, 
             int *index, 
             int *nerr);
void zmemad ( short *pvar, 
              void *pvarloc);
void znfile ( int *nfu, 
              char *kname, 
              int kname_s, 
              char *ktype, 
              int ktype_s, 
              int *nerr);
void znfiles ( FILE **nfu, 
               char *kname, 
               int kname_s, 
               char *ktype, 
               int ktype_s, 
               int *nerr);
void zopen_sac ( int *nfu, 
                 char *kname, 
                 int kname_s, 
                 char *ktype, 
                 int ktype_s, 
                 int *nerr);
void zopenc ( int *pfd, 
              char *pfname, 
              int *pnewfl, 
              int *pro, 
              int *pnerr, 
              int pfnlen);
void zopens ( FILE **nfu, 
              char *kname, 
              int kname_s, 
              char *ktype, 
              int ktype_s, 
              int *nerr);
void zputc ( char *str, 
             int strlen, 
             int *array, 
             int pnumc);
void zquit (void);
void zrabs ( int *pfd, 
             char *array, 
             int pnwords, 
             int *pswords, 
             int *pnerr);
void zrun ( FILE **nfun, 
            char *runfile, 
            int runfile_s, 
            int *nerr);
void zrunname ( char *name, 
                int name_s, 
                char *args, 
                int args_s, 
                FILE **nfun, 
                char *runfile, 
                int runfile_s, 
                int *nerr);
void zruntext ( char *text, 
                int text_s, 
                FILE *nfun, 
                int *nerr);
void zsleep ( int timeout);
int  timer  (int set_get, 
             int usec);
void zsysop ( char *comstr, 
              int dummylen, 
              int *pnumc, 
              int *perr);
void zwabs ( int *pfd, 
             char *array, 
             int pnwords, 
             int *pswords, 
             int *pnerr);

#if MISSING_FUNC_COPYSIGN
double copysign(double x, double y);
#endif

#if MISSING_FUNC_ROUND
double round(double x);
#endif

#endif /* _CO_H_ */

