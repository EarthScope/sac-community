
%{

#include <stdio.h>  
#include <stdlib.h>

#include "rc.h"

  extern int verbose;

  int yylex(void);
  void yyerror(char const *c);
  void eof_found();

  rc_function *rc_set;
 
%}

%union {
  int    i;   /* Integer */
  float  d;   /* Double, floating point */
  char*  s;   /* String */
}

/* Tokens, defined for the Parser */
%token INTEGER STRING NUMBER BOOL_TRUE BOOL_FALSE EQUALS

%% /* Grammar rules and action follows */

commands:  /* empty */ | commands command ;

command: 
STRING EQUALS INTEGER    { 
  if(verbose) {
    fprintf(stderr, "INTEGER <%s> = %d   \n", $<s>1,$<i>3); 
  }
  (rc_set)($<s>1, RC_INTEGER, &$<i>3);
} 
| 
STRING EQUALS STRING     { 
  if(verbose) {
    fprintf(stderr, "STRING <%s> = <%s> \n", $<s>1,$<s>3); 
  }
  (rc_set)($<s>1, RC_STRING, $<s>3);
}
| 
STRING EQUALS NUMBER     { 
  if(verbose) {
    fprintf(stderr, "NUMBER <%s> = %e\n", $<s>1,$<d>3); 
  }
  (rc_set)($<s>1, RC_REAL, &$<d>3);
}
| 
STRING EQUALS BOOL_TRUE  { 
  int b = TRUE;
  if(verbose) {  
    fprintf(stderr, "BOOL <%s> = %d\n", $<s>1,b); 
  }
  (rc_set)($<s>1, RC_BOOL, &b);
}
  | 
STRING EQUALS BOOL_FALSE { 
  int b = FALSE;
  if(verbose) {
    fprintf(stderr, "BOOL <%s> = %d\n", $<s>1,b); 
  }
  (rc_set)($<s>1, RC_BOOL, &b);
}
;
%%


void
rc_set_function(rc_function *func) {
  rc_set = func;
}

