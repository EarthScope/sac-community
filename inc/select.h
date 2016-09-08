
#include "config.h"

#ifndef _SELECT_H_
#define _SELECT_H_

#ifdef SOLARIS
#ifndef __sun__
#define __sun__
#endif /* __sun__ */
#endif /* SOLARIS */

#include "config.h"

#include <stdio.h>
#ifdef WIN32
#include <Winsock2.h>
#else
#include <sys/time.h>
#endif

#ifdef READLINE
  #ifdef READLINE_READLINE
    //#error "readline and no editline"
    #include <readline/readline.h>
    #include <readline/history.h>
  #endif
  #ifdef READLINE_EDITLINE
    //#error "readline and editline"
    #include <editline/readline.h>
  #endif 
  typedef void readline_callback(char *);
#endif

#define SAC_HISTORY_FILE ".sac_history"

#define SELECT_ON      1
#define SELECT_OFF     0
#define SELECT_QUERY - 1

#define SELECT_MSG_SET 0

#define OPTION_ON      1
#define OPTION_OFF     0
#define OPTION_GET    -1

/*
  Function: 
  select_loop_continue - Tell whether to continue in the select loop
  w - SELECT_QUERY => return value stating whether to stay in select
      SELECT_ON    => start select
      SELECT_OFF   => stop select
  OUTPUT -
      SELECT_ON    => stay within select
      SELECT_OFF   => do not stay within select
*/
int select_loop_continue(int w);

/* 
   Function:
   select_loop_message - Set or Get the message obtained by the select_loop
   p     - Character String (INPUT of OUTPUT)
   len   - Set <= 0
           Get - Maximum Length of character string including trailing '\0'
    
   Note: The last two characters of the string will be a space followed by
         a string terminator '\0'
	 
*/
int select_loop_message(char *p, int len);
/* 
   Function:
   select_loop        Loop on STDIN and X11 for input 
   prmt              - Prompt to print                  INPUT
   prmtlen           - length of prmt                   INPUT
   msg               - Output message                   OUTPUT
   msglen            - maximum length of output message INPUT
                       message will be truncated 
                       to this length 
   timeout           - structure holding the time in    INPUT
                       seconds and microseconds 
                       telling select how long 
                       to wait for a signal
   void func(char *) - Function to be called when a     INPUT
                       newline is obtained
		       Assumes readline/editline/libedit is being used
   OUTPUT
    Should return 0 

*/
#ifdef READLINE
int select_loop(char *prmt, int prmtlen, char *msg, int msglen,
                struct timeval *timeout, readline_callback * func, int stdin_on,
                int gui_on);
#endif

int show_prompt_without_tty(int getset);

#endif
