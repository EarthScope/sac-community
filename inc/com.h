/** 
 * @file   com.h
 * 
 * @brief  Command Parsing Functions
 * 
 */

#ifndef _COM_H_
#define _COM_H_

/** 
 * @param MCOM 
 *    Maximum Number of commands on the command stack
 */
#define	MCOM	200
/** 
 * @param MKARGS 
 *    Maximum Length of arguments to commands
 */
#define MKARGS  100

struct t_kmcom {
  //char   kcom[MCOM][9];    /* command stack, 8 chars long plus '\0' */
  char **kargs;		   /* arguments to accompany commands in kcom. */
  int    nkargs;           /* number of arguments. */
  int    nkargs_allocated; /* number of allocated spaces for arguments. */
} kmcom;

struct t_cmcom {
  //int   jcom;   /* indexes a command from kcom and argument from kargs */
  //int   ncom;	/* number of commands in stack. */
  //int   itypcm[MCOM]; /* Type of command - inumbr or ialpha */
  //float	flnum[MCOM]; /* Float (real) in command  */
  //int   inumbr; /* is a argument a number ? -- Never changes */
  //int   ialpha; /* is a argument a string ? -- Never changes */
  //int   icont;  /* Never used */
  int   ncerr;  /* Command Error number */
} cmcom;


#ifdef DOINITS

//   float *const Flnum = &cmcom.flnum[0] - 1;
//   int *const Itypcm = &cmcom.itypcm[0] - 1;

#else

//   extern float *const Flnum;
//   extern int *const Itypcm;

#endif

#endif /* _COM_H_ */
