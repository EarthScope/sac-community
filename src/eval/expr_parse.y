
%include {
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "token.h"
#include "eval.h"
#include "expr_parse.h"
#include "string_utils.h"
#include "bot.h"
#include "bbs.h"

#include "co.h"

int gettime(int lmax, int lvalue, double tvalue, double *value);


static int tdebug = 0;

#define TSET(A,B) A.type = B;

 void parse_error(Token *value, int error);
 void value_num(Token *value, double v);

static int eval_asfloat = TRUE;
static char *eval_out = NULL;

}

%ifdef MATH_NONE
%name ParseNoOp
%endif

%token_type {Token}
%extra_argument {Token *value}
%start_symbol main

%nonassoc RPAREN LPAREN.

%ifdef MATH_NORMAL
%left PLUS MINUS.
%left DIVIDE TIMES.
%right EXP.
%endif

%ifdef MATH_NONE
%left PLUS MINUS DIVIDE TIMES EXP.
%endif

%type NUM {Token}

%stack_overflow {
  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
  UNUSED(yypMinor);
}

%parse_failure {
  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
}

%parse_accept {
  if(value->error <= 0) {
    value->error = TOKEN_STATUS_OK;
  }
}

%token_destructor {
  yypParser->value = value;
  UNUSED(yypminor);
}

%syntax_error {
  UNUSED(yymajor);
  UNUSED(yyminor);
  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
}

/* Entry Point */

main       ::= pin(B).                     { 
  if(value->error <=0){
    token_copy(value, &B);
  } 
}

pin         ::= . 
pin(A)      ::= pbits_list(B).             { token_copy(&A,&B);      }
pin(A)      ::= pbits_list(B) NEWLINE.     { token_copy(&A,&B);      }

pbits_list(A)  ::= pbits(B).               { token_copy(&A,&B);      }
pbits_list(A)  ::= pbits_list(B) pbits(C). { token_append(&A,&B,&C);  }

pbits(A)    ::= number(B).                 { token_copy(&A,&B);     }
pbits(A)    ::= STRING(B).                 { token_copy(&A,&B);      }
pbits(A)    ::= QUOTED_STRING(B).          { token_copy(&A,&B);      }
pbits(A)    ::= ESCAPE_STRING(B).          { token_copy(&A,&B);      }
pbits(A)    ::= LPAREN(E) expr(B) RPAREN.  { B.col = E.col; token_copy(&A,&B);      }
pbits(A)    ::= EQUALS(B).                 { token_copy(&A,&B);      }
pbits(A)    ::= GE(B).                     { token_copy(&A,&B);      }
pbits(A)    ::= LE(B).                     { token_copy(&A,&B);      }
pbits(A)    ::= EQ(B).                     { token_copy(&A,&B);      }
pbits(A)    ::= LT(B).                     { token_copy(&A,&B);      }
pbits(A)    ::= GT(B).                     { token_copy(&A,&B);      }
pbits(A)    ::= NE(B).                     { token_copy(&A,&B);      }
pbits(A)    ::= COMMA(B).                  { token_copy(&A,&B);      }
pbits(A)    ::= WILD(B).                   { token_copy(&A,&B);      }

pin(A)        ::= evaluate(B) .  { 

  Token *tmp;
  token_copy(&A, &B);
  tmp = token_last(&B);
  if(eval_out && strcasecmp(eval_out, "term") != 0) {
    if(eval_asfloat) {
      setbb(eval_out, VAR_VALUE, tmp->value);
    } else {
      setbb(eval_out, VAR_INTEGER, (int)tmp->value);
    }
  } else {
    A.next = tmp;
  }
  eval_asfloat = 1;
}
evaluate(A)  ::= EVALUATE(C) evaloptsp(D) expr(B) . { 
    Token *p;
    C.type = STRING; 
    if(!eval_asfloat) {
      B.value = (int)B.value;
    }
    token_copy(&A, &C);

    if(D.type) {
      p = token_new(0,0,NULL,0,0);
      token_copy(p, &D);
      token_last(&A)->next = p;
    }

    p = token_new(0,0,NULL,0,0);
    token_copy(p, &B);
    token_last(&A)->next = p;
}
evaloptsp    ::= .
evaloptsp(A) ::= evalopts(B).           { token_copy(&A,&B);  }
evalopts(A)  ::= evalo(B) .             { token_copy(&A,&B); }
evalopts(A)  ::= evalopts(B) evalo(C) . { token_append(&A,&B,&C); }
evalo(A)     ::= TO(C) STRING(B) .      { 
   if(eval_out) {
     free(eval_out); 
     eval_out = NULL; 
   }
   eval_out = strdup(B.str); 
   C.type = STRING;
   token_append(&A,&C,&B);
}
evalo(A)     ::= AS(B) FLOAT(C) .     { eval_asfloat = 1; B.type = C.type = STRING; token_append(&A,&B,&C); }
evalo(A)     ::= AS(B) INTEGER(C) .   { eval_asfloat = 0;  B.type = C.type = STRING; token_append(&A,&B,&C); }

/* Commands */
//commands    ::= .
//commands    ::= expr(B) . { value_num(value, B.value); }

/* Debug Toggle Switch */
pbits        ::= DEBUG_PARSER . {
  if(!tdebug) {
    tdebug = 1;
%ifdef MATH_NORMAL
    ParseTrace(stdout, "expr: ");
%endif
%ifdef MATH_NONE
    ParseNoOpTrace(stdout, "expr: ");
%endif
  } else {
    tdebug = 0;
%ifdef MATH_NORMAL
    ParseTrace(NULL, "expr: ");
%endif
%ifdef MATH_NONE
    ParseNoOpTrace(NULL, "expr: ");
%endif
  }
}

/* Numeric Embedded Arithmetic Functions */
expr(A)    ::= state(B).            { token_copy(&A,&B);            }
number(A)  ::= NUM(B).              { token_copy(&A,&B);            }
number(A)  ::= TOK_PI(B).           { token_value(&A, M_PI, B.col);      }
/*number(A)  ::= TOK_E(B).            { token_value(&A, M_E, B.col);       }*/
number(A)  ::= MINUS(C) number(B) . { token_value(&A, -B.value, C.col);  }

number(A)     ::= blackboard(B) .      { token_copy(&A,&B); }
number(A)     ::= header(B) .          { token_copy(&A,&B); }
number(A)     ::= key(B) .             { token_copy(&A,&B); }
blackboard(A) ::= BLACKBOARD(B) . { 
  if(!token_var(&A, BLACKBOARD, B.str, B.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_BLACKBOARD_VARIABLE);
    value->str = B.str;
  } else {
    if(A.type == STRING) {
      Token *t;
      t = token_to_token_list(&A);
      token_copy(&A, t);
      FREE(t);
    }
  }
}
header(A)     ::= HEADER(B) .     { 
  if(!token_var(&A, HEADER, B.str, B.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE);
    value->str = B.str;
  } 
}
key(A)        ::= VARIABLE(B) .   { 
  if(!token_var(&A, VARIABLE, B.str, B.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE);
    value->str = B.str;
  }
}

state(A)   ::= number(B).                     { token_copy(&A,&B);        }
state(A)   ::= func(B).                       { token_copy(&A,&B);    }
state(A)   ::= MINUS(C) func(B).            { token_value(&A,-B.value, C.col);  }

state(A)   ::= state(B) PLUS  state(C).      { token_value(&A, B.value + C.value, B.col); }
state(A)   ::= state(B) MINUS state(C).      { token_value(&A, B.value - C.value, B.col); }
state(A)   ::= state(B) TIMES state(C).      { token_value(&A, B.value * C.value, B.col); }

state(A)   ::= LPAREN state(B) RPAREN.        { token_copy(&A,&B);       }

num_or_pstate(A) ::= number(B).               { token_copy(&A,&B);}
num_or_pstate(A) ::= LPAREN state(B) RPAREN.  { token_copy(&A,&B);}

func(A)    ::= SIN(E) num_or_pstate(B) .   { token_value(&A, sin( B.value ), E.col);  }
func(A)    ::= COS(E) num_or_pstate(B) .   { token_value(&A, cos( B.value ), E.col);  }
func(A)    ::= TAN(E) num_or_pstate(B) .   { token_value(&A, tan( B.value ), E.col);  }

func(A)    ::= SINH(E) num_or_pstate(B) .  { token_value(&A, sinh( B.value ), E.col); }
func(A)    ::= COSH(E) num_or_pstate(B) .  { token_value(&A, cosh( B.value ), E.col); }
func(A)    ::= TANH(E) num_or_pstate(B) .  { token_value(&A, tanh( B.value ), E.col); }

func(A)    ::= ASIN(E) num_or_pstate(B) .  { token_value(&A, asin( B.value ), E.col); }
func(A)    ::= ACOS(E) num_or_pstate(B) .  { token_value(&A, acos( B.value ), E.col); }
func(A)    ::= ATAN(E) num_or_pstate(B) .  { token_value(&A, atan( B.value ), E.col); }

func(A)    ::= ABS(E)   num_or_pstate(B) .  { token_value(&A, fabs(B.value), E.col);   }
func(A)    ::= FLOOR(E) num_or_pstate(B) .  { token_value(&A, floor(B.value), E.col);  }
func(A)    ::= CEIL(E)  num_or_pstate(B) .  { token_value(&A, ceil(B.value), E.col);   }
func(A)    ::= EXPON(E) num_or_pstate(B) .  { token_value(&A, exp(B.value), E.col);    }
func(A)    ::= ROUND(E) num_or_pstate(B) .  { token_value(&A, round(B.value), E.col);  }

state(A)    ::= state(B) EXP    state(C).      { 
  if(B.value < 0.0 && fabs(round(C.value) - C.value) >= 1e-15) {
    parse_error(value, TOKEN_STATUS_ERROR_NEGATIVE_FRACTION_POWER);
  } else {
    token_value(&A, pow(B.value, C.value), B.col);
  }
}

func(A)    ::= ATAN2(E) LPAREN state(B) COMMA state(C) RPAREN . { 
  token_value(&A, atan2( B.value, C.value ), E.col); 
}
func(A)    ::= LENGTH(E) LPAREN state(B) COMMA state(C) RPAREN . { 
  token_value(&A, sqrt( B.value*B.value + C.value*C.value ), E.col); 
}
func(A)    ::= LOGN(E)  num_or_pstate(B) .  { 
  if(B.value > 0.0) {
    token_value(&A, log(B.value), E.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
func(A)    ::= LOG10(E) num_or_pstate(B) . { 
  if(B.value > 0.0) {
    token_value(&A, log10(B.value), E.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
state(A)    ::= state(B) DIVIDE state(C).     { 
    if(C.value != 0.0) {
      token_value(&A, B.value / C.value, B.col);
    } else {
      parse_error(value, TOKEN_STATUS_ERROR_DIVIDE_BY_ZERO);
    } 
  }
func(A)    ::= SQRT(E) num_or_pstate(B) . { 
  if(B.value >= 0.0) {
    token_value(&A, sqrt ( B.value ), E.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_SQRT_NEGATIVE);
  }
}

/* Comma or not to Comma, that is the question */
commas ::= COMMA .
commas ::= .

/* Minimum and Maximum Functions: max(,,,)  min(,,,) */
/*
list(A) ::= LPAREN state(B).  { 
  A.value = B.value; 
  A.next  = NULL;
}
list(A) ::= list(B) COMMA state(C). { token_append(&A,&B,&C); }
*/
/* max(#,#,#) with parens and commas, can use anything between commas */
/*
func(A) ::= MAX(E) list(B) RPAREN. { 
  token_value(&A, token_foreach(&B, token_max), E.col);
  token_free(B.next);
}
*/
/* min(#,#,#) with parens and commas, can use anything between commas */
/*
func(A) ::= MIN(E) list(B) RPAREN. {
  token_value(&A, token_foreach(&B, token_min), E.col);
  token_free(B.next);
}
*/
/* max # # # without parens and optional commas, 
   limited to numbers, list math [sac_math], 
   headers, blackboard, and macro variables,
   and constants */
sac_math(A) ::= MINIMUM(E) xlist(B) . {
  token_value(&A, token_foreach(&B, token_min), E.col);
  token_free(B.next);
}
/* max # # # without parens and optional commas, 
   limited to numbers, list math [sac_math], 
   headers, blackboard, and macro variables,
   and constants */
sac_math(A) ::= MAXIMUM(E) xlist(B) . {
  token_value(&A, token_foreach(&B, token_max), E.col);
  token_free(B.next);
}

/* SAC Regular Arithmetic Functions ( ) */  
state(A) ::= LPAREN sac_math(B) RPAREN . { token_copy(&A,&B); }
expr(A)  ::= sac_math(B) . { token_copy(&A,&B); }

list_item(A) ::= number (B). { token_copy(&A,&B); }
list_item(A) ::= LPAREN state(B) RPAREN . { token_copy(&A,&B); }
xlist(A)     ::= list_item(B) . { token_copy(&A,&B); }
xlist(A)     ::= xlist(B) commas list_item(C) . { token_append(&A,&B,&C); }

sac_math(A) ::= ADD(E) xlist(B) . {token_value(&A, token_foreach(&B, token_add), E.col); }
sac_math(A) ::= SUB(E) xlist(B) . {token_value(&A, token_foreach(&B, token_sub), E.col); }
sac_math(A) ::= MUL(E) xlist(B) . {token_value(&A, token_foreach(&B, token_mul), E.col); }
sac_math(A) ::= DIV(E) xlist(B) . {token_value(&A, token_foreach(&B, token_div), E.col); }

sac_math(A) ::= GETTIME(E) MINIMUM .       { 
  if(!gettime_expr(&A, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    A.col = E.col;
  }
}
sac_math(A) ::= GETTIME(E) .       { 
  if(!gettime_expr(&A, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    A.col = E.col;
  }
}
sac_math(A) ::= GETTIME(E) MAXIMUM .       { 
  if(!gettime_expr(&A, NULL, TRUE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    A.col = E.col;
  }
}
sac_math(A) ::= GETTIME(E) MINIMUM state(B) . { 
  if(!gettime_expr(&A, &B, FALSE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    A.col = E.col;
  }
}
sac_math(A) ::= GETTIME(E) MAXIMUM state(B) . { 
  if(!gettime_expr(&A, &B, TRUE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    A.col = E.col;
  }
}


func(A) ::= INTEGER(E)    num_or_pstate(B). { token_value(&A, (int) B.value, E.col);   }
func(A) ::= POWER(E)      num_or_pstate(B). { token_value(&A, pow(10,B.value), E.col); }
func(A) ::= ALOG(E)       num_or_pstate(B). { 
  if(B.value > 0.0) {
    token_value(&A, log(B.value), E.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
func(A) ::= ALOG10(E) num_or_pstate(B).     { 
  if(B.value > 0.0) {
    token_value(&A,log10(B.value), E.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}


/* String Functions */
expr(A) ::= fstring(B) .         { token_copy(&A,&B); }

string(A) ::= STRING(B) .        { token_string(&A, B.str, B.col); }
string(A) ::= QUOTED_STRING(B) . { token_string(&A, B.str, B.col); }
string(A) ::= ESCAPE_STRING(B) . { token_string(&A, B.str, B.col); }
string(A) ::= number(B) .           { 
  if(B.type == NUM) {
    char *s;
    if(floor(B.value) == B.value) {
      asprintf(&s, "%d", (int)B.value);
    } else {
      asprintf(&s, "%g", B.value);
    }
    token_string(&A, s, B.col); 
  } else {
    token_string(&A, B.str, B.col);
  }
}

/* String List */
string_list(A) ::= .             { token_string(&A, strdup(""), lexer_col()); }
string_list(A) ::= string_list(B) string(C). { 
  if(B.str && C.str) {
    string *s = string_new(B.str);
    string_append(s, C.str);
    token_string(&A, strdup(string_string(s)), B.col);
    string_free(&s);
    FREE(B.str);
    FREE(C.str);
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
  }
}

/* String are always enclosed in  ( ... ) */
string(A) ::= LPAREN fstring(B) RPAREN . {  token_copy(&A,&B); }

fstring(A) ::= CHANGE(E) string(B) string(C) string(D). {
  string *s = string_new( D.str );
  s = string_replace(s, B.str, C.str );
  token_string(&A, strdup(string_string(s)), E.col);
  string_free(&s);
  FREE(B.str);
  FREE(C.str);
}
fstring(A) ::= SUBSTRING(E) number(B) number(C) string(D) . {
  string *s = string_new(D.str);
  string *s2 = string_substr(s, (int)B.value-1, (int)C.value-(int)B.value+1);
  token_string(&A, strdup(string_string(s2)), E.col);
  string_free(&s);
  string_free(&s2);
  FREE(D.str);
}
fstring(A) ::= DELETE(E) string(B) string(C) . {
  string *s = string_new(C.str);
  string_replace(s, B.str, "");
  token_string(&A, strdup(string_string(s)), E.col);
  string_free(&s);
  FREE(B.str);
  FREE(C.str);
}

fstring(A) ::= CONCAT(E) string_list(C) . { C.col = E.col; token_copy(&A, &C); }

fstring(A) ::= BEFORE(E) string(B) string(C). {
  string *s = string_new(C.str);
  char *f = strstr(C.str, B.str);
  string_remove(s, f-C.str, -1);
  token_string(&A, strdup(string_string(s)), E.col);
  string_free(&s);
  FREE(B.str);
  FREE(C.str);
}
fstring(A) ::= AFTER(E) string(B) string(C) . {
  string *s = string_new(C.str);
  char *f = strstr(C.str, B.str);
  string_remove(s, 0, (f-C.str)+strlen(B.str));
  token_string(&A, strdup(string_string(s)), E.col);
  string_free(&s);
  FREE(B.str);
  FREE(C.str);
}
fstring(A) ::= REPLY(E) string(B) . {
  char *a, *b, *def;
  char in[1024];
  memset(in, 0, sizeof(in));
  fprintf(stdout, "%s", B.str);
  a = strrchr(B.str, '[');
  b = strrchr(B.str, ']');
  if(a && b && a < b) {
    a++;
    def = (char *)malloc(sizeof(char) * (b-a)+1);
    strncpy(def, a, b-a);
    def[b-a] = 0;
  } else {
    def = strdup("");
  }
  if(fgets(in, 1024, stdin) == NULL) {
    parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
  } else {
    rstrip(in);
    if(strlen(in) <= 0) {
      if(isnumx(def)) {
        token_value(&A, atof(def), E.col);
      } else {
        token_string(&A, strdup(def), E.col);
      }
    } else {
      if(isnumx(in)) {
        token_value(&A, atof(in), E.col);
      } else {
        token_string(&A, strdup( in ), E.col);
      }
    }
  }
  FREE(def);
}
fstring(A) ::= FTOA(E) state(B) . { 
  char *s;
  asprintf(&s, "%g", B.value); 
  token_string(&A, s, E.col);
}
