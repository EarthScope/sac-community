
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <sacio/sacio.h>
#include "cprint.h"

#include "defs.h"
#include "strip.h"

int
use_color(FILE *fp) {
    int n = fileno(fp);
    int tty = isatty(n);
    return tty;
}
void
parse_color(const char *color, char *code, size_t n) {
    char *string = NULL, *orig = NULL;
    orig = string = strdup(color);
    char *tok = NULL;
    while((tok = strsep(&string, ",")) != NULL) {
        tok = fern_lstrip(fern_rstrip(tok));
        if(strcasecmp(tok, "bold") == 0) {
            fern_strlcat(code, BOLD, n);
        } else if(strcasecmp(tok, "red") == 0) {
            fern_strlcat(code, RED, n);
        } else if(strcasecmp(tok, "black") == 0) {
            fern_strlcat(code, BLACK, n);
        } else if(strcasecmp(tok, "green") == 0) {
            fern_strlcat(code, GREEN, n);
        } else if(strcasecmp(tok, "blue") == 0) {
            fern_strlcat(code, BLUE, n);
        } else if(strcasecmp(tok, "magenta") == 0) {
            fern_strlcat(code, MAGENTA, n);
        } else if(strcasecmp(tok, "cyan") == 0) {
            fern_strlcat(code, CYAN, n);
        } else if(strcasecmp(tok, "white") == 0) {
            fern_strlcat(code, WHITE, n);
        }
    }
    FREE(orig);
}
int
vcfprintf(FILE *fp, const char *color, const char *fmt, va_list args) {
    char code[32] = {0};
    int n = 0;
    int on = use_color(fp);
    if(on) {
        parse_color(color, code, sizeof(code));
        n += fprintf(fp, "%s", code);
    }
    n += vfprintf(fp, fmt, args);
    if(on) {
        n += fprintf(fp, "%s", RESET);
    }
    fflush(fp);
    return n;
}

int
cfprintf(FILE *fp, const char *color, const char *fmt, ...) {
    va_list args;
    int n;
    va_start(args, fmt);
    n = vcfprintf(fp, color, fmt, args);
    va_end(args);
    return n;
}
int
cprintf(const char *color, const char *fmt, ...) {
    va_list args;
    int n;
    va_start(args, fmt);
    n = vcfprintf(stdout, color, fmt, args);
    va_end(args);
    return n;
}
