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

void
history_print() {
    int j;
    HIST_ENTRY *he;
    while ((he = next_history())) {
        continue;
    }
    j = -1;

    for (he = current_history(); he != NULL; he = previous_history()) {
        if (j > 0) {
            printf("%5d  %s\n", j, he->line);
        }
        j++;
    }

    return;
}

const char *
history_navigate(int direction) {
    HIST_ENTRY *he = NULL;

    switch (direction) {
        case 0:
            he = current_history();
            break;
        case 1:
            he = next_history();
            break;
        case -1:
            he = previous_history();
            break;
    }
    if (he) {
        return he->line;
    }
    return NULL;
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

char *
AddToHistory(char *line) {
    return strdup(line);
}
#endif

/** 
 * @param SAC_HISTORY_MAX
 *     1024
 *     Maximum size of the internal sac history
 */
#define SAC_HISTORY_MAX 1024

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
        sac_history_size = INT_MAX;
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

static char *sac_history_filename = NULL;
static int sac_history_loaded = FALSE;

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
