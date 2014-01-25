/** 
 * @file   zgtmsg.c
 * 
 * @brief  Print a prompt and get a reply
 * 
 */
#include <stdio.h>

#include "co.h"

#include "config.h"
 

#ifndef READLINE
void
zgtmsg(prmt,prmtlen,msg,msglen)
char *prmt;		/* pointer to prompt message */
int prmtlen;		/* length of prmt array */
char *msg;		/* pointer to character array to receive input */
int msglen;		/* length of msg array */
 
{
 
	while (*prmt != '$')
		putchar (*(prmt++));	/* print prompt */
	putchar('\n');
 
	getline_sac (stdin,msg,(short) msglen);
	return;
}
 
#else 

#include "select.h"

/** 
 * Process a command line 
 * 
 * @param p 
 *   Command line to process
 *
 */
static 
void process_line(char *p) { 
  select_loop_continue(SELECT_OFF);
  select_loop_message(p, SELECT_MSG_SET);
  return; 
}

/** 
 * Print a prompt and receive a reply
 * 
 * @param prmt 
 *    Prompt
 * @param prmtlen 
 *    Length of \p prmt
 * @param msg 
 *    Reply
 * @param msglen 
 *    Length of \p msg
 *
 * @note Calls co/select_loop()
 * @bug This looks similar to zgpmsg, except the command is not added
 *      to the history
 */
void
zgtmsg(char *prmt, 
       int   prmtlen,
       char *msg, 
       int   msglen) {
  select_loop(prmt, prmtlen, msg, msglen, NULL, process_line);
}


#endif /* ! READLINE */
