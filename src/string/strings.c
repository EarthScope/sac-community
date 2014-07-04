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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

/* For tilde expansion - OS/X */
#include <sys/types.h>
#ifdef WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef POSIX
#include <pwd.h>
#endif

#include "config.h"

#include "string_utils.h"

#define UNUSED(x) (void) x

string * string_alloc();
int string_count(string *s, char *c);


int
string_nalloc(string *s) {
  if(!s) {
      return 0;
  }
  return s->alloc;
}

int
string_length(string *s) {
  if(!s) {
    return 0;
  }
  return s->n;
}

char *
string_string(const string *s) {
  if(s == NULL || s->str == NULL) {
    return NULL;
  }
  return s->str;
}

int 
string_check_length(string *s) {
  return TRUE;

  if(s->n != strlen(s->str)) {
      fprintf(stderr, "string error: length mismatch: %d - %d\n", (int) s->n, (int) strlen(s->str));
    return FALSE;
  }
  return TRUE;
}

string *
string_check(string *s) {
  if(s == NULL) {
    return string_alloc();
  }
  return s;
}

string *
string_grow(string *s, int len) {
  char *tmp;
  size_t n;
  s->alloc = 2;
  while(s->alloc <= (size_t) len) {
    s->alloc *= 2;
  }
  n = s->alloc;
  tmp = realloc(s->str, n);
  if(tmp) {
      s->str = tmp;
      memset(s->str + s->n, 0, s->alloc - s->n);
  } else {
      fprintf(stderr, "string: error expanding\n");
  }
  return(s);
}

void string_free_v(void **s) { string_free((string **)s); }

void
string_free(string **ss) {
  string *s = *ss;
  if(ss == NULL) {
    return;
  }
  if(s == NULL) {
    return;
  }
  s->n     = 0;
  s->alloc = 0;
  free(s->str);

  s->str   = NULL;
  free(s);
  *ss = NULL;
  return;
}



string *
string_append_int(string *z, int d) {
  return string_printf_append(z, "%d", d);
}

string *
string_prepend_int(string *z, int d) {
  return string_printf_prepend(z, "%d", d);
}

string *
string_new(char *c) {
  return string_prepend(NULL, c);
}

void string_dump_v(void *s) { string_dump((string *)s); }

void
string_dump(string *s) {
  if(s == NULL) {
    fprintf(stderr, "string: %p\n", NULL);
    return;
  }
  fprintf(stderr,"string: %p alloc: %d n: %d str: <%s>\n",s,(int)s->alloc,(int)s->n,s->str);
}

string *
string_prepend(string *s, char *c) {
  size_t n;
  s = string_check(s);
  
  n = strlen(c) + s->n;
  if(n >= s->alloc) {
    s = string_grow(s, n);
  }
  memmove(s->str + strlen(c), s->str, s->n);
  memmove(s->str, c, strlen(c));
  s->str[n] = 0;
  s->n = n;
  string_check_length(s);
  return s;
}

string *
string_append(string *s, char *c) {
  size_t cn;
  size_t n;

  cn = strlen(c);
  s = string_check(s);
  n = cn + s->n;
  if(n >= s->alloc) {
    s = string_grow(s, n);
  }
  memcpy(s->str + s->n, c, cn);
  //s->str = strcat(s->str, c);
  s->str[n] = 0;
  s->n = n;
  string_check_length(s);
  return s;
}

string *
string_printf_prepend(string *z, char *fmt, ...) {
  va_list args;
  
  va_start(args, fmt);
  z = string_printf_prepend_internal(z, fmt, args);
  va_end(args);
  return z;
}

string *
string_printf_append(string *z, char *fmt, ...) {
  va_list args;
  
  va_start(args, fmt);
  z = string_printf_append_internal(z, fmt, args);
  va_end(args);
  return z;
}

string *
string_printf_append_internal(string *z, char *fmt, va_list args) {
  char *s;
  int n;

  z = string_check(z);

  n = vasprintf(&s, fmt, args);
  if(n == -1) {
    fprintf(stderr, "libstring: Error allocting space for string\n");
    return z;
  }
  z = string_append(z, s);
  free(s);
  return z;
}

string *
string_printf_prepend_internal(string *z, char *fmt, va_list args) {
  char *s;
  int n;

  n = vasprintf(&s, fmt, args);
  if(n == -1) {
    fprintf(stderr, "libstring: Error allocting space for string\n");
    return NULL;
  }
  z = string_prepend(z, s);
  free(s);
  return z;
}


string *
string_printf(string *z, char *fmt, ...) {
  va_list args;

  z = string_check(z);
  z = string_trunc(z);

  va_start(args, fmt);
  z = string_printf_append_internal(z, fmt, args);
  va_end(args);
  return z;
}

string *
string_trunc(string *s) {
  s = string_check(s);
  s->str[0] = 0;
  s->n = 0;
  string_check_length(s);
  return s;
}

string *
string_alloc() {
  string *s;
  s = (string *) malloc(sizeof(string));
  s->alloc  = 2;
  s->str    = (char *) malloc(sizeof(char) * s->alloc);
  s->n      = 0;
  s->str[0] = 0;
  return s;
}

string *
string_copy(string *s) {
  string *to = NULL;
  if(!s) {
      return string_check(NULL);
  }
  to = string_append(to, s->str);
  return(to);
}

int
string_equal_char(string *s, char *c) {
  if(s      == NULL || 
     s->str == NULL || 
     c      == NULL) {
    return 0;
  }
  return( strcmp(s->str, c) == 0 );
}

int
string_compare_v(const void *a, const void *b) {
  const string **sa = (const string **) a;
  const string **sb = (const string **) b;
  return string_compare(*sa, *sb);
}

int 
string_compare_v_inv(const void *a, const void *b) {
  const string **sa = (const string **) a;
  const string **sb = (const string **) b;
  return string_compare(*sb, *sa);  
}

int
string_compare(const string *a, const string *b) {
  if(!string_string(a)) {
    if(!string_string(b)) {
      return 0;
    }
    return -1;
  }
  if(!string_string(b)) {
    if(!string_string(a)) {
      return 0;
    }
    return 1;
  }
  return strcmp( string_string(a), string_string(b) );
}

int
string_equal(string *a, string *b) {
  if(a == NULL      || b == NULL      ||
     a->str == NULL || b->str == NULL) {
    return 0;
  }
  return( strcmp(a->str, b->str) == 0 );
}

string *
string_remove(string *s, int n, int len) {
  
  char *start;
  
  s = string_check(s);

  if(n < 0) {
    n = 0;
  }
  if((size_t)n > s->n) { /* Starting point greater than end */
    return s;
  }
  start = s->str + n;
  
  if((size_t)(n + len) >= s->n || len < 0) { /* Remove remainder of the string */
    memset( start, 0, s->n - n);
    s->n = strlen(s->str);
  } else {
    memmove(start, 
	    start + len, 
	    s->n - n - len );
    memset( s->str + s->n - len, 0, s->alloc - s->n + len);
    s->n -= len;
    s->str[s->n] = 0;
  }
  string_check_length(s);
  return s;
}

string *
string_insert(string *s, int n, char *in) {
  size_t inlen;
  s = string_check(s);
  if(in == NULL) {
    return s;
  }
  inlen = strlen(in);
  if(s->n + inlen >= s->alloc) { 
    string_grow(s, s->n + inlen);
  }
  if(n < 0) {
    n = 0;
  }
  if((size_t)n >= s->n) {
    n = s->n;
  }
  memmove(s->str + n + inlen, 
	  s->str + n , 
	  s->n - n + 1);
  s->str[s->n + inlen] = 0;

  memmove(s->str + n, in, inlen);
  s->n = strlen(s->str);
  string_check_length(s);
  return(s);
}

string *
string_replace(string *s, char *find, char *replace) {
  size_t nfind;
  size_t offset;
  char *tmp;
  if(s == NULL || s->str == NULL) {
    return NULL;
  }

  nfind    = strlen(find);
  while((tmp = strstr_escape(s->str, find)) != NULL) {
    offset = tmp - s->str;
    s = string_remove(s, offset, nfind);
    s = string_insert(s, offset, replace);
  }
  return s;
}

string *
string_substr(string *s, int n, int len) {
  string *out = NULL;
  if(!s) {
      return string_new("");
  }
  
  out = string_copy(s);

  if((size_t)n > s->n) {
    return out;
  }
  if(n < 0) {
    n = 0;
  }
  if(len < 0) {
    len = s->n;
  }
  if((size_t)(n + len) > s->n) {
    len = s->n - n;
  }

  out = string_remove(out, n + len, -1);
  out = string_remove(out, -1, n);
  return string_pack(out);
}

char *
string_substr_char(string *s, int n, int len) {
  string *out = string_substr(s, n, len);
  char *c     = strdup(string_string(out));
  string_free(&out);
  return c;
}

string *
string_pack(string *s) {
  char *tmp;
  if(!s) {
    return string_check(s);
  }
  s->alloc = s->n + 1;
  if(s->alloc <= 0) {
      s->n = 1;
      s->alloc = 1;
  }
  tmp = realloc(s->str, s->alloc);
  if(tmp) {
      s->str = tmp;
  } else {
      fprintf(stderr, "libstring: Error during packing, string unmodified\n");
  }
  return(s);
}

/* Find first occurance which is not preceeded by a backslash */
char *
strstr_escape(char *big, char *little) {
  char *c;
  c = strstr(big, little);
  while(c != NULL && c != big && c[-1] == '\\') {
    c = strstr(c + strlen(little), little);
  }
  return c;
}

char *
strchr_reverse(char *s, int n, int c) {
  char *p;
  UNUSED(n);
  p = s + strlen(s) - 1;
  while(*p != c && p != s) {
    p--;
  }
  return p;
}

/* Find the last character which is not character c */
char *
strchr_reverse_isnot(char *s, int n, int c) {
  char *p;
  UNUSED(n);
  p = s + strlen(s) - 1;
  while(*p == c && p != s) {
    p--;
  }
  return(p);
}

string *
string_from_fortran(char *c, long int *pn) {
  char *p, *t;
  string *out;
  int n = *pn;

  if(n < 0) {
    n = strlen(c);
  }
  p = (char *) malloc(sizeof(char) * (n + 1));
  p[0] = '\0';
  p = strncpy(p, c, n);
  p[n] = '\0';

  t = strchr_reverse_isnot(p, n, ' ');
  t[1] = '\0';
  out = string_new(p);
  free(p);
  return out;  
}

int
string_count(string *s, char *c) {
  int n = 0;
  char *p = s->str;
  while((p = strstr_escape(p, c)) != NULL) {
    n++;
    p = p + strlen(c);
  }
  return(n);
}

#ifdef POSIX
string *
string_tilde_expand(string *s) {
  char *p, *e;
  string *user;
  struct passwd *pw;


  if((p = strstr_escape(s->str, "~")) == NULL) {
    return s;
  }
  e = strstr_escape(p, "/");
  if(e != NULL) {
    user = string_substr(s, (int)(p - s->str), (int)(e - p));
  } else {
    user = string_substr(s, (int)(p - s->str), -1);
  }

  if(string_equal_char(user, "~")) {
    if( (pw = getpwuid(getuid())) == NULL) {
      string_free(&user) ;
     return s;
    }
  } else {
    user = string_replace(user, "~", "");
    if( (pw = getpwnam(user->str)) == NULL) {
      string_free(&user);
      return s;
    }
    user = string_prepend(user, "~");
  }

  string_replace(s, user->str, pw->pw_dir);
  string_free(&user);
  return s;
}
#endif

#ifdef MISSING_FUNC_VASPRINTF

#define VASPRINTF_BUFSIZE 2

int
vasprintf(char **strp, const char *fmt, va_list args) {
  va_list args_copy;
  int needed;
  int status;
  char *c;
  
  /* Work around for buggy Solaris vsnprintf(), 
     it requires a char *, a NULL and a length of 0 does not work */
  c = malloc(sizeof(char) * VASPRINTF_BUFSIZE);
  memset(c, 0, VASPRINTF_BUFSIZE);

  needed = 2;
  va_copy(args_copy, args);
  needed = vsnprintf(c, VASPRINTF_BUFSIZE, fmt, args_copy);
  va_end(args_copy);
  free(c);

  if(needed < 0) {
    *strp = NULL;
    return needed;
  }
  *strp = malloc(needed+1);
  if(*strp == NULL) {
    return -1;
  }
  status = vsnprintf(*strp, needed + 1, fmt, args);
  if(status >= 0) {
    return status;
  }
  free(*strp);
  *strp = NULL;
  return status;
}

int 
asprintf(char **strp, const char *fmt, ...) {
  va_list args;
  int status;

  va_start(args, fmt);
  status = vasprintf(strp, fmt, args);
  va_end(args);
  return status;
}

#endif /* MISSING_FUNC_VASPRINTF */


/** 
* Reads at most one less than \p size from \p stream and save the
* result into string \p buf. Reading stops following a newline
* character, linefeed, end-of-file character, or an error. On
* encounter with a newline or linefeed character, the other is
* checked for. The newline character is kept. On return with
* no errors the string \p buf has a '\0' character added to
* the end of the string. Line terminators are replace with
* Unix line terminators.
*
* - Unix - <\n>
* - Mac - <\r>
* - Dos - <\r\n>
*
* @param buf
* String to store the result in
* @param size
* Length of \p buf
* @param stream
* Stream to read from
*
* @return A string consisting of a single line read in from \p stream.
* On error NULL will be returned.
*
*/
char *
fgetsp(char *s,
       int n,
       FILE *stream) {
  register int c;
  register char *cs;

  c = EOF;
  /* Set a temporary pointer to the initial value of the storage pointer */
  cs = s;
  
  while(--n > 0 && (c = fgetc(stream)) != EOF) {

    *cs++ = c;
    if(c == '\n') { /* Newline - Unix Style */
      break;
    }
    if(c == '\r') { /* Carriage Return - Mac and maybe Windows */
      /* Replace the Return with the Newline */
      cs--;
      *cs++ = '\n';
      if(n > 0 && (c = fgetc(stream)) != EOF) {
        if(c != '\n') { /* Newline - Windows */
          ungetc(c, stream);
        }
      }
      break;
    }
  }

  /* Null Terminate the String */
  *cs = '\0';

  /* The first character was EOF and nothing was added to the string
   * and the string was not advanced a character
   */
  if(c == EOF && cs == s)
    return NULL;

  return s;
}


void
debug(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
}

#ifdef MISSING_FUNC_STRSEP

/*- Only for strsep
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Get next token from string *stringp, where tokens are possibly-empty
 * strings separated by characters from delim.
 *
 * Writes NULs into the string at *stringp to end tokens.
 * delim need not remain constant from call to call.
 * On return, *stringp points past the last NUL written (if there might
 * be further tokens), or is NULL (if there are definitely no more tokens).
 *
 * If *stringp is NULL, strsep returns NULL.
 */
char *
strsep(char **stringp, const char *delim)
{
	register char *s;
	register const char *spanp;
	register int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

#endif /* MISSING_FUNC_STRSEP */

#ifdef MISSING_FUNC_MKSTEMPS

/* Only for mkstemps */
/*	$OpenBSD: mktemp.c,v 1.30 2010/03/21 23:09:30 schwarze Exp $ */
/*
 * Copyright (c) 1996-1998, 2008 Theo de Raadt
 * Copyright (c) 1997, 2008-2009 Todd C. Miller
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef WIN32
typedef int mode_t;
static const mode_t S_IRUSR      = (mode_t)(_S_IREAD);     ///< read by user
static const mode_t S_IWUSR      = (mode_t)(_S_IWRITE);    ///< write by user
#endif

#define MKTEMP_NAME	0
#define MKTEMP_FILE	1
#define MKTEMP_DIR	2

#define TEMPCHARS	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define NUM_CHARS	(sizeof(TEMPCHARS) - 1)

static int
mktemp_internal(char *path, int slen, int mode)
{
	char *start, *cp, *ep;
	const char *tempchars = TEMPCHARS;
	unsigned int r, tries;
	struct stat sb;
	size_t len;
	int fd;

	len = strlen(path);
	if (len == 0 || slen >= len) {
		errno = EINVAL;
		return(-1);
	}
	ep = path + len - slen;

	tries = 1;
	for (start = ep; start > path && start[-1] == 'X'; start--) {
		if (tries < INT_MAX / NUM_CHARS)
			tries *= NUM_CHARS;
	}
	tries *= 2;

	do {
		for (cp = start; cp != ep; cp++) {
            r = rand() % NUM_CHARS;
			*cp = tempchars[r];
		}

		switch (mode) {
#ifdef POSIX
		case MKTEMP_NAME:
			if (lstat(path, &sb) != 0)
				return(errno == ENOENT ? 0 : -1);
			break;
#endif
		case MKTEMP_FILE:
			fd = open(path, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
			if (fd != -1 || errno != EEXIST)
				return(fd);
			break;
#ifdef POSIX			
		case MKTEMP_DIR:
			if (mkdir(path, S_IRUSR|S_IWUSR|S_IXUSR) == 0)
				return(0);
			if (errno != EEXIST)
				return(-1);
			break;
#endif			
		}
	} while (--tries);

	errno = EEXIST;
	return(-1);
}

int
mkstemps(char *path, int slen)
{
	return(mktemp_internal(path, slen, MKTEMP_FILE));
}

#endif /* MISSING_FUNC_MKSTEMPS */


 
 
 
 #define LONG       0x00001 /* l: long or double  */
 #define LONGBL     0x00002 /* L: long double  */
 #define SHORT      0x00004 /* h: short */
 #define SHORTSHORT 0x00008 /* hh: 8 bit integer */
 #define LLONG      0x00010 /* ll: long long  */
 #define POINTER    0x00020 /* p: void * */
 #define SIZEINT    0x00040 /* z: size_t */
 #define MAXINT     0x00080 /* j: intmax_t */
 #define PTRINT     0x00100 /* t: ptrdiff_t */
 #define NOSKIP     0x00200 /* [ or c: do not skip blanks */
 #define SUPPRESS   0x00400 /* *: suppress assignment */
 #define UNSIGNED   0x00800 /* %[oupxX]: conversions */
 #define SUBWIDTH   0x01000 /* 9.5f */ 
 
 enum {
   CT_INT,
   CT_STRING,
   CT_CHAR,
   CT_FLOAT
 };

 int
 read_chars(char *in, char *arg, int width, int flags) {
   int i = 0;
   if(flags & SUPPRESS) {
     while(i < width && *in) {
       in++;
       i++;
     }
   } else {
     while(i < width && *in) {
       arg[i] = *in++;
       i++;
     }
   }
   if(i != width) {
     return -1;
   }
   arg[width] = 0;
   return width;
 }
 
 #define ADD(a,b) do { *a = b; a++; a[0] = 0; } while(0)

/**
 *  sscanff - Like sscanf, but strictly interperts width
 *
 * @var inp - Input String to scan
 * @var fmt - Input format to scan with
 * @var variables - assigned variables
 *
 */
 
 int
 sscanff(char *inp, char *fmt, ...) {
   char c;
   char *in;
   va_list ap;
   char *ifmt;
   char *fmtp;
   int flags;
   int width;
   int nassigned;
   int nread;
   char *sfmt;
 
   in = inp;
 
   sfmt = strdup(fmt);
   va_start(ap, fmt);
   nread = 0;
   nassigned = 0;
 
   ifmt = (char *) malloc(sizeof(char) * (strlen(fmt) + 1));
   memset(ifmt, 0, strlen(fmt)+1);
 
   /* Scan the string */
   for(;;) {
     c = *fmt++;
     if(c == 0) { /* End of the String */
       goto error;
     }
     if(c != '%') { /* Literal Conversion Character, Check against current character */
       goto literal;
     }
     width = 0;
     flags = 0;
     fmtp = ifmt; /* Reset Format Pointer */
     ADD(fmtp, '%');
   again:
     c = *fmt++;
     if(isdigit(c)) {
       if(!(flags & SUBWIDTH)) {
         ADD(fmtp, c);
       }
     } else {
       ADD(fmtp, c);
     }
 
     switch(c) {
     case '%':
   literal:
       if(c != *in) {
         fprintf(stderr, "sscanff(): literal value does not match input '%c' '%c'\n", c, *in);
         fprintf(stderr, "in:  %s\n", inp);
         fprintf(stderr, "fmt: %s\n", sfmt);
         goto error;
       }
       in++;
       nread++;
     continue;
 
     case '*':
       flags |= SUPPRESS;
       goto again;
     case 'L':
       flags |= LONG;
       goto again;
     case 'h':
       if(*fmt == 'h') {
         flags |= SHORTSHORT;
         fmt++;
       }
       flags |= SHORTSHORT;
       goto again;
     case 'l':
       if(*fmt == 'l') {
         flags |= LLONG;
         fmt++;
       }
       flags |= LONG;
       goto again;
     case '0':
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
     case '8':
     case '9':
       if(!(flags & SUBWIDTH) ) {
         width = width * 10 + c - '0';
       }
       goto again;
 
     case '.':
       flags |= SUBWIDTH;
       fmtp--;
       fmtp[0] = 0;
       goto again;
 
    /* Conversions */
     case 'D':
       flags |= LONG;
     case 'd':
     case 'i':
       c = CT_INT;
       break;

     case 'a':
     case 'A':
     case 'E':
     case 'G':
     case 'e':
     case 'f':
     case 'F':
     case 'g':
       c = CT_FLOAT;
       break;
     case 's':
       c = CT_STRING;
       break;
     case 'c':
       c = CT_CHAR;
       break;
     case '\0':
       goto error;
     default:
       fprintf(stderr, "sscanff(): Unexpected character <%c>\n", c);
       break;
     }
     /* Exit with an error if the width was not defined */
     if(width == 0) {
       fprintf(stderr, "sscanff(): Unknwon Width, returning\n");
       goto error;
     }

     /* Conversion Time
      *   Read in \p width characters and convert based on the conversion
      */
     {
       char *cut;
       int r;
       /* Read in width */
       cut = (char *) malloc(sizeof(char) * (width + 1));
       if( (r = read_chars(in, cut, width, flags)) < 0) {
         fprintf(stderr, "Error reading in %d characters\n", width);
         goto error;
       }
       cut[width] = 0;
       nread += r;
       in += r;
       /* Convert to wanted type */
       switch(c) {
       case CT_STRING:
       case CT_CHAR: {
         char *out;
         if(! (flags & SUPPRESS) ) {
           out = va_arg(ap, char *);
           strncpy(out, cut, width);
           out[width] = 0;
           nassigned++;
         }
       }
         break;
       case CT_INT: {
         int ret;
         if(flags & SHORT) {
           ret = sscanf(cut, ifmt, va_arg(ap, short int *));
         } else if(flags & LONG) {
           ret = sscanf(cut, ifmt, va_arg(ap, long int *));
         } else if(flags & LLONG) {
           ret = sscanf(cut, ifmt, va_arg(ap, long long int *));
         } else {
           ret = sscanf(cut, ifmt, va_arg(ap, int *));
         }
         if(ret != 1) {
           fprintf(stderr, "Error reading: %s [%s]\n",  cut, ifmt);
           goto error;
         }
         nassigned++;
         break;
       }
       case CT_FLOAT: {
         int ret;
         if(flags & LONG) {
           ret = sscanf(cut, ifmt, va_arg(ap, double *));
         } else if(flags & LLONG) {
           ret = sscanf(cut, ifmt, va_arg(ap, long double *));
         } else  {
           ret = sscanf(cut, ifmt, va_arg(ap, float *));
         }
         if(ret != 1) {
           fprintf(stderr, "Error reading: %s [%s]\n",  cut, ifmt);
           goto error;
         }
         nassigned++;
         break;
       }

       }
       free(cut);
     }
   }
  error:
   va_end(ap);
   free(ifmt);
   if(nread != (int)strlen(inp)) {
     fprintf(stderr, "Error reading entire input string\n");
   }
   return nassigned;
 }

#ifdef MISSING_FUNC_BZERO
void
bzero(void *s, size_t n) {
	memset(s, 0, n);
}
#endif

#ifdef MISSING_FUNC_INDEX
char *
index(const char *s, int c) {
	return strchr(s, c);
}
#endif

#ifdef MISSING_FUNC_RINDEX
char *
rindex(const char *s, int c) {
	return strrchr(s, c);
}
#endif

#ifdef MISSING_FUNC_SLEEP
unsigned int 
sleep(unsigned int seconds) {
#ifdef WIN32
	Sleep(1000 * seconds);
	return 0;
#else
	#error(sleep undefined);
#endif
}
#endif

#ifdef MISSING_FUNC_VSNPRINTF

int vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}
#endif

#ifdef MISSING_FUNC_SNPRINTF

int snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}


#endif

#ifdef MISSING_FUNC_STRCASECMP

int
strcasecmp(const char *s1, const char *s2) {
  return stricmp(s1,s2);
}

#endif

#ifdef MISSING_FUNC_STRNCASECMP

int
strncasecmp(const char *s1, const char *s2, size_t n) {
  return strnicmp(s1,s2,n);
}

#endif

#ifdef MISSING_FUNC_STRLCPY
size_t
strlcpy(char *dst, const char *src, size_t size)
{
    size_t length, copy;

    length = strlen(src);
    if (size > 0) {
        copy = (length >= size) ? size - 1 : length;
        memcpy(dst, src, copy);
        dst[copy] = '\0';
    }
    return length;
}
#endif
