/** 
 * @file   bbs.h
 * 
 * @brief  Blackboard Storage
 * 
 */

#ifndef _BBS_H_
#define _BBS_H_

#include "mach.h"
#include "token.h"
#include "vars.h"
/** 
 * @struct kmbbs
 *    Blackboard Charaters
 */
struct t_kmbbs {
  char knmbbs[MCPFN+1];
  char kbbsinit[9];
} kmbbs;

/** 
 * @struct cmbbs
 *    Blackboard Lengths
 */
struct t_cmbbs {
  int nlnbbs;
} cmbbs;

void createbbs ( int *nerr );
void deletebbs ( int *nerr );
void getbbv    ( char *kname, 
                 char *kvalue, 
                 int *nerr, 
                 int kname_s, 
                 int kvalue_s);
void inibbs    ( void);
void readbbf   ( char *kname, 
                 int *nerr, 
                 int kname_s);
void setbbv    ( char *kname, 
                 char *kvalue, 
                 int  *nerr, 
                 int   kname_s, 
                 int   kvalue_s);
void unsetbbv  ( char *kname, 
                 int *nerr, 
                 int kname_s);

int setbb(char *name, int type, ...);
var *getbb(char *name);
int token_to_bb(Token *tok, char *name);

#endif /* _BBS_H_ */
