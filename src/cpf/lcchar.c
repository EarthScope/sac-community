/** 
 * @file   lcchar.c
 * 
 * @brief  Parse a string
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"
#include "bot.h"


#include "co.h"
#include "ucf.h"
#include "debug.h"

/** 
 * Parse a character string command
 * 
 * @param mchar 
 *    Maximum Length of \p kchar
 * @param kchar 
 *    Output character string
 * @param kchar_s 
 *    Lenght of \p kchar on input
 * @param nchar 
 *    Lenght of \p kchar on output
 * 
 * @return 
 *    - TRUE if the string was found
 *    - FALSE if the string was not found
 *
 * @date  870211:  Added check to see if command has more tokens.
 * @date  830208:  Allowed for a numeric token to be used as alphanumeric.
 * @date  820423:  Adjustments due to new command parsing system.
 * @date  820312:  Factored test for key to LCCKEY function.
 * @date  810928:  Modified due to changes in TOKENS.
 * @date  810416:  Removed option to use underline as substitute for blank.
 * @date  810208:  Original version.
 */
int 
lcchar(int   mchar, 
       char *kchar, 
       int   kchar_s, 
       int  *nchar) {

  int n;
  Token *t;

  UNUSED(kchar_s);

  if(!(t = arg())) {
    return FALSE;
  } 
  memset(kchar, 0, mchar);
  if(token_is_string(t) || token_is_quoted_string(t) || token_is_escape_string(t)) {
    if(strcmp(t->str, "-12345  ") == 0) {
      snprintf(kchar, mchar, "UNDEFINED");
    } else {
      n = min(mchar,strlen(t->str));
      strncpy(kchar, t->str, n);
      kchar[n] = 0;
    }
  } else if(token_is_number(t)) {
    if(t->value == -12345.0) {
      snprintf(kchar, mchar, "UNDEFINED");
    } else if(token_is_int_precision(t, TOKEN_INT_PRECISION_NON_ARGUMENT)) {
      snprintf(kchar, mchar, "%d", (int)t->value);
    } else {
      snprintf(kchar, mchar, "%g", t->value);
    }
  }
  *nchar = indexb(kchar, mchar);
  arg_next(); 
  return TRUE;

}

int 
lcchar_base(int   mchar, 
            char *kchar, 
            int   kchar_s, 
            int  *nchar) {
  int n;
  Token *t;

  UNUSED(kchar_s);

  if(!(t = arg())) {
    return FALSE;
  } 
  memset(kchar, 0, mchar);
  if(token_is_string(t) || token_is_quoted_string(t) || token_is_escape_string(t)) {
    if(strcmp(t->str, "-12345  ") == 0) {
      snprintf(kchar, mchar, "UNDEFINED");
    } else {
      n = min(mchar,strlen(t->str));
      strncpy(kchar, t->str, n);
      kchar[n] = 0;
    }
  } else if(token_is_number(t)) {
    if(t->str) {
      n = min(mchar,strlen(t->str));
      strncpy(kchar, t->str, n);
      kchar[n] = 0;
    } else if(t->value == -12345.0) {
      snprintf(kchar, mchar, "UNDEFINED");
    } else if(token_is_int_precision(t, TOKEN_INT_PRECISION_NON_ARGUMENT)) {
      snprintf(kchar, mchar, "%d", (int)t->value);
    } else {
      snprintf(kchar, mchar, "%lf", t->value);
    }
  }
  *nchar = indexb(kchar, mchar);
  arg_next(); 
  return TRUE;

}

char *
token_string_join(Token *t, char *c) {
  Token *tmp;
  char *out, *p;
  string *s;
  s = NULL;
  tmp = t;
  while(tmp) {
    p = token_as_string(tmp);
    s = string_append(s, p);
    s = string_append(s, c);
    tmp = tmp->next;
    FREE(p);
  }
  s = string_remove(s, string_length(s)-strlen(c), -1);
  out = strdup(string_string(s));
  string_free(&s);
  return out;
}

int
lcchar_split(int mchar, char *kchar, int kchar_s, int *nchar) {
  int n;
  char *p;
  Token *t, *new;
  UNUSED(kchar_s);

  if(!(t = arg())) {
    return FALSE;
  }

  /* Quoted and number are not subject to this function */
  if(token_is_quoted_string(t) || token_is_number(t) || token_is_escape_string(t)) {
    return lcchar(mchar, kchar, kchar_s, nchar);
  }
  /* Make sure the token is a string */
  if(!token_is_string(t)){
    return FALSE;
  }
  /* If string does not contain spaces */
  if(!index(t->str, ' ')) {
    return lcchar(mchar, kchar, kchar_s, nchar);
  }
  /* Split string and reconstruct with '\n ' */
  new = token_to_token_list(t);
  p = token_string_join(new, "\n ");
  token_free(new);
  /* Copy output */
  n = min(strlen(p), mchar);
  strncpy(kchar, p, n);
  kchar[n] = 0;
  FREE(p);
  arg_next();
  return TRUE;
}
