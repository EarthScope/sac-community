
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "vars.h"
#include "chash.h"
#include "bool.h"
#include "bbf.h"
#include "bot.h"
#include "debug.h"

static dict *sac_vars = NULL;

#define FREE(x) do {  \
  if(x) {             \
    free(x);          \
    x = NULL;         \
  }                   \
} while(0);

var *
var_init(var * v) {
    if (v) {
        v->type = VAR_UNKNOWN;
        v->name = NULL;
        v->value = 0.0;
        v->str = NULL;
        memset(v->flag, 0, sizeof(int) * 7);
    }
    return v;
}

var *
var_alloc() {
    return (var *) malloc(sizeof(var));
}

var *
var_new_value(char *name, double value) {
    var *v;
    if ((v = var_init(var_alloc()))) {
        v->type = VAR_VALUE;
        v->name = strdup(name);
        v->value = value;
    }
    return v;
}

var *
var_new_string(char *name, char *string) {
    var *v;
    if ((v = var_init(var_alloc()))) {
        v->type = VAR_STRING;
        v->name = strdup(name);
        v->str = strdup(string);
    }
    return v;
}

var *
var_new_int(char *name, int i) {
    var *v;
    if ((v = var_init(var_alloc()))) {
        v->type = VAR_INTEGER;
        v->name = strdup(name);
        v->ival = i;
    }
    return v;
}

var *
var_new_list(char *name, Token * t) {
    var *v;
    if ((v = var_init(var_alloc()))) {
        v->type = VAR_LIST;
        v->name = strdup(name);
        v->list = t;
    }
    return v;
}

var *
var_new_internal(char *name, int type, va_list ap) {
    var *v = NULL;
    switch (type) {
        case VAR_VALUE:
            v = var_new_value(name, va_arg(ap, double));
            break;
        case VAR_STRING:
            v = var_new_string(name, va_arg(ap, char *));
            break;
        case VAR_INTEGER:
            v = var_new_int(name, va_arg(ap, int));
            break;
        case VAR_LIST:
            v = var_new_list(name, va_arg(ap, Token *));
            break;
    }
    return v;
}

var *
var_new(char *name, int type, ...) {
    var *v;
    va_list ap;
    va_start(ap, type);
    v = var_new_internal(name, type, ap);
    va_end(ap);
    return v;
}

void
var_free(void *p) {
    var *v = (var *) p;
    if (v) {
        FREE(v->str);
        FREE(v->name);
        FREE(v);
    }
}

void
sac_vars_init() {
    if (!sac_vars) {
        sac_vars = dict_new();
    }
}

void
dict_free_var(void *p) {
    dict_free((dict *) p, var_free);
}

void
sac_vars_free() {
    if (sac_vars) {
        dict_free(sac_vars, dict_free_var);
    }
    sac_vars = NULL;
}

dict *
sac_vars_get_internal(char *group, int file) {
    dict *d;
    if ((d = dict_get(sac_vars, group))) {
        return d;
    }
    if (file) {
        sac_vars_read(group);
        if ((d = dict_get(sac_vars, group))) {
            return d;
        }
    }
    return NULL;
}

dict *
sac_vars_get(char *group) {
    return sac_vars_get_internal(group, TRUE);
}

int
sac_vars_exists(char *group) {
    return (sac_vars_get(group) != NULL);
}

int
sac_vars_create(char *group) {
    dict *d;
    if (!group) {
        return 0;
    }
    if (dict_get(sac_vars, group)) {
        fprintf(stdout, "variable group already exists: '%s'\n", group);
        return 0;
    }
    d = dict_new();
    return dict_put(sac_vars, group, d);
}

var *
sac_vars_get_var(char *group, char *name) {
    var *v;
    dict *d;
    if (!(d = sac_vars_get(group))) {
        return NULL;
    }
    if (!(v = dict_get(d, name))) {
        DEBUG("sac_vars: variable does not exist: '%s'\n", name);
        return NULL;
    }
    return v;
}

int
sac_vars_get_value(char *group, char *name, double *val) {
    var *v;
    if (!(v = sac_vars_get_var(group, name)) || (v->type != VAR_VALUE)) {
        return 0;
    }
    *val = v->value;
    return 1;
}

int
sac_vars_get_string(char *group, char *name, char **val) {
    var *v;
    if (!(v = sac_vars_get_var(group, name)) || (v->type != VAR_STRING)) {
        return 0;
    }
    *val = strdup(v->str);
    return 1;
}

int
sac_vars_get_int(char *group, char *name, int *i) {
    var *v;
    if (!(v = sac_vars_get_var(group, name)) || (v->type != VAR_STRING)) {
        return 0;
    }
    *i = v->ival;
    return 1;
}

int
sac_vars_put_var(char *group, char *name, int type, ...) {
    var *v;
    dict *d;
    va_list ap;

    if (!(d = sac_vars_get(group))) {
        return 0;
    }
    va_start(ap, type);
    v = var_new_internal(name, type, ap);
    va_end(ap);

    if ((dict_get(d, name))) {
        if (!dict_remove(d, name, var_free)) {
            return 0;
        }
    }
    return dict_put(d, name, v);
}

int
sac_vars_delete_var(char *group, char *name) {
    dict *d;
    if (!(d = sac_vars_get(group))) {
        return 0;
    }
    if (!(dict_get(d, name))) {
        return 0;
    }
    return dict_remove(d, name, var_free);
}

int
sac_vars_delete(char *group) {
    dict *d;
    if (!(d = sac_vars_get_internal(group, FALSE))) {
        return 0;
    }
    dict_remove(sac_vars, group, NULL);
    dict_free(d, NULL);
    return 1;
}

char **
sac_vars_keys(char *group) {
    dict *d;
    if (!(d = sac_vars_get(group))) {
        return NULL;
    }
    return dict_keys(d);
}

int
len4(char *s) {
    return (int) (ceil(strlen(s) / 4.0));
}

bbf *
sac_vars_to_bbf(char *group) {
    bbf *b;
    struct Header h;
    unsigned int hdr;
    int i, n;
    char **keys;

    b = bbf_new();
    keys = sac_vars_keys(group);
    i = 0;
    n = 0;
    while (keys[i]) {
        var *v = sac_vars_get_var(group, keys[i]);
        i++;
        memset(&h, 0, sizeof(int) * 11);
        h.type = v->type;
        h.namelength = len4(v->name);

        switch (v->type) {
            case VALUESTRING:
                h.valuelength = len4(v->str);
                hdr = encodeHeader(&h);
                bbf_set_var(b, &h, hdr, v->name, v->str);
                break;
            case VALUEINTEGER:
                h.valuelength = 1;
                hdr = encodeHeader(&h);
                bbf_set_var(b, &h, hdr, v->name, v->ival);
                break;
            case VALUEDOUBLE:
                h.valuelength = 2;
                hdr = encodeHeader(&h);
                bbf_set_var(b, &h, hdr, v->name, v->value);
                break;
            case VALUENIL:
                break;
        }
        n = n + h.valuelength + 1 + h.namelength;
    }
    memset(&h, 0, sizeof(int) * 11);
    h.valuelength = 0;
    h.type = VALUENIL;
    h.namelength = 1;
    hdr = encodeHeader(&h);
    bbf_set_var(b, &h, hdr, "NIL ", 0);
    n = n + h.valuelength + 1 + h.namelength;

    memset(&h, 0, sizeof(int) * 11);
    h.namelength = len4(group);
    h.type = VALUELIST;
    h.valuelength = n;
    hdr = encodeHeader(&h);
    bbf_set_header(b, &h, BBF_HEADER_IDENTITY, BBF_HEADER_VERSION, hdr, group);

    return b;
}

void
bbf_to_sacvar(bbf * b) {
    int i;
    rstrip(b->name);
    for (i = 0; i < b->n; i++) {
        vars *v = &(b->v[i]);
        rstrip(v->name);
        switch (v->type) {
            case VALUENIL:
                break;
            case VALUESTRING:
                rstrip(v->value);
                sac_vars_put_var(b->name, v->name, v->type, v->value);
                break;
            case VALUEDOUBLE:
                sac_vars_put_var(b->name, v->name, v->type, v->val);
                break;
            case VALUEINTEGER:
                sac_vars_put_var(b->name, v->name, v->type, v->ival);
                break;
        }
    }
}

int
sac_vars_write(char *group, char *file) {
    int err;
    bbf *b;
    b = sac_vars_to_bbf(group);
    err = bbf_write(b, file, 0);
    bbf_free(b);
    return err;
}

int
sac_vars_read(char *group) {
    bbf *b;
    int doswap;
    doswap = 0;
    if (!(b = bbf_read(group, &doswap))) {
        return doswap;          /* Error Condition stored in doswap */
    }
    FREE(b->name);
    b->name = strdup(group);
    if (!sac_vars_get_internal(b->name, FALSE)) {
        DEBUG("create variable group '%s'\n", b->name);
        sac_vars_create(b->name);
    }
    DEBUG("set variables in '%s'\n", b->name);
    bbf_to_sacvar(b);
    bbf_free(b);
    return 0;
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
setvar_ap(char *group, char *name, int type, va_list ap) {
    char *key;
    key = upcase_dup(name);
    switch (type) {
        case VAR_STRING:
            sac_vars_put_var(group, key, type, va_arg(ap, char *));
            break;
        case VAR_INTEGER:
            sac_vars_put_var(group, key, type, va_arg(ap, int));
            break;
        case VAR_VALUE:
            sac_vars_put_var(group, key, type, va_arg(ap, double));
            break;
    }
    if (key) {
        free(key);
        key = NULL;
    }
    return 1;
}

int
setvar(char *group, char *name, int type, ...) {
    int retval;
    va_list ap;
    char *vname = rstrip(strdup(name));
    va_start(ap, type);
    retval = setvar_ap(group, vname, type, ap);
    va_end(ap);
    FREE(vname);
    return retval;
}
