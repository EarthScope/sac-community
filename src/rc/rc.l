
%option nounput

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rc.h"
#include "rc.tab.h"

  extern YYSTYPE yylval;

  int yyparse(void *);  
  void yyerror(const char *c);
  
  #define YY_EXIT_FAILURE 2
  #define YY_FATAL_ERROR(msg) yy_fatal_error_new(msg)
  void yy_fatal_error_new(char *s);
  
  int linebyline       = 0;
  int lineno           = 0;
  int verbose          = 0;
  char *current_line   = NULL;
  int tokenpos         = 0;
  int startline        = -1;

#define ZZ     { tokenpos += strlen(yytext); }
#define __S__  { yylval.s = strdup(yytext);  ZZ; }
#define __I__  { yylval.i = atoll(yytext);   ZZ; }
#define __R__  { yylval.d = atof(yytext);    ZZ; }

%}

COMMENT  [#;]
WORD     [a-zA-Z0-9]+
CAPITAL  [A-Z0-9]
DIGIT    [0-9]
QUOTE    \"
INTEGER  [-+]?{DIGIT}+

STRING   [a-zA-Z0-9][a-zA-Z0-9/\.\*\?\[\]\{\}{\(\)\_\-\\]*
QSTRING  {QUOTE}[a-zA-Z0-9][\ a-zA-Z0-9/\.\*\?\[\]\{\}{\(\)\_\-\\]*{QUOTE}


NUMBER [-+]?(({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}+)|({DIGIT}+))([eE][+-]?{DIGIT}+)?

VALUEON   "on"|"true"|"yes"
VALUEOFF  "off"|"false"|"no"|"of"

EQUALS    "="

%%

{EQUALS}    ZZ; return EQUALS;

{VALUEON}   ZZ; return BOOL_TRUE;
{VALUEOFF}  ZZ; return BOOL_FALSE;

{INTEGER}   __I__ return INTEGER;

{NUMBER}    __R__ return NUMBER;

{QSTRING}   __S__ return STRING;

{STRING}    __S__ return STRING;


{COMMENT}.*      ZZ;
" "              ZZ;

<<EOF>>                 { yyterminate(); }

\n.*                   {
  /* NEWLINE 
     Grab the Newline and the entire next line
     Save the entire next line into the current_line variable
     Return all but the first character, (the newline to the parser)
     Lex and Yacc, Levine, J. R., T. Mason, and D. Brown, pp. 246-247
  */
                          free(current_line);
                          current_line = strdup(yytext+1);
			  lineno++;
			  tokenpos = 0;
			  if(linebyline) {
			    fprintf(stderr, "%8d: <%s>\n",
				    lineno, current_line);
			  }
			  /* Return all but one character to the input stream */
			  yyless(1);
                        }

.                    { 
  /* ERRORS 
       This should eat up and spit out 
       any non-recognized characters
  */
                           ZZ;
                           yyerror(yytext); 
			   yyterminate(); 
   }

%%

void
yyerror(const char *c) {
  if(strcmp("parse error", c) != 0) {
    fprintf(stdout, "Unrecognized character string at line %d column %d: "
	    "-->%s<--\n%s\n", 
	    lineno, tokenpos, c, current_line);  
    fprintf(stdout, "%*s\n", tokenpos, "^");
  } else {
    fprintf(stderr, "Unable to interpret line %d: <%s>\n", 
	    lineno,current_line);
  }
}

void
yy_fatal_error_new(char *s) {
   yy_fatal_error(s);
}

int yywrap() { return 1; }

void
first_line(FILE *fp) {
  char c[512];
  fgets(&c[0], 512, fp);
  rewind(fp);
  
  lineno = 1;
  current_line = strdup(c);
  current_line[strlen(current_line)-1] = '\0';
  if(linebyline > 0 && current_line) {
    fprintf(stderr, "%8d: <%s>\n", lineno, current_line);
  }
  tokenpos = 0;
}

int
rc_read(char *file, rc_function *func)  {
  /* Set yyin as the input stream */
  yyin = fopen(file, "r");
  if(yyin == NULL) {
    fprintf(stderr, "Error opening $HOME/.sacrc file for reading\n");
    return FALSE;
  }
  rc_set_function(func);
  first_line(yyin);
  if(yyparse(yyin)) {
    return FALSE;
  }
  fclose(yyin);
  return TRUE;
}

int
rc_debug(int verbose_level, int output_lines, int parse_debug) {
  verbose    = verbose_level;
  linebyline = output_lines;
  yy_flex_debug = parse_debug;
  return TRUE;
}


int rc_linenumber() { return lineno; }
char *rc_line()     { return current_line; }
int rc_verbose()    { return verbose; }
