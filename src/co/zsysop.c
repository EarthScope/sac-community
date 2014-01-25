/** 
 * @file   zsysop.c
 * 
 * @brief  Execute a system command
 * 
 */
#include <stdlib.h>

#include "co.h"
#include "debug.h"
#include "errors.h"

/*#define USE_LOCAL_SYSTEM_COMMAND*/
#ifdef USE_LOCAL_SYSTEM_COMMAND
#include <unistd.h>
#include <signal.h>
#include <errno.h>
int system_local(const char *command);
#endif

/** 
 * Execute a system command
 * 
 * @param comstr 
 *    String to execute through system()
 * @param dummylen 
 *    Length of something, not comstr
 * @param pnumc 
 *    Length of \p comstr
 * @param perr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_EXECUTING_SYSTEM_COMMAND
 *
 * @bug The return value of system() is not checked properly.
 * @bug The length of comstr is passed in twice.
 *
 * @date 07/25/84    Under development--D. Trimmer	
 * @date 07/25/84    Tested--D. Trimmer
 * @date 10/01/84    Convert command string to lower case--D. Trimmer
 * @date 08/01/89    Deleted forced conversion to lower case--J. Tull
 * 
 */ 
void
zsysop(char *comstr,
       int   dummylen,
       int  *pnumc,
       int  *perr) {

	*perr = 0;
  UNUSED(dummylen);
	comstr[(int) *pnumc] = '\0';

#ifdef USE_LOCAL_SYSTEM_COMMAND
	if (system_local(comstr) != -1)
	  return;
#else
	if (system(comstr) != -1)
	  return;
#endif

	*perr = ERROR_EXECUTING_SYSTEM_COMMAND;
	return;
}


#ifdef USE_LOCAL_SYSTEM_COMMAND

int
system_local(const char *command)
{
	pid_t pid, savedpid;
	int pstat;
	struct sigaction ign, intact, quitact;
	sigset_t newsigblock, oldsigblock;

	if (!command)		/* just checking... */
		return(1);

	/*
	 * Ignore SIGINT and SIGQUIT, block SIGCHLD. Remember to save
	 * existing signal dispositions.
	 */
	ign.sa_handler = SIG_IGN;
	(void)sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	(void)sigaction(SIGINT, &ign, &intact);
	(void)sigaction(SIGQUIT, &ign, &quitact);
	(void)sigemptyset(&newsigblock);
	(void)sigaddset(&newsigblock, SIGCHLD);
	(void)sigprocmask(SIG_BLOCK, &newsigblock, &oldsigblock);
	switch(pid = fork()) {
	case -1:			/* error */
		break;
	case 0:				/* child */
		/*
		 * Restore original signal dispositions and exec the command.
		 */
		(void)sigaction(SIGINT, &intact, NULL);
		(void)sigaction(SIGQUIT,  &quitact, NULL);
		(void)sigprocmask(SIG_SETMASK, &oldsigblock, NULL);
		execl("/bin/sh", "sh", "-c", command, (char *)NULL);
		exit(127);
	default:			/* parent */
		savedpid = pid;
		do {
			pid = wait4(savedpid, &pstat, 0, (struct rusage *)0);
		} while (pid == -1 && errno == EINTR);
		break;
	}
	(void)sigaction(SIGINT, &intact, NULL);
	(void)sigaction(SIGQUIT,  &quitact, NULL);
	(void)sigprocmask(SIG_SETMASK, &oldsigblock, NULL);
	return(pid == -1 ? -1 : pstat);
}

#endif
