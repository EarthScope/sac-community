/** 
 * @file   xw.c
 * 
 * @brief  Write
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "sddhdr.h"
#include "amf.h"
#include "wild.h"
#include "bot.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "ncpf.h"
#include "errors.h"

/* -- Prepare output file name:
 * --- If directory option is ON (lwrdir=.TRUE. and nwrdir>0), 
 *     concatenate directory name with file name part of write file list.
 * --- If directory option is CURRENT (lwrdir=.TRUE. and nwrdir=0), 
 *     use file name part of write file list.
 * --- If directory option is OFF, use write file list. */
char *
prepare_output_filename(char *in, int ldir, char *dir) {
    char *out;
    char *b;
    if (ldir) {
        b = basename(in);
        asprintf(&out, "%s%s", dir, b);
    } else {
        out = strdup(in);
    }
    return out;
}

int
set_output_path(char *ktemp) {
    int lwrdir = FALSE;
    rstrip(ktemp);
    if (strcasecmp(ktemp, "OFF") == 0) {
        lwrdir = FALSE;
    } else if (strcasecmp(ktemp, "CURRENT") == 0) {
        lwrdir = TRUE;
        ktemp[0] = 0;
    } else if (ktemp[strlen(ktemp) - 1] != KDIRDL) {
        /* If the string is mising the "/" path separator */
        strcat(ktemp, "/");
        lwrdir = TRUE;
    } else {
        /* Path is not OFF, CURRENT and has the "/" at the end */
        lwrdir = TRUE;
    }
    return lwrdir;
}

int
files_append(string_list * files, char *kstr) {
    int i, nerr;
    sac *s;
    char *out;
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) {
            return nerr;
        }
        asprintf(&out, "%s%s", s->m->filename, kstr);
        string_list_put(files, out, MCPFN + 1);
        FREE(out);
    }
    return 0;
}

int
files_prepend(string_list * files, char *kstr) {
    int i, nerr;
    sac *s;
    char *out;
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) {
            return nerr;
        }
        asprintf(&out, "%s%s", kstr, s->m->filename);
        string_list_put(files, out, MCPFN + 1);
        FREE(out);
    }
    return 0;
}

int
files_delete(string_list * files, char *kstr) {
    int i, nerr, n;
    sac *s;
    char *out, *p;
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) {
            return nerr;
        }
        out = strdup(s->m->filename);
        if ((p = strstr(out, kstr))) {
            n = strlen(p) - strlen(kstr);
            memmove(p, p + strlen(kstr), n);
            p[n] = 0;
        }
        string_list_put(files, out, MCPFN + 1);
        FREE(out);
    }
    return 0;
}

int
files_change(string_list * files, char *from, char *to) {
    int i, nerr, n;
    sac *s;
    char *out, *p;
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) {
            return nerr;
        }
        out =
            (char *) malloc(sizeof(char) *
                            (strlen(s->m->filename) + strlen(to) + 1));
        strcpy(out, s->m->filename);
        if ((p = strstr(out, from))) {
            n = strlen(p) - strlen(from) + strlen(to);
            memmove(p, p + strlen(from), n);
            memmove(p, to, strlen(to));
            p[n] = 0;
        }
        string_list_put(files, out, MCPFN + 1);
        FREE(out);
    }
    return 0;
}

/** 
 * Write a File to disk
 * 
 * @param lsdd 
 *    Set the Output to be a SDD file
 * @param nerr 
 *    Error Return Flag 
 *    - 0 on Success
 *
 * @date   970702:  Changed lckey and lkchar to lckeyExact and lkcharExact
 *                  throughout xw.c.  This will allow files to begin with 
 *                  the same string as the various options (eg. sacxz.021.z)
 *                  maf.
 * @date   910731:  Bug fixed in options PREPEND, DELETE, CHANGE.
 * @date   900904:  Added SDD as a format for write.
 * @date   881228:  Added four new options for generating write file list
 *                  from data file list: APPEND, PREPEND, CHANGE, DELETE.
 * @date   880204:  Fixed logic involving use of DIR option in READ and WRITE
 *                  by adding an ON/OFF flag as well as a directory name.
 * @date   880115:  Deleted call that forced default directory to lowercase.
 * @date   870626:  Added default directory option.
 * @date   860917:  Changed to character lists for storing data file names.
 * @date   850730:  Deleted SOCKITTOME  format.
 * @date   830120:  Added SOCK and CI output formats.
 * @date   820721:  Changed to newest set of parsing and checking functions.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   810203:  Fixed bug in file overwrite option.
 *
 */
void
xw(int lsdd, int *nerr) {

    int i;
    char kchange[MCPFN + 1];
    char kstring[MCPFN + 1];
    int lexpnd;
    int jdfl, nchar;
    int nstring;
    static int lwrdir = FALSE;
    sac *s;
    char *file, *pfile;
    string_list *list, *files;

    kschan[12] = '\0';
    kschdr[80] = '\0';
    ksclas[4] = '\0';
    kscom[40] = '\0';
    ksevnm[8] = '\0';
    ksfrmt[8] = '\0';
    ksstnm[8] = '\0';
    memset(kchange, 0, sizeof(kchange));
    memset(kstring, 0, sizeof(kstring));

    lexpnd = FALSE;

    *nerr = 0;

    files = string_list_init();
    list = NULL;

    if (lsdd)
        cmdfm.iwfmt = 3;

    /* PARSING PHASE: */
    /* - Loop on each token in command: */
    while (lcmore(nerr)) {

        /* -- "SAC|ALPHA":  set format to be used in writing files. */
        if (lckeyExact("SAC#$", 6))
            cmdfm.iwfmt = 1;

        else if (lckeyExact("ALPHA#$", 8))
            cmdfm.iwfmt = 2;

        else if (lckeyExact("CI#$", 5))
            cmdfm.iwfmt = 2;

        else if (lckeyExact("SDD#$", 6))
            cmdfm.iwfmt = 3;

        else if (lckeyExact("XDR#$", 6)) {
            cmdfm.iwfmt = 4;
        } else if (lckeyExact("SEGY#$", 7))
            cmdfm.iwfmt = 5;

        /* -- "OVER":  overwrite files from previous READ command. */
        else if (lckeyExact("OVER#$", 7)) {
            cmdfm.lovrrq = TRUE;
            lexpnd = FALSE;
            for (i = 0; i < saclen(); i++) {
                if (!(s = sacget(i, TRUE, nerr))) {
                    goto L_8888;
                }
                string_list_put(files, s->m->filename, -1);
            }
        }

        /* generate names from the KSTCMP header field */
        else if (lckeyExact("KSTCMP#$", 9)) {
            lexpnd = FALSE;
            gennames("KSTCMP ", 7, files, saclen(), nerr);
            if (*nerr != 0)
                goto L_8888;
        }

        /* -- "APPEND string": append string to filenames from READ command. */
        else if (lkcharExact
                 ("APPEND#$", 9, MCPFN, kstring, MCPFN + 1, &nstring)) {
            if ((*nerr = files_append(files, kstring))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "PREPEND string": prepend string to filenames from READ command. */
        else if (lkcharExact
                 ("PREPEND#$", 10, MCPFN, kstring, MCPFN + 1, &nstring)) {
            if ((*nerr = files_prepend(files, kstring))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "DELETE string": delete string from filenames from READ command. */
        else if (lkcharExact
                 ("DELETE#$", 9, MCPFN, kstring, MCPFN + 1, &nstring)) {
            if ((*nerr = files_delete(files, kstring))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "CHANGE string1 string2": change string1 to string2 in READ filenames. */
        else if (lkcharExact
                 ("CHANGE#$", 9, MCPFN, kstring, MCPFN + 1, &nstring)) {
            lcchar(kchange, sizeof(kchange));
            if ((*nerr = files_change(files, kstring, kchange))) {
                goto L_8888;
            }
            cmdfm.lovrrq = FALSE;
            lexpnd = TRUE;
        }

        /* -- "DIR ON|OFF|CURRENT|name":  set the name of the default subdirectory. */
        else if (lkcharExact
                 ("DIR#$", 6, MCPFN, kmdfm.kwrdir, MCPFN + 1, &nchar)) {
            lwrdir = set_output_path(kmdfm.kwrdir);
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

        /* -- "filelist":  write files using names in new filelist. */
        else if ((list = lcdfl())) {
            cmdfm.lovrrq = FALSE;
            lexpnd = FALSE;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }                           /* end while ( lcmore( nerr ) ) */

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */
    if (!list) {
        list = files;
    } else {
        /* List + Modifiers :: Use List */
        string_list_free(files);
        files = NULL;
    }

    /* - Check for null write filelist. */
    if (string_list_length(list) <= 0) {
        setmsg("ERROR", *nerr = 1311);
        goto L_8888;
    }

    /* - Make sure the write filelist has as many entries as read filelist. */

    if (string_list_length(list) != saclen()) {
        error(*nerr =
              1312, "%d, in memory: %d", string_list_length(list), saclen());
        goto L_8888;
    }

    /* EXECUTION PHASE: */

    /* - Commit or rollback data according to cmdfm.icomORroll */
    alignFiles(nerr);
    if (*nerr)
        return;

    /* - Echo expanded filelist if requested. */

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
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
        file = string_list_get(list, jdfl - 1);

        /* -- Check overwrite-protect flag in header record. */
        if (cmdfm.lovrrq && !s->h->lovrok) {
            *nerr = 1303;
            setmsg("ERROR", *nerr);
            apcmsg2(file, strlen(file) + 1);
            outmsg();
            clrmsg();
            goto L_8888;
        }

        pfile = prepare_output_filename(file, lwrdir, kmdfm.kwrdir);

        /* -- Write file in appropriate format. */
        if (cmdfm.iwfmt == 2) {
            wrci(jdfl, pfile, -1, "%#15.7g", nerr);
        } else if (cmdfm.iwfmt == 3)
            wrsdd(jdfl, pfile, -1, TRUE, nerr);

        else if (cmdfm.iwfmt == 4)
            wrxdr(jdfl, pfile, -1, TRUE, nerr);

        else if (cmdfm.iwfmt == 5)
            wrsegy(jdfl, pfile, nerr);

        else
            wrsac(jdfl, pfile, -1, TRUE, nerr);

        FREE(pfile);
        if (*nerr != 0)
            goto L_8888;

    }                           /* end for ( jdfl ) */
  L_8888:
    return;
}
