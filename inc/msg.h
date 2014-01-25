/** 
 * @file   msg.h
 * 
 * @brief  Messaging subsystem
 * 
 */

#ifndef _MSG_H_
#define _MSG_H_

#include <stdio.h>

#include "mach.h"

#define	MCOMMANDS	4
#define	MERRORS  	1
#define	MFMSG	        500
#define	MLIMSG	        5
#define	MMACROS	        5
#define	MOUTPUT	        3
#define	MPROCESSED	6
#define	MTPMSG	        6
#define	MUNITS	        5
#define	MWARNINGS	2


/** 
 * @struct kmmsg
 *    Message Characters
 */
struct t_kmmsg {
  char	ktpmsg[MTPMSG][9]; 
  char  klimsg[MLIMSG][MCMSG+1]; /* list of messages */
  char  kfmsg[MFMSG][MCMSG+1];
} kmmsg;


/** 
 * @struct kmmsg
 *    Message Lengths
 */
struct t_cmmsg {
  int   nummsg;
  int   itpmsg;
  int   nlimsg;
  int   nchmsg;
  int   autoout;
  int   nunits;
  FILE *iunits[MUNITS];
  int   lsend[MUNITS][MTPMSG];
  int   nfmsg;
  int   ifmsg[MFMSG];
} cmmsg;


#ifdef DOINITS

   int *const Ifmsg = &cmmsg.ifmsg[0] - 1;

#else

   extern int *const Ifmsg;
   extern int *const Iunits;

#endif

void apcmsg ( char *kalpha, 
	      int kalpha_s);
void apcmsg2 ( char *kalpha, 
	       int kalpha_s);
void apcmsgnum ( int number);
void apfmsg ( double float_);
void apimsg ( int integr);
void aplmsg ( char *kalpha, 
	      int kalpha_s);
void autooutmsg ( int lmode);
void clrmsg (void);
void getsmsg ( int number, 
	       char *kmsg, 
	       int kmsg_s);
void inimsg (void);

void inquiremsg ( FILE *unitnumber, 
		  int *activate, 
		  int send_[]);
void outmsg (void);
void pltmsg ( float *xloc, 
	      float *yloc);
void sacmsg ( int *nerr);
void sendmesg ( FILE *unitnumber, 
		int activate, 
		int send_[]);
void setmsg ( char *ktype, 
	      int number);
void typmsg ( char *ktype);
void wrtmsg ( FILE *nunit);

void error     (int error, char *message, ...);
void warning   (int error, char *message, ...);
void processed (int val, char *message, ...);
void out       (char *fmt, ...);
void message   (int type, int num, char *message, ...);


void sac_warning_stdout();
void sac_warning_stderr();
void sac_warning_off();

void sac_error_stdout();
void sac_error_stderr();
void sac_error_off();

void sac_output_stdout();
void sac_output_stderr();
void sac_output_off();

void bell();
void bell_off();
void bell_on();

#endif /* _MSG_H_ */
