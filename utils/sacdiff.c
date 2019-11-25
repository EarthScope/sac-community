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

char *SacHeaderName[] = {
  "empty",
  "delta",		/* RF time increment, sec    */
  "depmin",		/*    minimum amplitude      */
  "depmax",		/*    maximum amplitude      */
  "scale",		/*    amplitude scale factor */
  "odelta",       /*    observed time inc      */
  "b",			/* RD initial time - wrt nz* */
  "e",			/* RD end time               */
  "o",        /*    event start            */
  "a",        /*    1st arrival time       */
  "Fmt",      /*    internal use           */

  "t0",       /*    user-defined time pick */
  "t1",       /*    user-defined time pick */
  "t2",       /*    user-defined time pick */
  "t3",       /*    user-defined time pick */
  "t4",       /*    user-defined time pick */
  "t5",       /*    user-defined time pick */
  "t6",       /*    user-defined time pick */
  "t7",       /*    user-defined time pick */
  "t8",       /*    user-defined time pick */
  "t9",       /*    user-defined time pick */

  "F",        /*    event end, sec > 0     */
  "resp0",		/*    instrument respnse parm*/
  "resp1",		/*    instrument respnse parm*/
  "resp2",		/*    instrument respnse parm*/
  "resp3",		/*    instrument respnse parm*/
  "resp4",		/*    instrument respnse parm*/
  "resp5",		/*    instrument respnse parm*/
  "resp6",		/*    instrument respnse parm*/
  "resp7",		/*    instrument respnse parm*/
  "resp8",		/*    instrument respnse parm*/

  "resp9",		/*    instrument respnse parm*/
  "stla",		/*  T station latititude     */
  "stlo",		/*  T station longitude      */
  "stel",		/*  T station elevation, m   */
  "stdp",		/*  T station depth, m       */
  "evla",		/*    event latitude         */
  "evlo",		/*    event longitude        */
  "evel",		/*    event elevation        */
  "evdp",		/*    event depth            */
  "mag",          /*    reserved for future use*/

  "user0",		/*    available to user      */
  "user1",		/*    available to user      */
  "user2",		/*    available to user      */
  "user3",		/*    available to user      */
  "user4",		/*    available to user      */
  "user5",		/*    available to user      */
  "user6",		/*    available to user      */
  "user7",		/*    available to user      */
  "user8",		/*    available to user      */
  "user9",		/*    available to user      */

  "dist",		/*    stn-event distance, km */
  "az",			/*    event-stn azimuth      */
  "baz",		/*    stn-event azimuth      */
  "gcarc",		/*    stn-event dist, degrees*/
  "sb",       /*    internal use           */
  "sdelta",     /*    internal use           */
  "depmen",		/*    mean value, amplitude  */
  "cmpaz",		/*  T component azimuth      */
  "cmpinc",		/*  T component inclination  */
  "xminimum",		/*    reserved for future use*/

  "xmaximum",		/*    reserved for future use*/
  "yminimum",		/*    reserved for future use*/
  "ymaximum",		/*    reserved for future use*/
  "unused6",		/*    reserved for future use*/
  "unused7",		/*    reserved for future use*/
  "unused8",		/*    reserved for future use*/
  "unused9",		/*    reserved for future use*/
  "unused10",		/*    reserved for future use*/
  "unused11",		/*    reserved for future use*/
  "unused12",		/*    reserved for future use*/

  /* ints */
  "nzyear",   /*  F zero time of file, yr  */
  "nzjday",   /*  F zero time of file, day */
  "nzhour",   /*  F zero time of file, hr  */
  "nzmin",    /*  F zero time of file, min */
  "nzsec",    /*  F zero time of file, sec */
  "nzmsec",   /*  F zero time of file, msec*/
  "nvhdr",          /*  R header version number  */
  "norid",    /*    internal use           */
  "nevid",    /*    internal use           */
  "npts",   /* RF number of samples      */

  "nsnpts",   /*    internal use           */
  "nwfid",    /*    internal use           */
  "xsize",    /*    reserved for future use*/
  "ysize",    /*    reserved for future use*/
  "unused15",   /*    reserved for future use*/
  "iftype",   /* RA type of file           */
  "idep",   /*    type of amplitude      */
  "iztype",   /*    zero time equivalence  */
  "unused16",   /*    reserved for future use*/
  "iinst",    /*    recording instrument   */
  "istreg",   /*    stn geographic region  */
  "ievreg",   /*    event geographic region*/
  "ievtyp",   /*    event type             */
  "iqual",    /*    quality of data        */
  "isynth",   /*    synthetic data flag    */
  "imagtyp",        /*    reserved for future use*/
  "imagsrc",        /*    reserved for future use*/
  "unused19",   /*    reserved for future use*/
  "unused20",   /*    reserved for future use*/
  "unused21",   /*    reserved for future use*/
  "unused22",   /*    reserved for future use*/
  "unused23",   /*    reserved for future use*/
  "unused24",   /*    reserved for future use*/
  "unused25",   /*    reserved for future use*/
  "unused26",   /*    reserved for future use*/
  "leven",    /* RA data-evenly-spaced flag*/
  "lpspol",   /*    station polarity flag  */
  "lovrok",   /*    overwrite permission   */
  "lcalda",   /*    calc distance, azimuth */
  "unused27",   /*    reserved for future use*/
  "kstnm",    /*  F station name           */
  "kevnm",    /*    event name             */
  "kevnm empty",        /*                           */
  "khole",    /*    man-made event name    */
  "ko",     /*    event origin time id   */
  "ka",     /*    1st arrival time ident */
  "kt0",    /*    time pick 0 ident      */
  "kt1",    /*    time pick 1 ident      */
  "kt2",    /*    time pick 2 ident      */
  "kt3",    /*    time pick 3 ident      */
  "kt4",    /*    time pick 4 ident      */
  "kt5",    /*    time pick 5 ident      */
  "kt6",    /*    time pick 6 ident      */
  "kt7",    /*    time pick 7 ident      */
  "kt8",    /*    time pick 8 ident      */
  "kt9",    /*    time pick 9 ident      */
  "kf",     /*    end of event ident     */
  "kuser0",   /*    available to user      */
  "kuser1",   /*    available to user      */
  "kuser2",   /*    available to user      */
  "kcmpnm",   /*  F component name         */
  "knetwk",   /*    network name           */
  "kdatrd",   /*    date data read         */
  "kinst"               /*    instrument name        */
};

#define ENDIAN(swap) ((swap) ? "non-native" : "native")

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
    
    int i = 0;
    int j = 0;

    int nerr = 0;
    sac *s1 = NULL;
    sac *s2 = NULL;

    char *file1 = NULL;
    char *file2 = NULL;

    double f1   = 0.0;
    double f2   = 0.0;
    int n1      = 0;
    int n2      = 0;
    char c1[32] = {0};
    char c2[32] = {0};
    float *y1 = NULL;
    float *y2 = NULL;

    double max_diff = 1e-15;
    int byte_order = TRUE;
    int retval = 0;
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

    if(byte_order && s1->m->swap != s2->m->swap) {
        if(!quiet) {
            printf("byte-order: %s %s\n", ENDIAN(s1->m->swap), ENDIAN(s2->m->swap));
        }
        retval = 1;
    }

    for(i = SAC_DELTA; i <= SAC_UN70; i++) {
        sac_get_float(s1, i, &f1);
        sac_get_float(s2, i, &f2);
        if(fabs(f1 - f2) > max_diff) {
            if(!quiet) {
                printf("%-10s %f %f\n", SacHeaderName[i], f1, f2);
            }
            retval = 1;
        }
    }
    for(i = SAC_YEAR; i <= SAC_UN110; i++) {
        sac_get_int(s1, i, &n1);
        sac_get_int(s2, i, &n2);
        if(n1 != n2) {
            if(!quiet) {
                printf("%-10s %d %d\n", SacHeaderName[i], n1, n2);
            }
            retval = 1;
        }
    }
    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_EVENT2) {
            continue;
        }
        sac_get_string(s1, i, c1, sizeof c1);
        sac_get_string(s2, i, c2, sizeof c2);
        if(strcmp(c1, c2) != 0) {
            if(!quiet) {
                printf("%-10s '%s' '%s'\n", SacHeaderName[i], c1, c2);
            }
            retval = 1;
        }
    }
    n1 = sac_comps(s1);
    n2 = sac_comps(s2);
    if(n1 != n2) {
        if(!quiet) {
            printf("%-10s %d %d\n", "data-comps", n1, n2);
        }
        goto done;
    }
    sac_get_int(s1, SAC_NPTS, &n1);
    sac_get_int(s2, SAC_NPTS, &n2);
    if(n1 != n2) {
        goto done;
    }

    for(j = 0; j < sac_comps(s1); j++) {
        y1 = (j == 0) ? s1->y : s1->x;
        y2 = (j == 0) ? s2->y : s2->x;

        for(i = 0; i < n2; i++) {
            double df = fabs(y1[i] - y2[i]);
            if(df > max_diff) {
                if(!quiet) {
                    printf("%s[%d]: %e %e diff: %.16e\n", (j == 0) ? "y" : "x", i,
                           y1[i], y2[i], df);
                }
                retval = 1;
            }
        }
    }
 done:
    if(retval) {
        printf("Files %s and %s differ\n", file1, file2);
    }
    return retval;
}
