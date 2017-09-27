/** 
 * @file   getvvstring.c
 * 
 * @brief  Get a string vars value 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "amf.h"
#include "msg.h"
#include "bbs.h"
#include "vars.h"
#include "co.h"
#include "bot.h"
#include "errors.h"
#include "debug.h"
#include "token.h"
#include "eval/expr_parse.h"

/** 
 * Get a vars value of type string
 * 
 * @param vars 
 *    Name of vars list
 * @param vars_s 
 *    Length of \p vars
 * @param name 
 *    Name of vars entry
 * @param name_s 
 *    Length of \p name
 * @param numchars 
 *    Number of characters in output string \p value
 * @param value 
 *    Requested Output string 
 * @param value_s 
 *    Length of \p value
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   881107:  Changed name of subroutine.
 * @date   870302:  Blank filled value field before returning.
 * @date   861229:  Original version.
 * @date   881107:  Documented/Reviewed
 *
 */
void
getvvstring(char *vars, int vars_s, char *name, int name_s, int *numchars,
            char *value, int value_s, int *nerr) {
    char *s1;
    char *s2;
    var *v;
    *nerr = 0;
    s2 = NULL;
    UNUSED(vars_s);
    UNUSED(name_s);
    s2 = NULL;
    s1 = upcase_dup(name);
    if (!(v = sac_vars_get_var(vars, s1))) {
        *nerr = ERROR_FINDING_VARIABLE;
        return;
    }
    switch (v->type) {
        case VAR_STRING:
            s2 = strdup(v->str);
            break;
        case VAR_VALUE:
            asprintf(&s2, "%lf", v->value);
            break;
        case VAR_INTEGER:
            asprintf(&s2, "%d", v->ival);
            break;
        case VAR_LIST:
            s2 = token_to_line(v->list);
            break;
        default:
            break;
    }
    *numchars = min(value_s, (int) strlen(s2));
    strncpy(value, s2, *numchars);
    value[*numchars] = 0;
    free(s2);
    s2 = NULL;
    free(s1);
    s1 = NULL;
    return;
}

char *
getvvstringZ(char *vars, int vars_s, char *name, int name_s, int *numchars,
             int *nerr) {

    char *s1;
    char *s2;
    *nerr = 0;
    s2 = NULL;
    UNUSED(vars_s);
    UNUSED(name_s);
    s1 = upcase_dup(name);
    sac_vars_get_string(vars, s1, &s2);
    *numchars = strlen(s2);
    free(s1);
    return s2;
}

char *
token_to_line(Token * t) {
    char *out;
    Token *p;
    string *s = string_new("");
    p = t;
    while (p) {
        switch (p->type) {
            case WILD:
            case HEADER:
            case VARIABLE:
            case BLACKBOARD:
            case STRING:
                string_printf_append(s, "%s ", p->str);
                break;
            case ESCAPE_STRING:
                string_printf_append(s, "%s ", p->str);
                break;
            case QUOTED_STRING:
                string_printf_append(s, "\"%s\" ", p->str);
                break;
            case EQUALS:
                string_printf_append(s, "= ");
                break;
            case COMMA:
                string_printf_append(s, ", ");
                break;
            case NUM:
                if (floor(p->value) == p->value) {
                    string_printf_append(s, "%d ", (int) p->value);
                } else {
                    char fmt[256];
                    sprintf(fmt, "%s ", float_format());
                    string_printf_append(s, fmt, p->value);
                }
                break;
            case EQ:
            case NE:
            case GE:
            case LE:
            case GT:
            case LT:
                string_printf_append(s, "%s ", p->str);
                break;
            default:
                string_printf_append(s, "[%d?] ", p->type);
                break;
        }
        p = p->next;
    }
    out = strdup(string_string(s));
    string_free(&s);
    return out;
}

char *
upcase_dup(char *s) {
    char *p, *new;
    p = new = strdup(s);
    while (*p) {
        *p = toupper(*p);
        p++;
    }
    return new;
}

char *
float_format() {
    char *fmts[] = {"%g",       "%.4f",  "%.15f", "%.5g",   "%.15g", "%#.4e",  "%#.15e"};
    char *names[] = {"default", "short", "long",   "shortG", "longG", "shortE", "longE"};

    var *vname;
    char *name = NULL;
    char *fmt = NULL;

    if(!(vname = getbb("OUTPUT_FORMAT"))) {
        return fmts[0];
    } else {
        if(vname->type != VAR_STRING) {
            fprintf(stderr, "Unknown output format\n");
            return fmts[0];
        } else {
            name = vname->str;
        }
    }
    for(int i = 0; i < sizeof(names)/sizeof(char *); i++) {
        if(strcasecmp(name, names[i]) == 0) {
            fmt = fmts[i];
        }
    }
    if(fmt == NULL) {
        return fmts[0];
    }
    return fmt;
}
