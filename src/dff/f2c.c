/** 
 * @file   f2c.c
 * 
 * @brief  Convert fortran strings to C strings
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dff.h"

/** 
 * Identify a function as deprecated
 * 
 * @param old_func 
 *    Old, deprecated function
 * @param new_func 
 *    New function to use
 *
 */
void
DEPRECATED(char *old_func, char *new_func) {
  fprintf(stderr, "%s has been deprecated. Inform the developer or use %s\n",
	  old_func, new_func);
  return;
}

/** 
 *  Find Frist of a series of Trailing Whitespace (Spaces) at the 
 *     end of a string and the truncate '\0' the string there.
 *  
 * @param s
 *    Input string to trim
 *
 * @return 
 *    Trimmed string, same as input argument
 *
 */ 
char *
fstrtrim(char *s) {
  char *p;
  if(strlen(s) <= 0) {
    return s;
  }
  p = s + strlen(s) - 1;
  while(*p == ' ' && p != s) {
    p--;
  }
  s[p-s+1] = 0;
  if(strlen(s) == 1 && *s == ' ') {
    s[0] = 0;
  }
  return s;
}

/** 
 * Duplicate a fortran style string 
 * 
 * @param s 
 *    Fortran style string to duplicate
 * @param n 
 *    Possible length
 *    n < 0 then assume C style string, use strlen() to get length
 *    n > 0 then assume Fortran style string, trim and return
 *
 * @return 
 *    New C style string, trimmed to length
 *
 */
char *
fstrdup(char *s, int n) {
  char *q;
  if(n < 0) {
    n = strlen(s);
  }
  /* Length of string plus terminator */
  q = (char *)malloc(sizeof(char) * (n + 1)); 
  q[0] = '\0';
  q = strncpy(q,s, (size_t) n);
  /* Null - Terminate String */
  q[n] = '\0'; 
  /* Find First of All Remaining Whitespace and Terminate There */
  q = fstrtrim(q);
  return q; 
}

/** 
 * Set a Fortran style string
 * 
 * @param in 
 *    Input string
 * @param out 
 *    Output string
 * @param n 
 *     Length of \p in
 * 
 * @return 
 *     Fortran style string
 */
char *
fstrset(char *in, char *out, int n) {
  memset(out, ' ', (size_t) n);
  strncpy(out, in, strlen(in));
  return out;
}

#ifdef __TESTING_F2C__

void
cfunc(char *s, int len) {
  char *q = fstrdup(s,len);
  free(q);
  return;
}

void cfunc_ (char *s, int len) { cfunc(s, len); }
void cfunc__(char *s, int len) { cfunc(s, len); }


void
ccall_() {
  char *a;
  a = strdup("hello");
  cfunc(a, -1);
  a = strdup("hello    ");
  cfunc(a, -1);
  a = strdup("hel lo ");
  cfunc(a, -1);
  a = strdup(" hel lo   ");
  cfunc(a, -1);
  cfunc("hello", -1);
  cfunc(" hello", -1);
  cfunc("hello ", -1);
  cfunc("he llo", -1);
  cfunc(" he llo", -1);
  cfunc("he llo ", -1);
  cfunc(" he llo ", -1);
  cfunc(" he llo fjfjdj  jfjdjj j jfjdj j j jfjdkfkdk j j jfjd jj j    ", -1);
  cfunc("jfdkjfdksjfdkslfjdskfjdslkfjdslkfjsdlkfjsdlfkjsdlfkdjsl", -1);
}
#endif /* __TESTING_F2C__  */

