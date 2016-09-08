/** 
 * @file   select.c
 * 
 * @brief  Control input from different sources
 * 
 */
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <limits.h>

#include "unistdx.h"
#include <errno.h>

#include "co.h"
#include "string_utils.h"
#include "proto.h"
#include "bool.h"
#include "select.h"
#include "gd3.x11.h"
#include "gdm.h"
#include "comlists.h"
#include "debug.h"
#include "sac_history.h"

COMLISTS_EXTERN

/** 
 * Set and Get a message from the command line
 * 
 * @param p 
 *    Command from the command line
 * @param len 
 *    - > 0 Get the command, length of \p p
 *    - < 0 Set and save the command
 * 
 * @return 
 *   Command from the command line
 *
 */
int
select_loop_message(char *p, int len) {
    static char *str = NULL;

    if (len <= 0) {
        if (str) {
            free(str);
            str = NULL;
        }
        if (!p) {
            return 0;
        }
        str = strdup(p);
        return (strlen(str));
    }
    if (!str) {
        str = strdup("        ");
    }

    len = len - 2;              /* 1 for \0 and 1 for a space */
    len = ((int) strlen(str) > len) ? len : (int) strlen(str);

    strncpy(p, str, len);
    p[len++] = ' ';             /* Space */
    p[len++] = '\0';            /* String Teminator */

    return (len);
}

/** 
 * Toggle the select loop on and off
 * 
 * @param w 
 *    - SELECT_ON  - Turn on select loop
 *    - SELECT_OFF - Turn off select loop
 *    - SELECT_QUERY - Get select loop status (do not set)
 * 
 * @return 
 *    Current select loop status
 *
 */
int
select_loop_continue(int w) {
    static int flag;
    if (w == SELECT_ON || w == SELECT_OFF) {
        flag = w;
    }
    return (flag);
}

/** 
 * Fix a timeval structure; place extra microseconds into seconds
 * 
 * @param t 
 *    Timeval structure to fix
 *
 */
void
timeval_fix(struct timeval *t) {
    if (t) {
        t->tv_sec = t->tv_sec + floor(t->tv_usec / (1000 * 1000));
        t->tv_usec = t->tv_usec - t->tv_sec * (1000 * 1000);
    }
}

#ifndef TERMIOS
/** 
 * Check if input is available from the file descriptor
 * 
 * @param i 
 *    File Descriptor
 * @param fd 
 *    File Descriptor Set (Collection)
 * 
 * @return 
 *    - TRUE if input is available on \p i
 *    - FALSE if input is not available on \p i
 *
 */
static int
input(int i, fd_set * fd) {
    if (i >= 0) {
        return (FD_ISSET(i, fd));
    }
    return (0);
}
#endif /* TERMIOS */

/** 
 * Determine whether to display a prompt or not. Depends on the enviornment
 *    variable SAC_SCRIPT_PROMPT_DISPLAY and if sac has a controlled tty
 *
 * @return 
 *   - TRUE - if the prompt is desired
 *   - FALSE - if the prompt is not desired
 *
 */
int
show_prompt_without_tty(int getset) {
    static int flag = -1;
    char *c;
    if (getset == OPTION_ON || getset == OPTION_OFF) {
        flag = getset;
    }
    if (flag == -1) {
        flag = FALSE;
        if ((c = getenv("SAC_SCRIPT_PROMPT_DISPLAY")) != NULL) {
            if (strcmp(c, "1") != 0 && strcmp(c, "0") != 0) {
                fprintf(stderr,
                        "SAC warning: SAC_SCRIPT_PROMPT_DISPLAY must be 0 or 1\n");
                flag = FALSE;
            } else {
                flag = atoi(c);
            }
        }
    }
    return flag;
}

char *
strdup_trim(char *s) {
    int n;
    char *p, *out;
    p = strchr(s, ' ');
    if (!p) {
        n = strlen(s);
    } else {
        n = (p - s) + 1;
    }
    n = n + 1;
    out = (char *) malloc(sizeof(char) * n);
    strncpy(out, s, n - 1);

    out[n] = 0;
    return out;
}

char *
strdup_lower(char *in) {
    char *p;
    char *out = strdup_trim(in);
    p = out;
    while (*p != '\0') {
        *p = tolower(*p);
        p++;
    }
    return out;
}

int
string_case_compare(const void *pa, const void *pb) {
    char **a = (char **) pa;
    char **b = (char **) pb;
    return strcasecmp(*a, *b);
}

char **
uniq(char **s, int *np) {
    int i, j, n;
    char **tmp;
    n = *np;
    if (n <= 0) {
        return s;
    }
    /* Sort Strings */
    qsort(s, n, sizeof(char *), string_case_compare);

    /* Count unique strings */
    tmp = (char **) malloc(sizeof(char *) * n);
    j = 1;
    tmp[0] = strdup(s[0]);
    for (i = 1; i < n; i++) {
        if (strcasecmp(s[i], s[i - 1]) != 0) {
            tmp[j++] = strdup(s[i]);
        }
    }
    n = j;
    for (i = 0; i < n; i++) {
        free(s[i]);
        s[i] = NULL;
    }
    free(s);
    s = NULL;
    *np = j;

    return tmp;
}

char **
uniq_cmd(int *np) {
    int i, j, n;
    char **cmd;

    n = Ncomlistentries[3] + Icomliststart[3];
    cmd = (char **) malloc(sizeof(char *) * n);
    j = 0;
    for (i = 0; i < n; i++) {
        if (kmcomlists.kcomnames_full[i][0] != ' ' &&
            kmcomlists.kcomnames_full[i][0] != 0) {
            cmd[i] = strdup_lower(kmcomlists.kcomnames_full[i]);
            j++;
        }
    }
    n = j;
    cmd = uniq(cmd, &n);
    *np = n;
    return cmd;
}

char *
sac_attempt_complete_command(const char *text, int state) {
    static int i, n, len;
    static int init = TRUE;
    static char **cmd;
    int j;

    if (init) {
        init = FALSE;
        cmd = uniq_cmd(&n);
    }

    /* New Word -- initialize */
    if (!state) {
        i = 0;
        len = strlen(text);
    }
    while (i < n) {
        j = i++;
        if (strncasecmp(cmd[j], text, len) == 0) {
            return strdup(cmd[j]);
        }
    }
    return (char *) NULL;
}

int
is_single_match(char **s) {
    int j;
    if (s == NULL) {            /* No matches */
        return FALSE;
    }
    /* Count matches */
    j = 0;
    while (s[j] != NULL) {
        j++;
    }

    /* Matches
     * #1 - Replacement text
     * #2 - 1st Possible completion (Single)
     * #3 - 2nd possible complettion (Not single)
     */
    if (j <= 2) {
        return TRUE;
    }
    return FALSE;
}

#ifndef TERMIOS

int
is_directory(char *s) {
    struct stat stbuf;
    if (stat(s, &stbuf) == 0) {
        return S_ISDIR(stbuf.st_mode);
    }
    return FALSE;
}

char **
sac_attempt_complete(const char *text, int start, int end) {
    char **matches;
    UNUSED(end);
    rl_completion_append_character = '\0';
    if (start == 0) {
        matches = rl_completion_matches(text, sac_attempt_complete_command);
        if (is_single_match(matches)) {
            rl_completion_append_character = ' ';
        }
    } else {
        matches = rl_completion_matches(text, rl_filename_completion_function);
        if (is_single_match(matches)) {
            if (is_directory(matches[0])) {
                rl_completion_append_character = '/';
            } else {
                rl_completion_append_character = ' ';
            }
        }
    }
    return matches;
}
#endif
/** 
 * Select different input from a variety of sources. Primarilly 
 *    the command line (stdin) through readline/editline and the X11
 *    window system.
 * 
 * @param prmt 
 *    Prompt to display on the command line
 * @param prmtlen 
 *    Length of \p prmt
 * @param msg 
 *    Output message from the command line
 * @param msglen 
 *    Length of \p msg
 * @param timeout 
 *    Timeout value if requested
 * @param func 
 *    Function to call when a full command line has been entered
 * 
 * @return 
 */

int
select_loop(char *prmt, int prmtlen, char *msg, int msglen,
            struct timeval *timeout, VCPFunction * func, int stdin_on,
            int gui_on) {

    int i;                      /* index for prefilling string w/ NULLs */

    int retval;
    fd_set fd;
    int max_fd, stdin_fd, gui_fd;
    int nerr;
    char kprmt[128];
    char *getline_msg;
    char *event_msg;

    UNUSED(prmtlen);

    sac_history_load(NULL);

    /* Show the Prompt */
    i = 0;
    while (prmt[i] != '$') {
        kprmt[i] = prmt[i];
        i++;
    }
    kprmt[i] = '\0';

    if (stdin_on) {
        if (use_tty()) {
            rl_callback_handler_remove();
            rl_set_prompt(NULL);
            rl_callback_handler_install(kprmt, func);
            rl_completion_append_character = '\0';
            rl_attempted_completion_function = sac_attempt_complete;
        }
    }
    fflush(stdout);
    /* Take care of printing the prompt when there is no tty
     *    This normally happends during script processing 
     */
    if (!use_tty() && show_prompt_without_tty(OPTION_GET)) {
        fprintf(stdout, "%s", kprmt);
        fflush(stdout);
    }

    stdin_fd = -1;
    gui_fd = -1;

    /* Loop until we encounter a newline */
    select_loop_continue(SELECT_ON);

    timeval_fix(timeout);

    while (select_loop_continue(SELECT_QUERY)) {
        max_fd = -1;
        FD_ZERO(&fd);

        /* Add STDIN to the File Descriptor Set (FD_SET) */
        if (!use_tty() && timeout) {    /* This is here due to co/xpause.c and co/zsleep.c */
            max_fd = -1;
        } else if (stdin_on) {
            stdin_fd = 0;
            FD_SET(stdin_fd, &fd);
            if (stdin_fd > max_fd) {
                max_fd = stdin_fd;
            }
        }
        /* Add GUI to the File Descriptor Set (FD_SET) */
        if (gui_on) {
            if ((gui_fd = get_file_descriptor()) > 0) {
                FD_SET(gui_fd, &fd);
                if (gui_fd > max_fd) {
                    max_fd = gui_fd;
                }
            }
        }

        /* Wait until we get life from one the File Descriptors, then act */
        retval = select(max_fd + 1, &fd, NULL, NULL, timeout);
        switch (retval) {
            case -1:           /* Error Condition */
                if (errno != EINTR) {
                    perror("SAC: Select Error");
                    exit(-1);
                }
                break;
            case 0:            /* Timeout Expired */
                break;
            default:
                if (input(stdin_fd, &fd)) {
                    if (!use_tty()) {
                        if ((getline_msg = getline_stdin()) != NULL) {
                            select_loop_message(getline_msg, -1);
                            select_loop_continue(SELECT_OFF);
                            free(getline_msg);
                            getline_msg = NULL;
                        }
                    } else {
                        rl_callback_read_char();
                    }
                    if (!use_tty() && select_loop_continue(SELECT_QUERY)) {
                        /* Assumes the the entire line is read in at once and processline is called
                           for each entry into rl_callback_read_char().  This will probably break on
                           some machine, some where, probably when using the GNU readline library. 
                         */
                        fprintf(stderr,
                                "SAC Error: EOF/Quit\n"
                                "     SAC executed from a script: quit command missing\n"
                                "     Please add a quit to the script to avoid this message\n"
                                "     If you think you got this message in error, \n"
                                "     please report it to: %s\n",
                                PACKAGE_BUGREPORT);
                        select_loop_continue(SELECT_OFF);
                        select_loop_message("quit", -1);
                    }
                }
                if (input(gui_fd, &fd)) {
                    if ((event_msg = handle_event(&nerr))) {
                        select_loop_continue(SELECT_OFF);
                        select_loop_message(event_msg, -1);
                        FREE(event_msg);
                    }
                }
        }

        if (timeout) {
            return (0);
        }
    }
    select_loop_message(msg, msglen);
    select_loop_continue(SELECT_OFF);
    return (0);
}
