/** 
 * @file   saccommands.c
 * 
 * @brief  Process sac commands
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "config.h"
#include "amf.h"
#include "top.h"
#include "dfm.h"
#include "cnv.h"
#include "exm.h"
#include "bool.h"
#include "com.h"

#include "bot.h"
#include "vars.h"
#include "ucf.h"
#include "ssi.h"
#include "msg.h"
#include "bbs.h"
#include "cpf.h"
#include "co.h"
#include "debug.h"

COM_EXTERN
EXM_EXTERN

#define SAC_BLACKBOARD_SACNFILES         "SACNFILES"

#include "token.h"
#include "debug.h"

static Token *tok0 = NULL;      /* First token   */
static Token *tok = NULL;       /* Current token */
static eval *e0 = NULL;

Token *
arg() {
    return tok;
}

void
arg_reset() {
    if (tok0) {
        token_free(tok0);
        tok0 = NULL;
    }
}

void
arg_next() {
    tok = tok->next;
}

void
arg_prev() {
    Token *p = tok0;            /* Global first token */
    while (p && p->next != tok) {
        p = p->next;
    }
    tok = p;
}

void
arg_end() {
    Token *p = tok0;            /* Global first token */
    while (p && p->next) {
        p = p->next;
    }
    tok = p;
}

void
arg_change(char *str) {
    arg_delete();
    arg_insert(str);
}

void
arg_delete() {
    Token *tmp = tok->next;
    tok0 = token_remove(tok0, tok);
    tok = tmp;
}

void
arg_truncate() {
    while (tok) {
        arg_delete();
    }
    arg_end();
}

void
arg_append(char *str) {
    Token *new = token_new_string(str);
    arg_end();
    tok->next = new;
    tok = new;
}

void
arg_insert(char *str) {
    Token *new = token_new_string(str);
    if (new) {
        tok0 = token_insert_before(tok0, tok, new);
        tok = new;
    }
}

Token *
arg_begin() {
    return tok0;
}

char *
strchr_quote(char *p) {
    char quote;
    int escape;
    char *k;

#define SEMI ';'
#define SINGLE '\''
#define DOUBLE '"'
#define ESCAPE '\\'

    if (!strchr(p, ';')) {
        return NULL;
    }

    k = p;
    quote = 0;
    escape = 0;
    while (k && *k && (quote || *k != SEMI || (escape && *k == SEMI))) {
        if (escape) {
            escape = 0;
        } else {
            switch (*k) {
                case DOUBLE:
                case SINGLE:
                    if (!quote || quote == *k) {
                        quote = (quote) ? 0 : *k;
                    }
                    break;
                case ESCAPE:
                    escape = 1;
                    break;
            }
        }
        k++;
    }
    return k;
}

void
saccommands_cleanup(eval * e) {
    if (!e) {
        e = e0;
    }
    if(e) {
        eval_free(e);
    }
    e0 = NULL;
    if (tok0) {
        token_free(tok0);
        tok0 = NULL;
    }
}

char *
char_repeat(char *in, char c) {
    char *p, *t, *out;
    UNUSED(c);
    t = out = (char *) malloc(sizeof(char) * ((2 * strlen(in)) + 1));
    p = in;
    while (p && *p) {
        if (*p == '%') {
            *t = '%';
            t++;
        }
        *t = *p;
        t++;
        p++;
    }
    *t = 0;
    return out;
}

eval *
tokenize_line(char *in) {
    eval *e;
    e = eval_new();
    eval_input(e, in);
    arg_reset();
    tok = tok0 = parse(e);
    if (e->status == EVAL_STATUS_SYNTAX_ERROR) {
        if (e->error_message) {
            bell();
            fprintf(stdout, "%s", e->error_message);
        }
        token_free(tok0);
        tok0 = NULL;
        eval_free(e);
        return NULL;            //goto L_8888;
    }
    if (e->processed) {
        char *str = token_to_line(tok0);
        if (strchr(str, '%')) {
            char *out = char_repeat(str, '%');
            FREE(str);
            str = out;
        }
        processed(99, str);
        free(str);
    }
    return e;
}

char *
process_line(char *in) {
    char *s;
    eval *e;
    e = tokenize_line(in);
    if (!e) {
        return NULL;
    }
    s = token_to_line(tok0);
    eval_free(e);
    arg_reset();
    return s;
}

/** 
 * Set the number of files to the blackboard variable SACNFILES
 *
 * Post the number of files to the black board before returning.
 * gets saclen() in ascii to post SACNFILES 
 *
 * @param nerr 
 *    Error Return Code
 *    - 0 on Succes
 *    - Non-Zero on Error
 * 
 * @see setbbv
 *
 * @date July 24, 2008 - Isolated from the saccommands function <savage@uri.edu>
 * @date October 10, 1996 - Original Version. MAF
 */
void
sac_report_files_in_memory(int *nerr) {
    UNUSED(nerr);
    setbb(SAC_BLACKBOARD_SACNFILES, VAR_INTEGER, saclen());
}

/** 
 * Execute one or more SAC commands in a message
 * 
 * - If a command exists, continue, otherwise exit 
 * - Convert command name to upper case -- modcase()
 * - Echo command if requested -- wrcom()
 * - Find the command module and index number -- findcommand()
 * - Execute command if the command was found -- executecommand()
 * - If not found, assume it is a system command -- zsysop()
 * - Loop until the command stack is empty
 * - Report the number of sac files in memory
 *
 * @param kinmsg 
 *    Message to execute containing SAC commands
 * @param kinmsg_s 
 *    Length of message \p kinmsg
 * @param nerr 
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   961010:  Posts number of files to the black board after each
 *                  command. maf
 * @date   910624:  Changed error handling logic around after executecommand
 *                  to get the system command error to print (wct).
 * @date   891005:  Deleted "itpmsg" argument and changed "nerr" argument.
 * @date   890110:  Added call to "tracereport".
 * @date   870722:  Modified ISTAT output to include execution error reporting.
 * @date   860206:  Added call to REPERR to report current error status.
 * @date   820901:  Major modification due to new command parsing logic.
 * @date   811125:  Added call to ZSHSG.
 * @date   810429:  Original version.
 *
 */
void
saccommands(char *kinmsg, int kinmsg_s, int *nerr) {
    char kcommand[30] = "        ";
    int lfound;
    int index, module;
    char *temp;
    int n;
    char *p, *p1, *in;
    eval *e;

    UNUSED(kinmsg_s);
    *nerr = 0;
    kcommand[0] = '\0';         /* fix an access violation, maf 980507 */

    /* Comment as First '*' Character on Line */
    if (kinmsg && *kinmsg == '*') {
        if (cmexm.lecho) {
            wrcom();
        }
        return;
    }

    if (SeisMgrCode(kinmsg, nerr))
        goto L_8888;

    p = rstrip(kinmsg);

    if (p && !*p) {
        processed(99, p);
    }

    while (p && *p) {
        if ((p1 = strchr_quote(p))) {
            n = p1 - p;
        } else {
            n = strlen(p);
        }
        if(n < 1) {
            p = (p1) ? p1 + 1 : NULL;
            continue;
        }
        in = strcut(p, 1, n);
        if(!in) {
            p = (p1) ? p1 + 1 : NULL;
            continue;
        }
        e0 = e = tokenize_line(in);
        free(in);
        if (!e) {
            *nerr = 101;
            return;
        }

        /* -- Get command name and convert to uppercase. */
        lcchar(kcommand, sizeof(kcommand));
        modcase(TRUE, kcommand, strlen(kcommand), kcommand);

        /* -- Echo command if requested. */
        if (cmexm.lecho && strcmp(kcommand, "ECHO") != 0)
            wrcom();
        //printf("com: '%s'\n", kcommand);
        /* -- Validate command name and find module and index number. */
        findcommand(kcommand, &lfound, &module, &index);

        /* -- If valid, execute command.
         *    Report any errors. */
        if (lfound) {
            executecommand(module, index, nerr);
            reperr(*nerr);
            if (*nerr != 0) {
                setmsg("ERROR", *nerr);
                proerr(nerr);
            }
            if (cmexm.ntraces > 0)
                tracereport(nerr);
        } else {
            if (strlen(kinmsg) > 0) {
                temp = kinmsg;
                while ((*temp == ' ') || (*temp == '\t'))
                    temp++;
                /* make sure that the first char is not something like *, and */
                /* disable the dangerous rm command.                          */
                if (isalpha((int) *temp) || (*temp == '/')) {
                    if (strncmp(temp, "rm ", 3) != 0) {
                        arg_prev();
                        xsystemcommand(nerr);
                    } else {
                        *nerr = 1106;
                    }
                }
            }
            if (*nerr != 0) {
                *nerr = 1106;
                setmsg("ERROR", *nerr);
                reperr(*nerr);
                if (*nerr != 0)
                    proerr(nerr);
            }
            goto L_8888;
        }
        p = (p1) ? p1 + 1 : NULL;

        saccommands_cleanup(e);
        /* Reset Error Condition */
        cmcom.ncerr = 0;

    }

  L_8888:
    sac_report_files_in_memory(nerr);
    return;
}
