/**
BSD 2-Clause License

Copyright (c) 2019, Brian Savage
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sacio.h>

#define PROGNAME "sacdiff"


void
usage() {
    printf("Usage: %s [-bhq] [-f value] file1 file2\n", PROGNAME);
    printf("      -q Quiet output, only report if files are different [off]\n");
    printf("      -f Maximum difference for floating point values [1e-15] \n");
    printf("      -b Ignore byte order differences [off]\n");
    printf("      -h Output this help\n");
    exit(1);
}

int
main(int argc, char *argv[]) {

    int ch;

    int nerr = 0;
    sac *s1 = NULL;
    sac *s2 = NULL;

    char *file1 = NULL;
    char *file2 = NULL;

    double max_diff = 1e-15;
    int byte_order = TRUE;
    int quiet = 0;

    while((ch = getopt(argc, argv, "hqbf:")) != -1) {
        switch(ch) {
        case 'q':
            quiet = TRUE;
            break;
        case 'f':
            max_diff = atof(optarg);
            break;
        case 'b':
            byte_order = FALSE;
            break;
        case 'h':
            usage();
            exit(1);
            break;
        default:
            printf("Unknown argument: %s\n", optarg);
            break;
        }
    }
    argc -= optind;
    argv += optind;
    if(argc < 2) {
        usage();
    }

    file1 = argv[0];
    file2 = argv[1];

    if(!(s1 = sac_read(file1, &nerr))) {
        printf("%s: Error, could not read sac-file: %s\n", PROGNAME, file1);
        return -1;
    }
    if(!(s2 = sac_read(file2, &nerr))) {
        printf("%s: Error, could not read sac-file: %s\n", PROGNAME, file2);
        return -1;
    }

    return sac_compare(s1, s2, max_diff, byte_order, !quiet);

}
