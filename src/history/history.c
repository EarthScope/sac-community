/**
 * @file   history.c
 *
 * @brief  History command
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "select.h"
#include "bool.h"
#include "exm.h"

#include "debug.h"

#define SAC_USE_HISTORY "SAC_USE_HISTORY"

#define HISTORY_COMMAND "history"
#define HISTORY_COMMAND_LENGTH strlen( HISTORY_COMMAND )

#define READLINE_COMMAND_EXPANSION  1
#define READLINE_COMMAND_PRINT      2

static char *sac_history_filename = NULL;
static int sac_history_loaded = FALSE;

int
use_history(int getset) {
    static int virgin = TRUE;
    static int using_history = TRUE;
    if (getset == OPTION_ON || getset == OPTION_OFF) {
        using_history = getset;
    }
    if (virgin) {
        virgin = FALSE;
        using_history = env_bool(SAC_USE_HISTORY, using_history);
    }
    return using_history;
}

#ifdef READLINE

/**
 * @param SAC_HISTORY_MAX
 *     1024
 *     Maximum size of the internal sac history
 */
#define SAC_HISTORY_MAX 1024

/* SAC's own history list: the source of truth for the "history" command,
   for !-expansion, and for the ~/.sac_history file. Each entry is also
   mirrored into linenoise (linenoiseHistoryAdd()) purely so the arrow keys
   can recall it while editing. */
static char **hist_lines = NULL;
static int hist_count = 0;
static int hist_cap = 0;
static int hist_maxlen = SAC_HISTORY_MAX;

/**
 * Append a line to SAC's history list, evicting the oldest entry if the
 *    list is already at its maximum length.
 *
 * @param line
 *    Line to append
 */
void
add_history(const char *line) {
    int i;

    if (hist_count >= hist_maxlen) {
        int drop = hist_count - hist_maxlen + 1;
        for (i = 0; i < drop; i++) {
            free(hist_lines[i]);
        }
        memmove(hist_lines, hist_lines + drop,
                sizeof(char *) * (hist_count - drop));
        hist_count -= drop;
    }
    if (hist_count >= hist_cap) {
        hist_cap = hist_cap ? hist_cap * 2 : 16;
        hist_lines = (char **) realloc(hist_lines, sizeof(char *) * hist_cap);
    }
    hist_lines[hist_count++] = strdup(line);
    linenoiseHistoryAdd(line);
}

/**
 * Set the maximum number of retained history entries, evicting the oldest
 *    entries if the list is already longer than the new maximum. Clamped
 *    to SAC_HISTORY_MAX regardless of the requested size.
 *
 * @param size
 *    New maximum length
 */
void
stifle_history(int size) {
    int i, drop;

    if (size < 1) {
        size = 1;
    }
    if (size > SAC_HISTORY_MAX) {
        size = SAC_HISTORY_MAX;
    }
    hist_maxlen = size;
    if (hist_count > hist_maxlen) {
        drop = hist_count - hist_maxlen;
        for (i = 0; i < drop; i++) {
            free(hist_lines[i]);
        }
        memmove(hist_lines, hist_lines + drop,
                sizeof(char *) * (hist_count - drop));
        hist_count -= drop;
    }
    linenoiseHistorySetMaxLen(hist_maxlen);
}

/**
 * Discard all retained history entries.
 */
void
clear_history(void) {
    int i;
    for (i = 0; i < hist_count; i++) {
        free(hist_lines[i]);
    }
    FREE(hist_lines);
    hist_count = 0;
    hist_cap = 0;
}

/**
 * Read one newline-terminated line from a file into a growable buffer.
 *
 * @param fp
 *    File to read from
 *
 * @return
 *    Heap-allocated line with the trailing newline (and any carriage
 *    return) removed, or NULL at end of file
 */
static char *
sac_history_read_line(FILE * fp) {
    char *line;
    size_t cap, len;
    int c;

    cap = 64;
    line = (char *) malloc(cap);
    if (!line) {
        return NULL;
    }
    len = 0;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (len + 1 >= cap) {
            char *grown;
            cap *= 2;
            grown = (char *) realloc(line, cap);
            if (!grown) {
                free(line);
                return NULL;
            }
            line = grown;
        }
        line[len++] = (char) c;
    }
    if (len == 0 && c == EOF) {
        free(line);
        return NULL;
    }
    if (len > 0 && line[len - 1] == '\r') {
        len--;
    }
    line[len] = '\0';
    return line;
}

/**
 * Load history entries from a file, oldest first, one per line.
 *
 * @param file
 *    File to read
 */
void
read_history(char *file) {
    FILE *fp;
    char *line;

    fp = fopen(file, "r");
    if (!fp) {
        return;
    }
    while ((line = sac_history_read_line(fp)) != NULL) {
        if (line[0] != '\0') {
            add_history(line);
        }
        free(line);
    }
    fclose(fp);
}

/**
 * Write all retained history entries to a file, oldest first, one per
 *    line.
 *
 * @param file
 *    File to write
 */
void
write_history(char *file) {
    FILE *fp;
    int i;

    fp = fopen(file, "w");
    if (!fp) {
        return;
    }
    for (i = 0; i < hist_count; i++) {
        fprintf(fp, "%s\n", hist_lines[i]);
    }
    fclose(fp);
}

void
history_print() {
    int j;
    for (j = 0; j < hist_count; j++) {
        printf("%5d  %s\n", j + 1, hist_lines[j]);
    }
}

/**
 * Expand the documented subset of csh-style history event designators:
 *    !!, !n, !-n, !str. A bare '!' followed by nothing, space, tab,
 *    newline, '=' or '(' is left literal.
 *
 * @param line
 *    Line as typed
 * @param output
 *    OUTPUT - always a heap-allocated string the caller must free: either
 *    the expansion, or (when there is no expansion or the event is not
 *    found) a copy of \p line
 *
 * @return
 *    READLINE_COMMAND_EXPANSION if \p line was expanded, 0 if it was left
 *    as-is, or a negative value if an event designator was given but no
 *    matching history entry was found
 */
int
history_expand(char *line, char **output) {
    const char *match = NULL;
    char c;

    if (line[0] != '!') {
        *output = strdup(line);
        return 0;
    }
    c = line[1];
    if (c == '\0' || c == ' ' || c == '\t' || c == '\n' || c == '=' ||
        c == '(') {
        *output = strdup(line);
        return 0;
    }

    if (c == '!') {
        if (hist_count > 0) {
            match = hist_lines[hist_count - 1];
        }
    } else if (c == '-' && isdigit((unsigned char) line[2])) {
        int n = atoi(line + 2);
        int idx = hist_count - n;
        if (idx >= 0 && idx < hist_count) {
            match = hist_lines[idx];
        }
    } else if (isdigit((unsigned char) c)) {
        int n = atoi(line + 1);
        if (n >= 1 && n <= hist_count) {
            match = hist_lines[n - 1];
        }
    } else {
        size_t len = strlen(line + 1);
        int j;
        for (j = hist_count - 1; j >= 0; j--) {
            if (strncmp(hist_lines[j], line + 1, len) == 0) {
                match = hist_lines[j];
                break;
            }
        }
    }

    if (!match) {
        *output = strdup(line);
        return -1;
    }
    *output = strdup(match);
    return READLINE_COMMAND_EXPANSION;
}

/* ************** Exported functions ************** */

char *
AddToHistory(char *line) {

    int result;
    char *expansion;
    char *p;
    char *out;

    DEBUG("%s\n", line);
    result = history_expand(line, &expansion);
    DEBUG("result: %d %d '%s' [PRINT]\n", result, READLINE_COMMAND_PRINT,
          expansion);
    if (result == READLINE_COMMAND_PRINT) {
        printf("%s\n", expansion);
        free(expansion);
        line[0] = ' ';
        line[1] = 0;
        return NULL;
    }

    if (result < 0) {
        free(expansion);
        line[0] = ' ';
        line[1] = 0;
        return NULL;
    }
    DEBUG("result: %d %d [EXPANSION] \n", result, READLINE_COMMAND_EXPANSION);
    if (result == READLINE_COMMAND_EXPANSION) {
        out = strdup(expansion);
    } else {
        out = strdup(line);
    }
    DEBUG("out: '%s'\n", out);
    free(expansion);
    expansion = NULL;

    DEBUG("result: %s %s [HISTORY COMMAND] \n", line, HISTORY_COMMAND);
    if (strncmp(out, HISTORY_COMMAND, HISTORY_COMMAND_LENGTH) == 0) {
        history_print();
    }

    /* Check for empty lines */
    p = &out[0];
    while (isspace(p[0])) {
        p++;
    };
    if (p[0] == '\0') {
        out[0] = 0;
    }
    if (strlen(out) > 0 && use_history(OPTION_GET)) {
        add_history(out);
    }
    DEBUG("Return\n");
    return out;

}

#else

void
read_history(char *file) {
}

void
stifle_history(int size) {
}

void
write_history(char *file) {
}

void
clear_history(void) {
}

char *
AddToHistory(char *line) {
    return strdup(line);
}
#endif

/* Not compiled above when READLINE is undefined (--disable-editing). */
#ifndef SAC_HISTORY_MAX
#define SAC_HISTORY_MAX 1024
#endif

static int sac_history_size = SAC_HISTORY_MAX;

/**
 * Set the internal history size
 *
 * @param value
 *    New history size
 *    - < 0 sets the value to its maximum [ SAC_HISTORY_MAX ]
 */
void
history_size_set(int value) {
    if (value <= 0) {
        sac_history_size = SAC_HISTORY_MAX;
    } else {
        sac_history_size = value;
    }
    return;
}

/**
 * Get the current history size
 *
 * @return
 *    Current histroy size
 *
 * @see SAC_HISTORY_MAX
 * @see history_size_set()
 *
 */
int
history_size() {
    return sac_history_size;
}

void
sac_history_filename_free() {
    FREE(sac_history_filename);
}

/**
 * Set the sac_history filename. Free any previous history name.
 *    Copy the name if it is specified, otherwise derive the name
 *    from the user's home directory and the variable SAC_HISTORY_FILE
 *
 * @param name
 *    New file name for the sac history file
 *
 * @see SAC_HISTORY_FILE
 *
 */
void
sac_history_file_set(char *name) {
    int len;
    char *sachistory;
    char *home;
    if (sac_history_filename) {
        /* Free previous, if any */
        free(sac_history_filename);
        sac_history_filename = NULL;
    }
    if (name) {
        /* Duplicate for save -- never know whether static area pointed at! */
        sac_history_filename = strdup(name);
    } else {
        /* Null name signifies default */
        home = getenv("HOME");
        if (home) {
            len = strlen(home) + strlen(SAC_HISTORY_FILE) + 2;
            sachistory = (char *) malloc(sizeof(char) * len);
            sprintf(sachistory, "%s/%s", home, SAC_HISTORY_FILE);
            sachistory[len - 1] = '\0';
        } else {
            sachistory = NULL;
        }
        sac_history_filename = sachistory;
    }
}

/**
 * Get the sac history filename
 *
 * @return
 *    File name for the sac history
 */
char *
sac_history_file() {
    return sac_history_filename;
}

/**
 * Load the sac history file from a file
 *
 * @param where
 *    Filename to load the history from
 *
 */
void
sac_history_load(char *where) {
    if (sac_history_loaded) {
        return;
    }
    if(!use_history(OPTION_GET)) {
        sac_history_loaded = TRUE;
        return;
    }
    stifle_history(history_size());
    if (!where) {
        where = sac_history_file();
    }
    if (where) {
        read_history(where);
    }
    sac_history_loaded = TRUE;
}
