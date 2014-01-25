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
#define YYNSTATE 181
#define YYNRULE 109
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
#define YY_ACTTAB_COUNT (780)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     3,  181,   24,  162,   91,  174,  173,    6,    5,   83,
 /*    10 */   170,  169,  168,  161,  163,   11,  124,   95,   94,  123,
 /*    20 */     9,  170,  169,  168,   42,  126,  172,  167,  166,  165,
 /*    30 */    59,   58,   57,   56,   55,   54,   53,   52,   51,   50,
 /*    40 */    49,   48,   47,   46,   90,   89,   45,   44,   43,   23,
 /*    50 */    22,   21,   20,   19,   18,   85,   41,   40,   39,   37,
 /*    60 */    61,   35,   72,   32,   30,   28,    4,    3,  128,   24,
 /*    70 */   162,   98,  118,  173,   86,   84,   83,  170,  169,  168,
 /*    80 */   161,  163,   96,  124,   97,   33,  123,   87,  170,  169,
 /*    90 */   168,   42,  155,  172,  167,  166,  165,   59,   58,   57,
 /*   100 */    56,   55,   54,   53,   52,   51,   50,   49,   48,   47,
 /*   110 */    46,   90,   89,   45,   44,   43,   23,   22,   21,   20,
 /*   120 */    19,   18,   85,   41,   40,   39,    3,   38,   24,    1,
 /*   130 */    71,   93,  173,  291,   92,   25,  100,  178,  292,   99,
 /*   140 */   163,  292,  292,  292,  170,  169,  168,  170,  169,  168,
 /*   150 */    42,  154,  172,  167,  166,  165,   59,   58,   57,   56,
 /*   160 */    55,   54,   53,   52,   51,   50,   49,   48,   47,   46,
 /*   170 */    90,   89,   45,   44,   43,   62,  253,  253,  292,  173,
 /*   180 */   292,  292,   41,   40,   39,  292,   37,   61,   35,   72,
 /*   190 */    32,   30,   28,    4,  292,  292,  133,   42,  292,  172,
 /*   200 */   167,  166,  165,   59,   58,   57,   56,   55,   54,   53,
 /*   210 */    52,   51,   50,   49,   48,   47,   46,   90,   89,   45,
 /*   220 */    44,   43,  127,  292,   17,   15,   12,   14,   13,   41,
 /*   230 */    40,   39,    2,  292,   62,  292,  292,  292,  173,  180,
 /*   240 */   177,  176,  175,  110,  109,  108,  107,  106,  105,  104,
 /*   250 */   103,  102,   17,   15,   12,   14,   13,  101,  172,  167,
 /*   260 */   166,  165,    2,  292,   62,  254,  254,  292,  173,   10,
 /*   270 */   177,  176,  175,  110,  109,  108,  107,  106,  105,  104,
 /*   280 */   103,  102,   63,  292,  292,  133,  292,  101,  172,  167,
 /*   290 */   166,  165,   64,  292,   62,  292,  292,   16,  173,   62,
 /*   300 */   122,  121,  120,  173,    7,  162,   62,  261,  261,  292,
 /*   310 */   173,  125,  170,  169,  168,  161,  292,  292,  172,  167,
 /*   320 */   166,  165,  292,  172,  167,  166,  165,  133,  292,  292,
 /*   330 */   172,  167,  166,  165,  156,  162,   17,   15,   12,   14,
 /*   340 */    13,   78,  170,  169,  168,  161,  292,  142,   88,   17,
 /*   350 */    15,   12,   14,   13,   17,   15,   12,   14,   13,  141,
 /*   360 */   292,   17,   15,   12,   14,   13,   62,  262,  262,  292,
 /*   370 */   173,    8,  292,  131,  132,   17,   15,   12,   14,   13,
 /*   380 */   292,  170,  169,  168,  263,  263,  292,  133,   70,  129,
 /*   390 */   172,  167,  166,  165,  132,  292,  292,  292,  132,  292,
 /*   400 */   292,  170,  169,  168,  133,  170,  169,  168,   69,  129,
 /*   410 */   132,  292,   68,  129,  132,  292,  292,  170,  169,  168,
 /*   420 */   292,  170,  169,  168,   67,  129,  132,  292,   66,  129,
 /*   430 */   264,  264,  292,  170,  169,  168,  292,  162,  292,  292,
 /*   440 */    65,  129,  292,   79,  170,  169,  168,  161,  162,  292,
 /*   450 */   133,  292,  292,  162,  160,  170,  169,  168,  161,  159,
 /*   460 */   170,  169,  168,  161,  162,  292,  292,  292,  292,  162,
 /*   470 */   158,  170,  169,  168,  161,  157,  170,  169,  168,  161,
 /*   480 */   162,  292,  292,  292,  292,  162,   77,  170,  169,  168,
 /*   490 */   161,   76,  170,  169,  168,  161,  162,  292,  292,  292,
 /*   500 */   292,  162,   75,  170,  169,  168,  161,   74,  170,  169,
 /*   510 */   168,  161,  162,   17,   15,   12,   14,   13,   73,  170,
 /*   520 */   169,  168,  161,  162,  292,  292,  292,  292,  162,   82,
 /*   530 */   170,  169,  168,  161,   81,  170,  169,  168,  161,  162,
 /*   540 */   292,  292,  292,  292,  163,   80,  170,  169,  168,  161,
 /*   550 */   163,  170,  169,  168,  292,  153,  292,  170,  169,  168,
 /*   560 */   163,  152,  292,  292,  292,  163,  292,  170,  169,  168,
 /*   570 */   163,  151,  170,  169,  168,  292,  150,  170,  169,  168,
 /*   580 */   163,  149,  292,  292,  292,  163,  292,  170,  169,  168,
 /*   590 */   163,  148,  170,  169,  168,  163,  147,  170,  169,  168,
 /*   600 */   292,  146,  170,  169,  168,  163,  145,  292,  292,  292,
 /*   610 */   163,  292,  170,  169,  168,  292,  144,  170,  169,  168,
 /*   620 */   163,  143,  292,  292,  292,  163,  292,  170,  169,  168,
 /*   630 */   163,  140,  170,  169,  168,  163,  139,  170,  169,  168,
 /*   640 */   163,  138,  170,  169,  168,  163,  137,  170,  169,  168,
 /*   650 */   292,  136,  170,  169,  168,  163,  135,  292,  292,  292,
 /*   660 */   132,  292,  170,  169,  168,  119,  134,  170,  169,  168,
 /*   670 */   119,  292,  170,  169,  168,  130,  119,  170,  169,  168,
 /*   680 */   292,  119,   27,  170,  169,  168,  292,  117,  170,  169,
 /*   690 */   168,  292,  119,   34,  292,  119,  292,  292,  116,  170,
 /*   700 */   169,  168,  170,  169,  168,  292,  119,  292,  292,  115,
 /*   710 */   119,  292,   31,  170,  169,  168,  292,  170,  169,  168,
 /*   720 */   292,  119,  292,  114,  119,  292,  292,   29,  170,  169,
 /*   730 */   168,  170,  169,  168,  292,  119,  292,  292,  113,  119,
 /*   740 */   292,  112,  170,  169,  168,  292,  170,  169,  168,  179,
 /*   750 */   178,  292,   26,  171,  292,  292,  111,  170,  169,  168,
 /*   760 */   170,  169,  168,  164,  171,  292,  292,   60,  292,  292,
 /*   770 */    36,  170,  169,  168,  170,  169,  168,  170,  169,  168,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,    0,    4,   74,   75,    1,    8,   51,   52,   80,
 /*    10 */    81,   82,   83,   84,   74,    2,   87,   25,   26,   90,
 /*    20 */     2,   81,   82,   83,   26,   85,   28,   29,   30,   31,
 /*    30 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*    40 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    50 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    60 */    62,   63,   64,   65,   66,   67,   68,    2,    1,    4,
 /*    70 */    74,   75,    1,    8,   23,   24,   80,   81,   82,   83,
 /*    80 */    84,   74,   10,   87,   79,   92,   90,   90,   81,   82,
 /*    90 */    83,   26,   85,   28,   29,   30,   31,   32,   33,   34,
 /*   100 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*   110 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   120 */    55,   56,   57,   58,   59,   60,    2,   86,    4,   77,
 /*   130 */    78,   79,    8,   70,   71,   72,   73,   74,   93,   76,
 /*   140 */    74,   93,   93,   93,   81,   82,   83,   81,   82,   83,
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
 /*   290 */    30,   31,    2,   93,    4,   93,   93,    2,    8,    4,
 /*   300 */    10,   11,   12,    8,    2,   74,    4,    0,    1,   93,
 /*   310 */     8,   80,   81,   82,   83,   84,   93,   93,   28,   29,
 /*   320 */    30,   31,   93,   28,   29,   30,   31,   20,   93,   93,
 /*   330 */    28,   29,   30,   31,    1,   74,    3,    4,    5,    6,
 /*   340 */     7,   80,   81,   82,   83,   84,   93,    1,   87,    3,
 /*   350 */     4,    5,    6,    7,    3,    4,    5,    6,    7,    1,
 /*   360 */    93,    3,    4,    5,    6,    7,    4,    0,    1,   93,
 /*   370 */     8,   20,   93,    1,   74,    3,    4,    5,    6,    7,
 /*   380 */    93,   81,   82,   83,    0,    1,   93,   20,   88,   89,
 /*   390 */    28,   29,   30,   31,   74,   93,   93,   93,   74,   93,
 /*   400 */    93,   81,   82,   83,   20,   81,   82,   83,   88,   89,
 /*   410 */    74,   93,   88,   89,   74,   93,   93,   81,   82,   83,
 /*   420 */    93,   81,   82,   83,   88,   89,   74,   93,   88,   89,
 /*   430 */     0,    1,   93,   81,   82,   83,   93,   74,   93,   93,
 /*   440 */    88,   89,   93,   80,   81,   82,   83,   84,   74,   93,
 /*   450 */    20,   93,   93,   74,   80,   81,   82,   83,   84,   80,
 /*   460 */    81,   82,   83,   84,   74,   93,   93,   93,   93,   74,
 /*   470 */    80,   81,   82,   83,   84,   80,   81,   82,   83,   84,
 /*   480 */    74,   93,   93,   93,   93,   74,   80,   81,   82,   83,
 /*   490 */    84,   80,   81,   82,   83,   84,   74,   93,   93,   93,
 /*   500 */    93,   74,   80,   81,   82,   83,   84,   80,   81,   82,
 /*   510 */    83,   84,   74,    3,    4,    5,    6,    7,   80,   81,
 /*   520 */    82,   83,   84,   74,   93,   93,   93,   93,   74,   80,
 /*   530 */    81,   82,   83,   84,   80,   81,   82,   83,   84,   74,
 /*   540 */    93,   93,   93,   93,   74,   80,   81,   82,   83,   84,
 /*   550 */    74,   81,   82,   83,   93,   85,   93,   81,   82,   83,
 /*   560 */    74,   85,   93,   93,   93,   74,   93,   81,   82,   83,
 /*   570 */    74,   85,   81,   82,   83,   93,   85,   81,   82,   83,
 /*   580 */    74,   85,   93,   93,   93,   74,   93,   81,   82,   83,
 /*   590 */    74,   85,   81,   82,   83,   74,   85,   81,   82,   83,
 /*   600 */    93,   85,   81,   82,   83,   74,   85,   93,   93,   93,
 /*   610 */    74,   93,   81,   82,   83,   93,   85,   81,   82,   83,
 /*   620 */    74,   85,   93,   93,   93,   74,   93,   81,   82,   83,
 /*   630 */    74,   85,   81,   82,   83,   74,   85,   81,   82,   83,
 /*   640 */    74,   85,   81,   82,   83,   74,   85,   81,   82,   83,
 /*   650 */    93,   85,   81,   82,   83,   74,   85,   93,   93,   93,
 /*   660 */    74,   93,   81,   82,   83,   74,   85,   81,   82,   83,
 /*   670 */    74,   93,   81,   82,   83,   89,   74,   81,   82,   83,
 /*   680 */    93,   74,   91,   81,   82,   83,   93,   91,   81,   82,
 /*   690 */    83,   93,   74,   91,   93,   74,   93,   93,   91,   81,
 /*   700 */    82,   83,   81,   82,   83,   93,   74,   93,   93,   91,
 /*   710 */    74,   93,   91,   81,   82,   83,   93,   81,   82,   83,
 /*   720 */    93,   74,   93,   91,   74,   93,   93,   91,   81,   82,
 /*   730 */    83,   81,   82,   83,   93,   74,   93,   93,   91,   74,
 /*   740 */    93,   91,   81,   82,   83,   93,   81,   82,   83,   73,
 /*   750 */    74,   93,   91,   74,   93,   93,   91,   81,   82,   83,
 /*   760 */    81,   82,   83,   84,   74,   93,   93,   74,   93,   93,
 /*   770 */    74,   81,   82,   83,   81,   82,   83,   81,   82,   83,
};
#define YY_SHIFT_USE_DFLT (-45)
#define YY_SHIFT_COUNT (92)
#define YY_SHIFT_MIN   (-44)
#define YY_SHIFT_MAX   (510)
static const short yy_shift_ofst[] = {
 /*     0 */   260,   -2,   -2,   65,  124,  124,  124,  124,  124,  124,
 /*    10 */   124,  124,  124,  124,  124,  124,  124,  124,  302,  302,
 /*    20 */   302,  302,  302,  302,  171,  230,  290,  290,  290,  290,
 /*    30 */   290,  290,  290,  290,  290,  290,  290,  290,  302,  295,
 /*    40 */   295,  295,  295,  295,  295,  295,  295,  295,  295,  295,
 /*    50 */   295,  295,  295,  295,  295,  295,  295,  295,  295,  295,
 /*    60 */   362,  362,  362,   51,  125,  430,  384,  367,  307,  265,
 /*    70 */   176,   51,  -45,  372,  358,  351,  346,  249,  333,  221,
 /*    80 */   510,  510,  510,  510,   -8,  -44,   72,   71,   67,   18,
 /*    90 */    13,    4,    1,
};
#define YY_REDUCE_USE_DFLT (-72)
#define YY_REDUCE_COUNT (72)
#define YY_REDUCE_MIN   (-71)
#define YY_REDUCE_MAX   (696)
static const short yy_reduce_ofst[] = {
 /*     0 */    63,   -4,  -71,  261,  465,  454,  449,  438,  427,  422,
 /*    10 */   411,  406,  395,  390,  379,  374,  363,  231,  352,  340,
 /*    20 */   336,  324,  320,  300,  679,  676,  665,  661,  650,  647,
 /*    30 */   636,  632,  621,  618,  607,  602,  596,  591,  586,  581,
 /*    40 */   571,  566,  561,  556,  551,  546,  536,  531,  521,  516,
 /*    50 */   511,  506,  496,  491,  486,  476,  470,   66,    7,  -60,
 /*    60 */   696,  693,  690,   52,   -3,   41,   41,   41,   41,   41,
 /*    70 */    41,    5,   -7,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   182,  290,  290,  290,  290,  267,  265,  290,  290,  290,
 /*    10 */   290,  290,  290,  290,  290,  290,  290,  290,  290,  290,
 /*    20 */   290,  290,  290,  290,  290,  183,  290,  290,  290,  290,
 /*    30 */   290,  290,  290,  285,  290,  290,  290,  290,  290,  290,
 /*    40 */   290,  290,  290,  290,  290,  290,  290,  290,  290,  290,
 /*    50 */   290,  290,  290,  290,  290,  290,  290,  290,  290,  290,
 /*    60 */   290,  290,  290,  203,  290,  252,  252,  252,  252,  252,
 /*    70 */   252,  204,  279,  290,  290,  290,  290,  290,  290,  290,
 /*    80 */   289,  269,  268,  211,  290,  266,  290,  290,  290,  290,
 /*    90 */   290,  290,  290,  205,  209,  208,  207,  206,  202,  201,
 /*   100 */   185,  210,  200,  199,  198,  197,  196,  195,  194,  193,
 /*   110 */   192,  282,  288,  287,  286,  280,  284,  283,  281,  278,
 /*   120 */   277,  276,  275,  274,  256,  224,  230,  229,  255,  259,
 /*   130 */   260,  258,  257,  251,  273,  272,  271,  270,  250,  248,
 /*   140 */   247,  246,  245,  243,  242,  241,  240,  239,  238,  237,
 /*   150 */   236,  235,  234,  233,  232,  231,  227,  249,  244,  226,
 /*   160 */   225,  222,  221,  228,  223,  220,  219,  218,  217,  216,
 /*   170 */   215,  214,  213,  212,  191,  190,  189,  188,  187,  186,
 /*   180 */   184,
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
  "AS",            "FLOAT",         "INTEGER",       "DEBUG",       
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
  "func",          "num_or_pstate",  "commas",        "sac_math",    
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
 /*  29 */ "pbits ::= DEBUG",
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
 /*  49 */ "func ::= SIN num_or_pstate",
 /*  50 */ "func ::= COS num_or_pstate",
 /*  51 */ "func ::= TAN num_or_pstate",
 /*  52 */ "func ::= SINH num_or_pstate",
 /*  53 */ "func ::= COSH num_or_pstate",
 /*  54 */ "func ::= TANH num_or_pstate",
 /*  55 */ "func ::= ASIN num_or_pstate",
 /*  56 */ "func ::= ACOS num_or_pstate",
 /*  57 */ "func ::= ATAN num_or_pstate",
 /*  58 */ "func ::= ABS num_or_pstate",
 /*  59 */ "func ::= FLOOR num_or_pstate",
 /*  60 */ "func ::= CEIL num_or_pstate",
 /*  61 */ "func ::= EXPON num_or_pstate",
 /*  62 */ "func ::= ROUND num_or_pstate",
 /*  63 */ "state ::= state EXP state",
 /*  64 */ "func ::= ATAN2 LPAREN state COMMA state RPAREN",
 /*  65 */ "func ::= LENGTH LPAREN state COMMA state RPAREN",
 /*  66 */ "func ::= LOGN num_or_pstate",
 /*  67 */ "func ::= LOG10 num_or_pstate",
 /*  68 */ "state ::= state DIVIDE state",
 /*  69 */ "func ::= SQRT num_or_pstate",
 /*  70 */ "commas ::= COMMA",
 /*  71 */ "commas ::=",
 /*  72 */ "sac_math ::= MINIMUM xlist",
 /*  73 */ "sac_math ::= MAXIMUM xlist",
 /*  74 */ "state ::= LPAREN sac_math RPAREN",
 /*  75 */ "expr ::= sac_math",
 /*  76 */ "list_item ::= number",
 /*  77 */ "list_item ::= LPAREN state RPAREN",
 /*  78 */ "xlist ::= list_item",
 /*  79 */ "xlist ::= xlist commas list_item",
 /*  80 */ "sac_math ::= ADD xlist",
 /*  81 */ "sac_math ::= SUB xlist",
 /*  82 */ "sac_math ::= MUL xlist",
 /*  83 */ "sac_math ::= DIV xlist",
 /*  84 */ "sac_math ::= GETTIME MINIMUM",
 /*  85 */ "sac_math ::= GETTIME",
 /*  86 */ "sac_math ::= GETTIME MAXIMUM",
 /*  87 */ "sac_math ::= GETTIME MINIMUM state",
 /*  88 */ "sac_math ::= GETTIME MAXIMUM state",
 /*  89 */ "func ::= INTEGER num_or_pstate",
 /*  90 */ "func ::= POWER num_or_pstate",
 /*  91 */ "func ::= ALOG num_or_pstate",
 /*  92 */ "func ::= ALOG10 num_or_pstate",
 /*  93 */ "expr ::= fstring",
 /*  94 */ "string ::= STRING",
 /*  95 */ "string ::= QUOTED_STRING",
 /*  96 */ "string ::= ESCAPE_STRING",
 /*  97 */ "string ::= number",
 /*  98 */ "string_list ::=",
 /*  99 */ "string_list ::= string_list string",
 /* 100 */ "string ::= LPAREN fstring RPAREN",
 /* 101 */ "fstring ::= CHANGE string string string",
 /* 102 */ "fstring ::= SUBSTRING number number string",
 /* 103 */ "fstring ::= DELETE string string",
 /* 104 */ "fstring ::= CONCAT string_list",
 /* 105 */ "fstring ::= BEFORE string string",
 /* 106 */ "fstring ::= AFTER string string",
 /* 107 */ "fstring ::= REPLY string",
 /* 108 */ "fstring ::= FTOA state",
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
    case 27: /* DEBUG */
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

#line 769 "expr_parse_noop.c"
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
#line 955 "expr_parse_noop.c"
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
  { 86, 1 },
  { 86, 0 },
  { 87, 2 },
  { 87, 2 },
  { 80, 3 },
  { 75, 1 },
  { 89, 1 },
  { 89, 3 },
  { 88, 1 },
  { 88, 3 },
  { 87, 2 },
  { 87, 2 },
  { 87, 2 },
  { 87, 2 },
  { 87, 2 },
  { 87, 1 },
  { 87, 2 },
  { 87, 3 },
  { 87, 3 },
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
#line 1182 "expr_parse_noop.c"
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
#line 1200 "expr_parse_noop.c"
        break;
      case 3: /* pin ::= pbits_list NEWLINE */
#line 91 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,9,&yymsp[0].minor);
}
#line 1206 "expr_parse_noop.c"
        break;
      case 5: /* pbits_list ::= pbits_list pbits */
#line 94 "expr_parse_noop.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0);  }
#line 1211 "expr_parse_noop.c"
        break;
      case 6: /* pbits ::= number */
#line 96 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);     }
#line 1216 "expr_parse_noop.c"
        break;
      case 10: /* pbits ::= LPAREN expr RPAREN */
#line 100 "expr_parse_noop.y"
{ yymsp[-1].minor.yy0.col = yymsp[-2].minor.yy0.col; token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1222 "expr_parse_noop.c"
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
#line 1242 "expr_parse_noop.c"
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
#line 1264 "expr_parse_noop.c"
        break;
      case 23: /* evaloptsp ::= evalopts */
#line 146 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);  }
#line 1269 "expr_parse_noop.c"
        break;
      case 24: /* evalopts ::= evalo */
      case 34: /* number ::= blackboard */ yytestcase(yyruleno==34);
      case 35: /* number ::= header */ yytestcase(yyruleno==35);
      case 36: /* number ::= key */ yytestcase(yyruleno==36);
      case 75: /* expr ::= sac_math */ yytestcase(yyruleno==75);
      case 76: /* list_item ::= number */ yytestcase(yyruleno==76);
      case 78: /* xlist ::= list_item */ yytestcase(yyruleno==78);
      case 93: /* expr ::= fstring */ yytestcase(yyruleno==93);
#line 147 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0); }
#line 1281 "expr_parse_noop.c"
        break;
      case 25: /* evalopts ::= evalopts evalo */
#line 148 "expr_parse_noop.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1286 "expr_parse_noop.c"
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
#line 1299 "expr_parse_noop.c"
        break;
      case 27: /* evalo ::= AS FLOAT */
#line 158 "expr_parse_noop.y"
{ eval_asfloat = 1; yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1304 "expr_parse_noop.c"
        break;
      case 28: /* evalo ::= AS INTEGER */
#line 159 "expr_parse_noop.y"
{ eval_asfloat = 0;  yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1309 "expr_parse_noop.c"
        break;
      case 29: /* pbits ::= DEBUG */
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
#line 1333 "expr_parse_noop.c"
        break;
      case 30: /* expr ::= state */
      case 31: /* number ::= NUM */ yytestcase(yyruleno==31);
#line 187 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);            }
#line 1339 "expr_parse_noop.c"
        break;
      case 32: /* number ::= TOK_PI */
#line 189 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, M_PI, yymsp[0].minor.yy0.col);      }
#line 1344 "expr_parse_noop.c"
        break;
      case 33: /* number ::= MINUS number */
#line 191 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, -yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1349 "expr_parse_noop.c"
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
#line 1366 "expr_parse_noop.c"
        break;
      case 38: /* header ::= HEADER */
#line 209 "expr_parse_noop.y"
{ 
  if(!token_var(&yygotominor.yy0, HEADER, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  } 
}
#line 1376 "expr_parse_noop.c"
        break;
      case 39: /* key ::= VARIABLE */
#line 215 "expr_parse_noop.y"
{ 
  if(!token_var(&yygotominor.yy0, VARIABLE, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  }
}
#line 1386 "expr_parse_noop.c"
        break;
      case 40: /* state ::= number */
#line 222 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);        }
#line 1391 "expr_parse_noop.c"
        break;
      case 41: /* state ::= func */
#line 223 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);    }
#line 1396 "expr_parse_noop.c"
        break;
      case 42: /* state ::= MINUS func */
#line 224 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0,-yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1401 "expr_parse_noop.c"
        break;
      case 43: /* state ::= state PLUS state */
#line 226 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value + yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 1407 "expr_parse_noop.c"
        break;
      case 44: /* state ::= state MINUS state */
#line 227 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value - yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 1413 "expr_parse_noop.c"
        break;
      case 45: /* state ::= state TIMES state */
#line 228 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value * yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 1419 "expr_parse_noop.c"
        break;
      case 46: /* state ::= LPAREN state RPAREN */
#line 230 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);         yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1426 "expr_parse_noop.c"
        break;
      case 47: /* num_or_pstate ::= number */
#line 232 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);}
#line 1431 "expr_parse_noop.c"
        break;
      case 48: /* num_or_pstate ::= LPAREN state RPAREN */
#line 233 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);  yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1438 "expr_parse_noop.c"
        break;
      case 49: /* func ::= SIN num_or_pstate */
#line 235 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, sin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1443 "expr_parse_noop.c"
        break;
      case 50: /* func ::= COS num_or_pstate */
#line 236 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, cos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1448 "expr_parse_noop.c"
        break;
      case 51: /* func ::= TAN num_or_pstate */
#line 237 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, tan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1453 "expr_parse_noop.c"
        break;
      case 52: /* func ::= SINH num_or_pstate */
#line 239 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, sinh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1458 "expr_parse_noop.c"
        break;
      case 53: /* func ::= COSH num_or_pstate */
#line 240 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, cosh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1463 "expr_parse_noop.c"
        break;
      case 54: /* func ::= TANH num_or_pstate */
#line 241 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, tanh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1468 "expr_parse_noop.c"
        break;
      case 55: /* func ::= ASIN num_or_pstate */
#line 243 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, asin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1473 "expr_parse_noop.c"
        break;
      case 56: /* func ::= ACOS num_or_pstate */
#line 244 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, acos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1478 "expr_parse_noop.c"
        break;
      case 57: /* func ::= ATAN num_or_pstate */
#line 245 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, atan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1483 "expr_parse_noop.c"
        break;
      case 58: /* func ::= ABS num_or_pstate */
#line 247 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, fabs(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1488 "expr_parse_noop.c"
        break;
      case 59: /* func ::= FLOOR num_or_pstate */
#line 248 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, floor(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1493 "expr_parse_noop.c"
        break;
      case 60: /* func ::= CEIL num_or_pstate */
#line 249 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, ceil(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1498 "expr_parse_noop.c"
        break;
      case 61: /* func ::= EXPON num_or_pstate */
#line 250 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, exp(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);    }
#line 1503 "expr_parse_noop.c"
        break;
      case 62: /* func ::= ROUND num_or_pstate */
#line 251 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, round(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1508 "expr_parse_noop.c"
        break;
      case 63: /* state ::= state EXP state */
#line 253 "expr_parse_noop.y"
{ 
  if(yymsp[-2].minor.yy0.value < 0.0 && fabs(round(yymsp[0].minor.yy0.value) - yymsp[0].minor.yy0.value) >= 1e-15) {
    parse_error(value, TOKEN_STATUS_ERROR_NEGATIVE_FRACTION_POWER);
  } else {
    token_value(&yygotominor.yy0, pow(yymsp[-2].minor.yy0.value, yymsp[0].minor.yy0.value), yymsp[-2].minor.yy0.col);
  }
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 1520 "expr_parse_noop.c"
        break;
      case 64: /* func ::= ATAN2 LPAREN state COMMA state RPAREN */
#line 261 "expr_parse_noop.y"
{ 
  token_value(&yygotominor.yy0, atan2( yymsp[-3].minor.yy0.value, yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1530 "expr_parse_noop.c"
        break;
      case 65: /* func ::= LENGTH LPAREN state COMMA state RPAREN */
#line 264 "expr_parse_noop.y"
{ 
  token_value(&yygotominor.yy0, sqrt( yymsp[-3].minor.yy0.value*yymsp[-3].minor.yy0.value + yymsp[-1].minor.yy0.value*yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1540 "expr_parse_noop.c"
        break;
      case 66: /* func ::= LOGN num_or_pstate */
      case 91: /* func ::= ALOG num_or_pstate */ yytestcase(yyruleno==91);
#line 267 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1552 "expr_parse_noop.c"
        break;
      case 67: /* func ::= LOG10 num_or_pstate */
#line 274 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1563 "expr_parse_noop.c"
        break;
      case 68: /* state ::= state DIVIDE state */
#line 281 "expr_parse_noop.y"
{ 
    if(yymsp[0].minor.yy0.value != 0.0) {
      token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value / yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);
    } else {
      parse_error(value, TOKEN_STATUS_ERROR_DIVIDE_BY_ZERO);
    } 
    yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 1575 "expr_parse_noop.c"
        break;
      case 69: /* func ::= SQRT num_or_pstate */
#line 288 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value >= 0.0) {
    token_value(&yygotominor.yy0, sqrt ( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_SQRT_NEGATIVE);
  }
}
#line 1586 "expr_parse_noop.c"
        break;
      case 70: /* commas ::= COMMA */
#line 297 "expr_parse_noop.y"
{
  yy_destructor(yypParser,20,&yymsp[0].minor);
}
#line 1593 "expr_parse_noop.c"
        break;
      case 72: /* sac_math ::= MINIMUM xlist */
#line 326 "expr_parse_noop.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_min), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1601 "expr_parse_noop.c"
        break;
      case 73: /* sac_math ::= MAXIMUM xlist */
#line 334 "expr_parse_noop.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_max), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1609 "expr_parse_noop.c"
        break;
      case 74: /* state ::= LPAREN sac_math RPAREN */
      case 77: /* list_item ::= LPAREN state RPAREN */ yytestcase(yyruleno==77);
#line 340 "expr_parse_noop.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1617 "expr_parse_noop.c"
        break;
      case 79: /* xlist ::= xlist commas list_item */
#line 346 "expr_parse_noop.y"
{ token_append(&yygotominor.yy0,&yymsp[-2].minor.yy0,&yymsp[0].minor.yy0); }
#line 1622 "expr_parse_noop.c"
        break;
      case 80: /* sac_math ::= ADD xlist */
#line 348 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_add), yymsp[-1].minor.yy0.col); }
#line 1627 "expr_parse_noop.c"
        break;
      case 81: /* sac_math ::= SUB xlist */
#line 349 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_sub), yymsp[-1].minor.yy0.col); }
#line 1632 "expr_parse_noop.c"
        break;
      case 82: /* sac_math ::= MUL xlist */
#line 350 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_mul), yymsp[-1].minor.yy0.col); }
#line 1637 "expr_parse_noop.c"
        break;
      case 83: /* sac_math ::= DIV xlist */
#line 351 "expr_parse_noop.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_div), yymsp[-1].minor.yy0.col); }
#line 1642 "expr_parse_noop.c"
        break;
      case 84: /* sac_math ::= GETTIME MINIMUM */
#line 353 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
  }
  yy_destructor(yypParser,51,&yymsp[0].minor);
}
#line 1654 "expr_parse_noop.c"
        break;
      case 85: /* sac_math ::= GETTIME */
#line 360 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[0].minor.yy0.col;
  }
}
#line 1665 "expr_parse_noop.c"
        break;
      case 86: /* sac_math ::= GETTIME MAXIMUM */
#line 367 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, TRUE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
  }
  yy_destructor(yypParser,52,&yymsp[0].minor);
}
#line 1677 "expr_parse_noop.c"
        break;
      case 87: /* sac_math ::= GETTIME MINIMUM state */
#line 374 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, FALSE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-2].minor.yy0.col;
  }
  yy_destructor(yypParser,51,&yymsp[-1].minor);
}
#line 1689 "expr_parse_noop.c"
        break;
      case 88: /* sac_math ::= GETTIME MAXIMUM state */
#line 381 "expr_parse_noop.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, TRUE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-2].minor.yy0.col;
  }
  yy_destructor(yypParser,52,&yymsp[-1].minor);
}
#line 1701 "expr_parse_noop.c"
        break;
      case 89: /* func ::= INTEGER num_or_pstate */
#line 390 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, (int) yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);   }
#line 1706 "expr_parse_noop.c"
        break;
      case 90: /* func ::= POWER num_or_pstate */
#line 391 "expr_parse_noop.y"
{ token_value(&yygotominor.yy0, pow(10,yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); }
#line 1711 "expr_parse_noop.c"
        break;
      case 92: /* func ::= ALOG10 num_or_pstate */
#line 399 "expr_parse_noop.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0,log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1722 "expr_parse_noop.c"
        break;
      case 94: /* string ::= STRING */
      case 95: /* string ::= QUOTED_STRING */ yytestcase(yyruleno==95);
      case 96: /* string ::= ESCAPE_STRING */ yytestcase(yyruleno==96);
#line 411 "expr_parse_noop.y"
{ token_string(&yygotominor.yy0, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col); }
#line 1729 "expr_parse_noop.c"
        break;
      case 97: /* string ::= number */
#line 414 "expr_parse_noop.y"
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
#line 1746 "expr_parse_noop.c"
        break;
      case 98: /* string_list ::= */
#line 429 "expr_parse_noop.y"
{ token_string(&yygotominor.yy0, strdup(""), lexer_col()); }
#line 1751 "expr_parse_noop.c"
        break;
      case 99: /* string_list ::= string_list string */
#line 430 "expr_parse_noop.y"
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
#line 1767 "expr_parse_noop.c"
        break;
      case 100: /* string ::= LPAREN fstring RPAREN */
#line 444 "expr_parse_noop.y"
{  token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1774 "expr_parse_noop.c"
        break;
      case 101: /* fstring ::= CHANGE string string string */
#line 446 "expr_parse_noop.y"
{
  string *s = string_new( yymsp[0].minor.yy0.str );
  s = string_replace(s, yymsp[-2].minor.yy0.str, yymsp[-1].minor.yy0.str );
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-2].minor.yy0.str);
  FREE(yymsp[-1].minor.yy0.str);
}
#line 1786 "expr_parse_noop.c"
        break;
      case 102: /* fstring ::= SUBSTRING number number string */
#line 454 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string *s2 = string_substr(s, (int)yymsp[-2].minor.yy0.value-1, (int)yymsp[-1].minor.yy0.value-(int)yymsp[-2].minor.yy0.value+1);
  token_string(&yygotominor.yy0, strdup(string_string(s2)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  string_free(&s2);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1798 "expr_parse_noop.c"
        break;
      case 103: /* fstring ::= DELETE string string */
#line 462 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string_replace(s, yymsp[-1].minor.yy0.str, "");
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1810 "expr_parse_noop.c"
        break;
      case 104: /* fstring ::= CONCAT string_list */
#line 471 "expr_parse_noop.y"
{ yymsp[0].minor.yy0.col = yymsp[-1].minor.yy0.col; token_copy(&yygotominor.yy0, &yymsp[0].minor.yy0); }
#line 1815 "expr_parse_noop.c"
        break;
      case 105: /* fstring ::= BEFORE string string */
#line 473 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, f-yymsp[0].minor.yy0.str, -1);
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1828 "expr_parse_noop.c"
        break;
      case 106: /* fstring ::= AFTER string string */
#line 482 "expr_parse_noop.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, 0, (f-yymsp[0].minor.yy0.str)+strlen(yymsp[-1].minor.yy0.str));
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1841 "expr_parse_noop.c"
        break;
      case 107: /* fstring ::= REPLY string */
#line 491 "expr_parse_noop.y"
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
#line 1880 "expr_parse_noop.c"
        break;
      case 108: /* fstring ::= FTOA state */
#line 526 "expr_parse_noop.y"
{ 
  char *s;
  asprintf(&s, "%g", yymsp[0].minor.yy0.value); 
  token_string(&yygotominor.yy0, s, yymsp[-1].minor.yy0.col);
}
#line 1889 "expr_parse_noop.c"
        break;
      default:
      /* (1) pin ::= */ yytestcase(yyruleno==1);
      /* (22) evaloptsp ::= */ yytestcase(yyruleno==22);
      /* (71) commas ::= */ yytestcase(yyruleno==71);
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
#line 1943 "expr_parse_noop.c"
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
#line 1963 "expr_parse_noop.c"
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
#line 1987 "expr_parse_noop.c"
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
