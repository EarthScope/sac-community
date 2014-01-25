/*
Copyright (c) 2009, Brian Savage
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
    * The names of the contributors may be used to endorse or promote products 
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "config.h"

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

typedef struct _string string;
struct _string {
  size_t   alloc;
  size_t   n;
  char *str;
};


/* String Creation and Destruction*/
 string *string_new(char *c);
 void    string_free(string **s);
 void    string_free_v(void **s);
 string *string_from_fortran(char *c, long int *pn);

/* String Utilities */
 string *string_check(string *s);
 string *string_grow(string *s, int len);
 string *string_alloc();
 string *string_pack(string *s);
 char   *strstr_escape(char *big, char *little);
 char   *strchr_reverse(char *s, int n, int c);
 char   *strchr_reverse_isnot(char *s, int n, int c);

/* Copy String */
 string *string_copy(string *s);

/* String Formatted Addition and Creation */
 string *string_printf(string *z, char *fmt, ...);
 string *string_printf_append_internal(string *z, char *fmt, va_list args);
 string *string_printf_append(string *z, char *fmt, ...);
 string *string_printf_prepend_internal(string *z, char *fmt, va_list args);
 string *string_printf_prepend(string *z, char *fmt, ...);
 string *string_append_int(string *z, int d);
 string *string_prepend_int(string *z, int d);

/* String Addition */
 string *string_append(string *z, char *c);
 string *string_prepend(string *z, char *c);
 string *string_insert(string *s, int n, char *in);

/* String Removal */
 string *string_remove(string *s, int n, int len);

/* String Trunctation */
 string *string_trunc(string *s);

/* String Comparison */
 int     string_equal_char(string *s, char *c);
 int     string_equal(string *a, string *b);
 int     string_compare(const string *a, const string *b);
 int     string_compare_v(const void *a, const void *b);
 int     string_compare_v_inv(const void *a, const void *b);

/* String Contents */
 char   *string_string(const string *s);
 int     string_length(string *s);
 int     string_nalloc(string *s);
/* Get a Portion of a String */
 string *string_substr(string *s, int n, int len); 
 char   *string_substr_char(string *s, int n, int len);

/* String Find and Replace */
 string *string_replace(string *s, char *find, char *replace);
 string *string_tilde_expand(string *s);

/* Print out the contents of a String */
 void    string_dump(string *s);
 void    string_dump_v(void *s);

/* Read in a line regardless of line terminator */
char   * fgetsp(char *s, int n, FILE *stream);
string * string_read(FILE *stream, int n);

#ifdef MISSING_FUNC_VASPRINTF

int  vasprintf(char **strp, const char *fmt, va_list args);
int  asprintf(char **strp, const char *fmt, ...);

#endif /* MISSING_FUNC_VASPRINTF */
void debug(char *fmt, ...);

#ifdef MISSING_FUNC_STRSEP
char * strsep(char **stringp, const char *delim);
#endif /* MISSING_FUNC_STRSEP */

#ifdef MISSING_FUNC_MKSTEMPS
int mkstempsp(char *path, int slen);
#endif /* MISSING_FUNC_MKSTEMPS */

int  sscanff(char *inp, char *fmt, ...);

#endif /* __STRING_UTILS_H__ */
