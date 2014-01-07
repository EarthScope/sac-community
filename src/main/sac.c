/** 
 * @file   sac.c
 * 
 * @brief  Main execution loop, start here
 * 
 */

#include <stdio.h>
#include <termcap.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <execinfo.h>
#include <signal.h>

#include "mach.h"
#include "exm.h"
#include "bool.h"
#include "select.h"

#include "config.h"

#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "top.h"
#include "bbs.h"
#include "co.h"
#include "ncpf.h"
#include "ssi.h"

#include "debug.h"

void sac_command_line_options(int argc, char **argv);
void sac_command_line_copyright(int argc, char **argv);
void sac_initialize(int argc, char **argv);
void main_command(char *kmsg, int n);
void execute_command_line(char *kmsg, int len);

#ifdef X11_APPLICATION
void set_constrain_plot_ratio_x11( int set );

void set_constrain_plot_ratio_x11( int set );

void
segfault_backtrace(int sig) {
 void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stdout, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDOUT_FILENO);
  exit(-11);
}

/** 
 * Main command execution loop for SAC, called by the system on startup
 * 
 * @param argc 
 *    Number of command line arguments
 * @param argv 
 *    Command line arguments
 * 
 * @return 
 *    - 0 on Success
 *    - Non-Zero on Failure
 *
 * @note Variables
 *   - kmsg:    Message received from terminal. [c]
 *   - ncmsg:   Length of KMSG without trailing blanks. [i]
 *   - kline:   Message after being processed to substitute blackboard
 *              and header variables. [c]
 *   - ncline:  Length of KLINE without trailing blanks. [i]
 *   - nerr:    Error return flag. [i]
 *             =0 if ok.
 *             >0 if message was invalid or command encountered errors.
 *
 * @date   900804:  Minor change in subroutine names.
 * @date   891005:  General cleanup: included call to zstart for system
 *                  dependent startup code; deleted some obsolete code;
 *                  changed calling logic to saccommands.
 * @date   870421:  Added call to processline.
 * @date   870109:  Changed to zgpmsg to handle "graphics events."
 * @date   841219:  Retyped on a ROS3.2/RIDGE 32, removed interrupt control.
 * @date   820923:  Added interrupt control and documented subroutine.
 * @date   820430:  Mod due to change in ZGPMSG.
 * @date   810000:  Original version.
 *
 */

int
main(int    argc, 
     char **argv ) {

	char kmsg[MCMSG+1];
	int ic, ic1, ic2, itype, nc, ncline, ncmsg, nerr, i;
    char *s1, *s2;
    int dumb ;
    
    float *a;
    
    memset(&(kmsg[0]), ' ', MCMSG);
	kmsg[0] = '\0' ;
	kmsg[MCMSG] = '\0' ;

	/* - Initialize common. */
    sac_initialize(argc, argv);

	/* - Get the input line message, if any.
	 *   This should be the name of the a default SAC macro to execute. */

	zgimsg(argc,argv,kmsg,MCMSG+1 );
  execute_command_line( kmsg, MCMSG+1 );

	/* - THIS IS THE MAIN LOOP OF THE PROGRAM.
	 *   (1) "zgpmsg" sends a prompt to the user and gets a message back.
	 *   (2) The message is first passed through "processline" which evaluates any
	 *       blackboard or header variables and substitutes them into the message.
	 *   (3) The message is then passed to "saccommands" which parses and executes
	 *       the command(s) that are contained within the message.
	 *   (4) When all the commands have been executed or an error has
	 *       occured, "saccommands" returns and the process is repeated.
	 *   (5) Program termination is handled by the QUIT command found
	 *       in the Executive Module (subroutine "xexmc"). */

	/* the main loop of the program */
	while( TRUE ){
	    zgpmsg( kmexm.kprmt,13, kmsg,MCMSG+1 );
        main_command(kmsg, MCMSG);
	} 
}

#endif /* X11_APPLICATION */

void
execute_command_line(char *kmsg, int len) {
  int nc, ic, ic1, ic2, itype, nerr;
  char *s1, *s2;
	nc = indexb( kmsg, len );
	if( nc > 0 ){
	    ic = 0;
	    poptok( kmsg, nc, &ic, &ic1, &ic2, &itype );
	    setmsg( "COMMAND", 99 );
	    apcmsg( "(INPUT LINE) MACRO",19 );
	    apcmsg( kmsg,MCMSG+1 );
	    outmsg();
	    clrmsg();

      strncpy((s1=malloc(ic2-ic1+2)),kmsg+ic1 - 1,ic2-ic1+1);
      s1[ic2-ic1+1] = '\0';
      strncpy((s2=malloc(nc-ic+2)),kmsg+ic - 1,nc-ic+1);
      s2[nc-ic+1] = '\0';

	    executemacro( s1, ic2-ic1+2, s2, nc-ic+2, &nerr );

	    free(s1);
	    free(s2);
	}
}

void
main_command(char *kmsg, int n) {
    char kline[MCMSG];
    int ncline, nerr, dumb, ncmsg;
    char *msgout;
    
    msgout = AddToHistory(kmsg);
    if(!msgout) {
        return;
    }

    n = strlen(msgout);

    ncmsg = indexb(msgout,n+1);
    if( ncmsg > MCMSG) {
        error(99, "Cmd line exceeds buffer limited to num of chars: %d", MCMSG);
        outmsg();
        clrmsg();
        return;
    }
    setmsg("COMMAND", 99);
    apcmsg( msgout, n+1);
    outmsg();
    clrmsg();
    
    processline( "terminal", 9, msgout, n+1, ncmsg, kline, MCMSG+1, &ncline, &nerr );
    
    if( nerr != 0 ){
        reperr( nerr );
        if( nerr != 0 ) {
            proerr( &nerr );
        }
        return;
    }
    dumb = ( strlen( kline ) < strlen( msgout ) ?
             strlen( kline ) : strlen( msgout ) ) ;
    
    /* if processline() had an effect */
    if( !dumb || memcmp(kline,msgout,dumb) != 0 ){
        setmsg( "PROCESSED", 99 );	     /* let it be noted. */
        apcmsg( kline,MCMSG+1 );
        outmsg();
        clrmsg();
    }
    if( ncline > 0 ) {
        /* saccommands:  executes the commands in kline. */
        saccommands( kline,MCMSG+1, &nerr );
    }
    if(msgout) {
      free(msgout);
      msgout = NULL;
    }
}

void
sac_initialize(int argc, char **argv) {
    sac_command_line_copyright(argc, argv);
    initsac();
    sac_command_line_options(argc, argv);
}

void
sac_command_line_copyright(int argc, char **argv) {
    int i;
    for( i=1; i<argc; i++ ){
        if(strcmp(argv[i], "--copyright-off") == 0) {
            display_copyright( OPTION_OFF );
        }
        if(strcmp(argv[i], "--copyright-on") == 0) {
            display_copyright( OPTION_ON );
        }
    }
}

void
sac_command_line_options(int argc, char **argv) {
  int i;
  int macro_start = 0;
    /* check for "gui" execute line arg. */
    for( i=1; i<argc; i++ ){

#ifdef X11_APPLICATION
      if(strcmp(argv[i], "--letter") == 0) {
      if(strcmp(argv[i], "--bell-off") == 0) {
        bell_off();
        macro_start ++;
      }
      else if(strcmp(argv[i], "--bell-on") == 0) {
        bell_on();
        macro_start ++;
      }
      else if(strcmp(argv[i], "--letter") == 0) {
        set_constrain_plot_ratio_x11( TRUE );
        macro_start ++;
      }
#endif /* X11_APPLICATION */
      if(strcmp(argv[i], "--no-tty") == 0) {
        tty_force(OPTION_OFF);
        macro_start ++;
      }
      if(strcmp(argv[i], "--stdout") == 0) {
        sac_output_stdout();
        sac_warning_stdout();
        sac_error_stdout();
        macro_start ++;
      }
      if(strcmp(argv[i], "--show-prompt") == 0) {
        show_prompt_without_tty(OPTION_ON);
        macro_start ++;
      }
      if(strcmp(argv[i], "--database-on") == 0) {
        use_database(OPTION_ON);
        macro_start ++;
      }
      if(strcmp(argv[i], "--database-off") == 0) {
        use_database(OPTION_OFF);
        macro_start ++;
      }
      if(strcmp(argv[i], "--history-off") == 0) {
        use_history(OPTION_OFF);
        macro_start ++;
      }
      if(strcmp(argv[i], "--history-on") == 0) {
        use_history(OPTION_ON);
        macro_start ++;
      }
      if(strcmp(argv[i], "--set-default-station-name") == 0) {
        set_default_station_name(OPTION_ON);
        macro_start ++;
      }
      if(strcmp(argv[i], "--gdb-debug") == 0) {
        tty_force(OPTION_OFF);
        sac_output_stdout();
        sac_warning_stdout();
        sac_error_stdout();
        macro_start ++;
        show_prompt_without_tty(OPTION_ON);
      }
    }

}
