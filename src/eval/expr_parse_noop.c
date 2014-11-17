/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 2 "expr_parse_noop.y"

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

#line 38 "expr_parse_noop.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseNoOpTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseNoOpTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseNoOpARG_SDECL     A static variable declaration for the %extra_argument
**    ParseNoOpARG_PDECL     A parameter declaration for the %extra_argument
**    ParseNoOpARG_STORE     Code to store %extra_argument into yypParser
**    ParseNoOpARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 94
#define YYACTIONTYPE unsigned short int
#define ParseNoOpTOKENTYPE Token
typedef union {
  int yyinit;
  ParseNoOpTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseNoOpARG_SDECL Token *value;
#define ParseNoOpARG_PDECL ,Token *value
#define ParseNoOpARG_FETCH Token *value = yypParser->value
#define ParseNoOpARG_STORE yypParser->value = value
#define YYNSTATE 183
#define YYNRULE 110
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (785)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     4,  183,   24,  164,   92,  176,  175,    8,    7,   82,
 /*    10 */   172,  171,  170,  163,  165,  132,    1,   71,   94,  130,
 /*    20 */    12,  172,  171,  170,   42,  113,  174,  169,  168,  167,
 /*    30 */    59,   58,   57,   56,   55,   54,   53,   52,   51,   50,
 /*    40 */    49,   48,   47,   46,   91,   90,   45,   44,   43,   23,
 /*    50 */    22,   21,   20,   19,   18,   84,   41,   40,   39,   37,
 /*    60 */    61,   35,   72,   32,   30,   28,    6,    4,  131,   24,
 /*    70 */   164,   89,   10,  175,   96,   95,   82,  172,  171,  170,
 /*    80 */   163,  165,  132,   85,   83,  125,  130,  116,  172,  171,
 /*    90 */   170,   42,  157,  174,  169,  168,  167,   59,   58,   57,
 /*   100 */    56,   55,   54,   53,   52,   51,   50,   49,   48,   47,
 /*   110 */    46,   91,   90,   45,   44,   43,   23,   22,   21,   20,
 /*   120 */    19,   18,   84,   41,   40,   39,    4,  114,   24,  164,
 /*   130 */    99,   97,  175,   33,   38,   82,  172,  171,  170,  163,
 /*   140 */   165,  132,   98,   88,  295,  130,  295,  172,  171,  170,
 /*   150 */    42,  156,  174,  169,  168,  167,   59,   58,   57,   56,
 /*   160 */    55,   54,   53,   52,   51,   50,   49,   48,   47,   46,
 /*   170 */    91,   90,   45,   44,   43,   62,  256,  256,  295,  175,
 /*   180 */   295,  295,   41,   40,   39,  295,   37,   61,   35,   72,
 /*   190 */    32,   30,   28,    6,  295,  295,  135,   42,  295,  174,
 /*   200 */   169,  168,  167,   59,   58,   57,   56,   55,   54,   53,
 /*   210 */    52,   51,   50,   49,   48,   47,   46,   91,   90,   45,
 /*   220 */    44,   43,  115,  295,   17,   16,   13,   15,   14,   41,
 /*   230 */    40,   39,    3,  295,   62,  295,  295,  295,  175,  182,
 /*   240 */   179,  178,  177,  111,  110,  109,  108,  107,  106,  105,
 /*   250 */   104,  103,   17,   16,   13,   15,   14,  102,  174,  169,
 /*   260 */   168,  167,    3,  295,   62,  257,  257,  295,  175,   11,
 /*   270 */   179,  178,  177,  111,  110,  109,  108,  107,  106,  105,
 /*   280 */   104,  103,   63,  295,  295,  135,  295,  102,  174,  169,
 /*   290 */   168,  167,   64,  295,   62,  295,  295,  295,  175,  295,
 /*   300 */   129,  128,  127,  294,   93,   25,  101,  180,  295,  100,
 /*   310 */   264,  264,  295,  295,  172,  171,  170,  295,  174,  169,
 /*   320 */   168,  167,    5,  295,   62,  295,  295,    2,  175,   62,
 /*   330 */   135,  295,  164,  175,  295,  295,  295,  295,   78,  172,
 /*   340 */   171,  170,  163,  295,   86,  295,  295,  295,  174,  169,
 /*   350 */   168,  167,  164,  174,  169,  168,  167,  295,   77,  172,
 /*   360 */   171,  170,  163,  158,   87,   17,   16,   13,   15,   14,
 /*   370 */   144,  295,   17,   16,   13,   15,   14,   17,   16,   13,
 /*   380 */    15,   14,  143,  295,   17,   16,   13,   15,   14,   62,
 /*   390 */   134,  295,  295,  175,    9,  295,  295,  172,  171,  170,
 /*   400 */   265,  265,  295,  134,   70,  133,  295,  295,  295,  295,
 /*   410 */   172,  171,  170,  174,  169,  168,  167,   69,  133,  134,
 /*   420 */   135,  266,  266,  134,  295,  295,  172,  171,  170,  295,
 /*   430 */   172,  171,  170,   68,  133,  134,  295,   67,  133,  134,
 /*   440 */   295,  135,  172,  171,  170,  295,  172,  171,  170,   66,
 /*   450 */   133,  164,  295,   65,  133,  295,  164,  112,  172,  171,
 /*   460 */   170,  163,  162,  172,  171,  170,  163,  295,  164,  267,
 /*   470 */   267,  295,  295,  164,  161,  172,  171,  170,  163,  160,
 /*   480 */   172,  171,  170,  163,  295,  164,  295,  295,  295,  135,
 /*   490 */   164,  159,  172,  171,  170,  163,   76,  172,  171,  170,
 /*   500 */   163,  164,  295,  295,  295,  295,  164,   75,  172,  171,
 /*   510 */   170,  163,   74,  172,  171,  170,  163,  164,  295,  295,
 /*   520 */   295,  295,  164,   73,  172,  171,  170,  163,   81,  172,
 /*   530 */   171,  170,  163,  164,  295,  295,  295,  295,  164,   80,
 /*   540 */   172,  171,  170,  163,   79,  172,  171,  170,  163,   17,
 /*   550 */    16,   13,   15,   14,  165,  295,  295,  295,  295,  295,
 /*   560 */   165,  172,  171,  170,  295,  155,  165,  172,  171,  170,
 /*   570 */   295,  154,  165,  172,  171,  170,  295,  153,  295,  172,
 /*   580 */   171,  170,  165,  152,  295,  295,  295,  165,  295,  172,
 /*   590 */   171,  170,  165,  151,  172,  171,  170,  165,  150,  172,
 /*   600 */   171,  170,  165,  149,  172,  171,  170,  165,  148,  172,
 /*   610 */   171,  170,  295,  147,  172,  171,  170,  165,  146,  295,
 /*   620 */   295,  295,  165,  295,  172,  171,  170,  165,  145,  172,
 /*   630 */   171,  170,  295,  142,  172,  171,  170,  165,  141,  295,
 /*   640 */   295,  295,  165,  295,  172,  171,  170,  165,  140,  172,
 /*   650 */   171,  170,  165,  139,  172,  171,  170,  165,  138,  172,
 /*   660 */   171,  170,  134,  137,  172,  171,  170,  126,  136,  172,
 /*   670 */   171,  170,  126,  295,  172,  171,  170,  117,  126,  172,
 /*   680 */   171,  170,  295,  126,   27,  172,  171,  170,  295,  124,
 /*   690 */   172,  171,  170,  295,  295,   34,  295,  126,  295,  295,
 /*   700 */   123,  126,  295,  295,  172,  171,  170,  295,  172,  171,
 /*   710 */   170,  295,  126,  295,  122,  295,  295,  126,   31,  172,
 /*   720 */   171,  170,  295,  126,  172,  171,  170,  295,  126,  121,
 /*   730 */   172,  171,  170,  295,   29,  172,  171,  170,  295,  126,
 /*   740 */   120,  295,  126,  295,  295,  119,  172,  171,  170,  172,
 /*   750 */   171,  170,  181,  180,  295,  295,   26,  295,  173,  118,
 /*   760 */   172,  171,  170,  295,  173,  172,  171,  170,  166,  295,
 /*   770 */    60,  172,  171,  170,  295,   36,  295,  172,  171,  170,
 /*   780 */   295,  295,  172,  171,  170,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,    0,    4,   74,   75,    1,    8,   51,   52,   80,
 /*    10 */    81,   82,   83,   84,   74,   86,   77,   78,   79,   90,
 /*    20 */     2,   81,   82,   83,   26,   85,   28,   29,   30,   31,
 /*    30 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*    40 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    50 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    60 */    62,   63,   64,   65,   66,   67,   68,    2,    1,    4,
 /*    70 */    74,   75,    2,    8,   25,   26,   80,   81,   82,   83,
 /*    80 */    84,   74,   86,   23,   24,    1,   90,    1,   81,   82,
 /*    90 */    83,   26,   85,   28,   29,   30,   31,   32,   33,   34,
 /*   100 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*   110 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   120 */    55,   56,   57,   58,   59,   60,    2,    1,    4,   74,
 /*   130 */    75,   10,    8,   92,   87,   80,   81,   82,   83,   84,
 /*   140 */    74,   86,   79,   90,   93,   90,   93,   81,   82,   83,
 /*   150 */    26,   85,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   160 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   170 */    46,   47,   48,   49,   50,    4,    0,    1,   93,    8,
 /*   180 */    93,   93,   58,   59,   60,   93,   61,   62,   63,   64,
 /*   190 */    65,   66,   67,   68,   93,   93,   20,   26,   93,   28,
 /*   200 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   210 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*   220 */    49,   50,    1,   93,    3,    4,    5,    6,    7,   58,
 /*   230 */    59,   60,    2,   93,    4,   93,   93,   93,    8,    9,
 /*   240 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*   250 */    20,   21,    3,    4,    5,    6,    7,   27,   28,   29,
 /*   260 */    30,   31,    2,   93,    4,    0,    1,   93,    8,   20,
 /*   270 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*   280 */    20,   21,   22,   93,   93,   20,   93,   27,   28,   29,
 /*   290 */    30,   31,    2,   93,    4,   93,   93,   93,    8,   93,
 /*   300 */    10,   11,   12,   70,   71,   72,   73,   74,   93,   76,
 /*   310 */     0,    1,   93,   93,   81,   82,   83,   93,   28,   29,
 /*   320 */    30,   31,    2,   93,    4,   93,   93,    2,    8,    4,
 /*   330 */    20,   93,   74,    8,   93,   93,   93,   93,   80,   81,
 /*   340 */    82,   83,   84,   93,   86,   93,   93,   93,   28,   29,
 /*   350 */    30,   31,   74,   28,   29,   30,   31,   93,   80,   81,
 /*   360 */    82,   83,   84,    1,   86,    3,    4,    5,    6,    7,
 /*   370 */     1,   93,    3,    4,    5,    6,    7,    3,    4,    5,
 /*   380 */     6,    7,    1,   93,    3,    4,    5,    6,    7,    4,
 /*   390 */    74,   93,   93,    8,   20,   93,   93,   81,   82,   83,
 /*   400 */     0,    1,   93,   74,   88,   89,   93,   93,   93,   93,
 /*   410 */    81,   82,   83,   28,   29,   30,   31,   88,   89,   74,
 /*   420 */    20,    0,    1,   74,   93,   93,   81,   82,   83,   93,
 /*   430 */    81,   82,   83,   88,   89,   74,   93,   88,   89,   74,
 /*   440 */    93,   20,   81,   82,   83,   93,   81,   82,   83,   88,
 /*   450 */    89,   74,   93,   88,   89,   93,   74,   80,   81,   82,
 /*   460 */    83,   84,   80,   81,   82,   83,   84,   93,   74,    0,
 /*   470 */     1,   93,   93,   74,   80,   81,   82,   83,   84,   80,
 /*   480 */    81,   82,   83,   84,   93,   74,   93,   93,   93,   20,
 /*   490 */    74,   80,   81,   82,   83,   84,   80,   81,   82,   83,
 /*   500 */    84,   74,   93,   93,   93,   93,   74,   80,   81,   82,
 /*   510 */    83,   84,   80,   81,   82,   83,   84,   74,   93,   93,
 /*   520 */    93,   93,   74,   80,   81,   82,   83,   84,   80,   81,
 /*   530 */    82,   83,   84,   74,   93,   93,   93,   93,   74,   80,
 /*   540 */    81,   82,   83,   84,   80,   81,   82,   83,   84,    3,
 /*   550 */     4,    5,    6,    7,   74,   93,   93,   93,   93,   93,
 /*   560 */    74,   81,   82,   83,   93,   85,   74,   81,   82,   83,
 /*   570 */    93,   85,   74,   81,   82,   83,   93,   85,   93,   81,
 /*   580 */    82,   83,   74,   85,   93,   93,   93,   74,   93,   81,
 /*   590 */    82,   83,   74,   85,   81,   82,   83,   74,   85,   81,
 /*   600 */    82,   83,   74,   85,   81,   82,   83,   74,   85,   81,
 /*   610 */    82,   83,   93,   85,   81,   82,   83,   74,   85,   93,
 /*   620 */    93,   93,   74,   93,   81,   82,   83,   74,   85,   81,
 /*   630 */    82,   83,   93,   85,   81,   82,   83,   74,   85,   93,
 /*   640 */    93,   93,   74,   93,   81,   82,   83,   74,   85,   81,
 /*   650 */    82,   83,   74,   85,   81,   82,   83,   74,   85,   81,
 /*   660 */    82,   83,   74,   85,   81,   82,   83,   74,   85,   81,
 /*   670 */    82,   83,   74,   93,   81,   82,   83,   89,   74,   81,
 /*   680 */    82,   83,   93,   74,   91,   81,   82,   83,   93,   91,
 /*   690 */    81,   82,   83,   93,   93,   91,   93,   74,   93,   93,
 /*   700 */    91,   74,   93,   93,   81,   82,   83,   93,   81,   82,
 /*   710 */    83,   93,   74,   93,   91,   93,   93,   74,   91,   81,
 /*   720 */    82,   83,   93,   74,   81,   82,   83,   93,   74,   91,
 /*   730 */    81,   82,   83,   93,   91,   81,   82,   83,   93,   74,
 /*   740 */    91,   93,   74,   93,   93,   91,   81,   82,   83,   81,
 /*   750 */    82,   83,   73,   74,   93,   93,   91,   93,   74,   91,
 /*   760 */    81,   82,   83,   93,   74,   81,   82,   83,   84,   93,
 /*   770 */    74,   81,   82,   83,   93,   74,   93,   81,   82,   83,
 /*   780 */    93,   93,   81,   82,   83,
};
#define YY_SHIFT_USE_DFLT (-45)
#define YY_SHIFT_COUNT (93)
#define YY_SHIFT_MIN   (-44)
#define YY_SHIFT_MAX   (546)
static const short yy_shift_ofst[] = {
 /*     0 */   260,   -2,   -2,   -2,   65,   65,  124,  124,  124,  124,
 /*    10 */   124,  124,  124,  124,  124,  124,  124,  124,  325,  325,
 /*    20 */   325,  325,  325,  325,  171,  230,  290,  290,  290,  290,
 /*    30 */   290,  290,  290,  290,  290,  290,  290,  290,  325,  320,
 /*    40 */   320,  320,  320,  320,  320,  320,  320,  320,  320,  320,
 /*    50 */   320,  320,  320,  320,  320,  320,  320,  320,  320,  320,
 /*    60 */   385,  385,  385,   60,  125,  469,  421,  400,  310,  265,
 /*    70 */   176,   60,  -45,  381,  374,  369,  249,  362,  221,  546,
 /*    80 */   546,  546,  546,   49,  -44,  121,  126,   86,   84,   67,
 /*    90 */    70,   18,    4,    1,
};
#define YY_REDUCE_USE_DFLT (-72)
#define YY_REDUCE_COUNT (72)
#define YY_REDUCE_MIN   (-71)
#define YY_REDUCE_MAX   (701)
static const short yy_reduce_ofst[] = {
 /*     0 */   233,   55,   -4,  -71,  278,  258,  464,  459,  448,  443,
 /*    10 */   432,  427,  416,  411,  399,  394,  382,  377,  365,  361,
 /*    20 */   349,  345,  329,  316,  684,  679,  668,  665,  654,  649,
 /*    30 */   643,  638,  627,  623,  609,  604,  598,  593,  588,  583,
 /*    40 */   578,  573,  568,  563,  553,  548,  543,  533,  528,  523,
 /*    50 */   518,  513,  508,  498,  492,  486,  480,   66,    7,  -60,
 /*    60 */   701,  696,  690,  -61,   53,   47,   47,   47,   47,   47,
 /*    70 */    47,   63,   41,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   184,  293,  293,  293,  293,  293,  293,  270,  268,  293,
 /*    10 */   293,  293,  293,  293,  293,  293,  293,  293,  293,  293,
 /*    20 */   293,  293,  293,  293,  293,  185,  293,  293,  293,  293,
 /*    30 */   293,  293,  293,  288,  293,  293,  293,  293,  293,  293,
 /*    40 */   293,  293,  293,  293,  293,  293,  293,  293,  293,  293,
 /*    50 */   293,  293,  293,  293,  293,  293,  293,  293,  293,  293,
 /*    60 */   293,  293,  293,  205,  293,  255,  255,  255,  255,  255,
 /*    70 */   255,  206,  282,  293,  293,  293,  293,  293,  293,  292,
 /*    80 */   272,  271,  213,  293,  269,  293,  293,  293,  293,  293,
 /*    90 */   293,  293,  293,  293,  207,  211,  210,  209,  208,  204,
 /*   100 */   203,  187,  212,  202,  201,  200,  199,  198,  197,  196,
 /*   110 */   195,  194,  226,  233,  232,  231,  258,  263,  285,  291,
 /*   120 */   290,  289,  283,  287,  286,  284,  281,  280,  279,  278,
 /*   130 */   277,  261,  259,  262,  260,  254,  276,  275,  274,  273,
 /*   140 */   253,  251,  250,  249,  248,  246,  245,  244,  243,  242,
 /*   150 */   241,  240,  239,  238,  237,  236,  235,  234,  229,  252,
 /*   160 */   247,  228,  227,  224,  223,  230,  225,  222,  221,  220,
 /*   170 */   219,  218,  217,  216,  215,  214,  193,  192,  191,  190,
 /*   180 */   189,  188,  186,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseNoOpARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseNoOpTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "RPAREN",        "LPAREN",        "PLUS",        
  "MINUS",         "DIVIDE",        "TIMES",         "EXP",         
  "NUM",           "NEWLINE",       "STRING",        "QUOTED_STRING",
  "ESCAPE_STRING",  "EQUALS",        "GE",            "LE",          
  "EQ",            "LT",            "GT",            "NE",          
  "COMMA",         "WILD",          "EVALUATE",      "TO",          
  "AS",            "FLOAT",         "INTEGER",       "DEBUG_PARSER",
  "TOK_PI",        "BLACKBOARD",    "HEADER",        "VARIABLE",    
  "SIN",           "COS",           "TAN",           "SINH",        
  "COSH",          "TANH",          "ASIN",          "ACOS",        
  "ATAN",          "ABS",           "FLOOR",         "CEIL",        
  "EXPON",         "ROUND",         "ATAN2",         "LENGTH",      
  "LOGN",          "LOG10",         "SQRT",          "MINIMUM",     
  "MAXIMUM",       "ADD",           "SUB",           "MUL",         
  "DIV",           "GETTIME",       "POWER",         "ALOG",        
  "ALOG10",        "CHANGE",        "SUBSTRING",     "DELETE",      
  "CONCAT",        "BEFORE",        "AFTER",         "REPLY",       
  "FTOA",          "error",         "main",          "pin",         
  "pbits_list",    "pbits",         "number",        "expr",        
  "evaluate",      "evaloptsp",     "evalopts",      "evalo",       
  "state",         "blackboard",    "header",        "key",         
  "func",          "num_or_pstate",  "sac_math",      "commas",      
  "xlist",         "list_item",     "fstring",       "string",      
  "string_list", 
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "main ::= pin",
 /*   1 */ "pin ::=",
 /*   2 */ "pin ::= pbits_list",
 /*   3 */ "pin ::= pbits_list NEWLINE",
 /*   4 */ "pbits_list ::= pbits",
 /*   5 */ "pbits_list ::= pbits_list pbits",
 /*   6 */ "pbits ::= number",
 /*   7 */ "pbits ::= STRING",
 /*   8 */ "pbits ::= QUOTED_STRING",
 /*   9 */ "pbits ::= ESCAPE_STRING",
 /*  10 */ "pbits ::= LPAREN expr RPAREN",
 /*  11 */ "pbits ::= EQUALS",
 /*  12 */ "pbits ::= GE",
 /*  13 */ "pbits ::= LE",
 /*  14 */ "pbits ::= EQ",
 /*  15 */ "pbits ::= LT",
 /*  16 */ "pbits ::= GT",
 /*  17 */ "pbits ::= NE",
 /*  18 */ "pbits ::= COMMA",
 /*  19 */ "pbits ::= WILD",
 /*  20 */ "pin ::= evaluate",
 /*  21 */ "evaluate ::= EVALUATE evaloptsp expr",
 /*  22 */ "evaloptsp ::=",
 /*  23 */ "evaloptsp ::= evalopts",
 /*  24 */ "evalopts ::= evalo",
 /*  25 */ "evalopts ::= evalopts evalo",
 /*  26 */ "evalo ::= TO STRING",
 /*  27 */ "evalo ::= AS FLOAT",
 /*  28 */ "evalo ::= AS INTEGER",
 /*  29 */ "pbits ::= DEBUG_PARSER",
 /*  30 */ "expr ::= state",
 /*  31 */ "number ::= NUM",
 /*  32 */ "number ::= TOK_PI",
 /*  33 */ "number ::= MINUS number",
 /*  34 */ "number ::= blackboard",
 /*  35 */ "number ::= header",
 /*  36 */ "number ::= key",
 /*  37 */ "blackboard ::= BLACKBOARD",
 /*  38 */ "header ::= HEADER",
 /*  39 */ "key ::= VARIABLE",
 /*  40 */ "state ::= number",
 /*  41 */ "state ::= func",
 /*  42 */ "state ::= MINUS func",
 /*  43 */ "state ::= state PLUS state",
 /*  44 */ "state ::= state MINUS state",
 /*  45 */ "state ::= state TIMES state",
 /*  46 */ "state ::= LPAREN state RPAREN",
 /*  47 */ "num_or_pstate ::= number",
 /*  48 */ "num_or_pstate ::= LPAREN state RPAREN",
 /*  49 */ "num_or_pstate ::= LPAREN sac_math RPAREN",
 /*  50 */ "func ::= SIN num_or_pstate",
 /*  51 */ "func ::= COS num_or_pstate",
 /*  52 */ "func ::= TAN num_or_pstate",
 /*  53 */ "func ::= SINH num_or_pstate",
 /*  54 */ "func ::= COSH num_or_pstate",
 /*  55 */ "func ::= TANH num_or_pstate",
 /*  56 */ "func ::= ASIN num_or_pstate",
 /*  57 */ "func ::= ACOS num_or_pstate",
 /*  58 */ "func ::= ATAN num_or_pstate",
 /*  59 */ "func ::= ABS num_or_pstate",
 /*  60 */ "func ::= FLOOR num_or_pstate",
 /*  61 */ "func ::= CEIL num_or_pstate",
 /*  62 */ "func ::= EXPON num_or_pstate",
 /*  63 */ "func ::= ROUND num_or_pstate",
 /*  64 */ "state ::= state EXP state",
 /*  65 */ "func ::= ATAN2 LPAREN state COMMA state RPAREN",
 /*  66 */ "func ::= LENGTH LPAREN state COMMA state RPAREN",
 /*  67 */ "func ::= LOGN num_or_pstate",
 /*  68 */ "func ::= LOG10 num_or_pstate",
 /*  69 */ "state ::= state DIVIDE state",
 /*  70 */ "func ::= SQRT num_or_pstate",
 /*  71 */ "commas ::= COMMA",
 /*  72 */ "commas ::=",
 /*  73 */ "sac_math ::= MINIMUM xlist",
 /*  74 */ "sac_math ::= MAXIMUM xlist",
 /*  75 */ "state ::= LPAREN sac_math RPAREN",
 /*  76 */ "expr ::= sac_math",
 /*  77 */ "list_item ::= number",
 /*  78 */ "list_item ::= LPAREN expr RPAREN",
 /*  79 */ "xlist ::= list_item",
 /*  80 */ "xlist ::= xlist commas list_item",
 /*  81 */ "sac_math ::= ADD xlist",
 /*  82 */ "sac_math ::= SUB xlist",
 /*  83 */ "sac_math ::= MUL xlist",
 /*  84 */ "sac_math ::= DIV xlist",
 /*  85 */ "sac_math ::= GETTIME MINIMUM",
 /*  86 */ "sac_math ::= GETTIME",
 /*  87 */ "sac_math ::= GETTIME MAXIMUM",
 /*  88 */ "sac_math ::= GETTIME MINIMUM state",
 /*  89 */ "sac_math ::= GETTIME MAXIMUM state",
 /*  90 */ "func ::= INTEGER num_or_pstate",
 /*  91 */ "func ::= POWER num_or_pstate",
 /*  92 */ "func ::= ALOG num_or_pstate",
 /*  93 */ "func ::= ALOG10 num_or_pstate",
 /*  94 */ "expr ::= fstring",
 /*  95 */ "string ::= STRING",
 /*  96 */ "string ::= QUOTED_STRING",
 /*  97 */ "string ::= ESCAPE_STRING",
 /*  98 */ "string ::= number",
 /*  99 */ "string_list ::=",
 /* 100 */ "string_list ::= string_list string",
 /* 101 */ "string ::= LPAREN fstring RPAREN",
 /* 102 */ "fstring ::= CHANGE string string string",
 /* 103 */ "fstring ::= SUBSTRING number number string",
 /* 104 */ "fstring ::= DELETE string string",
 /* 105 */ "fstring ::= CONCAT string_list",
 /* 106 */ "fstring ::= BEFORE string string",
 /* 107 */ "fstring ::= AFTER string string",
 /* 108 */ "fstring ::= REPLY string",
 /* 109 */ "fstring ::= FTOA state",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to ParseNoOp and ParseNoOpFree.
*/
void *ParseNoOpAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseNoOpARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
      /* TERMINAL Destructor */
    case 1: /* RPAREN */
    case 2: /* LPAREN */
    case 3: /* PLUS */
    case 4: /* MINUS */
    case 5: /* DIVIDE */
    case 6: /* TIMES */
    case 7: /* EXP */
    case 8: /* NUM */
    case 9: /* NEWLINE */
    case 10: /* STRING */
    case 11: /* QUOTED_STRING */
    case 12: /* ESCAPE_STRING */
    case 13: /* EQUALS */
    case 14: /* GE */
    case 15: /* LE */
    case 16: /* EQ */
    case 17: /* LT */
    case 18: /* GT */
    case 19: /* NE */
    case 20: /* COMMA */
    case 21: /* WILD */
    case 22: /* EVALUATE */
    case 23: /* TO */
    case 24: /* AS */
    case 25: /* FLOAT */
    case 26: /* INTEGER */
    case 27: /* DEBUG_PARSER */
    case 28: /* TOK_PI */
    case 29: /* BLACKBOARD */
    case 30: /* HEADER */
    case 31: /* VARIABLE */
    case 32: /* SIN */
    case 33: /* COS */
    case 34: /* TAN */
    case 35: /* SINH */
    case 36: /* COSH */
    case 37: /* TANH */
    case 38: /* ASIN */
    case 39: /* ACOS */
    case 40: /* ATAN */
    case 41: /* ABS */
    case 42: /* FLOOR */
    case 43: /* CEIL */
    case 44: /* EXPON */
    case 45: /* ROUND */
    case 46: /* ATAN2 */
    case 47: /* LENGTH */
    case 48: /* LOGN */
    case 49: /* LOG10 */
    case 50: /* SQRT */
    case 51: /* MINIMUM */
    case 52: /* MAXIMUM */
    case 53: /* ADD */
    case 54: /* SUB */
    case 55: /* MUL */
    case 56: /* DIV */
    case 57: /* GETTIME */
    case 58: /* POWER */
    case 59: /* ALOG */
    case 60: /* ALOG10 */
    case 61: /* CHANGE */
    case 62: /* SUBSTRING */
    case 63: /* DELETE */
    case 64: /* CONCAT */
    case 65: /* BEFORE */
    case 66: /* AFTER */
    case 67: /* REPLY */
    case 68: /* FTOA */
{
#line 70 "expr_parse_noop.y"

  yypParser->value = value;
  UNUSED(yypminor);

#line 772 "expr_parse_noop.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseNoOpAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseNoOpFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseNoOpStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseNoOpARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
#line 55 "expr_parse_noop.y"

  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
  UNUSED(yypMinor);
#line 958 "expr_parse_noop.c"
   ParseNoOpARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 70, 1 },
  { 71, 0 },
  { 71, 1 },
  { 71, 2 },
  { 72, 1 },
  { 72, 2 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 3 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 71, 1 },
  { 76, 3 },
  { 77, 0 },
  { 77, 1 },
  { 78, 1 },
  { 78, 2 },
  { 79, 2 },
  { 79, 2 },
  { 79, 2 },
  { 73, 1 },
  { 75, 1 },
  { 74, 1 },
  { 74, 1 },
  { 74, 2 },
  { 74, 1 },
  { 74, 1 },
  { 74, 1 },
  { 81, 1 },
  { 82, 1 },
  { 83, 1 },
  { 80, 1 },
  { 80, 1 },
  { 80, 2 },
  { 80, 3 },
  { 80, 3 },
  { 80, 3 },
  { 80, 3 },
  { 85, 1 },
  { 85, 3 },
  { 85, 3 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 80, 3 },
  { 84, 6 },
  { 84, 6 },
  { 84, 2 },
  { 84, 2 },
  { 80, 3 },
  { 84, 2 },
  { 87, 1 },
  { 87, 0 },
  { 86, 2 },
  { 86, 2 },
  { 80, 3 },
  { 75, 1 },
  { 89, 1 },
  { 89, 3 },
  { 88, 1 },
  { 88, 3 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 1 },
  { 86, 2 },
  { 86, 3 },
  { 86, 3 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 84, 2 },
  { 75, 1 },
  { 91, 1 },
  { 91, 1 },
  { 91, 1 },
  { 91, 1 },
  { 92, 0 },
  { 92, 2 },
  { 91, 3 },
  { 90, 4 },
  { 90, 4 },
  { 90, 3 },
  { 90, 2 },
  { 90, 3 },
  { 90, 3 },
  { 90, 2 },
  { 90, 2 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseNoOpARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0: /* main ::= pin */
#line 83 "expr_parse_noop.y"
{ 
  if(value->error <=0){
    token_copy(value, &yymsp[0].minor.yy0);
  } 
}
#line 1186 "expr_parse_noop.c"
        break;
      case 2: /* pin ::= pbits_list */
      case 4: /* pbits_list ::= pbits */ yytestcase(yyruleno==4);
      case 7: /* pbits ::= STRING */ yytestcase(yyruleno==7);
      case 8: /* pbits ::= QUOTED_STRING */ yytestcase(yyruleno==8);
      case 9: /* pbits ::= ESCAPE_STRING */ yytestcase(yyruleno==9);
      case 11: /* pbits ::= EQUALS */ yytestcase(yyruleno==11);
      case 12: /* pbits ::= GE */ yytestcase(yyruleno==12);
      case 13: /* pbits ::= LE */ yytestcase(yyruleno==13);
      case 14: /* pbits ::= EQ */ yytestcase(yyruleno==14);
      case 15: /* pbits ::= LT */ yytestcase(yyruleno==15);
      case 16: /* pbits ::= GT */ yytestcase(yyruleno==16);
      case 17: /* pbits ::= NE */ yytestcase(yyruleno==17);
      case 18: /* pbits ::= COMMA */ yytestcase(yyruleno==18);
      case 19: /* pbits ::= WILD */ yytestcase(yyruleno==19);
#line 90 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);      }
#line 1204 "expr_parse_noop.c"
        break;
      case 3: /* pin ::= pbits_list NEWLINE */
#line 91 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,9,&yymsp[0].minor);
}
#line 1210 "expr_parse_noop.c"
        break;
      case 5: /* pbits_list ::= pbits_list pbits */
#line 94 "expr_parse_noop.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0);  }
#line 1215 "expr_parse_noop.c"
        break;
      case 6: /* pbits ::= number */
#line 96 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);     }
#line 1220 "expr_parse_noop.c"
        break;
      case 10: /* pbits ::= LPAREN expr RPAREN */
#line 100 "expr_parse_noop.y"
{ yymsp[-1].minor.yy0.col = yymsp[-2].minor.yy0.col; token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1226 "expr_parse_noop.c"
        break;
      case 20: /* pin ::= evaluate */
#line 111 "expr_parse_noop.y"
{ 

  Token *tmp;
  token_copy(&yygotominor.yy0, &yymsp[0].minor.yy0);
  tmp = token_last(&yymsp[0].minor.yy0);
  if(eval_out && strcasecmp(eval_out, "term") != 0) {
    if(eval_asfloat) {
      setbb(eval_out, VAR_VALUE, tmp->value);
    } else {
      setbb(eval_out, VAR_INTEGER, (int)tmp->value);
    }
  } else {
    yygotominor.yy0.next = tmp;
  }
  eval_asfloat = 1;
}
#line 1246 "expr_parse_noop.c"
        break;
      case 21: /* evaluate ::= EVALUATE evaloptsp expr */
#line 127 "expr_parse_noop.y"
{ 
    Token *p;
    yymsp[-2].minor.yy0.type = STRING; 
    if(!eval_asfloat) {
      yymsp[0].minor.yy0.value = (int)yymsp[0].minor.yy0.value;
    }
    token_copy(&yygotominor.yy0, &yymsp[-2].minor.yy0);

    if(yymsp[-1].minor.yy0.type) {
      p = token_new(0,0,NULL,0,0);
      token_copy(p, &yymsp[-1].minor.yy0);
      token_last(&yygotominor.yy0)->next = p;
    }

    p = token_new(0,0,NULL,0,0);
    token_copy(p, &yymsp[0].minor.yy0);
    token_last(&yygotominor.yy0)->next = p;
}
#line 1268 "expr_parse_noop.c"
        break;
      case 23: /* evaloptsp ::= evalopts */
#line 146 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);  }
#line 1273 "expr_parse_noop.c"
        break;
      case 24: /* evalopts ::= evalo */
      case 34: /* number ::= blackboard */ yytestcase(yyruleno==34);
      case 35: /* number ::= header */ yytestcase(yyruleno==35);
      case 36: /* number ::= key */ yytestcase(yyruleno==36);
      case 76: /* expr ::= sac_math */ yytestcase(yyruleno==76);
      case 77: /* list_item ::= number */ yytestcase(yyruleno==77);
      case 79: /* xlist ::= list_item */ yytestcase(yyruleno==79);
      case 94: /* expr ::= fstring */ yytestcase(yyruleno==94);
#line 147 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0); }
#line 1285 "expr_parse_noop.c"
        break;
      case 25: /* evalopts ::= evalopts evalo */
#line 148 "expr_parse_noop.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1290 "expr_parse_noop.c"
        break;
      case 26: /* evalo ::= TO STRING */
#line 149 "expr_parse_noop.y"
{ 
   if(eval_out) {
     free(eval_out); 
     eval_out = NULL; 
   }
   eval_out = strdup(yymsp[0].minor.yy0.str); 
   yymsp[-1].minor.yy0.type = STRING;
   token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0);
}
#line 1303 "expr_parse_noop.c"
        break;
      case 27: /* evalo ::= AS FLOAT */
#line 158 "expr_parse_noop.y"
{ eval_asfloat = 1; yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1308 "expr_parse_noop.c"
        break;
      case 28: /* evalo ::= AS INTEGER */
#line 159 "expr_parse_noop.y"
{ eval_asfloat = 0;  yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1313 "expr_parse_noop.c"
        break;
      case 29: /* pbits ::= DEBUG_PARSER */
#line 166 "expr_parse_noop.y"
{
  if(!tdebug) {
    tdebug = 1;
                  
                                 
      
                
    ParseNoOpTrace(stdout, "expr: ");
      
  } else {
    tdebug = 0;
                  
                               
      
                
    ParseNoOpTrace(NULL, "expr: ");
      
  }
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
#line 1337 "expr_parse_noop.c"
        break;
      case 30: /* expr ::= state */
      case 31: /* number ::= NUM */ yytestcase(yyruleno==31);
#line 187 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);            }
#line 1343 "expr_parse_noop.c"
        break;
      case 32: /* number ::= TOK_PI */
#line 189 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, M_PI, yymsp[0].minor.yy0.col);      }
#line 1348 "expr_parse_noop.c"
        break;
      case 33: /* number ::= MINUS number */
#line 191 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, -yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1353 "expr_parse_noop.c"
        break;
      case 37: /* blackboard ::= BLACKBOARD */
#line 196 "expr_parse_noop.y"
{ 
  if(!token_var(&yygotominor.yy0, BLACKBOARD, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_BLACKBOARD_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  } else {
    if(yygotominor.yy0.type == STRING) {
      Token *t;
      t = token_to_token_list(&yygotominor.yy0);
      token_copy(&yygotominor.yy0, t);
      FREE(t);
    }
  }
}
#line 1370 "expr_parse_noop.c"
        break;
      case 38: /* header ::= HEADER */
#line 209 "expr_parse_noop.y"
{ 
  if(!token_var(&yygotominor.yy0, HEADER, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  } 
}
#line 1380 "expr_parse_noop.c"
        break;
      case 39: /* key ::= VARIABLE */
#line 215 "expr_parse_noop.y"
{ 
  if(!token_var(&yygotominor.yy0, VARIABLE, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  }
}
#line 1390 "expr_parse_noop.c"
        break;
      case 40: /* state ::= number */
#line 222 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);        }
#line 1395 "expr_parse_noop.c"
        break;
      case 41: /* state ::= func */
#line 223 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);    }
#line 1400 "expr_parse_noop.c"
        break;
      case 42: /* state ::= MINUS func */
#line 224 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0,-yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1405 "expr_parse_noop.c"
        break;
      case 43: /* state ::= state PLUS state */
#line 226 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value + yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 1411 "expr_parse_noop.c"
        break;
      case 44: /* state ::= state MINUS state */
#line 227 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value - yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 1417 "expr_parse_noop.c"
        break;
      case 45: /* state ::= state TIMES state */
#line 228 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value * yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 1423 "expr_parse_noop.c"
        break;
      case 46: /* state ::= LPAREN state RPAREN */
#line 230 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);         yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1430 "expr_parse_noop.c"
        break;
      case 47: /* num_or_pstate ::= number */
#line 232 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);}
#line 1435 "expr_parse_noop.c"
        break;
      case 48: /* num_or_pstate ::= LPAREN state RPAREN */
      case 49: /* num_or_pstate ::= LPAREN sac_math RPAREN */ yytestcase(yyruleno==49);
#line 233 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);  yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1443 "expr_parse_noop.c"
        break;
      case 50: /* func ::= SIN num_or_pstate */
#line 236 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, sin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1448 "expr_parse_noop.c"
        break;
      case 51: /* func ::= COS num_or_pstate */
#line 237 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, cos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1453 "expr_parse_noop.c"
        break;
      case 52: /* func ::= TAN num_or_pstate */
#line 238 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, tan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1458 "expr_parse_noop.c"
        break;
      case 53: /* func ::= SINH num_or_pstate */
#line 240 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, sinh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1463 "expr_parse_noop.c"
        break;
      case 54: /* func ::= COSH num_or_pstate */
#line 241 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, cosh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1468 "expr_parse_noop.c"
        break;
      case 55: /* func ::= TANH num_or_pstate */
#line 242 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, tanh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1473 "expr_parse_noop.c"
        break;
      case 56: /* func ::= ASIN num_or_pstate */
#line 244 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, asin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1478 "expr_parse_noop.c"
        break;
      case 57: /* func ::= ACOS num_or_pstate */
#line 245 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, acos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1483 "expr_parse_noop.c"
        break;
      case 58: /* func ::= ATAN num_or_pstate */
#line 246 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, atan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1488 "expr_parse_noop.c"
        break;
      case 59: /* func ::= ABS num_or_pstate */
#line 248 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, fabs(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1493 "expr_parse_noop.c"
        break;
      case 60: /* func ::= FLOOR num_or_pstate */
#line 249 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, floor(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1498 "expr_parse_noop.c"
        break;
      case 61: /* func ::= CEIL num_or_pstate */
#line 250 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, ceil(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1503 "expr_parse_noop.c"
        break;
      case 62: /* func ::= EXPON num_or_pstate */
#line 251 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, exp(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);    }
#line 1508 "expr_parse_noop.c"
        break;
      case 63: /* func ::= ROUND num_or_pstate */
#line 252 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, round(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1513 "expr_parse_noop.c"
        break;
      case 64: /* state ::= state EXP state */
#line 254 "expr_parse_noop.y"
{ 
  if(yymsp[-2].minor.yy0.value < 0.0 && fabs(round(yymsp[0].minor.yy0.value) - yymsp[0].minor.yy0.value) >= 1e-15) {
    parse_error(value, TOKEN_STATUS_ERROR_NEGATIVE_FRACTION_POWER);
  } else {
    token_value(&yygotominor.yy0, pow(yymsp[-2].minor.yy0.value, yymsp[0].minor.yy0.value), yymsp[-2].minor.yy0.col);
  }
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 1525 "expr_parse_noop.c"
        break;
      case 65: /* func ::= ATAN2 LPAREN state COMMA state RPAREN */
#line 262 "expr_parse_noop.y"
{ 
  token_value(&yygotominor.yy0, atan2( yymsp[-3].minor.yy0.value, yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1535 "expr_parse_noop.c"
        break;
      case 66: /* func ::= LENGTH LPAREN state COMMA state RPAREN */
#line 265 "expr_parse_noop.y"
{ 
  token_value(&yygotominor.yy0, sqrt( yymsp[-3].minor.yy0.value*yymsp[-3].minor.yy0.value + yymsp[-1].minor.yy0.value*yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1545 "expr_parse_noop.c"
        break;
      case 67: /* func ::= LOGN num_or_pstate */
      case 92: /* func ::= ALOG num_or_pstate */ yytestcase(yyruleno==92);
#line 268 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1557 "expr_parse_noop.c"
        break;
      case 68: /* func ::= LOG10 num_or_pstate */
#line 275 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1568 "expr_parse_noop.c"
        break;
      case 69: /* state ::= state DIVIDE state */
#line 282 "expr_parse_noop.y"
{ 
    if(yymsp[0].minor.yy0.value != 0.0) {
      token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value / yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);
    } else {
      parse_error(value, TOKEN_STATUS_ERROR_DIVIDE_BY_ZERO);
    } 
    yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 1580 "expr_parse_noop.c"
        break;
      case 70: /* func ::= SQRT num_or_pstate */
#line 289 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value >= 0.0) {
    token_value(&yygotominor.yy0, sqrt ( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_SQRT_NEGATIVE);
  }
}
#line 1591 "expr_parse_noop.c"
        break;
      case 71: /* commas ::= COMMA */
#line 298 "expr_parse_noop.y"
{
  yy_destructor(yypParser,20,&yymsp[0].minor);
}
#line 1598 "expr_parse_noop.c"
        break;
      case 73: /* sac_math ::= MINIMUM xlist */
#line 327 "expr_parse_noop.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_min), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1606 "expr_parse_noop.c"
        break;
      case 74: /* sac_math ::= MAXIMUM xlist */
#line 335 "expr_parse_noop.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_max), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1614 "expr_parse_noop.c"
        break;
      case 75: /* state ::= LPAREN sac_math RPAREN */
      case 78: /* list_item ::= LPAREN expr RPAREN */ yytestcase(yyruleno==78);
#line 341 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1622 "expr_parse_noop.c"
        break;
      case 80: /* xlist ::= xlist commas list_item */
#line 347 "expr_parse_noop.y"
{ token_append(&yygotominor.yy0,&yymsp[-2].minor.yy0,&yymsp[0].minor.yy0); }
#line 1627 "expr_parse_noop.c"
        break;
      case 81: /* sac_math ::= ADD xlist */
#line 349 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_add), yymsp[-1].minor.yy0.col); }
#line 1632 "expr_parse_noop.c"
        break;
      case 82: /* sac_math ::= SUB xlist */
#line 350 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_sub), yymsp[-1].minor.yy0.col); }
#line 1637 "expr_parse_noop.c"
        break;
      case 83: /* sac_math ::= MUL xlist */
#line 351 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_mul), yymsp[-1].minor.yy0.col); }
#line 1642 "expr_parse_noop.c"
        break;
      case 84: /* sac_math ::= DIV xlist */
#line 352 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_div), yymsp[-1].minor.yy0.col); }
#line 1647 "expr_parse_noop.c"
        break;
      case 85: /* sac_math ::= GETTIME MINIMUM */
#line 354 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
  }
  yy_destructor(yypParser,51,&yymsp[0].minor);
}
#line 1659 "expr_parse_noop.c"
        break;
      case 86: /* sac_math ::= GETTIME */
#line 361 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[0].minor.yy0.col;
  }
}
#line 1670 "expr_parse_noop.c"
        break;
      case 87: /* sac_math ::= GETTIME MAXIMUM */
#line 368 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, TRUE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
  }
  yy_destructor(yypParser,52,&yymsp[0].minor);
}
#line 1682 "expr_parse_noop.c"
        break;
      case 88: /* sac_math ::= GETTIME MINIMUM state */
#line 375 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, FALSE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-2].minor.yy0.col;
  }
  yy_destructor(yypParser,51,&yymsp[-1].minor);
}
#line 1694 "expr_parse_noop.c"
        break;
      case 89: /* sac_math ::= GETTIME MAXIMUM state */
#line 382 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, TRUE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-2].minor.yy0.col;
  }
  yy_destructor(yypParser,52,&yymsp[-1].minor);
}
#line 1706 "expr_parse_noop.c"
        break;
      case 90: /* func ::= INTEGER num_or_pstate */
#line 391 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, (int) yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);   }
#line 1711 "expr_parse_noop.c"
        break;
      case 91: /* func ::= POWER num_or_pstate */
#line 392 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, pow(10,yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); }
#line 1716 "expr_parse_noop.c"
        break;
      case 93: /* func ::= ALOG10 num_or_pstate */
#line 400 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0,log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1727 "expr_parse_noop.c"
        break;
      case 95: /* string ::= STRING */
      case 96: /* string ::= QUOTED_STRING */ yytestcase(yyruleno==96);
      case 97: /* string ::= ESCAPE_STRING */ yytestcase(yyruleno==97);
#line 412 "expr_parse_noop.y"
{ token_string(&yygotominor.yy0, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col); }
#line 1734 "expr_parse_noop.c"
        break;
      case 98: /* string ::= number */
#line 415 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.type == NUM) {
    char *s;
    if(floor(yymsp[0].minor.yy0.value) == yymsp[0].minor.yy0.value) {
      asprintf(&s, "%d", (int)yymsp[0].minor.yy0.value);
    } else {
      asprintf(&s, "%g", yymsp[0].minor.yy0.value);
    }
    token_string(&yygotominor.yy0, s, yymsp[0].minor.yy0.col); 
  } else {
    token_string(&yygotominor.yy0, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col);
  }
}
#line 1751 "expr_parse_noop.c"
        break;
      case 99: /* string_list ::= */
#line 430 "expr_parse_noop.y"
{ token_string(&yygotominor.yy0, strdup(""), lexer_col()); }
#line 1756 "expr_parse_noop.c"
        break;
      case 100: /* string_list ::= string_list string */
#line 431 "expr_parse_noop.y"
{ 
  if(yymsp[-1].minor.yy0.str && yymsp[0].minor.yy0.str) {
    string *s = string_new(yymsp[-1].minor.yy0.str);
    string_append(s, yymsp[0].minor.yy0.str);
    token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-1].minor.yy0.col);
    string_free(&s);
    FREE(yymsp[-1].minor.yy0.str);
    FREE(yymsp[0].minor.yy0.str);
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
  }
}
#line 1772 "expr_parse_noop.c"
        break;
      case 101: /* string ::= LPAREN fstring RPAREN */
#line 445 "expr_parse_noop.y"
{  token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1779 "expr_parse_noop.c"
        break;
      case 102: /* fstring ::= CHANGE string string string */
#line 447 "expr_parse_noop.y"
{
  string *s = string_new( yymsp[0].minor.yy0.str );
  s = string_replace(s, yymsp[-2].minor.yy0.str, yymsp[-1].minor.yy0.str );
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-2].minor.yy0.str);
  FREE(yymsp[-1].minor.yy0.str);
}
#line 1791 "expr_parse_noop.c"
        break;
      case 103: /* fstring ::= SUBSTRING number number string */
#line 455 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string *s2 = string_substr(s, (int)yymsp[-2].minor.yy0.value-1, (int)yymsp[-1].minor.yy0.value-(int)yymsp[-2].minor.yy0.value+1);
  token_string(&yygotominor.yy0, strdup(string_string(s2)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  string_free(&s2);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1803 "expr_parse_noop.c"
        break;
      case 104: /* fstring ::= DELETE string string */
#line 463 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string_replace(s, yymsp[-1].minor.yy0.str, "");
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1815 "expr_parse_noop.c"
        break;
      case 105: /* fstring ::= CONCAT string_list */
#line 472 "expr_parse_noop.y"
{ yymsp[0].minor.yy0.col = yymsp[-1].minor.yy0.col; token_copy(&yygotominor.yy0, &yymsp[0].minor.yy0); }
#line 1820 "expr_parse_noop.c"
        break;
      case 106: /* fstring ::= BEFORE string string */
#line 474 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, f-yymsp[0].minor.yy0.str, -1);
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1833 "expr_parse_noop.c"
        break;
      case 107: /* fstring ::= AFTER string string */
#line 483 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, 0, (f-yymsp[0].minor.yy0.str)+strlen(yymsp[-1].minor.yy0.str));
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1846 "expr_parse_noop.c"
        break;
      case 108: /* fstring ::= REPLY string */
#line 492 "expr_parse_noop.y"
{
  char *a, *b, *def;
  char in[1024];
  memset(in, 0, sizeof(in));
  fprintf(stdout, "%s", yymsp[0].minor.yy0.str);
  a = strrchr(yymsp[0].minor.yy0.str, '[');
  b = strrchr(yymsp[0].minor.yy0.str, ']');
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
        token_value(&yygotominor.yy0, atof(def), yymsp[-1].minor.yy0.col);
      } else {
        token_string(&yygotominor.yy0, strdup(def), yymsp[-1].minor.yy0.col);
      }
    } else {
      if(isnumx(in)) {
        token_value(&yygotominor.yy0, atof(in), yymsp[-1].minor.yy0.col);
      } else {
        token_string(&yygotominor.yy0, strdup( in ), yymsp[-1].minor.yy0.col);
      }
    }
  }
  FREE(def);
}
#line 1885 "expr_parse_noop.c"
        break;
      case 109: /* fstring ::= FTOA state */
#line 527 "expr_parse_noop.y"
{ 
  char *s;
  asprintf(&s, "%g", yymsp[0].minor.yy0.value); 
  token_string(&yygotominor.yy0, s, yymsp[-1].minor.yy0.col);
}
#line 1894 "expr_parse_noop.c"
        break;
      default:
      /* (1) pin ::= */ yytestcase(yyruleno==1);
      /* (22) evaloptsp ::= */ yytestcase(yyruleno==22);
      /* (72) commas ::= */ yytestcase(yyruleno==72);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseNoOpARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 60 "expr_parse_noop.y"

  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
#line 1948 "expr_parse_noop.c"
  ParseNoOpARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseNoOpARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 75 "expr_parse_noop.y"

  UNUSED(yymajor);
  UNUSED(yyminor);
  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
#line 1968 "expr_parse_noop.c"
  ParseNoOpARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseNoOpARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 64 "expr_parse_noop.y"

  if(value->error <= 0) {
    value->error = TOKEN_STATUS_OK;
  }
#line 1992 "expr_parse_noop.c"
  ParseNoOpARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseNoOpAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void ParseNoOp(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseNoOpTOKENTYPE yyminor       /* The value for the token */
  ParseNoOpARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseNoOpARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
