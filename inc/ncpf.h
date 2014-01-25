/** 
 * @file   ncpf.h
 * 
 * @brief  Macro processing and strings
 * 
 */

#ifndef _NCPF_H_
#define _NCPF_H_

#include <stdio.h>

void appendstring ( char *string, 
                    int string_s, 
                    char *input, 
                    int input_s, 
                    char *output, 
                    int output_s);
void changestring ( char *string1, 
                    int string1_s, 
                    char *string2, 
                    int string2_s, 
                    char *input, 
                    int input_s, 
                    char *output, 
                    int output_s);
void closemacro ( int *nerr);
void deletestring ( char *string, 
                    int string_s, 
                    char *input, 
                    int input_s, 
                    char *output, 
                    int output_s);
void executemacro ( char *kmacroname, 
                    int kmacroname_s, 
                    char *kmacroargs, 
                    int kmacroargs_s, 
                    int *nerr);
void getembeddedargs ( char *kfunction, 
                       int kfunction_s, 
                       int nc, 
                       int *ic, 
                       int maxargs, 
                       int iops[], 
                       float args[], 
                       int *nargs, 
                       int *nerr);
void getepoch ( char *kfunction, 
                int *nc, 
                int ic, 
                char *kvalue, 
                int *nerr);
double hrToEpoch ( int year, 
                   int month, 
                   int day, 
                   int hour, 
                   int min, 
                   float second, 
                   int *nerr);
void doyToMD ( int d, 
               int lp, 
               int *pm, 
               int *pd);
int MDtoDoy ( int month, 
              int day, 
              int leap);
int isLeapYear ( int yr);
int mdyToJul ( int mm, 
               int id, 
               int iyyy);
void getmacroinfo ( int *macrolevel, 
                    char *varsname, 
                    int varsname_s);
void getnumericargs ( char *kfunction, 
                      int kfunction_s, 
                      int nc, 
                      int *ic, 
                      int maxargs, 
                      float args[], 
                      int *nargs, 
                      int *nerr);
void getstringargs ( char *kfunction, 
                     int kfunction_s, 
                     int nc, 
                     int *ic, 
                     int maxargs, 
                     int icstart[], 
                     int icstop[], 
                     int *nargs, 
                     int *nerr);
int gettime (int lmax, int lvalue, double tvalue, double *value);
double tmMakeEpochTime ( int year, 
                         int month, 
                         int day, 
                         int hour, 
                         int min, 
                         float second);
int isleap ( int yr);
void mnday ( int d, 
             int lp, 
             int *pm, 
             int *pd);
int julday ( int mm, 
             int id, 
             int iyyy);
int indexs ( char *text, 
             int ntext, 
             char *string, 
             int nstring, 
             int lfwd, 
             int locc);
void inicpf (void);
void macrokeyword ( char *kmacroargs, 
                    int kmacroargs_s, 
                    FILE *nun, 
                    char *keys, 
                    int keys_s, 
                    int *nerr);
int macroline ( char *kmacroline, 
                int kmacroline_s, 
                int *ncmacroline, 
                int *nerr);
void macroordered ( char *kmacroargs, 
                    int kmacroargs_s, 
                    FILE *nun, 
                    char *kline, 
                    int kline_s, 
                    int *nerr);
void macropreamble ( char *kmacroargs, 
                     int kmacroargs_s, 
                     FILE *nun, 
                     int *nerr);
int macrostatus (void);
int nstrlensp ( char *kstring, 
                int kstring_s);
void openmacro ( char *kmacroname, 
                 int kmacroname_s, 
                 char *kmacroargs, 
                 int kmacroargs_s, 
                 int *nerr);
void prependstring ( char *string, 
                     int string_s, 
                     char *input, 
                     int input_s, 
                     char *output, 
                     int output_s);
void processembedded ( char *kfunction, 
                       int kfunction_s, 
                       int nc, 
                       int *ic, 
                       double first, 
                       char *kvalue, 
                       int kvalue_s, 
                       int *nerr);
void processfunction ( char *kfunction, 
                       int kfunction_s, 
                       char *kvalue, 
                       int kvalue_s, 
                       int *nerr);
void processline ( char *kname, 
                   int kname_s, 
                   char *kiline, 
                   int kiline_s, 
                   int niline, 
                   char *koline, 
                   int koline_s, 
                   int *noline, 
                   int *nerr);
void processnumeric ( char *kfunction, 
                      int kfunction_s, 
                      int nc, 
                      int *ic, 
                      int index, 
                      char *kvalue, 
                      int kvalue_s, 
                      int *nerr);
void processstring ( char *kfunction, 
                     int kfunction_s, 
                     int nc, 
                     int *ic, 
                     int *index, 
                     char *kvalue, 
                     int kvalue_s, 
                     int *nerr);
int isleap ( int yr);
char *tmGetLastError (void);
void tmPrintLastError (void);
void SetWarningMessage ( char *string);
int NotNumericString ( char *token);
void ValidateSec ( float *second);
void ValidateMin ( int *min);
void ValidateHour ( int *hour);
void ValidateDay ( int year, 
                   int month, 
                   int *day);
void ValidateMon ( int *month);
void ValidateDOY ( int year, 
                   int *DayOfYear);
int julday ( int mm, 
             int id, 
             int iyyy);
void caldat ( int julian, 
              int *mm, 
              int *id, 
              int *iyyy);
int yrday ( int mo, 
            int day, 
            int lp);
void mnday ( int d, 
             int lp, 
             int *pm, 
             int *pd);
double tmGetEpochTime (void);
double tmMakeEpochTime ( int year, 
                         int month, 
                         int day, 
                         int hour, 
                         int min, 
                         float second);
int tmDecodeEpochTime ( double time, 
                        int *year, 
                        int *month, 
                        int *day, 
                        int *hour, 
                        int *min, 
                        float *second);
char *tmListEpochTime ( double time, 
                        int form);
double tmStrToEpochTime ( char *string);
void setmacrolevel ( int imacrolevel);
void setmacrostatus ( char *kstatus, 
                      int kstatus_s);

#endif /* _NCPF_H_ */
