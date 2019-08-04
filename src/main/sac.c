/** 
 * @file   sac.c
 * 
 * @brief  Main execution loop, start here
 * 
 */
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef WIN32
#include <getopt.h>
#endif

#include "unistdx.h"
#include <signal.h>

#include "mach.h"
#include "exm.h"
#include "bool.h"
#include "select.h"

#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "top.h"
#include "bbs.h"
#include "co.h"
#include "ncpf.h"
#include "ssi.h"
#include "sac_history.h"
#include "debug.h"
#include "string_utils.h"

EXM_EXTERN

void sac_command_line_options(int argc, char **argv);
void sac_command_line_copyright(int argc, char **argv);
void sac_initialize(int argc, char **argv);
void main_command(char *kmsg, int n);
void execute_command_line(char *kmsg, int len);

void sac_main_loop();

#ifdef X11_APP
void set_constrain_plot_ratio_x11(int set);
#endif

void
usage() {
    printf("Usage sac [options] [sac-macro-file]\n"
           "   -b --bell-off      Turn off bell \n"
           "   -B --bell-on       Turn on bell\n"
           "   -c --copyright-off Turn copyright off\n"
           "   -C --copyright-on  Turn copyright on\n"
           "   -d --database-off  Turn off SeisMgr database \n"
           "   -D --database-on   Turn on SeisMgr database\n"
           "   -j --history-off   Turn off History\n"
           "   -J --history-on    Turn on History\n"
           "   -p --prompt-off    Turn off prompt without a tty, i.e. script\n"
           "   -P --prompt-on     Turn on prompt \n"
           "   -L --letter        Make plots letter page size (X11)\n"
           "   -t --no-tty        Force the tty off (no line editing)\n"
           //         "   -g --gdb-debug     \n"
           "   -s --stdout        All output as stdout\n"
           "   -n --set-default-station-name\n");
    exit(1);
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

#include <curl/curl.h>

#ifdef X11_APP
int
main(int argc, char **argv) {

    char kmsg[MCMSG + 1];

    curl_global_init(CURL_GLOBAL_DEFAULT);

    memset(&(kmsg[0]), ' ', MCMSG);

    kmsg[0] = '\0';
    kmsg[MCMSG] = '\0';

    /* - Initialize common. */
    sac_initialize(argc, argv);
    // extern optind from getopt.h
    argc -= optind - 1;
    argv += optind - 1;

    /* - Get the input line message, if any.
     *   This should be the name of the a default SAC macro to execute. */

    zgimsg(argc, argv, kmsg, MCMSG + 1);
    execute_command_line(kmsg, MCMSG + 1);

    sac_main_loop();

    curl_global_cleanup();
    return 0;
}

#endif /* X11_APP */

void
execute_command_line(char *kmsg, int len) {
    int nerr;
    char *s1 = NULL;
    UNUSED(len);
    if(strlen(kmsg) > 0) {
        s1 = NULL;
        asprintf(&s1, "macro %s", kmsg);
        saccommands(s1, strlen(s1), &nerr);
        FREE(s1);
    }
}

/*
 *   (1a) X11 - see sac_main_loop()
 *              zgpmsg() waits for input from stdin (user) or X11 events (windows)
 *                - X11 events are processed by handle_event()
 *                - stdin key pressed are consumed until a newline is reached
 *                  and then zpgmsg returns
 *   (1b) OSX - see sac.m (SAC implementation)
 *              - SAC.init saves itself to be referenced later
 *                - NSSacView also save a copy for later
 *                    - SAC_id (SAC program object)
 *                    - NSSac_id (SAC plot window(s) objects)
 *              - SAC.applicationDidFinishLaunching:(NSNotification *) note
 *                Inititilization is mostly done here
 *                sac_main_loop() is called in a secondary thread
 *                Graphics are handled on the main thread
 *                Drawing commands follow
 *                    - SAC display device for osx in src/osx/stubs.c
 *                    - to osx_sac_* functions that use NSSac_id in src/osx/osx_sac.c
 *                    - NSSacView_* functions that call NSSacView.* 
 *                       - Drawing commands are added to the stack
 *                Drawing is done in NSSacView.drawrect
 *                     - window updates required (refresh/resize)
 *                     - update called through endframe or flush_buffer
 *   (2) The message is first passed through "processline" which evaluates any
 *       blackboard or header variables and substitutes them into the message.
 *   (3) The message is then passed to "saccommands" which parses and executes
 *       the command(s) that are contained within the message.
 *   (4) When all the commands have been executed or an error has
 *       occured, "saccommands" returns and the process is repeated.
 *   (5) Program termination is handled by the QUIT command found
 *       in the Executive Module (subroutine "xexmc"). 
 */
void
main_command(char *kmsg, int n) {
    int nerr, ncmsg;
    char *msgout;

    msgout = AddToHistory(kmsg);
    if (!msgout) {
        return;
    }

    n = strlen(msgout);

    ncmsg = indexb(msgout, n + 1);
    if (ncmsg > MCMSG) {
        error(99, "Cmd line exceeds buffer limited to num of chars: %d", MCMSG);
        outmsg();
        clrmsg();
        return;
    }
    setmsg("COMMAND", 99);
    apcmsg(msgout, n + 1);
    outmsg();
    clrmsg();

    /* Remove prompt at beginning of a line */
    if (strncasecmp(msgout, "SAC> ", 5) == 0) {
        memmove(&msgout[0], &msgout[5], n-4);
    }

    saccommands(msgout, MCMSG + 1, &nerr);

    if (msgout) {
        FREE(msgout);
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
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--copyright-off") == 0) {
            display_copyright(OPTION_OFF);
        }
        if (strcmp(argv[i], "-c") == 0) {
            display_copyright(OPTION_OFF);
        }
        if (strcmp(argv[i], "--copyright-on") == 0) {
            display_copyright(OPTION_ON);
        }
        if (strcmp(argv[i], "-C") == 0) {
            display_copyright(OPTION_ON);
        }
    }
}

#ifdef WIN32
void
sac_command_line_options(int *argc_p, char ***argv_p) {
  return;
}
#else
void
sac_command_line_options(int argc, char **argv) {
    char ch;

    static struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"copyright-off", no_argument, NULL, 'c'},
        {"copyright-on", no_argument, NULL, 'C'},
        {"bell-off", no_argument, NULL, 'b'},
        {"bell-on", no_argument, NULL, 'b'},
        {"no-show-prompt", no_argument, NULL, 'p'},
        {"show-prompt", no_argument, NULL, 'P'},
        {"database-off", no_argument, NULL, 'd'},
        {"database-on", no_argument, NULL, 'D'},
        {"history-off", no_argument, NULL, 'j'},
        {"history-on", no_argument, NULL, 'J'},

#ifdef X11_APP
        {"letter", no_argument, NULL, 'L'},
#endif
        {"no-tty", no_argument, NULL, 't'},
        {"gdb-debug", no_argument, NULL, 'g'},
        {"set-default-station-name", no_argument, NULL, 'n'},
        {"stdout", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    while ((ch =
            getopt_long(argc, argv, "cCbBpPdDjJLtgnsh", longopts,
                        NULL)) != -1) {
        switch (ch) {
            case 'h':
                usage();
                break;
            case 'c':
                display_copyright(OPTION_OFF);
                break;
            case 'C':
                display_copyright(OPTION_ON);
                break;
            case 'b':
                bell_off();
                break;
            case 'B':
                bell_on();
                break;
            case 'p':
                show_prompt_without_tty(OPTION_OFF);
                break;
            case 'P':
                show_prompt_without_tty(OPTION_ON);
                break;
            case 'd':
                use_database(OPTION_OFF);
                break;
            case 'D':
                use_database(OPTION_ON);
                break;
            case 'j':
                use_history(OPTION_OFF);
                break;
            case 'J':
                use_history(OPTION_ON);
                break;
            case 't':
                tty_force(OPTION_OFF);
                break;
            case 'g':
                tty_force(OPTION_OFF);
                show_prompt_without_tty(OPTION_ON);
                /* Fall Through */
            case 's':
                sac_output_stdout();
                sac_warning_stdout();
                sac_error_stdout();
                break;
            case 'n':
                set_default_station_name(OPTION_ON);
                break;
  #ifdef X11_APP
            case 'L':
                set_constrain_plot_ratio_x11(TRUE);
                break;
#endif
            default:
                break;
        }
    }
}

#endif
        /* - THIS IS THE MAIN LOOP OF THE PROGRAM.
         *   (1) "zgpmsg" sends a prompt to the user and gets a message back.
         */
void
sac_main_loop() {
    char kmsg[MCMSG + 1];
    while (TRUE) {
        zgpmsg(kmexm.kprmt, 13, kmsg, MCMSG + 1);
        main_command(kmsg, MCMSG);
    }
}
