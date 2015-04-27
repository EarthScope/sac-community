
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rc.h"

static int verbose, linebyline, parse;

void
usage() {
    fprintf(stderr, "usage: sacparse \n");
    fprintf(stderr, "       -l Line by Line Output\n");
    fprintf(stderr, "       -p Parse Debug\n");
    fprintf(stderr, "       -h Help Statement\n");
    fprintf(stderr, "       -H Help Statement\n");
    fprintf(stderr, "       -? Help Statement\n");
    fprintf(stderr, "       -v Verbose \n");
    exit(-1);
}

int
main(int argc, char *argv[]) {
    int ch;

    verbose = FALSE;
    linebyline = FALSE;
    parse = FALSE;
    while ((ch = getopt(argc, argv, "plhH?v")) != -1) {
        switch (ch) {
            case 'v':
                verbose++;
                break;
            case 'l':
                linebyline = TRUE;
                break;
            case 'p':
                parse = TRUE;
                break;
            case 'h':
            case 'H':
            case '?':
            default:
                usage();
        }
    }
    argc -= optind;
    argv += optind;

    rc_debug(verbose, linebyline, parse);
    rc("commands", rc_set_option);

    return 0;
}
