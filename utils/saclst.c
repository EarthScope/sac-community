/************************************************************
*	List the SAC header fields
*	Usage
*		saclst header_variable_names ... f sac_files ...
*       History
*             1999  Original Coding             Lupei Zhi
*             2003  Further Updates             Qinya Liu
*       28 05 2006  Addition to SAC Codebase    Brian Savage
*                   Combined saclst.c and sacio.c
*       29 09 2019  Switch to sacio library; v7 Brian Savage
*       License
*               Distributed under the same License as the SAC Source
*               and binaries, used here under permission by Lupei Zhu
*
*************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sacio.h>

#define OUTPUT_DATE      201
#define OUTPUT_TIME      202
#define OUTPUT_MONTH     204
#define OUTPUT_DAY       205

#define OUTPUT_DEFAULT   301
#define OUTPUT_ALL       302
#define OUTPUT_FULL      303
#define OUTPUT_DEFAULT1  304

#define OUTPUT_PICKS     401

#define SAC_HEADER_FLOAT_MIN   SAC_DELTA
#define SAC_HEADER_FLOAT_MAX   SAC_UN70

#define SAC_HEADER_FIELDS      SAC_INST

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
  "ibody",   /*    Body / Spheroid */
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


extern char *enum_values[];

void
output_enums_list() {
    char *p = NULL;
    char tmp[32] = {0};
    struct eid *e = NULL;
    fprintf(stderr, "\t       Name    Value     Header   Category\n");
    for(int i = ITIME; i <= IODOR; i++) {
        strcpy(tmp, enum_values[i-1]);
        p = strchr(tmp, ' ');
        if(!p) {
            continue;
        }
        *p = 0;
        e = sac_enum_to_id(tmp, strlen(tmp));
        if(e) {
            fprintf(stderr, "\t%15s %4d     ", enum_values[i-1], e->id);
            switch(e->type) {
            case SAC_FILE_TYPE:   fprintf(stderr, "iftype   File Type"); break;
            case SAC_DEP_TYPE:    fprintf(stderr, "idep     Amplitude Type"); break;
            case SAC_ZERO_TIME:   fprintf(stderr, "iztype   Zero Time Reference"); break;
            case SAC_EVENT_TYPE:  fprintf(stderr, "ievtyp   Event Type"); break;
            case SAC_QUAL:        fprintf(stderr, "iqual    Quality Type"); break;
            case SAC_MAG_TYPE:    fprintf(stderr, "imagtyp  Magnitude Type"); break;
            case SAC_MAG_SRC:     fprintf(stderr, "imagsrc  Magnitude Source"); break;
            default:
                break;
            }
            fprintf(stderr, " \n");
        }
    }
    exit(-1);
}

void
output_header_list() {
    fprintf(stderr, "Usage: saclst header_values f file_lists\n");
    fprintf(stderr, "   ex. saclst delta npts kstnm f sacfile1 sacfile2\n");
    fprintf(stderr, "    All Values are case insensitive, except F\n");
    fprintf(stderr, "    If header_values = default  - All Defined Values, 2 Columns\n");
    fprintf(stderr, "    If header_values = default1 - All Defined Values, 1 Column\n");
    fprintf(stderr, "    If header_values = all      - All Values\n");
    fprintf(stderr, "    If header_values = Full     - All Values Formatted (capital F)\n");
    fprintf(stderr, "Available SAC Header Values\n");
    fprintf(stderr, "\tTime-series Values\n");
    fprintf(stderr, "\t   b e o a F ko ka kf\n");
    fprintf(stderr, "\t   npts delta depmin depmax depmen scale nvhdr\n");
    fprintf(stderr, "\tStation and Event Values\n");
    fprintf(stderr, "\t   kstnm stlo stla stel stdp\n");
    fprintf(stderr, "\t   kevnm evlo evla evel evdp\n");
    fprintf(stderr, "\t   dist az baz gcarc khole\n");
    fprintf(stderr, "\t   kcmpnm knetwk kdatrd kinst cmpaz cmpinc\n");
    fprintf(stderr, "\t   iftype idep iztype iinst istreg ievreg ievtyp iqual isynth\n");
    fprintf(stderr, "\tTiming Values\n");
    fprintf(stderr, "\t   kzdate kztime odelta\n");
    fprintf(stderr, "\t   nzyear nzjday nzmonth nzday nzhour nzmin nzsec nzmsec\n");
    fprintf(stderr, "\tPicks, Response, and User Values\n");
    fprintf(stderr, "\t   t0    t1    t2    t3    t4    t5    t6    t7    t8    t9\n");
    fprintf(stderr, "\t   kt0   kt1   kt2   kt3   kt4   kt5   kt6   kt7   kt8   kt9\n");
    fprintf(stderr, "\t   resp0 resp1 resp2 resp3 resp4 resp5 resp6 resp7 resp8 resp9\n");
    fprintf(stderr, "\t   user0 user1 user2 user3 user4 user5 user6 user7 user8 user9\n");
    fprintf(stderr, "\t   kuser0 kuser1 kuser2\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "\tFor a reference on enumerated values\n");
    fprintf(stderr, "\t   - See the the SAC Data file Format in the User Manual \n");
    fprintf(stderr, "\t   - saclst enums \n");
    exit(-1);
}


char *
sac_header_value_string(sac *s, struct hid *h, char *dst, size_t n) {
  int ipt;
  double fpt;

  switch(h->type) {
  case SAC_FLOAT_TYPE:
      sac_get_float(s, h->id, &fpt);
      snprintf(dst, n, "%11.5E", fpt);
      break;
  case SAC_INT_TYPE:
      sac_get_int(s, h->id, &ipt);
      snprintf(dst, n, "%11d", ipt);
      break;
  case SAC_ENUM_TYPE:
      sac_get_int(s, h->id, &ipt);
      if(ipt == SAC_INT_UNDEFINED) {
          snprintf(dst, n, "UNDEFINED");
      } else if(ipt >= 0 && ipt <= IMARS) {
          snprintf(dst, n, "%s", enum_values[ipt-1]);
      }
      break;
  case SAC_BOOL_TYPE:
      sac_get_int(s, h->id, &ipt);
      snprintf(dst, n, "%s", ipt ? "TRUE" : "FALSE");
      break;
  case SAC_STRING_TYPE:
  case SAC_LONG_STRING_TYPE:
      sac_get_string(s, h->id, dst, n);
      break;
  default:
      break;
  }
  return dst;
}

int
main(int argc, char **argv) {
    int nerr = 0;
    int   i,j,nl,k;
  double    fpt;
  int           ipt;
  char         cpt[36];
  int64_t       year;
  int           month = 0, day = 0, oday = 0;
  int           def, all, full;
  char         *header_name = NULL;
  int           newline;
  char          tmp[64] = {0};
  sac *s = NULL;
  timespec64 t = {0,0};


  struct hid ls[200];

  if(argc < 2) {
      fprintf(stderr,"Usage: saclst header_lists f file_lists\n");
      fprintf(stderr,"   ex. saclst delta npts kstnm f sacfile1 sacfile2\n");
      fprintf(stderr,"       saclst help - outputs a list of possible values\n");
      fprintf(stderr,"       saclst enums - outputs a list of possible enumerated values\n");
      return -1;
  }
  def = all = full = 0;

  nl=0; argv++; argc--;
  while ( *argv[0] != 'f' ) {
    if(strcasecmp("help", argv[0]) == 0) {
      output_header_list();
    }
    if(strcasecmp("enums", argv[0]) == 0) {
      output_enums_list();
    }
    struct hid *tmp = NULL;
    if((tmp = sac_keyword_to_header(argv[0], strlen(argv[0]))) != NULL) {
        ls[nl] = *tmp;
    } else {
        if(strcasecmp(argv[0], "default") == 0) {
            def = 1;
        } else if(strcasecmp(argv[0], "all") == 0) {
            all = 1;
        } else if(strcasecmp(argv[0], "full") == 0) {
            full = 1;
        } else if(strcasecmp(argv[0], "default1") == 0) {
            def = 2;
        } else if(strcasecmp(argv[0], "picks") == 0) {
            struct hid tt = { .name = "", .type = OUTPUT_PICKS, .id = 0 };
            ls[nl] = tt;
        } else {
            fprintf(stderr, "saclst: Error in header_list  %s\n", argv[0]);
            return -1;
        }
    }
    nl++; argv++; argc--;
  }

  if(def || all || full) {
    for(i = 1; i < argc; i++) {
        //if( read_sachead(argv[i], &hd) != -1 ) {
        if((s = sac_read_header(argv[i], &nerr)) != NULL) {
            newline = 0;
            printf("File: %s\n", argv[i]);
            for(j = SAC_DELTA; j <= SAC_HEADER_FIELDS; j++) {
                header_name = SacHeaderName[j];

                if(full) {
                    if(j== SAC_DELTA) {
                        printf(" REAL        INDEX  NAME        Real Value Real Value\n");
                    } else if(j == SAC_YEAR) {
                        printf(" INTEGER     INDEX  NAME        Int Value  Int Value\n");
                    } else if(j == SAC_FILE_TYPE) {
                        printf(" ENUMERATED  INDEX  NAME        Int Value  Enum Value\n");
                    } else if(j == SAC_EVEN) {
                        printf(" LOGICAL     INDEX  NAME        Int Value  Log Value\n");
                    } else if(j == SAC_STA) {
                        printf(" CHARACTER   INDEX  NAME        Int Value  Char Value\n");
                    }
                }
                struct hid *h = sac_keyword_to_header(header_name, strlen(header_name));
                if(!h) {
                    continue;
                }
                switch(h->type) {
                case SAC_FLOAT_TYPE:
                    sac_get_float(s, h->id, &fpt);
                    if( def && fpt != SAC_FLOAT_UNDEFINED ) {
                        printf("      %-8s  %16.5E", header_name, fpt);
                        newline++;
                    }
                    else if(all)
                        printf("               %3d  %-8s  %16.5E\n", j-1, header_name, fpt);
                    else if(full) {
                        printf("               %3d  %-8s  %16.5E %s\n", j-1, header_name, fpt,
                               sac_header_value_string(s, h, tmp, sizeof(tmp)));
                    }
                    break;

                case SAC_INT_TYPE:
                case SAC_ENUM_TYPE:
                case SAC_BOOL_TYPE:
                    sac_get_int(s, h->id, &ipt);
                    if( def && ipt != SAC_INT_UNDEFINED ) {
                        printf("      %-8s  %16d", header_name, ipt);
                        newline++;
                    }
                    else if(all)
                        printf("               %3d  %-8s  %16d\n", j-1, header_name, ipt);
                    else if(full) {
                        printf("               %3d  %-8s  %16d %s\n", j-1, header_name, ipt,
                               sac_header_value_string(s, h, tmp, sizeof(tmp)));
                    }
                    break;

                case SAC_STRING_TYPE:
                case SAC_LONG_STRING_TYPE:
                    sac_get_string(s, h->id, cpt, sizeof(cpt));
                    if( def && strncmp(cpt, SAC_CHAR_UNDEFINED, 8) != 0 ) {
                        printf("      %-8s  %16s", header_name, cpt);
                        newline++;
                    }
                    else if(all)
                        printf("               %3d  %-8s  %16s\n", j-1, header_name, cpt);
                    else if(full) {
                        printf("               %3d  %-8s  %16s %s\n", j-1, header_name, cpt,
                               sac_header_value_string(s, h, tmp, sizeof(tmp)));
                    }
                    break;
                }
                if(newline == 2 || (def == 2 && newline > 0)) {
                    printf("\n");
                    newline = 0;
                }
            }
            if(def && newline > 0)
                printf("\n");
        }
    }
    return 0;
  }
  for (i=1; i<argc; i++) {

      if((s = sac_read_header(argv[i], &nerr)) != NULL) {
        printf("%s ", argv[i]);
        for (j=0; j<nl; j++) {
            switch(ls[j].type) {
            case SAC_FLOAT_TYPE:
                sac_get_float(s, ls[j].id, &fpt);
                printf("%12.6g", fpt);
                break;
            case SAC_INT_TYPE:
                sac_get_int(s, ls[j].id, &ipt);
                printf("%10d", ipt);
                break;
            case SAC_ENUM_TYPE:
            case SAC_BOOL_TYPE:
                sac_get_int(s, ls[j].id, &ipt);
                printf("%10d/%s", ipt, sac_header_value_string(s, &ls[j], tmp, sizeof(tmp)));
                break;
            case SAC_STRING_TYPE:
            case SAC_LONG_STRING_TYPE:
                sac_get_string(s, ls[j].id, cpt, sizeof(cpt));
                printf("   %s", cpt);
                break;
            case SAC_AUX_TYPE:
                sac_get_time_ref(s, &t);
                timespec64_to_ymd(&t, &year, &month, &day, &oday);
                if(ls[j].id == SAC_DATE) {
                    printf(" %5d/%.02d/%.02d", s->h->nzyear, month, day);
                } else if(ls[j].id == SAC_TIME) {
                    printf(" %.02d:%.02d:%06.3f", s->h->nzhour, s->h->nzmin, s->h->nzsec + 0.001 * s->h->nzmsec);
                } else if(ls[j].id == SAC_MONTH) {
                    printf("%10d",month);
                } else if(ls[j].id == SAC_MONTH_DAY) {
                    printf("%10d",day);
                }
                break;
            default:
                if(ls[j].type == OUTPUT_PICKS) {
                    for(k = SAC_T0; k <= SAC_T9; k++) {
                        sac_get_float(s, k, &fpt);
                        printf("%12.6g", fpt);
                    }
                }
                break;
            }
        }
        printf("\n");
    }
  }
  return 0;
}

