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
#include <dirent.h>
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
            /* EOF Received - Likely Control-D */
            str = strdup("quit");
            return strlen(str);
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

#ifdef READLINE

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

    out[n - 1] = 0;
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

int
is_directory(char *s) {
    struct stat stbuf;
    if (stat(s, &stbuf) == 0) {
        return S_ISDIR(stbuf.st_mode);
    }
    return FALSE;
}

/**
 * Collect every SAC command name that case-insensitively starts with \p text.
 *
 * @param text
 *    Partial command name typed so far
 * @param np
 *    OUTPUT - number of matches returned
 *
 * @return
 *    Heap array of heap-allocated command names, to be freed by the caller
 */
static char **
sac_match_commands(const char *text, int *np) {
    static char **cmd = NULL;
    static int ncmd = 0;
    static int init = TRUE;
    char **out = NULL;
    int n = 0;
    size_t len = strlen(text);
    int i;

    if (init) {
        init = FALSE;
        cmd = uniq_cmd(&ncmd);
    }
    for (i = 0; i < ncmd; i++) {
        if (strncasecmp(cmd[i], text, len) == 0) {
            out = (char **) realloc(out, sizeof(char *) * (n + 1));
            out[n++] = strdup(cmd[i]);
        }
    }
    *np = n;
    return out;
}

/**
 * Collect every directory entry whose name starts with the filename portion
 *    of \p text, joined back with any directory portion \p text carried.
 *
 * @param text
 *    Partial file path typed so far
 * @param np
 *    OUTPUT - number of matches returned
 *
 * @return
 *    Heap array of heap-allocated "dir/name" strings, to be freed by the
 *    caller
 */
static char **
sac_match_filenames(const char *text, int *np) {
    DIR *dp;
    struct dirent *de;
    char **out = NULL;
    int n = 0;
    const char *slash;
    char dirpart[PATH_MAX];
    const char *fnprefix;
    size_t dirlen;
    size_t plen;

    slash = strrchr(text, '/');
    if (slash) {
        dirlen = (size_t) (slash - text) + 1;
        if (dirlen >= sizeof(dirpart)) {
            dirlen = sizeof(dirpart) - 1;
        }
        memcpy(dirpart, text, dirlen);
        dirpart[dirlen] = '\0';
        fnprefix = slash + 1;
    } else {
        dirpart[0] = '\0';
        dirlen = 0;
        fnprefix = text;
    }
    plen = strlen(fnprefix);

    dp = opendir(dirlen ? dirpart : ".");
    if (!dp) {
        *np = 0;
        return NULL;
    }
    while ((de = readdir(dp)) != NULL) {
        char *joined;
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }
        if (strncmp(de->d_name, fnprefix, plen) != 0) {
            continue;
        }
        joined = (char *) malloc(dirlen + strlen(de->d_name) + 1);
        sprintf(joined, "%s%s", dirpart, de->d_name);
        out = (char **) realloc(out, sizeof(char *) * (n + 1));
        out[n++] = joined;
    }
    closedir(dp);
    *np = n;
    return out;
}

/**
 * linenoise completion callback. At column 0, complete SAC command names;
 *    otherwise complete file names. A unique match gets a trailing
 *    separator appended (a space, or '/' for a directory); multiple
 *    matches are left as-is so the user keeps typing or cycles with TAB.
 *
 * @param line
 *    The line as typed so far
 * @param lc
 *    linenoise completion table to populate with full-line candidates
 */
static void
sac_completion(const char *line, linenoiseCompletions *lc) {
    size_t start;
    const char *text;
    char *prefix;
    char **matches;
    int i, n;

    start = strlen(line);
    while (start > 0 && !isspace((unsigned char) line[start - 1])) {
        start--;
    }
    text = line + start;

    prefix = (char *) malloc(start + 1);
    memcpy(prefix, line, start);
    prefix[start] = '\0';

    if (start == 0) {
        matches = sac_match_commands(text, &n);
    } else {
        matches = sac_match_filenames(text, &n);
    }

    for (i = 0; i < n; i++) {
        char *candidate;
        size_t clen;
        char sep = 0;

        if (n == 1) {
            sep = (start != 0 && is_directory(matches[i])) ? '/' : ' ';
        }
        clen = strlen(prefix) + strlen(matches[i]) + (sep ? 2 : 1);
        candidate = (char *) malloc(clen);
        if (sep) {
            snprintf(candidate, clen, "%s%s%c", prefix, matches[i], sep);
        } else {
            snprintf(candidate, clen, "%s%s", prefix, matches[i]);
        }
        linenoiseAddCompletion(lc, candidate);
        free(candidate);
        free(matches[i]);
    }
    free(matches);
    free(prefix);
}

#define SAC_LINE_INITIAL_LEN 4096
#define SAC_LINE_MAX_LEN     65536

static struct linenoiseState ls;
static int editor_active = FALSE;
static char *pending_partial_line = NULL;

/**
 * Start a line-editing session on stdin/stdout with \p prompt, restoring
 *    any partial line stashed by editor_stop_and_save_partial().
 *
 * @return
 *    0 on success, -1 on failure (e.g. out of memory)
 */
static int
editor_start(const char *prompt) {
    char *buf;

    buf = (char *) malloc(SAC_LINE_INITIAL_LEN);
    if (!buf) {
        return -1;
    }
    linenoiseSetCompletionCallback(sac_completion);
    if (linenoiseEditStart(&ls, 0, 1, buf, SAC_LINE_INITIAL_LEN, prompt) == -1) {
        free(buf);
        return -1;
    }
    ls.buflen_max = SAC_LINE_MAX_LEN;
    editor_active = TRUE;

    if (pending_partial_line) {
        linenoiseEditSetBuffer(&ls, pending_partial_line, strlen(pending_partial_line));
        FREE(pending_partial_line);
        pending_partial_line = NULL;
    }
    return 0;
}

/**
 * Stop a line-editing session, if one is active. Idempotent: callable from
 *    zquit(), a fatal X11 I/O error, and the end of a mat session, any of
 *    which may run after the session has already been stopped.
 */
static void
editor_stop(void) {
    if (!editor_active) {
        return;
    }
    linenoiseEditStop(&ls);
    free(ls.buf);
    ls.buf = NULL;
    editor_active = FALSE;
}

/**
 * Stop the active line-editing session, stashing whatever the user had
 *    typed so it can be restored by the next editor_start(). Used when a
 *    GUI event (the macOS command pipe) interrupts a line read.
 */
static void
editor_stop_and_save_partial(void) {
    if (!editor_active) {
        return;
    }
    FREE(pending_partial_line);
    pending_partial_line = (ls.len > 0) ? strdup(ls.buf) : NULL;
    editor_stop();
}

void
sac_line_editor_stop(void) {
    editor_stop();
}

/**
 * Select different input from a variety of sources. Primarilly
 *    the command line (stdin) through linenoise and the X11
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
            struct timeval *timeout, readline_callback * func, int stdin_on,
            int gui_on) {

    int i;                      /* index for prefilling string w/ NULLs */

    int retval;
    fd_set fd;
    int max_fd, stdin_fd, gui_fd;
    int nerr;
    char kprmt[128];
    char *getline_msg;
    char *event_msg;
    int use_editor = FALSE;
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
        if (use_tty() && !linenoiseIsUnsupportedTerm()) {
            if (editor_start(kprmt) == -1) {
                perror("SAC: Unable to start the line editor");
                exit(-1);
            }
            use_editor = TRUE;
        }
    }
    fflush(stdout);
    /* Take care of printing the prompt when there is no tty, or the
     *    terminal cannot support command line editing.
     *    This normally happends during script processing
     */
    if (!use_editor && show_prompt_without_tty(OPTION_GET)) {
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
                    if (!use_editor) {
                        if ((getline_msg = getline_stdin()) != NULL) {
                            select_loop_message(getline_msg, -1);
                            select_loop_continue(SELECT_OFF);
                            free(getline_msg);
                            getline_msg = NULL;
                        }
                        if (!use_tty() && select_loop_continue(SELECT_QUERY)) {
                            /* stdin became readable but no full line came
                               through: a script whose input ended without
                               a quit command. Force a clean shutdown
                               rather than spin forever. */
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
                    } else {
                        char *line;

                        errno = 0;
                        line = linenoiseEditFeed(&ls);
                        if (line == linenoiseEditMore) {
                            /* keep looping */
                        } else if (line != NULL) {
                            func(line);
                        } else if (errno == ENOENT || errno == 0) {
                            /* Ctrl-D on an empty line, or real EOF */
                            func(NULL);
                        } else if (errno == EINTR) {
                            /* Interrupted read: try again */
                        } else {
                            /* Ctrl-C (EAGAIN), or an I/O error: discard the
                               typed line and start over rather than quit */
                            editor_stop();
                            if (editor_start(kprmt) == -1) {
                                perror("SAC: Unable to restart the line editor");
                                exit(-1);
                            }
                        }
                    }
                }
                if (input(gui_fd, &fd)) {
                    if (use_editor) {
                        linenoiseHide(&ls);
                    }
                    event_msg = handle_event(&nerr);
                    if (use_editor) {
                        fflush(stdout);
                        if (event_msg) {
                            editor_stop_and_save_partial();
                            use_editor = FALSE;
                        } else {
                            linenoiseShow(&ls);
                        }
                    }
                    if (event_msg) {
                        select_loop_continue(SELECT_OFF);
                        select_loop_message(event_msg, -1);
                        FREE(event_msg);
                    }
                }
        }

        if (timeout) {
            editor_stop();
            return (0);
        }
    }
    select_loop_message(msg, msglen);
    select_loop_continue(SELECT_OFF);
    editor_stop();
    return (0);
}

#endif /* READLINE */
