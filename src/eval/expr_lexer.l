
%option nounput
%option case-insensitive

%{

#define YY_NO_INPUT

#define ESCAPE_CHAR '@'
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "expr_parse.h"
#include "token.h"
#include "eval.h"
#include "string_utils.h"

#include "bot.h"
#include "cpf.h"

void * ParseAlloc(void *(*mallocProc)(size_t));
void   ParseFree(void *p, void (*freeProc)(void*));
void   Parse(void *yyp, int yymajor, Token token, Token *value);
void   ParseNoOp(void *yyp, int yymajor, Token token, Token *value);

char * lex_replace(char *t, int *err);

int lineno    = 1;
int col     = 1;
int parens  = 0;
int strfunc = 0;
char *input_line = NULL;

int string_escaped = 0;

void yyerror(char *s, char *token);

#define STR do { strfunc = parens; BEGIN(notmath); } while(0)
char *unescape(char *in);

int
is_reduced(int x) {
  static int reduce = 0;
  if(x >= 0) {
    reduce = x;
  }
  return reduce;
}


/*
^" "*(addf|convert|deletechannel|divf|getbb|mulf|pickauthor|pickphase|plotalpha|read[a-z]*|saveimg|setmacro|subf|transcript|transfer|write[a-z]*) {
  fprintf(stderr, "%s\n", yytext);
  return STRING;
 }

<math>e          {  return TOK_E;     }


space \\" "
back  \\

%s notmathx
<notmathx>{words}+ {
  yylval.sval = unescape(yytext);
  return STRING;
}
<notmathx>{wilds}+ {
  return WILD;
}


*/
%}

%s evaluate
%s math
%s notmath
%s notmath_do

esc   @
alnum [0-9a-zA-Z_.]
word  [0-9a-zA-Z_.]
wild  [0-9a-zA-Z_.?*]
space {esc}" "
back  \\
forw  "/"
brak  (\[|\])

words ({word}|{space}|{back}|{forw}|{brak})
wilds ({wild}|{space}|{back}|{forw}|{brak})
alnums ({word}|{space}|{back})

%%

  /* Number # #.# */
[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {
  return NUM;
}

  /* Number #. #.e# */
[0-9]+\.([eE][-+]?[0-9]+)? {
  return NUM;
}

  /* whitespace, ignore  */
[ \t]   {
      col += (int) strlen(yytext);
}
__debug_parser__            { return DEBUG;      }

  /* Variables */
"$"[0-9a-zA-Z_]+"$"? {  is_reduced(TRUE); return VARIABLE; }
"%"[0-9a-zA-Z_]+"%"? {  is_reduced(TRUE); return BLACKBOARD; }
"&"([^, ]|{space})+,[0-9a-zA-Z]+"&"? {  is_reduced(TRUE); return HEADER; }

<math>to           { return TO; }
<math>as           { return AS; }
<math>float     { return FLOAT; }
<math>integer { return INTEGER; }

<math>"**"  {   return EXP;       }
<math>"^"   {   return EXP;       }
<math>"+"   {   return PLUS;      }
<math>"*"   {   return TIMES;     }
<math>"/"   {   return DIVIDE;    }
<notmath_do>"="        {   return EQUALS;    }
<notmath_do>(">="|ge)  {   return GE;    }
<notmath_do>("<="|le)  {   return LE;    }
<notmath_do>(">"|gt)   {   return GT;    }
<notmath_do>("<"|lt)   {   return LT;    }
<notmath_do>("=="|eq)  {   return EQ;    }
<notmath_do>("!="|ne)  {   return NE;    }

<math>"-"         {   return MINUS;     }
","               {   return COMMA;     }

<notmath,math>"(" {
  is_reduced(TRUE);
  parens++;
  BEGIN(math);
  return LPAREN;
}

")"         {
  parens--;
  if(strfunc && parens < strfunc) {
    BEGIN(math);
  }
  if(!parens){
    BEGIN(notmath);
  }
  return RPAREN;
}


<math>sin         {  return SIN;        }
<math>cos         {  return COS;        }
<math>tan         {  return TAN;        }

<math>asin        {  return ASIN;       }
<math>acos        {  return ACOS;       }
<math>atan        {  return ATAN;       }
<math>atan2       {  return ATAN2;      }

<math>sinh        {  return SINH;       }
<math>cosh        {  return COSH;       }
<math>tanh        {  return TANH;       }

<math>sqrt        {  return SQRT;       }

<math>abs|fabs    {  return ABS;        }

<math>norm|len    {  return LENGTH;     }

<math>exp         {  return EXPON;      }
<math>log|ln      {  return LOGN;       }
<math>log10       {  return LOG10;      }

<math>pi          {  return TOK_PI;         }

<math>floor       {  return FLOOR;      }
<math>ceil        {  return CEIL;       }
<math>round       {  return ROUND;      }

<math>max         {  return MAXIMUM;    }
<math>min         {  return MINIMUM;    }
<math>maximum     {  return MAXIMUM;    }
<math>minimum     {  return MINIMUM;    }

<math>cha|change  {  STR; return CHANGE;     }
<math>del|delete  {  STR; return DELETE;     }
<math>bef|before  {  STR; return BEFORE;     }
<math>aft|after   {  STR; return AFTER;      }
<math>rep|reply   {  STR; return REPLY;      }
<math>substring   {  STR; return SUBSTRING;  }
<math>str|string  {  STR; return FTOA;       }
<math>conc        {  STR; return CONCAT;     }
<math>concatenate {  STR; return CONCAT;     }
<math>concat     {  STR; return CONCAT;     }

<math>sine       {  return SIN;       }
<math>arcsine    {  return ASIN;    }
<math>asine      {  return ASIN;    }
<math>cosine     {  return COS;     }
<math>arccosine  {  return ACOS;  }
<math>acosine    {  return ACOS;  }
<math>tangent    {  return TAN;    }
<math>arctangent {  return ATAN; }
<math>atangent   {  return ATAN; }
<math>alog       {  return ALOG;       }
<math>power      {  return POWER;      }
<math>alog10     {  return ALOG10;     }
<math>absolute   {  return ABS;   }

<math>add        {  return ADD; }
<math>subtract   {  return SUB; }
<math>multiply   {  return MUL; }
<math>divide     {  return DIV; }

<math>gettime    { return GETTIME; }

  /* eval or evaluate to beginning of a line */
^" "*(eval|evaluate)    { 
   BEGIN(math); 
   parens++; 
   yylval.sval = strdup(lstrip(yytext));
   return EVALUATE; 
}

  /* Newline */
\n      {
  col = 0;
  ++lineno;
  BEGIN(notmath);
  return NEWLINE;
}

  /* Double quote string */
\"([^\"\n]|{esc}\")*\"? {
  int err;
  char *t;

  t = strdup(yytext+1);
  t[strlen(t)] = 0;
  if(t[strlen(t)-1] == '"') { /* " */
     t[strlen(t)-1] = 0;
  }
  t = lex_replace(t, &err);
  string_escaped = FALSE;
  if(err) {
    return err;
  }
  yylval.sval = t;
  return QUOTED_STRING;
}

  /* Single quote string */
\'([^\'\n]|{esc}\')*\'? {
  int err;
  char *t;
  t = strdup(yytext+1);
  t[strlen(t)-1] = 0;
  if(t[strlen(t)-1] == '\'') { /* ' */
    t[strlen(t)-1] = 0;
  }
  t = lex_replace(t, &err);
  string_escaped = FALSE;
  if(err){
    return err;
  }
  yylval.sval = t;
  return QUOTED_STRING;
}

<math>{alnums}+ {
  return STRING;
}

  /* DO | IF | WHILE | ELSEIF at beginning of a line */
^" "*(do|if|while|elseif) {
  BEGIN(notmath_do);
  yylval.sval = strdup(lstrip(yytext));
  return STRING;
 }

  /* String during a loop/conditional construct */
<notmath_do>([^ =,<>!()]|{space})+ {
  int err;
  char *t;
  if(isnum(yytext)) {
    return NUM;
  }
  t = strdup(yytext);
  t = lex_replace(t, &err);
  if(err) {
    string_escaped = FALSE;
    return err;
  }
  yylval.sval = t;
  return STRING;
}

  /* Simple string, with escaped special characters  */
<notmath>([^ ()\"\']|{space}|{esc}\(|{esc}\)|{esc}%|{esc}$|{esc}&|{esc}\"|{esc}\')+ {
  int err;
  char *t;
  if(isnum(yytext)) {
    return NUM;
  }
  t = strdup(yytext);
  t = lex_replace(t, &err);
  if(err) {
    string_escaped = FALSE;
    return err;
  }
  yylval.sval = t;
  if(string_escaped) {
    string_escaped = FALSE;
    return ESCAPE_STRING;
  }
  string_escaped = FALSE;
  return STRING;
}

  /* Anything else */
.       {

  return -1;
}

%%

char *
lex_replace(char *t, int *err) {
  *err = 0;
  if(!(t = replace(t, '$'))) {
    *err = -1014;
    goto REPLACE_ERROR;
  }
  if(!(t = replace(t, '%'))) {
    *err = -1201;
    goto REPLACE_ERROR;
  }
  if(!(t = replace(t, '&'))) {
    *err = -1337;
    goto REPLACE_ERROR;
  }
  string_escaped = FALSE;
  unescape(t);
 REPLACE_ERROR:
  return t;
}

void lexer_state() { 
  printf("lexer: line: %d col: %d\n", lineno, col);
}

char *lexer_input() { return input_line; }
int lexer_line() { return lineno; }
int lexer_col()  { return col;  }

void lexer_reset_store(void) {
  yylval.dval = 0.0;
  FREE(yylval.sval);
}
/**
 * reset the line and column count
 *
 *
 */
void lexer_reset(void) {
  lineno = 1;
  col  = 1;
  lexer_reset_store();
  yylval.sval = NULL;
}


/**
 * yyerror() is invoked when the lexer or the parser encounter
 * an error. The error message is passed via *s
 */
void yyerror(char *s, char *token) {
  printf("error: %s at line: %d col: %d '%s'\n", s, lineno, col, token);
}

int yywrap(void) {
  return 1;
}

static char *token_msg[] = {
  "Successful: ",
  "Error: ",
  "Syntax error: ",
  "Illegal Dividsion by zero: ",
  "Illegal Square Root of Negative Number: ",
  "Illegal Log of a Negative Number or Zero: ",
  "Illegal power of a negative number to a fractional power: ",
  "Unexpected character: ",
  " ERROR 1201: Could not find BLACKBOARD variable: ",
  " ERROR 1201: Could not find MACRO variable: ",
  " ERROR 1201: Could not find HEADER value: ",
};

static void
chomp(char *str) {
  if(str[strlen(str)-1] == '\n') {
    str[strlen(str)-1] = 0;
  }
}

int token_ok(Token *t)    {  return t->error == TOKEN_STATUS_OK; }
int token_error(Token *t) {  return t->error >= TOKEN_STATUS_ERROR; }

void
token_free(Token *t) {
  Token *p;
  while(t) {
    p = t->next;
    FREE(t->str);
    FREE(t);
    t = p;
  }
}


Token *
token_new(int type, double value, char *str, int lineno, int col) {
  Token *t = (Token *) malloc(sizeof(Token));
  t->type  = type;
  t->value = value;
  t->str   = NULL;
  if(str) {
    t->str = strdup(str);
  }
  t->next  = NULL;
  t->error = TOKEN_STATUS_UNKNOWN;
  t->col   = col;
  t->line  = lineno;
  return t;
}


Token * 
token_last(Token *t) {
  Token *tmp;
  tmp = t;
  while(tmp->next) {
    tmp = tmp->next;
  }
  return tmp;
}

void
token_set(Token *t, int type, double value, char *str, int line, int col) {
  t->type = type;
  t->line = line;
  t->col  = col;
  t->next = NULL;
  t->str  = NULL;
  if(str) {
    t->str  = strdup(str);
  }
  if(type == NUM) {
    t->value = value;
  }
}

static int no_op_order = 0;
int
set_op_order(int want_order) {
  if(want_order >= 0) {
    no_op_order = want_order;
  }
  return no_op_order;
}

Token *
parse(eval *e) {
  void *parser;
  Token tok;
  Token *state;
  int val;

  yylval.sval = NULL;
  lexer_reset();
  input_line = e->input;
  parser = ParseAlloc (malloc);

  e->processed = is_reduced(0);
  BEGIN(notmath);
  parens = 0;
  state = token_new(0, 0, NULL, 0, 0);
  yy_scan_string( e->input );
  while( (val = yylex()) != 0 ) {
    if(val < 0) {
      if(val == -1201) {
        state->error = TOKEN_STATUS_ERROR_UNKNOWN_BLACKBOARD_VARIABLE;
      }
      if(val == -1337) {
        state->error = TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE;
      }
      if(val == -1014) {
        state->error = TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE;
      }
      goto error;
    }
    e->status = EVAL_STATUS_PARSING;
    if(!yylval.sval) {
      yylval.sval = strdup(yytext);
    }
    token_set(&tok, val, atof(yylval.sval), yylval.sval, lexer_line(), lexer_col());
    col += (int) strlen(yytext);

    FREE(yylval.sval);
    /* Parse the current token */
    state->error = -1;
    if(no_op_order) {
      ParseNoOp(parser, tok.type, tok, state);
    } else {
      Parse(parser, tok.type, tok, state);
    }
    if( token_error(state) ) { /* Syntax Error */
      goto error;
    }
    lexer_reset_store();
  }
  if(no_op_order) {
    ParseNoOp(parser, 0, tok, state);
  } else {
    Parse(parser, 0,       tok, state);
  }
  e->processed = is_reduced(-1);
 error:
  /* Check for Unexpected Characters */
  if(! token_ok(state) ) {
    if(state->error == TOKEN_STATUS_UNKNOWN) {
       state->error = TOKEN_STATUS_ERROR_UNEXPECTED_CHARACTER;
       state->col = col+1;
    }
    eval_syntax_error(e, state);
  }

  yy_delete_buffer(YY_CURRENT_BUFFER);
  parens = 0;
  BEGIN(notmath);
  ParseFree(parser, free);

  return state;
}

void
eval_clear(eval *e) {
  e->type   = EVAL_OUTPUT_TYPE_UNKNOWN;
  e->result = 0.0;
  e->status = EVAL_STATUS_EMPTY;

  FREE(e->str);
  FREE(e->error_message);
  FREE(e->input);
}

void
eval_free(eval *e) {
  eval_clear(e);
  free(e);
  e = NULL;
}

eval *
eval_new() {
  eval *e;
  e = (eval *) malloc(sizeof(eval));
  if(!e) {
    return NULL;
  }
  e->str           = NULL;
  e->error_message = NULL;
  e->input         = NULL;
  e->processed     = FALSE;
  eval_clear(e);
  return e;
}

void
eval_input(eval *e, char *input) {
  e->input = strdup(input);
  chomp(e->input);
}


void
eval_eval(eval *e) {
  Token *out;

  if(strlen(e->input) == 0) {
    e->status = EVAL_STATUS_EMPTY;
  }

  out = parse(e);
  if(!out) {
    goto error;
  }
  if(e->status == EVAL_STATUS_OK) {
    e->result = out->value;
    e->type   = out->type;
    if(e->type == EVAL_OUTPUT_TYPE_STRING) {
      e->str = strdup(out->str);
    }
  }
 error:  
  if(out) {
    token_free(out);
  }
  out = NULL;
}

eval *
eval_compute(char *input) {
  eval *e;
  e = eval_new();
  eval_input(e, input);
  eval_eval(e);
  return e;
}

void
eval_syntax_error(eval *e, Token *t) {
  switch(t->error) {
  case TOKEN_STATUS_ERROR_SYNTAX:
  case TOKEN_STATUS_ERROR_UNEXPECTED_CHARACTER:
    asprintf(&(e->error_message),
             "%s%s\n"
             "%*s\n",
             token_msg[t->error], e->input,
             t->col + (int) strlen(token_msg[t->error]) -1, "^");
    break;
  case TOKEN_STATUS_ERROR_UNKNOWN_BLACKBOARD_VARIABLE:
  case TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE:
  case TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE:
  case TOKEN_STATUS_ERROR_GETTIME:
    break;
  default:
    asprintf(&(e->error_message), 
             "%s%s\n",
             token_msg[t->error], e->input);
    break;
  }
  e->status = EVAL_STATUS_SYNTAX_ERROR;
}

double token_add(Token *t, double v) { return v + t->value; }
double token_sub(Token *t, double v) { return v - t->value; }
double token_mul(Token *t, double v) { return v * t->value; }
double token_div(Token *t, double v) { return v / t->value; }
double token_max(Token *t, double v) { return (t->value > v) ? t->value : v; }
double token_min(Token *t, double v) { return (t->value < v) ? t->value : v; }
double token_print(Token *t, double v) { 
   UNUSED(v);
   switch(t->type) {
   case NUM : fprintf(stderr, "%f, ", t->value); break;
   case STRING : fprintf(stderr, "'%s', ", t->str); break;
   default: fprintf(stderr, "%d, ", t->type);
   }
   return 0;
}


double 
token_foreach(Token *t, double (*func)(Token *i, double v)) {
  double v;
  Token *p;
  v = t->value;
  p = t->next;
  while(p) {
    v = func(p, v);
    p = p->next;
  }
  return v;
}

char *
unescape(char *in) {
  int i;
  char *p, *s;
  s = in;
  p = (char *)malloc(sizeof(char *) * strlen(in));
  i = 0;
  while(s && *s) {
    if(*s != ESCAPE_CHAR) {
      p[i++] = *(s++);
    } else {
      switch(*(s+1)) {
      case ' ':
      case '@':
      case ';':
      case '"':
      case '\'':
      case '(':
      case ')':
      case '&':
      case '$':
      case '%':
        string_escaped = TRUE;
        is_reduced(TRUE);
        p[i++] = *(s+1); s+=2; break;
      default:
        p[i++] = *(s++);
        break;
      }
    }
  }
  p[i] = 0;
  strncpy(in,p,i);
  in[i] = 0;
  free(p);
  return in;
}

void
lexer_free() {
  yylex_destroy();
}
