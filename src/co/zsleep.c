/** 
 * @file   zsleep.c
 * 
 * @brief  Sleep a number of milliseconds
 * 
 */

#include "config.h"

#include "co.h"
 
#ifndef READLINE

/** 
 *
 */ 
void
zsleep(int timeout) {
        int i, j;
 
	for (i = 0; i < timeout; ++i)
		for (j=0; j<364; ++j);		/* 1 millisecond loop */
	return;
}
 
#else 

#include <sys/time.h>
#include <stdio.h>
#include "select.h"

#define TIMER_SET          0
#define TIMER_GET          1
#define TIMER_SEC_TO_USEC  1000000
#define TIMER_MSEC_TO_USEC 1000
#define ZSLEEP_PRMTLEN     2
#define ZSLEEP_MSGLEN      9

/** 
 * Internal command line handling 
 * 
 * @param p 
 *    Command 
 *
 */
static 
void process_line(char *p) { 
  select_loop_continue(SELECT_OFF);
  select_loop_message(p, SELECT_MSG_SET);
  return; 
}

/** 
 * Sleep a number of milliseconds
 * 
 * @param timeout 
 *    Milliseconds to sleep
 * 
 *    
 */
void
zsleep(int timeout) {

  int left;
  struct timeval time;
  char prmt[ZSLEEP_PRMTLEN];
  char msg[ZSLEEP_MSGLEN];

  prmt[0] = '$';
  prmt[1] = '\0';

  /* Convert milliseconds to microseconds */
  timeout = timeout * TIMER_MSEC_TO_USEC; 
  if((left = timer(TIMER_SET, timeout)) == 0) {
    return;
  }
  time.tv_sec = 0;
  time.tv_usec = left;
  
  while((left = timer(TIMER_GET, timeout)) > 0) {
    time.tv_sec = 0;
    time.tv_usec = left;
    select_loop(prmt, ZSLEEP_PRMTLEN, 
		msg, ZSLEEP_MSGLEN, 
		&time, process_line);
  }
  
  fprintf(stdout,"\n");
  rl_callback_handler_remove(); /* Returns Prompt */
  
}


/** 
 * Internal Timer
 * 
 * @param set_get 
 *    - TIMER_SET to get the timer to start (e.g. TIMER_START)
 *    - TIMER_GET to get the amount of time left in micro-seconds
 * @param usec 
 *    Length of the timer in microseconds
 * 
 * @return 
 *    Time left on timer in micro-seconds
 */
int 
timer(int set_get, int usec) {
  static struct timeval start;
  struct timeval now;
  int delta;

  if(set_get == TIMER_SET) {
    if(gettimeofday(&start, NULL) != 0) {
      perror("gettimeofday");
    } 
  }

  if(gettimeofday(&now, NULL) != 0) {
    perror("gettimeofday");
  } 
  delta = (now.tv_sec - start.tv_sec) * TIMER_SEC_TO_USEC + 
    (now.tv_usec - start.tv_usec);

  return(usec - delta);
}

#endif /* READLINE */
