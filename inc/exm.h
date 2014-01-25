/** 
 * @file   exm.h
 * 
 * @brief  Execution Module
 * 
 */


#ifndef _EXM_H_
#define _EXM_H_

#include <stdio.h>

#include "mach.h"

#define	MECTP	5
#define	MEVAL	10
#define	MFEVAL	30
#define	MFGTP	15
#define	MLEVAL	10
#define	MMCDIR	100
#define	MREPTP	30
#define	MTRACES	20
#define	MTRANSCRIPTS	5
#define	MTYPES	6


/** 
 * @struct kmexm
 *    Execution Module Characters
 */
struct t_kmexm {
  char	kstate[9];      /** Not Used */
  char kprmt[128];	/** String containing the prompt, terminated with '$' */
  char kecnof[9];       /** No Files Error Type*/
  char  kfgtp[MFGTP][9];       /** Names of available functions */
  char  kreptp[MREPTP][9];     /** Report Types */
  char  kbbcl[MCMSG+1];        /** getbb, Variables to report */
  char  knmbbwrite[MCPFN+1];   /** getbb, Name of file to report results to*/
  char  kectp[MECTP][9];       /** Warning Types */
  char  kpause[MCMSG+1];       /** pause, Prompt */
  char  kfeval[MFEVAL][9];     /** eval, Operators*/
  char  kleval[MLEVAL][9];     /** eval, Logicals */
  char  kevaln[MCMSG+1];       /** eval, Where to report results */
  char  knmprb[MCPFN+1];       /** Not Used */
  char  knmbbf[MCPFN+1];       /** Not Used */
  char  kmcdir[MMCDIR][MCPFN+1];  /** macro, Macro directories */
  char  kmcreq[MCPFN+1];       /** macro, Request directories */
  char  kmcnam[MCPFN+1];       /** Not used */
  char  kargs[MCMSG+1];        /** macro, arguments ???*/
  char  knametranscript[MTRANSCRIPTS][MCPFN+1];  /** transcript, names */
  char  ktextwait[9];          /** Waiting text ??? */
} kmexm;


/** 
 * @struct kmexm
 *    Execution Module Lengths
 */
struct t_cmexm {
  int    lprod;     /** Running in Production Mode */
  int    lcomf;     /** Running from a command file */
  int    lcomcr;    /** Command Correction variable */
  int    linsys;    /** Operating system version - ??? */
  int    nfgtp;     /** Funcgen, number of functions available */
  int    ifgtp;     /** Funcgen, current function */
  int    nfgpts;    /** Funcgen, number of point in generated function */
  double  fgdel;     /** Funcgen, sampling interval */
  double  fgbeg;     /** Funcgen, Beginning value */
  double  fgsico[2]; /** Funcgen, Coefficients used to generate sine function
                     *   [0] = f { Frequency in Hertz }
                     *   [1] = a { Phase angle in degrees }
                     *   y(t) = \sin(2 \pi [f t + (a/360.0) ] )
                     */
  double  fglico[2]; /** Funcgen, Coefficients used to generate linear function 
                     *   [0] = a  { Linear or "slope" }
                     *   [1] = b  { Constant or "intercept }
                     *   y(t) = a t + b
                     */
  double  fgquco[3]; /** Funcgen, Coefficients used to generate quadratic function
                     *   [0] = a { Quadratic coeff. term }
                     *   [1] = b { Linear coeff. term }
                     *   [2] = c { Constant term }
                     *   y(t) = a x^2 + b x  + c 
                     */
  double  fgcuco[4]; /** Funcgen, Coefficients used to generate cubic function
                     *   [0] = a { Cubic coeff. term }
                     *   [1] = b { Quadratic coeff. term }
                     *   [2] = c { Linear coeff. term }
                     *   [3] = d { Constant term }
                     *   y(t) = a x^3 + b x^2  + c x + d
                     */
  double  fgraco[2];     /** Funcgen, ??? */
  double  fgistr[1000];  /** Funcgen, Impulse String */
  int    nreptp;        /** report, Number of "Report" Variables */
  int    irep[MREPTP];  /** report, Report Indicies */
  int    nrep;          /** report, Number of current reports */
  int    lbball;        /** getbb, Show all variables */
  int    lnames;        /** getbb, Show name and "=" before value */
  int    lnewline;      /** getbb, Show a newline after each variable */
  FILE  *nunbbwrite;    /** getbb, File unit to report results on */
  int    nectp;         /** read error control, number of error types */
  int    lecho;         /** echo, to echo out commands */
  int    nfeval;        /** eval, Number of evaluation operators */
  int    nleval;        /** eval, Number of comparison operators */
  int    lfeval;        /** eval, If the current operator is evaluation */
  int    neval;         /** eval, Number of evaluations to do */
  int    ifeval[MEVAL]; /** eval, Which evaluation operator */
  double  feval[MEVAL-(0)+1]; /** eval, Evaluation values */
  int    ileval;        /** eval, Which logical comparison operator */
  int    lfloat;        /** eval, If the value is a floating point number */
  int    lprbon;        /** Not used */
  int    lprbfi;        /** Not used */
  FILE  *nunprb;        /** Not used */
  int    lperio;        /** pause, to pause by a time, or forever */
  int    nperio;        /** pause, Period length */
  int    nmcdir;        /** macro, Number of macro directories */
  int    ntranscripts;  /** transcript, Number of transcripts */
  int    itranscript;   /** transcript, Current transcript */
  int    imodetranscript; /** transcript, Type of transcript */
  FILE  *nuntranscript[MTRANSCRIPTS];  /** transcript, File units for transcripts */
  int    lsendtranscript[MTRANSCRIPTS][MTYPES]; /** transcript, Contents of the transcripts */
  int    ntraces;       /** trace, number of traced variables */
  char   ktracename[MTRACES][17]; /** trace, names of traced variables */
  int    lblackboard[MTRACES]; /** trace, If the variable is a blackboard variable, otherwise a header variable */
  char   ktracevalue[MTRACES][MCMSG+1]; /** trace, value of the traced variable */
} cmexm;


#ifdef DOINITS

double *const Fgcuco = &cmexm.fgcuco[0] - 1;
double *const Fglico = &cmexm.fglico[0] - 1;
double *const Fgquco = &cmexm.fgquco[0] - 1;
double *const Fgraco = &cmexm.fgraco[0] - 1;
double *const Fgsico = &cmexm.fgsico[0] - 1;
int  *const Ifeval = &cmexm.ifeval[0] - 1;
int  *const Irep = &cmexm.irep[0] - 1;
int  *const Lblackboard = &cmexm.lblackboard[0] - 1;

#else

extern double *const Fgcuco;
extern double *const Fglico;
extern double *const Fgquco;
extern double *const Fgraco;
extern double *const Fgsico;
extern int  *const Ifeval;
extern int  *const Irep;
extern int  *const Lblackboard;

#endif


void gettextwait ( char *mode, 
                   int mode_s);
void iniexm (void);
void proerr ( int *nerr);
void qam (void);
void qapf (void);
void qcolor (void);
void qcut (void);
void qdevices (void);
void qfid (void);
void qgtext (void);
void qhpf (void);
void qline (void);
void qmtw (void);
void qpicks (void);
void qsymbol (void);
void qtitle (void);
void qwidth (void);
void qxlabl (void);
void qxlim (void);
void qylabl (void);
void qylim (void);
void repav ( char *ktext, 
             int ktext_s, 
             char *av, 
             int av_s);
void reperr ( int nerr);
void repiv ( char *ktext, 
             int ktext_s, 
             int iv);
void repivl ( char *ktext, 
         int ktext_s, 
         int *iv, 
         int nv);
void repkv ( char *ktext, 
             int ktext_s, 
             char *kv, 
             int kv_s);
void replv ( char *ktext, 
             int ktext_s, 
             int lv);
void reprtw ( char *ktext, 
              int ktext_s, 
              int lrtw, 
              char *krtw, 
              int krtw_s, 
              float *ortw);
void reprv ( char *ktext, 
             int ktext_s, 
             double rv);
void setprompt ( char *prompt, 
                 int prompt_s);
void settextwait ( char *mode);
void tracereport ( int *nerr);
void tracevariable ( int activate, 
                     int blackboard, 
                     char *variable, 
                     int *nerr);
int env_bool ( char *env, 
               int def);
int display_copyright ( int getset);
int use_database ( int getset);
void xabout (void);
void xcd ( int *nerr);
void xecho ( int *nerr);
void echo_switch ( int value);
void xeval ( int *nerr);
void xexmc ( int index, 
             int *nerr);
void xfg ( int *nerr);
void xgetbb ( int *nerr);
void xhelp ( int lprint, 
             int *nerr);
void xinstallmacro ( int *nerr);
void xload ( int *nerr);
void xmacro ( int *nerr);
void xmsg ( int *nerr);
void xnews ( int *nerr);
void xpause ( int *nerr);
void xreadbbf ( int *nerr);
void xreport ( int *nerr);
void xsetbb ( int *nerr);
void xsetmacro ( int *nerr);
void xsyntx ( int *nerr);
void xsystemcommand ( int *nerr);
void xtrace ( int *nerr);
void xtranscript ( int *nerr);
void xunsetbb ( int *nerr);
void xwritebbf ( int *nerr);

#endif /* _EXM_H_ */
