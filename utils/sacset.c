
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

/* Based on the old sacset passcal distributed program */

#include <stdio.h>
#include <sacio.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <string.h>


int
bool_type(char *val) {
    char *on[]  = {"1", "true", "on", "yes"};
    char *off[] = {"0", "false", "off", "no"};
    for(int k = 0; k < 4; k++) {
        if(strcasecmp(val, on[k]) == 0) {
            return 1;
        }
    }
    for(int k = 0; k < 4; k++) {
        if(strcasecmp(val, off[k]) == 0) {
            return 0;
        }
    }
    return -1;
}

int
is_undefined(const char *s) {
    return
        strcasecmp(s, "undef") == 0 ||
        strcasecmp(s, "undefined") == 0;
}

#define PROGNAME "sacset"

void
usage() {
    printf("Usage: %s [-v] -header=value -header=value files \n", PROGNAME);
    printf("   -v Verbose output [off]\n");
    printf("   -header=value  set 'header' with 'value', e.g.\n");
    printf("       -t0=12.56 \n");
    printf("       -knetwk=II -kstnm=BORG\n");
    printf("       -iztype=IO\n");
    exit(1);
}

int
main(int argc, char *argv[]) {
    int nerr = 0;
    int i = 0, j = 0;
    char *key = NULL;
    char *val = NULL;
    char *empty = "";
    sac *s = NULL;
    double dv = 0.0;
    int iv = 0;
    int verbose = 0;
    char *endptr = NULL;
    char arg[2048] = {0};
    struct hid *h = NULL;
    struct eid *e = NULL;
    int files = 0;
    timespec64 t = {0,0};
    timespec64 t0 = {0,0};

    // Function pointers to sac_read_header and sac_write_header
    // If the header value needs to be changed, then the
    //    full file needs to be read and written
    sac * (*readf)(char *filename, int *nerr)         = sac_read_header;
    void (*writef)(sac *s, char *filename, int *nerr) = sac_write_header;

    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-v") == 0 ||
           strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if(strcasestr(argv[i], "-nvhdr") != NULL) {
            // Read and Write the full file
            // Set function pointers to sac_read() and sac_write()
            readf  = sac_read;
            writef = sac_write;
        } else if(argv[i][0] != '-') {
            files++;
        }
    }
    if(files == 0) {
        usage();
    }

    for(j = 1; j < argc; j++) {
        if(argv[j][0] == '-') {
            continue;
        }
        if((s = readf(argv[j], &nerr)) == NULL) {
            printf("File not found, skipping: %s\n", argv[j]);
            continue;
        }
        if(verbose) {
            printf("Working on file: %s\n", argv[j]);
        }
        for(i = 1; i < argc; i ++) {
            /* Only allow dashed options from here */
            if(argv[i][0] != '-') { 
                continue;
            }
            /* Skip verbose option */
            if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
                continue;
            }
            /* Parse --key=val */
            key = val = NULL;
            strcpy(arg, argv[i]);
            key = arg;
            /* Remove leading --- */
            while(*key == '-') {
                key++;
            }
            /* Find equal sign, splitting option */
            if((val = strchr(arg, '=')) == NULL) {
                printf("\tWarning, expected -header=value, found %s\n", arg);
                continue;
            }
            *val = 0; /* Set equal to terminator, null-terminating key string */
            val++; /* Set val to first character in value */
            if(!*key) {
                val--;
                *val = '=';
                printf("\tWarning, expected -header=value, found %s\n", arg);
                continue;
            }
            if(!*val) {
                val = empty;
            }

            /* Determine the Header ID */
            if((h = sac_keyword_to_header(key, strlen(key))) == NULL) {
                printf("Unrecognized header name, skipping: %s\n", key);
                continue;
            }
            /* Set the header value to 'val' */
            switch(h->type) {
            case SAC_FLOAT_TYPE:
                if(h->id == SAC_O && timespec64_parse(val, &t)) {
                    sac_set_time(s, t);
                    break;
                } else if(sac_is_timeval(h->id) && timespec64_parse(val, &t)) {
                    if(! sac_get_time_ref(s, &t0)) {
                        printf("error setting time for %s = %s (no reference time set)\n", key, val);
                        continue;
                    }
                    int64_t sec  = t.tv_sec - t0.tv_sec;
                    int64_t nsec = t.tv_nsec - t0.tv_nsec;
                    double dt = (double) sec + (double) nsec / (double) 1000000000;
                    sac_set_float(s, h->id, dt);
                    break;
                }
                if(is_undefined(val)) {
                    dv = SAC_FLOAT_UNDEFINED;
                } else {
                    dv = strtod(val, &endptr);
                    if(endptr == NULL || endptr == val || errno == ERANGE || errno == EINVAL || *endptr != 0) {
                        printf("error converting argument to number: %s\n", val);
                        continue;
                    }
                }
                sac_set_float(s, h->id, dv);
                break;
            case SAC_INT_TYPE:
                if(is_undefined(val)) {
                    iv = SAC_INT_UNDEFINED;
                } else {
                    iv = strtol(val, &endptr, 10);
                    if(endptr == val || errno == ERANGE || errno == EINVAL) {
                        printf("error converting argument to integer: %s\n", val);
                        continue;
                    }
                }
                sac_set_int(s, h->id, iv);
                break;
            case SAC_ENUM_TYPE:
                if(is_undefined(val)) {
                    sac_set_int(s, h->id, SAC_INT_UNDEFINED);
                } else {
                    e = sac_enum_to_id(val, strlen(val));
                    if(!e) {
                        printf("error, unknown enum type: %s\n", val);
                        continue;
                    }
                    sac_set_int(s, h->id, e->id);
                }
                break;
            case SAC_BOOL_TYPE:
                if(is_undefined(val)) {
                    iv = SAC_INT_UNDEFINED;
                } else {
                    if((iv = bool_type(val)) == -1) {
                        printf("error, unknown logical type: %s\n", val);
                        continue;
                    }
                }
                sac_set_int(s, h->id, iv);
                break;
            case SAC_STRING_TYPE:
            case SAC_LONG_STRING_TYPE:
                if(is_undefined(val)) {
                    sac_set_string(s, h->id, SAC_CHAR_UNDEFINED);
                } else {
                    sac_set_string(s, h->id, val);
                }
                break;
            case SAC_AUX_TYPE:
                if(h->id == SAC_DATE_TIME) {
                    if(!timespec64_parse(val, &t)) {
                        printf("error parsing date-time: %s\n", val);
                        continue;
                    }
                    sac_set_time(s, t);
                }
                break;
            default:
                printf("Unimplemented header, skipping: %s %d %d\n", key, h->type, h->id);
                continue;
            }
            if(verbose) {
                printf("\t%-12s %s\n", key, val);
            }
        }
        writef(s, argv[j], &nerr);
        sac_free(s);
        s = NULL;
    }
}
