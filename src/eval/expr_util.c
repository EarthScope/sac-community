
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "expr_parse.h"
#include "string_utils.h"

#include "amf.h"
#include "bbs.h"
#include "vars.h"
#include "bot.h"
#include "hdr.h"
#include "lhf.h"
#include "dfm.h"
#include "dff.h"
#include "msg.h"
#include "ncpf.h"
#include "co.h"
#include "cpf.h"

CPF_EXTERN
LHF_EXTERN
BBS_EXTERN

char *unescape(char *in);
Token *token_dup(Token * t);

#define ESCAPE_CHAR '@'

#ifndef __TESTING__
#include "cpf.h"
#define KMACRONAME kmcpf.kvarsname
#else
#define KMACRONAME "12345678"
#endif

#define FREE(x) do { \
    if(x) {          \
      free(x);       \
      x = NULL;      \
    }                \
  } while(0);

#define TOKEN_LIST_APPEND(top, cur, new) do {   \
    if(cur) {                                   \
      cur->next = new;                          \
    } else {                                    \
      top = new;                                  \
    }                                           \
    cur = new;                                  \
  } while(0)


enum {
    SAC_HEADER_FLOAT_TYPE = 1,
    SAC_HEADER_INT_TYPE,
    SAC_HEADER_ENUM_TYPE,
    SAC_HEADER_LOGICAL_TYPE,
    SAC_HEADER_STRING_TYPE,
    SAC_HEADER_AUX_TYPE,
};

int is_reduced(int x);

#ifdef __TESTING__

char *
rstrip(char *s) {
    char *back;
    if (*s == 0) {
        return s;
    }
    back = s + strlen(s) - 1;
    while (isspace(*back)) {
        --back;
    }
    *(back + 1) = 0;
    return s;
}

void
getbbv(char *key, char *value, int *nerr, int key_s, int value_s) {
    UNUSED(key);
    UNUSED(key_s);
    UNUSED(value_s);
    UNUSED(nerr);
    strcpy(value, "{VAR}");
    value[strlen("{VAR}")] = 0;
}

void
gethv(char *key, int key_s, char *value, int value_s, int *nerr) {
    UNUSED(key);
    UNUSED(key_s);
    UNUSED(value_s);
    UNUSED(nerr);
    strcpy(value, "{VAR}");
    value[strlen("{VAR}")] = 0;
}

void
getvvstring(char *name, int name_s, char *key, int key_s, int *n, char *value,
            int value_s, int *nerr) {
    UNUSED(key);
    UNUSED(key_s);
    UNUSED(value_s);
    UNUSED(nerr);
    UNUSED(name);
    UNUSED(name_s);
    UNUSED(n);
    strcpy(value, "{VAR}");
    value[strlen("{VAR}")] = 0;
}

#endif

int lexer_line();
int lexer_col();

Token *token_new_value(double v);

void
parse_error(Token * value, int error) {
    value->error = error;
    value->col = lexer_col();
    value->line = lexer_line();
}

void
value_num(Token * value, double v) {
    if (value->error == TOKEN_STATUS_UNKNOWN) {
        value->type = EVAL_OUTPUT_TYPE_NUMBER;
        value->value = v;
        value->error = 0;
    }
}

void
show_error() {
    outmsg();
    clrmsg();
}

int
gettime_expr(Token * A, Token * B, int lmax, int lvalue) {
    int err;
    double val;
    if ((lvalue && B->type != NUM) ||
        (err = gettime(lmax, lvalue, (lvalue) ? B->value : 0.0, &val))) {
        show_error();
        return FALSE;
    }
    token_value(A, val, 0);
    return TRUE;
}

int
getval_expr(Token *A, Token *time, Token *fileno, int lfile) {
    int n,m1,m2, nerr, it, i;
    Token *new, *cur;
    double t,val;
    sac *s;
    new = NULL;
    cur = NULL;
    n = saclen();
    if(time->type != NUM) { /* Make sure time token is a number */
        error(3141, "time is not a number\n");
        show_error();
        return FALSE;
    }
    if(lfile) { /* If 'FILE n' option was given */
        if (fileno->type != NUM) {
            error(3141, "file_number is not a number\n");
            show_error();
            return FALSE;
        }
        if( !token_is_int(fileno) ) {
            error(3141,"file_number must be an integer, found %f\n", fileno->value);
            show_error();
            return FALSE;
        }
        m1 = token_as_int(fileno);
        if(m1 < 1 || m1 > n) {
            error(3141,"file_number must be between 1 and n [n = %d], given: %d\n", n,m1);
            show_error();
            return FALSE;
        }
        m2 = m1+1;
    } else {
        m1 = 1;
        m2 = n+1;
    }
    for(i = m1; i < m2; i++) {
        if(!(s = sacget(i-1, TRUE, &nerr))) {
            error(3141, "error retrieving sac data file\n");
            show_error();
            return FALSE;
        }
        t = time->value;
        it = round( (t - s->h->b)/s->h->delta );
        if(it < 0 || it > s->h->npts-1) {
            error(3141, "time outside of file range [%f %f] (%s)\n",
                  s->h->b, s->h->e, s->m->filename);
            show_error();
            return FALSE;
        }
        val = s->y[it];
        if(cur) {
            new = token_new_value(val);
            cur->next = new;
            cur = new;
        } else {
            token_value(A, val, 0);
            cur = A;
        }
    }
    return TRUE;
}

void
token_string_rstrip(Token * t, char *s, int col) {
    char *p;
    p = strdup(s);
    if (strncasecmp(p, SAC_CHAR_UNDEFINED, 8) != 0 &&
        strncasecmp(p, SAC_CHAR_UNDEFINED_2, 16) != 0) {
        p = rstrip(p);
    } else {
        p = strdup("UNDEFINED");
    }
    token_string(t, p, col);
}

int
header_to_token(char *str, Token * t, int col) {
    int n, nerr;
    char key[1000], val[41], name[1000];
    int id, icat, item, ok;
    sac *s;
    ok = 0;
    memset(val, ' ', 41);
    val[40] = 0;
    if (sscanf(str, "%d,%s%n", &id, key, &n) == 2 && n == (int) strlen(str)) {
        ok = TRUE;
    } else if (sscanf(str, "%[^,],%s%n", name, key, &n) == 2 &&
               n == (int) strlen(str)) {
        unescape(name);
        if ((id = sac_find_filename(name)) >= 0) {
            ok = TRUE;
            id++;
        } else {
            error(1363, name);
            outmsg();
            clrmsg();
        }
    } else {
        printf("unknown header format: %d '%s'\n",
               sscanf(str, "%s,%s%n", name, key, &n), name);
    }
    if (!ok) {
        return FALSE;
    }

    if (!(s = sacget(id - 1, FALSE, &nerr))) {
        //getfil(id, FALSE, &hdr, &x, &y, &nerr);
        setmsg("ERROR", nerr);
        outmsg();
        clrmsg();
        return FALSE;
    }
    key[8] = 0;
    hdrfld(key, 9, &icat, &item, &ok);
    if (!ok) {
        error(1337, key);
        outmsg();
        clrmsg();
        return FALSE;
    }

    switch (icat) {
        case SAC_HEADER_FLOAT_TYPE:
            token_value(t, VALUE(fhdr(s, item)), col);
            break;
        case SAC_HEADER_INT_TYPE:
            token_value(t, (int) VALUE(nhdr(s, item)), col);
            break;
        case SAC_HEADER_ENUM_TYPE:
            token_string_rstrip(t,
                                kmlhf.kdiv[(int) VALUE(ihdr(s, item - 1)) - 1],
                                col);
            break;
        case SAC_HEADER_LOGICAL_TYPE:
            token_value(t, (int) VALUE(lhdr(s, item)), col);
            break;
        case SAC_HEADER_STRING_TYPE:{
                n = (item == 2) ? 2 : 1;
                memmove(val, khdr(s, item), n * 8);
                val[n * 8] = 0;
                token_string_rstrip(t, val, col);
            }
            break;
        case SAC_HEADER_AUX_TYPE:
            lgahdr(key, 9, val, 41);
            token_string_rstrip(t, val, col);
            break;
        default:
            return FALSE;
    }
    return TRUE;

}

#define SPACE ' '
#define SINGLE '\''
#define DOUBLE '"'
#define ESCAPE '@'

void
unescape_inline(char *str) {
    char *in, *out;
    out = in = str;
    while (in && *in) {
        *out = *in;
        if (*out == ESCAPE) {
            switch (*(out + 1)) {
                case ' ':
                case '@':
                case ';':
                case '\'':
                case '(':
                case ')':
                case '&':
                case '$':
                case '%':
                    in++;
                    *out = *in;
                    break;
                default:
                    break;
            }
        }
        out++;
        in++;
    }
    *out = 0;
}

Token *
string_to_token(char *b, char *e) {
    char *p;
    p = b;
    /* Handle enclosing quotes */
    if (*b == SINGLE || *b == DOUBLE) {
        if (*(e - 1) != *b) {
            /* Quote unclosed */
            return NULL;
        }
        *b = *(e - 1) = 0;      /* Remove Quotes */
        p = b + 1;
    }
    *e = 0;
    if (isnumx(p)) {
        return token_new_value(atof(p));
    }
    if ((strchr(p, ESCAPE))) {
        unescape_inline(p);
    }
    return token_new_string(p);
}

Token *
token_to_token_list(Token * t) {
    Token *list, *new;
    list = new = NULL;

    /* If this is already a list, simply copy it */
    if (t->next) {
        list = token_deep_copy(t);
    } else {
        /* Otherwise convert it */
        switch (t->type) {
            case QUOTED_STRING:
            case ESCAPE_STRING:
                list = token_deep_copy(t);
                break;
            case STRING:
                list = string_to_token_list(t->str);
                break;
            case NUM:
                list = token_dup(t);
                list->next = NULL;
                break;
            default:
                fprintf(stderr,
                        "unknown token type while converting to list\n");
        }
    }
    return list;
}

Token *
string_to_token_list(char *in) {
    Token *t, *cur, *new;
    char *b, *p, *p0;
    int escape;
    char quote;

    p = p0 = strdup(in);
    t = NULL;
    cur = NULL;
    new = NULL;
    quote = 0;
    escape = 0;
    b = p;
    if (p && *p == 0) {
        new = token_new_string("");
        TOKEN_LIST_APPEND(t, cur, new);
    }
    while (p && *p) {
        switch (*p) {
            case DOUBLE:
            case SINGLE:
                if (!escape) {
                    if (!quote || quote == *p) {
                        quote = (quote) ? 0 : *p;
                    }
                }
                break;
            case ESCAPE:
                escape = 1;
                break;
            case SPACE:
                if (b == p) {
                    b++;        /* SPACES */
                } else if (!quote && !escape) {
                    if ((new = string_to_token(b, p)) == NULL) {
                        return NULL;
                    }
                    TOKEN_LIST_APPEND(t, cur, new);
                    b = p + 1;
                }
                break;
            default:
                break;
        }
        escape = (*p == ESCAPE);
        p++;
    }
    if (b < p) {
        if ((new = string_to_token(b, p)) == NULL) {
            return NULL;
        }
        TOKEN_LIST_APPEND(t, cur, new);
    }
    free(p0);
    return t;
}

int
macro_variable_no_value(char *k) {
    Token *t, *new, *cur;
    char *prmt;
    char msg[4096];
    char *p;

    p = upcase_dup(k);

    memset(msg, 0, sizeof(msg));
    asprintf(&prmt, "%s?  $", k);
    zgpmsg(prmt, 0, msg, 4095);

    t = string_to_token_list(msg);
    if (!t) {
        new = cur = NULL;
        if (!(new = token_new_string(""))) {
            return 0;
        }
        TOKEN_LIST_APPEND(t, cur, new);
    }
    if(!sac_vars_put_var(KMACRONAME, p, VAR_LIST, t)) {
        FREE(p);
        FREE(prmt);
        return 0;
    }
    FREE(p);
    FREE(prmt);
    return 1;
}

int
token_var(Token * a, int type, char *key, int col) {
    char *k, *p, c;
    var *v;
    int retval;

    switch (type) {
        case BLACKBOARD:
            c = '%';
            break;
        case HEADER:
            c = '&';
            break;
        case VARIABLE:
            c = '$';
            break;
        default:
            return FALSE;
    }

    p = (*key == c) ? key + 1 : key;
    asprintf(&k, "%s", p);
    if ((p = strrchr(k, c))) {
        *p = 0;
    }

  AGAIN:
    v = NULL;
    retval = FALSE;

    switch (type) {
        case HEADER:
            retval = header_to_token(k, a, col);
            break;
        case BLACKBOARD:
            if (!(v = getbb(k))) {
                error(1201, k);
                outmsg();
                clrmsg();
            }
            break;
        case VARIABLE:
            p = upcase_dup(k);
            v = sac_vars_get_var(KMACRONAME, p);

            if (!v ||
                (v && v->type == VAR_STRING &&
                 strcmp(v->str, kmcpf.knoval) == 0)) {
                if (macro_variable_no_value(k)) {
                    goto AGAIN;
                }
            }
            free(p);
            break;
    }
    /* Deallocate token's string and replace with value 
       Normally the string value is passed and kept in the token
       and released after the command is done.
       Here the token string is replaced and need to be deallocated directly
       BAM !!! 
     */
    if (v) {
        retval = TRUE;
        switch (v->type) {
            case VAR_VALUE:
                token_value(a, v->value, col);
                break;
            case VAR_STRING:
                token_string(a, strdup(v->str), col);
                break;
            case VAR_INTEGER:
                token_int(a, v->ival, col);
                break;
            case VAR_LIST:{
                    Token *b = token_deep_copy(v->list);
                    token_copy(a, b);
                }
                break;
            default:
                retval = FALSE;
                break;
        }
    }
    /* Force Quoted strings for variables , <= 101.5 behavior */
    if (v && v->type == VAR_LIST && type == VARIABLE) {
        Token *tmp;
        tmp = a;
        while (tmp) {
            if (tmp->type == STRING && index(tmp->str, ' ')) {
                tmp->type = QUOTED_STRING;
            }
            tmp = tmp->next;
        }
    }

    FREE(k);
    if (retval) {
        FREE(key);              /* Should this be here if an error occurs */
    }
    return retval;
}

char *
replace(char *t, char k) {
    int n, nerr;
    char *s, *e, *p, v[1001], key[1001];
    char *key_cap;
    size_t m = 0;
    p = t;
    memset(v, 0, 1001);
    while (p && (s = strchr(p, k))) {
        if (s > p && *(s - 1) == ESCAPE_CHAR) {
            p = s + 1;
            continue;
        }
        is_reduced(TRUE);
        memset(key, 0, 1001);
        e = s + 1;
        while (e && *e && (*e != k && (*e != ' ' || *(e - 1) == ESCAPE_CHAR))) {
            e++;
        }

        strncpy(key, s + 1, e - s);
        if (*e == k) {          /* Change character to space */
            if (key[e - s - 1] == k) {
                key[e - s - 1] = ' ';
            }
            e++;
        }
        m = strlen(key);
        if (m > 0 && key[m - 1] != ' ') { /* Append space if needed */
            key[m + 1] = 0;
            key[m] = ' ';
        }
        switch (k) {
            case '$':
              REPLACE_AGAIN:
                if (key[strlen(key) - 1] == ' ') {
                    key[strlen(key) - 1] = 0;
                }
                key_cap = upcase_dup(key);
                getvvstring(KMACRONAME, 9, key_cap, strlen(key_cap), &n, v,
                            1001, &nerr);
                FREE(key_cap);
                if (nerr || strcmp(v, kmcpf.knoval) == 0) {
                    if (macro_variable_no_value(key)) {
                        goto REPLACE_AGAIN;
                    }
                }
                break;
            case '%':{
                    Token a;
                    char *tmp;
                    rstrip(&key[0]);
                    if (!token_var(&a, BLACKBOARD, strdup(key), 0)) {
                        return NULL;
                    }
                    tmp = token_as_string(&a);
                    strcpy(v, tmp);
                    FREE(tmp);
                }
                break;
            case '&':{
                    Token a;
                    char *tmp;
                    rstrip(key);
                    if (!header_to_token(key, &a, 0)) {
                        return NULL;
                    }
                    if ((token_is_number(&a) && a.value == -12345.0) ||
                        (token_is_string(&a) &&
                         strcmp(a.str, "-12345  ") == 0)) {
                        snprintf(&v[0], 1001, "UNDEFINED");
                    } else {
                        tmp = token_as_string(&a);
                        strcpy(v, tmp);
                        FREE(tmp);
                    }
                }
                break;
        }
        rstrip(v);
        if ((int) strlen(v) > (e - s)) {
            int ns, ne, np, nt;
            ns = s - t;
            ne = e - t;
            np = p - t;
            nt = strlen(t) + 1 + strlen(v) - (e - s);
            char *tmp = (char *) realloc(t, sizeof(char) * nt);
            if (!tmp) {
                fprintf(stderr, "error increasing string buffer\n");
                return NULL;
            }
            s = tmp + ns;
            p = tmp + np;
            e = tmp + ne;
            t = tmp;
        }
        n = strlen(e);
        memmove(s + strlen(v), e, n);
        strncpy(s, v, strlen(v));
        s[strlen(v) + n] = 0;
        p = s + strlen(v);
    }
    return t;
}

void
token_string(Token * t, char *str, int col) {
    t->type = STRING;
    t->str = str;
    t->col = col;
}

void
token_value(Token * t, double v, int col) {
    t->type = NUM;
    t->value = v;
    t->col = col;
}

void
token_int(Token * t, int v, int col) {
    t->type = NUM;
    t->value = v;
    t->col = col;
}

int
token_is_equals(Token * t) {
    return (t && t->type == EQUALS);
}

int
token_is_comma(Token * t) {
    return (t && t->type == COMMA);
}

int
is_string_like(int type) {
    return (type == STRING || type == QUOTED_STRING || type == ESCAPE_STRING ||
            type == WILD || type == HEADER || type == VARIABLE ||
            type == BLACKBOARD);
}

int
token_is_string(Token * t) {
    return t->type == STRING;
}

int
token_is_number(Token * t) {
    return t->type == NUM;
}

int
token_is_int_precision(Token * t, double precision) {
    return token_is_number(t) &&
        (fabs(round(t->value) - t->value) <= precision);
}

int
token_is_int(Token * t) {
    return token_is_int_precision(t, 1e-4);
}

int
token_as_int(Token * t) {
    return (int) round(t->value);
}

Token *
token_new_value(double v) {
    return token_new(NUM, v, NULL, 0, 0);
}

Token *
token_new_int(int i) {
    return token_new(NUM, (double) i, NULL, 0, 0);
}

Token *
token_new_string(char *s) {
    return token_new(STRING, 0, s, 0, 0);
}

int
isnumx(char *s) {
    char *e;
    if (*s == 0 || *s == ' ') {
        return 0;
    }
    e = NULL;
    strtod(s, &e);
    if (!e || *e == 0) {
        return 1;
    }
    return 0;
}

int
isnum(char *s) {
    if (*s == '-' || *s == '+') {
        return isnumx(s);
    }
    return 0;
}

void
token_copy(Token * a, Token * b) {
    a->type = b->type;
    a->value = b->value;
    a->str = b->str;
    a->next = b->next;
    a->line = b->line;
    a->col = b->col;
}

/* 
   A       = B
   A->next = C
 */
void
token_append(Token * a, Token * b, Token * c) {
    Token *d = token_new(0, 0, NULL, 0, 0);
    token_copy(d, c);
    token_copy(a, b);
    token_last(a)->next = d;
}

Token *
token_get_prev(Token * head, Token * t) {
    Token *tmp, *last;
    tmp = head;
    last = NULL;
    while (tmp && tmp != t) {
        last = tmp;
        tmp = tmp->next;
    }
    if (tmp != t) {
        return NULL;
    }
    return last;
}

Token *
token_insert_before(Token * head, Token * t, Token * new) {
    Token *last;
    if (!head || !t) {
        return head;
    }
    last = token_get_prev(head, t);
    if (!last) {
        if (head != t) {        /* Not found */
            return head;
        }
        new->next = head;       /* First item */
        head = new;
    } else {                    /* In the list */
        last->next = new;
        new->next = t;
    }
    return head;
}

Token *
token_remove(Token * head, Token * t) {
    Token *last;
    if (!head || !t) {
        return head;
    }
    last = token_get_prev(head, t);
    if (!last) {
        if (head != t) {        /* Not found */
            return head;
        }
        head = t->next;         /* First item */
    } else {                    /* In the list */
        last->next = t->next;
    }
    t->next = NULL;
    token_free(t);
    return head;
}

char *
token_as_string(Token * p) {
    char *s = NULL;
    switch (p->type) {
        case WILD:
        case HEADER:
        case VARIABLE:
        case BLACKBOARD:
        case QUOTED_STRING:
        case ESCAPE_STRING:
        case STRING:
            asprintf(&s, "%s", p->str);
            break;
        case EQUALS:
            asprintf(&s, "EQ");
            break;
        case COMMA:
            asprintf(&s, "[,]");
            break;
        case NUM:
            if (token_is_int_precision(p, TOKEN_INT_PRECISION_NON_ARGUMENT)) {
                asprintf(&s, "%d", token_as_int(p));
            } else {
                asprintf(&s, float_format(), p->value);
            }
            break;
        case EQ:
            asprintf(&s, "EQ");
            break;
        case NE:
            asprintf(&s, "NE");
            break;
        case GE:
            asprintf(&s, "GE");
            break;
        case LE:
            asprintf(&s, "LE");
            break;
        case GT:
            asprintf(&s, "GT");
            break;
        case LT:
            asprintf(&s, "LT");
            break;
        default:
            asprintf(&s, "%d", p->type);
            break;
    }
    return s;
}

char *
token_to_string(Token * t) {
    char *out;
    Token *p;
    string *s = string_new(" ");
    p = t;
    while (p) {
        switch (p->type) {
            case WILD:
            case HEADER:
            case VARIABLE:
            case BLACKBOARD:
            case QUOTED_STRING:
            case ESCAPE_STRING:
            case STRING:
                string_printf_append(s, "%s, ", p->str);
                break;
            case EQUALS:
                string_printf_append(s, "EQ, ");
                break;
            case COMMA:
                string_printf_append(s, "[,], ");
                break;
            case NUM:
                string_printf_append(s, "%f, ", p->value);
                break;
            case EQ:
                string_printf_append(s, "EQ, ");
                break;
            case NE:
                string_printf_append(s, "NE, ");
                break;
            case GE:
                string_printf_append(s, "GE, ");
                break;
            case LE:
                string_printf_append(s, "LE, ");
                break;
            case GT:
                string_printf_append(s, "GT, ");
                break;
            case LT:
                string_printf_append(s, "LT, ");
                break;
            default:
                string_printf_append(s, "%d, ", p->type);
                break;
        }
        p = p->next;
    }
    out = strdup(string_string(s));
    string_free(&s);
    return out;
}


int
token_is_quoted_string(Token * t) {
    return (t && t->type == QUOTED_STRING);
}

int
token_is_escape_string(Token * t) {
    return (t && t->type == ESCAPE_STRING);
}

Token *
token_dup(Token * t) {
    Token *p;
    p = token_new(0, 0, NULL, 0, 0);
    token_copy(p, t);
    if (p->str) {
        p->str = strdup(p->str);
    }
    return p;
}

Token *
token_deep_copy(Token * t) {
    Token *start, *last, *new, *p;
    start = last = token_dup(t);
    p = t->next;
    while (p) {
        new = token_dup(p);
        last->next = new;
        last = new;
        p = p->next;
    }
    return start;
}

int
token_is_eq(Token * t) {
    return t->type == EQ;
}

int
token_is_ne(Token * t) {
    return t->type == NE;
}

int
token_is_gt(Token * t) {
    return t->type == GT;
}

int
token_is_lt(Token * t) {
    return t->type == LT;
}

int
token_is_ge(Token * t) {
    return t->type == GE;
}

int
token_is_le(Token * t) {
    return t->type == LE;
}

int
token_strncasecmp(Token * t, char *s, size_t n) {
    return token_is_string(t) && strncasecmp(t->str, s, n) == 0;
}
int
token_to_var(Token * tok, char *group, char *name) {
    if (!tok || !name || !group) {
        return FALSE;
    }
    if (token_is_string(tok) || token_is_quoted_string(tok) ||
        token_is_escape_string(tok)) {
        setvar(group, name, VAR_STRING, tok->str);
    } else if (token_is_int_precision(tok, TOKEN_INT_PRECISION_NON_ARGUMENT)) {
        setvar(group, name, VAR_INTEGER, token_as_int(tok));
    } else if (token_is_number(tok)) {
        setvar(group, name, VAR_VALUE, tok->value);
    } else {
        return FALSE;
    }
    return TRUE;
}

int
token_to_bb(Token * tok, char *name) {
    return token_to_var(tok, kmbbs.knmbbs, name);
}
