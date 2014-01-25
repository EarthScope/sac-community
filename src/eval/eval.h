
#ifndef __EVAL_H__
#define __EVAL_H__

#include "token.h"

#define UNUSED(x) (void) x
#define FREE(x) do { \
    if(x) {          \
      free(x);       \
      x = NULL;      \
    }                \
  } while(0);

#ifndef YYSTYPE
typedef union {
  double    dval;
  char     *sval;
  struct symtab *symp;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

/* extern YYSTYPE yylval; */
YYSTYPE yylval; 

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

int    yylex(void);

YY_BUFFER_STATE  yy_scan_string(const char *);

void   lexer_state();
void   lexer_reset();
int    lexer_line();
int    lexer_col();

void   token_free(Token *t);
double token_add(Token *t, double v);
double token_sub(Token *t, double v);
double token_mul(Token *t, double v);
double token_div(Token *t, double v);
double token_max(Token *t, double v);
double token_min(Token *t, double v);
double token_print(Token *t, double v);
double token_foreach(Token *t, double (*func)(Token *i, double v));
 
#endif /* __EVAL_H__ */
