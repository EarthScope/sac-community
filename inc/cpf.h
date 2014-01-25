/** 
 * @file   cpf.h
 * 
 * @brief  Command Parsing Functions (CPF) Variables
 * 
 */

#ifndef _CPF_H_
#define _CPF_H_

#include "mach.h"
#include "clf.h"

#define	MNUMERICABBREVS	5
#define	MNUMERICFUNCS	22
#define	MSTRINGFUNCS	10


/* These vars are initialized in ncfp/inicpf.c */
struct t_kmcpf {
  /* designates that a particular macro keyword has no current value */
  char	knoval[9];


  char	kbb;		/* blackboard variable key */
  char  khdr;		/* header variable key */
  char  karg;		/* argument key */
  char  kescape;	/* escape character */
  char  kfunctionbegin;	/* beginning of inline function */
  char  kfunctionend;	/* end of inline function */

  char	kstringfuncs[MSTRINGFUNCS][9];
  char  knumericfuncs[MNUMERICFUNCS][9];
  char  knumericabbrevs[MNUMERICABBREVS][9];
  char  kvarsname[9];

  char	kinputline[MCMSG+1];	/* processed input line */
} kmcpf;

struct t_cmcpf {
  int nstringfuncs;
  int nstringargs[MSTRINGFUNCS];
  int nnumericfuncs;
  int nnumericargs[MNUMERICFUNCS];
  int nnumericabbrevs;
  int inumericabbrevs[MNUMERICABBREVS];
  int nmacrolevel;
  int lmacrostatus;
} cmcpf;

void      cerr         (int     nerr);
void      cfmt         (char   *kmsg2, 
			int     kmsg2_s);
void      cresp        ( );
void      csinit       (char   *kmsg, 
			int     ncmsg, 
			int    *nerr);
void      cspop        (char   *kcom, 
			int     kcom_s, 
			int     mcom, 
			int    *ninvok, 
			int    *ncom, 
			int    *nerr);
void      cspush       (int     ninvok, 
			int    *nerr);
void      cszero       ( );
void      ctype        (int    *jdx);
void      gc           (int    *lexist, 
			int    *nerr);
char *    getarg       (int     argno,
			int    *nchar);
void      ictok        (int     inc);
void      inicom       ( );
void      inicsf       ( ) ;
void      initcomlists (int    *nerr);
void      initpf       ( );

int       lcchar       (int     mchar, 
			char   *kchar, 
			int     kchar_s, 
			int    *nchar);
int       lcchar_base  (int     mchar, 
                        char   *kchar, 
                        int     kchar_s, 
                        int    *nchar);
int       lccl         (char   *kcl, 
			int     kcl_s, 
			int    *ncl);
string_list *  lcdfl ();
string_list *  lcdfl_wild ();

int       lcia         (int     mnint, 
			int     mxint, 
			int    *ia, 
			int    *nia);
int       lcidi        (int    *int1, 
			int    *int2);
int       lcint        (int    *intv);
int       lcirc        (int     intmn, 
			int     intmx, 
			int    *intv);
int       lckey        (char   *kkey, 
			int     kkey_s);
int       lcircp       (int     intmn, 
			int     intmx, 
			int    *intv1, 
			int    *intv2);
int       lckeyExact   (char   *kkey, 
			int     kkey_s);
int       lclist       (char   *klist, 
			int     klist_s, 
			int     nlist, 
			int    *index);
int       lclog        (int    *logv);
int       lclog2       (char   *ktrue, 
			int     ktrue_s, 
			char   *kfalse, 
			int     kfalse_s, 
			int    *logv);
int       lcmore       (int    *nerr);
int       lcquot       (int     mquot, 
			char   *kquot, 
			int     kquot_s, 
			int    *nquot);
int       lcra         (int      nramn, 
                        int      nramx, 
                        double  *ra, 
                        int     *nra);
int       lcreal       (double  *realv);
int       lcrest       (int     mchar, 
			char   *kchar, 
			int     kchar_s, 
			int    *nchar);
int       lcrrcp       (double  realmn, 
			double  realmx, 
			double  *realv1, 
			double  *realv2);
int       lcrtw        (int    *lrtw, 
			char   *krtw, 
			int     krtw_s, 
			double  *ortw);
int       lctok        (char   *ktok, 
			int     ktok_s, 
			int    *lnumbr, 
			double  *rnumbr);
int       lkchar       (char   *kkey, 
			int     kkey_s, 
			int     mchar, 
			char   *kchar, 
			int     kchar_s, 
			int    *nchar);
int       lkcharExact  (char   *kkey, 
			int     kkey_s, 
			int     mchar, 
			char   *kchar, 
			int     kchar_s, 
			int    *nchar);
int       lkentries    (char   *kkey, 
			int     kkey_s, 
			char   *klist, 
			int     klist_s, 
			int     nlist, 
			int    *llist);
int       lkia         (char   *kkey, 
			int     kkey_s, 
			int     mnint, 
			int     mxint, 
			int    *ia, 
			int    *nia);
int       lkint        (char   *kkey, 
			int     kkey_s, 
			int    *intv);
int       lkirc        (char   *kkey, 
			int     kkey_s, 
			int     intmn, 
			int     intmx, 
			int    *intv);
int       lklog        (char   *kkey, 
			int     kkey_s, 
			int    *logv);
int       lklog2       (char   *kkey, 
			int     kkey_s, 
			char   *ktrue, 
			int     ktrue_s, 
			char   *kfalse, 
			int     kfalse_s, 
			int    *logv);
int       lklogc       (char   *kkey, 
			int     kkey_s, 
			int    *logv, 
			char   *kchar, 
			int     kchar_s);
int       lklogi       (char   *kkey, 
			int     kkey_s, 
			int    *logv, 
			int    *intv);
int       lklist       (char   *kkey, 
			int     kkey_s, 
			char   *klist, 
			int     klist_s, 
			int     nlist, 
			int    *index);
int       lklogr       (char   *kkey, 
			int     kkey_s, 
			int    *logv, 
			double  *realv);
int       lklogra      (char   *kkey, 
                        int     kkey_s, 
                        int    *offOnFlt, 
                        int     nramn, 
                        int     nramx, 
                        double *ra, 
                        int    *nra, 
                        int    *nerr);
int        lkquot      (char   *kkey, 
			int     kkey_s, 
			int     mquot, 
			char   *kquot, 
			int     kquot_s, 
			int    *nquot);
int        lkra        (char   *kkey, 
                        int     kkey_s, 
                        int     nramn, 
                        int     nramx, 
                        double *ra, 
                        int    *nra);
int        lkreal      (char   *kkey, 
			int     kkey_s, 
			double  *realv);
int        lkrest      (char   *kkey, 
			int     kkey_s, 
			int     mchar, 
			char   *kchar, 
			int     kchar_s, 
			int    *nchar);
int        lkrrc       (char   *kkey, 
			int     kkey_s, 
			double  realmn, 
			double  realmx, 
			double  *realv);
int        lkrrcp      (char   *kkey, 
			int     kkey_s, 
			double  realmn, 
			double  realmx, 
			double  *realv1, 
			double  *realv2);
int        lkrtw       (char   *kkey, 
			int     kkey_s, 
			int    *lrtw, 
			char   *krtw, 
			int     krtw_s, 
			double  *ortw);
void       lkt         (int    *nerr);
int        non_num_com (char   *command, 
			int     command_s);
void       pcmsg       (char   *kmsg, 
			int     kmsg_s, 
			int    *nerr);
void       savearg     (int     argno,
			char   *arg,
			int     nchar);
void       sctok       (int     num);
void       setcomlist  (int     number);
void       tokens      (char   *kstrg, 
			int     ncstrg, 
			int     mtok, 
			char   *ktok, 
			int     ktok_s, 
			int    *jtok, 
			int    *jcstrg);
void       xclog       (int    *log, 
			int    *nerr);
void       xclogr      (int    *log, 
			double  *real, 
			int    *nerr);
void       xcrrcp      (double  realmn, 
			double  realmx, 
			double  *real1, 
			double  *real2, 
			int    *nerr);
void       xcrtw       (int    *lrtw, 
			char   *krtw, 
			int     krtw_s, 
			double  *ortw, 
			int    *nerr);
int lcequals();
int lccomma();


#ifdef DOINITS

   int *const Inumericabbrevs = &cmcpf.inumericabbrevs[0] - 1;
   int *const Nnumericargs = &cmcpf.nnumericargs[0] - 1;
   int *const Nstringargs = &cmcpf.nstringargs[0] - 1;

#else

   extern int *const Inumericabbrevs;
   extern int *const Nnumericargs;
   extern int *const Nstringargs;

#endif

#include "token.h"

Token * arg();
void    arg_next();
void    arg_prev();
void    arg_end();
void    arg_reset();
Token * arg_begin();
void    arg_append(char *str);
void    arg_truncate();
void    arg_delete();
void    arg_insert(char *str);
void    arg_change(char *str);
void    arg_msg(char *msg);

#endif /* _CPF_H_ */
