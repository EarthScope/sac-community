
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rc.h"

extern struct rc_options Options[];

int
rc_set_option(char *key, int type, void *value) {
    int n1, n2;
    int i;

    i = 0;
    n1 = strlen(key);
    while (Options[i].key != NULL) {
        n2 = strlen(Options[i].key);
        if (n1 == n2) {
            if (strncmp(key, Options[i].key, n1) == 0 &&
                type == Options[i].type) {
                (Options[i].set) (value);
                return TRUE;
            }
        }
        i++;
    }
    fprintf(stderr,
            "rc: Line %d Unrecognized option: '%s' -- skipping\n" "    %s\n",
            rc_linenumber(), key, rc_line());
    return FALSE;
}

char *
rc_get_rcfile() {
    int len;
    char *sacrc;
    char *home = getenv("HOME");
    if (home) {
        len = strlen(home) + strlen(SAC_RC_FILE) + 1;
        sacrc = (char *) malloc(sizeof(char) * len);
        sprintf(sacrc, "%s/%s", home, SAC_RC_FILE);
        sacrc[len] = '\0';
    } else {
        sacrc = NULL;
    }
    return sacrc;
}

void
rc() {
    char *rcfile = rc_get_rcfile();
    if (rcfile == NULL)
        return;
    rc_debug(FALSE, FALSE, FALSE);
    rc_read(rcfile, rc_set_option);
    free(rcfile);
    return;
}
