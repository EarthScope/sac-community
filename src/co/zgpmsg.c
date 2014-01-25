/** 
 * @file   zgpmsg.c
 * 
 * @brief  Print a prompt and get a reply
 * 
 */

#include <stdio.h>
#include <string.h>

#include "mach.h"
#include "co.h"

#include "config.h"

#include "select.h"
#include "debug.h"

#if !defined(READLINE) && !defined(USE_X11_DOUBLE_BUFFER)
void
zgpmsg(prmt,prmtlen,msg,msglen)
char *prmt;		/* pointer to prompt message */
int prmtlen;		/* length of prmt array */
char *msg;		/* pointer to character array to receive input */
int msglen;		/* length of msg array */
 
{
	int i;			/* index for prefilling string w/ NULLs */
	char *psave;		/* save msg */
 
	psave = msg;
	for (i=0;i<(int)msglen;++i)	/* prefill with NULLs */
		*(psave++) = '\0';
 
	while (*prmt != '$')
		putchar (*(prmt++));	/* print prompt */


        fflush(stdout);

/* A control-d sets the message response to quit */
	if ( getfline (stdin,msg,(short)msglen) == -1)
	   if ( msglen >= 5 ) strncpy (msg, "quit", 4);
	
}

#else 

/** 
 * Process a command line
 * 
 * @param p 
 *   String (command) to process
 *
 */
static void
process_line(char *p) {
  select_loop_continue(SELECT_OFF); /* Turn off select loop */
  select_loop_message(p, SELECT_MSG_SET); /* Set the outgoing message */
  FREE(p);
  rl_callback_handler_remove();
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
 */
void
zgpmsg(char *prmt,
       int   prmtlen,
       char *msg,
       int   msglen) {
  select_loop(prmt, prmtlen, msg, msglen, NULL, process_line);
}

#endif /* !READLINE */
