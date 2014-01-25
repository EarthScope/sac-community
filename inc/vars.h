/** 
 * @file vars.h
 *
 * @brief Variables
 *
 */

#ifndef _VARS_H_
#define _VARS_H_

#include <stdio.h>
#include <stdarg.h>

#include "token.h"

#define VARS_SEARCH_MODE_MEMORY_THEN_DISK "BOTH"

#define	BADDATABLOCKFLAG       1207
#define	BADINPUT	       1210
#define	BADVALUETYPE	       1204
#define	BIGVALUE	       65536
#define	MAXCENAME	128
#define	MAXCVNAME	128
#define	MAXLEVELS	10
#define	MAXVARS	400
#define	MAXVARSEXCEEDED	1202
#define	MLEVELS	10
#define	NOCURRENTLIST	1209
#define	NOTIMPLEMENTED	1206
#define	NOTVARSFILE	1208
#define	VALUEAPPLICATION       31
#define	VALUEBYTE	8
#define	VALUECOMPLEX	12
#define	VALUEDATA	2
#define	VALUEDBLCOMPLEX	13
#define	VALUEDOUBLE	11
#define	VALUEINTEGER	4
#define	VALUELIST	1
#define	VALUELOGICAL	14
#define	VALUENIL	0
#define	VALUEREAL	10
#define	VALUESHORTINT	6
#define	VALUESTRING	3
#define	VALUESYMBOL	15
#define	VALUEUNSBYTE	9
#define	VALUEUNSINT	5
#define	VALUEUNSSHORTINT	7
#define	VARNODELETE	1205
#define	VARNOTFOUND	1201
#define	VARSINCR	(int)32
#define	VARSLISTEXISTS	1211
#define	VARSLISTNOTFOUND	1203
#define	VARSTERMINATOR	0
#define	VARSVERSION	1
#define NVFILELIST     10
#define NVFILEINC       5



struct t_kmvars {
  char vabsflag;
  char vlistdelim;
  char cfill1;
  char cfill2;
  char varsname[MAXVARS][MAXCVNAME+1];
  char varsidcode[5];
  char valuename[MAXCVNAME+1];
} kmvars;

struct t_cmvars {
  int varslength[MAXVARS];
  int numvars;
  int varsindex[MAXVARS];
  int varsnilindex[MAXVARS];
  int varsmodified[MAXVARS];
  int varsindirect[MAXVARS];
  int lvarsinit;
  int ncvarsname[MAXVARS];
  int currentnode;
  int varsnode;
  int varsnode1; 
  int descindex;
  int desclength;
  int valuelength;
  int valuetype; 
  int namelength;
  int deleteflag;
  int readonlyflag;
  int indirectflag;
  int sharedflag;
  int reservedflag;
  int applflag1; 
  int applflag2;
} cmvars;

struct t_kmgetvlist {
  char sublistnames[MAXLEVELS][MAXCENAME+1];
} kmgetvlist;

struct t_cmgetvlist {
  int indexsave[MAXLEVELS];
  int nlevelsgt;
} cmgetvlist;

struct t_cmcopyvlist {
  int node1savecp[MAXLEVELS];
  int node2savecp[MAXLEVELS];
  int indexsavecp[MAXLEVELS];
  int nlevelscp;
} cmcopyvlist;

struct t_kmprintvlist {
  char varssavepr[MLEVELS][MAXCVNAME+1];
} kmprintvlist;

struct t_cmprintvlist {
  int indexsavepr[MLEVELS], nlevelspr;
} cmprintvlist;

struct varsfile {
  char *varsname;
  char *variable;
  FILE *value;
};

struct t_varsfile {
  int nallocated;
  int nentries;
  struct varsfile *filelist;
} vfilelist;
          
enum {
  VAR_UNKNOWN = 0,
  VAR_VALUE   = VALUEDOUBLE,
  VAR_STRING  = VALUESTRING,
  VAR_INTEGER = VALUEINTEGER,
  VAR_LIST    = VALUELIST,
};

enum {
  DELETE_F = 1,
  READ_ONLY,
  INDIRECT,
  SHARED,
  RESERVED,
  APPL1,
  APPL2,
};

typedef struct var_t var;
struct var_t {
  int    type;
  char  *name;  /* VAR_STRING */
  double value; /* VAR_VALUE */ 
  int    ival;  /* VAR_INTEGER */
  Token *list;  /* VAR_LIST, null terminated */
  char  *str;
  int    flag[7];
};


void createvlist ( char *fullvars, 
                   int fullvars_s, 
                   int length, 
                   int *node, 
                   int *nerr);
void deletev ( char *vars, 
               int vars_s, 
               char *name, 
               int name_s, 
               int *nerr);
void deletevlist ( char *vars, 
                   int vars_s, 
                   char *mode, 
                   int *nerr);
void getvFILEptr ( char *vars, 
                   int vars_s, 
                   char *name, 
                   int name_s, 
                   FILE **value, 
                   int *nerr);
void getvvstring ( char *vars, 
                   int vars_s, 
                   char *name, 
                   int name_s, 
                   int *numchars, 
                   char *value, 
                   int value_s, 
                   int *nerr);
void initializevars (void);
void inivars (void);
void putvFILEptr ( char *vars, 
                   int vars_s, 
                   char *name, 
                   int name_s, 
                   FILE *value, 
                   int *nerr);
void putvvstring ( char *vars, 
                   int vars_s, 
                   char *name, 
                   int name_s, 
                   int numchars, 
                   char *value, 
                   int value_s, 
                   int *nerr);
void readvfile ( char *fullvars, 
                 int fullvars_s, 
                 int *node, 
                 int *nerr);
void writevfile ( char *vars, 
                  int vars_s, 
                  char *file, 
                  int *nerr);

char * getvvstringZ(char *vars,
                    int   vars_s,
                    char *name,
                    int   name_s,
                    int  *numchars,
                    int  *nerr);


void    sac_vars_init       ();
int     sac_vars_get_value  (char *group, char *name, double *val);
int     sac_vars_get_string (char *group, char *name, char **val);
int     sac_vars_put_var    (char *group, char *name, int type, ...);
int     sac_vars_delete_var (char *group, char *name);
var *   sac_vars_get_var    (char *group, char *name);
char ** sac_vars_keys       (char *group);
int     sac_vars_create     (char *group);
int     sac_vars_delete     (char *group);
int     sac_vars_write      (char *group, char *file);
int     sac_vars_read       (char *group);
int     sac_vars_exists     (char *group);

int     token_to_var (Token *tok, char *group, char *name);
int     setvar       (char *group, char *name, int type, ...);
int     setvar_ap    (char *group, char *name, int type, va_list ap);

#ifdef DOINITS
   int *const Indexsave = &cmgetvlist.indexsave[0] - 1;
   int *const Indexsavecp = &cmcopyvlist.indexsavecp[0] - 1;
   int *const Indexsavepr = &cmprintvlist.indexsavepr[0] - 1;
   int *const Ncvarsname = &cmvars.ncvarsname[0] - 1;
   int *const Node1savecp = &cmcopyvlist.node1savecp[0] - 1;
   int *const Node2savecp = &cmcopyvlist.node2savecp[0] - 1;
   int *const Varsindex = &cmvars.varsindex[0] - 1;
   int *const Varsindirect = &cmvars.varsindirect[0] - 1;
   int *const Varslength = &cmvars.varslength[0] - 1;
   int *const Varsmodified = &cmvars.varsmodified[0] - 1;
   int *const Varsnilindex = &cmvars.varsnilindex[0] - 1;
#else
   extern int *const Indexsave;
   extern int *const Indexsavecp;
   extern int *const Indexsavepr;
   extern int *const Ncvarsname;
   extern int *const Node1savecp;
   extern int *const Node2savecp;
   extern int *const Varsindex;
   extern int *const Varsindirect;
   extern int *const Varslength;
   extern int *const Varsmodified;
   extern int *const Varsnilindex;
#endif

#endif /* _VARS_H_ */
