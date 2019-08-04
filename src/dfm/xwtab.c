/** 
 * @file   xwtab.c
 * 
 * @brief  Write a Table Ascii file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "amf.h"


#include "extfunc.h"

#include "wild.h"
#include "bot.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "ncpf.h"
#include "debug.h"

#define MALPHA  100
#define MAXCH   40
#define MBLKSZ  500
#define MENTRY  40

DFM_EXTERN

/** 
 * Execute the command WRITETABLE which reads in columns of data
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 *    000419:  Original version, plagerized from xrtab.c.
 *
 */
void
xwtab(int *nerr) {

    char kchange[MCPFN + 1], kfile[MCPFN + 1], kstring[MCPFN + 1];

    int nderr;
    int lexpnd, liftype, lheader, lwrdir;
    int idx, jdfl, nchar, nstring;
    FILE *nun;

    char *file, *pfile;
    string_list *list;
    sac *s;
    nun = 0;

    lwrdir = FALSE;
    file = pfile = NULL;
    *nerr = 0;
    lexpnd = TRUE;

    /* PARSING PHASE: */
    /* - Loop on each token in command: */

    list = string_list_init();

    while (lcmore(nerr)) {

        /* -- "DIR CURRENT|name":  set name of the default subdirectory. */
        if (lkchar("DIR#$", 6, MCPFN, kmdfm.kwrdir, MCPFN + 1, &nchar)) {
            lwrdir = set_output_path(kmdfm.kwrdir);
        }

        /* -- "IFTYPE": provide the data type, default is off. */
        else if (lklog("&IFTYPE$", 9, &liftype)) {
            cmdfm.liftype = liftype;
        }

        /* -- "HEADER": prints begin and delta on a line at the top. */
        else if (lklog("&HEADER$", 9, &lheader)) {
            cmdfm.lheader = lheader;
        }

        /* -- "COMMIT|RECALLTRACE|ROLLBACK":
           how to treat existing data */
        else if (lckeyExact("COMMIT", 7))
            cmdfm.icomORroll = COMMIT;
        else if (lckeyExact("RECALLTRACE", 12))
            cmdfm.icomORroll = RECALL;
        else if (lckeyExact("RECALL", 7))
            cmdfm.icomORroll = RECALL;
        else if (lckeyExact("ROLLBACK", 9))
            cmdfm.icomORroll = ROLLBACK;

        /* generate names from the KSTCMP header field */
        else if (lckeyExact("KSTCMP#$", 9)) {
            lexpnd = FALSE;
            gennames("KSTCMP ", 7, list, saclen(), nerr);
            if (*nerr != 0)
                goto L_8888;
        }

        /* -- "APPEND string": append string to filenames */
        else if (lkcharExact
                 ("APPEND#$", 9, MCPFN, kstring, MCPFN + 1, &nstring)) {
            if ((*nerr = files_append(list, kstring))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "PREPEND string": prepend string to filenames */
        else if (lkcharExact
                 ("PREPEND#$", 10, MCPFN, kstring, MCPFN + 1, &nstring)) {
            if ((*nerr = files_prepend(list, kstring))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "DELETE string": delete string from filenames */
        else if (lkcharExact
                 ("DELETE#$", 9, MCPFN, kstring, MCPFN + 1, &nstring)) {
            if ((*nerr = files_delete(list, kstring))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "CHANGE string1 string2": change string1 to string2 in filenames */
        else if (lkcharExact
                 ("CHANGE#$", 9, MCPFN, kstring, MCPFN + 1, &nstring)) {
            lcchar(kchange, sizeof(kchange));
            if ((*nerr = files_change(list, kstring, kchange))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "filelist":  define a new filelist (or add to old filelist) */
        else if ((list = lcdfl())) {    /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */
    /* - Check for null write filelist. */
    if (string_list_length(list) <= 0) {
        *nerr = 1311;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }

    /* - Make sure the write filelist has as many entries as read filelist */
    if (string_list_length(list) != saclen()) {
        error(*nerr =
              1312, " %d, file in memory: %d", string_list_length(list), saclen());
        goto L_8888;
    }

    /* EXECUTION PHASE: */
    /* - Commit or rollback data according to cmdfm.icomORroll */
    alignFiles(nerr);
    if (*nerr)
        return;

    if (cmdfm.lechof && lexpnd) {
        setmsg("OUTPUT", 0);
        display_file_list(list);
        wrtmsg(MUNOUT);
    }

    /* - Write each file in memory to disk. */
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        /* -- Get file from memory manager. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        /* isolate file name */
        file = string_list_get(list, jdfl - 1);
        /* -- Check overwrite-protect flag in header record. */
        if (cmdfm.lovrrq && !s->h->lovrok) {
            error(*nerr = 1303, "%s", file);
            outmsg();
            clrmsg();
            goto L_8888;
        }

        pfile = prepare_output_filename(file, lwrdir, kmdfm.kwrdir);

        /* - Create file. */
        zdest(pfile, strlen(kfile) + 1, &nderr);
        if((nun = fopen(pfile, "wb+")) == NULL) {
            *nerr = ERROR_OPENING_FILE;
            goto L_8888;
        }
        if (cmdfm.liftype) {
            /* Write the data type. */
            switch (s->h->iftype) {
                case ITIME:
                    fprintf(nun, "Time Series\n");
                    break;

                case IRLIM:
                    fprintf(nun, "Spectral:  real/imaginary\n");
                    break;

                case IAMPH:
                    fprintf(nun, "Spectral:  amplitude/phase\n");
                    break;

                case IXY:
                    fprintf(nun, "XY file\n");
                    break;

                case IXYZ:
                    fprintf(nun, "XYZ file\n");
                    break;

                default:
                    if (s->n <= 1)
                        fprintf(nun,
                                "No datatype specified:  treated as Time Series\n");
                    else
                        fprintf(nun,
                                "No datatype specified: treated as XY data\n");
                    break;
            }                   /* end switch */
        }
        /* end if( cmdfm.liftype ) */
        if (cmdfm.lheader) {
            /* if leven, write begin and delta */
            if (s->h->leven) {
                fprintf(nun, "Begin time: %f ;  Delta time: %f\n", s->h->b,
                        s->h->delta);
            } else
                fprintf(nun, "Unevenly spaced data.\n");

            if (s->h->iftype == ITIME && s->h->leven == TRUE) {
                for (idx = 0; idx < s->h->npts; idx++) {
                    fprintf(nun, "%0 13.6e\n", s->y[idx]);
                }
                fflush(nun);
            } else {
                for (idx = 0; idx < s->h->npts; idx++) {
                    fprintf(nun, "%0 13.6e\t%0 13.6e\n", s->y[idx], s->x[idx]);
                }
                fflush(nun);
            }
        }
        /* end if( cmdfm.lheader ) */
        else if (s->h->leven) {
            switch (s->h->iftype) {
                case ITIME:    /* one calculated column, one read column */
                    if (s->h->b == SAC_FLOAT_UNDEFINED ||
                        s->h->delta == SAC_FLOAT_UNDEFINED) {
                        warning(1393, " %d", jdfl);
                        outmsg();
                        clrmsg();
                    }
                    for (idx = 0; idx < s->h->npts; idx++) {
                        fprintf(nun, "%0 13.6e\t%0 13.6e\n",
                                s->h->b + (s->h->delta * (float) idx),
                                s->y[idx]);
                    }
                    fflush(nun);
                    break;

                case IRLIM:
                case IAMPH:
                case IXYZ:     /* one calc, two read */
                    if (s->h->b == SAC_FLOAT_UNDEFINED ||
                        s->h->delta == SAC_FLOAT_UNDEFINED) {
                        warning(1393, " %d", jdfl);
                        outmsg();
                        clrmsg();
                    }
                    for (idx = 0; idx < s->h->npts; idx++) {
                        fprintf(nun, "%0 13.6e\t%0 13.6e\t%0 13.6e\n",
                                s->h->b + (s->h->delta * (float) idx),
                                s->y[idx], s->x[idx]);
                    }
                    fflush(nun);
                    break;

                case IXY:      /* two read columns */
                    for (idx = 0; idx < s->h->npts; idx++) {
                        fprintf(nun, "%0 13.6e\t%0 13.6e\n", s->y[idx],
                                s->x[idx]);
                    }
                    fflush(nun);
                    break;

                default:
                    warning(1393, " %d", jdfl);
                    outmsg();
                    clrmsg();
                    break;
            }                   /* end switch */
        }
        /* end elseif( s->h->leven ) */
        else {                  /* lheader if false, s->h->leven is false */
            switch (s->h->iftype) {
                case ITIME:
                case IXY:      /* write 2 read columns */
                    for (idx = 0; idx < s->h->npts; idx++) {
                        fprintf(nun, "%0 13.6e\t%0 13.6e\n", s->y[idx],
                                s->x[idx]);
                    }
                    fflush(nun);
                    break;

                case IRLIM:
                case IAMPH:
                case IXYZ:     /* pretend it's leven */
                    if (s->h->b == SAC_FLOAT_UNDEFINED ||
                        s->h->delta == SAC_FLOAT_UNDEFINED) {
                        warning(1393, " %d", jdfl);
                        outmsg();
                        clrmsg();
                    }
                    for (idx = 0; idx < s->h->npts; idx++) {
                        fprintf(nun, "%0 13.6e\t%0 13.6e\t%0 13.6e\n",
                                s->h->b + (s->h->delta * (float) idx),
                                s->y[idx], s->x[idx]);
                    }
                    fflush(nun);
                    break;

                default:
                    warning(1393, " %d", jdfl);
                    outmsg();
                    clrmsg();
                    break;
            }                   /* end switch */
        }
    }                           /* end for ( jdfl ) */

  L_8888:
    if (nun != 0) {
        zcloses(&nun, &nderr);
    }
    FREE(pfile);
    return;

}
