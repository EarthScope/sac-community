/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 2 "expr_parse.y"

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

#line 58 "expr_parse.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 96
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL Token *value;
#define ParseARG_PDECL ,Token *value
#define ParseARG_FETCH Token *value = yypParser->value
#define ParseARG_STORE yypParser->value = value
#define YYNSTATE             104
#define YYNRULE              112
#define YY_MAX_SHIFT         103
#define YY_MIN_SHIFTREDUCE   188
#define YY_MAX_SHIFTREDUCE   299
#define YY_MIN_REDUCE        300
#define YY_MAX_REDUCE        411
#define YY_ERROR_ACTION      412
#define YY_ACCEPT_ACTION     413
#define YY_NO_ACTION         414
/************* End control #defines *******************************************/

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
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
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
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (841)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     4,   95,   26,    1,   74,  212,  219,  413,  103,   27,
 /*    10 */   192,  194,   41,  208,  235,   15,   17,   16,  222,  223,
 /*    20 */   224,  222,  223,  224,   45,    8,  220,  225,  226,  227,
 /*    30 */    62,   61,   60,   59,   58,   57,   56,   55,   54,   53,
 /*    40 */    52,   51,   50,   49,   98,   97,   48,   47,   46,   25,
 /*    50 */    24,   23,   22,   21,   20,   91,    6,  213,   44,   43,
 /*    60 */    42,   39,   64,   37,   75,   34,   32,   30,    7,    4,
 /*    70 */   237,   26,  228,  209,   35,  219,   92,   90,   87,  222,
 /*    80 */   223,  224,  229,  235,  264,  215,  216,  214,  284,  263,
 /*    90 */   222,  223,  224,   45,  283,  220,  225,  226,  227,   62,
 /*   100 */    61,   60,   59,   58,   57,   56,   55,   54,   53,   52,
 /*   110 */    51,   50,   49,   98,   97,   48,   47,   46,   25,   24,
 /*   120 */    23,   22,   21,   20,   91,    6,  291,   44,   43,   42,
 /*   130 */     4,  266,   26,  228,   96,   12,  219,   10,    9,   87,
 /*   140 */   222,  223,  224,  229,  235,  264,   14,   16,  198,  284,
 /*   150 */   300,  222,  223,  224,   45,  282,  220,  225,  226,  227,
 /*   160 */    62,   61,   60,   59,   58,   57,   56,   55,   54,   53,
 /*   170 */    52,   51,   50,   49,   98,   97,   48,   47,   46,    4,
 /*   180 */   302,   26,  384,  384,  302,  219,  302,   40,   44,   43,
 /*   190 */    42,  302,   39,   64,   37,   75,   34,   32,   30,    7,
 /*   200 */   302,  302,  259,   45,  302,  220,  225,  226,  227,   62,
 /*   210 */    61,   60,   59,   58,   57,   56,   55,   54,   53,   52,
 /*   220 */    51,   50,   49,   98,   97,   48,   47,   46,   65,  383,
 /*   230 */   383,  302,  219,  302,  302,  228,  102,   44,   43,   42,
 /*   240 */   302,   87,  222,  223,  224,  229,  302,  264,  302,  259,
 /*   250 */    45,  284,  220,  225,  226,  227,   62,   61,   60,   59,
 /*   260 */    58,   57,   56,   55,   54,   53,   52,   51,   50,   49,
 /*   270 */    98,   97,   48,   47,   46,  254,  302,   19,   18,   15,
 /*   280 */    17,   16,  193,  194,   44,   43,   42,    3,  302,   65,
 /*   290 */   222,  223,  224,  219,  302,  195,  196,  197,  199,  200,
 /*   300 */   201,  202,  203,  204,  205,  206,  207,   66,  302,  302,
 /*   310 */   302,  302,  217,  220,  225,  226,  227,    3,  302,   65,
 /*   320 */   302,  302,  302,  219,  191,  195,  196,  197,  199,  200,
 /*   330 */   201,  202,  203,  204,  205,  206,  207,   19,   18,   15,
 /*   340 */    17,   16,  217,  220,  225,  226,  227,   67,  302,   65,
 /*   350 */   382,  382,  302,  219,   11,  285,  286,  287,  302,    2,
 /*   360 */   288,   65,  302,  302,  302,  219,  302,  222,  223,  224,
 /*   370 */   259,  302,  228,  220,  225,  226,  227,  292,   80,  222,
 /*   380 */   223,  224,  229,  228,   94,  220,  225,  226,  227,   81,
 /*   390 */   222,  223,  224,  229,    5,   93,   65,  381,  381,  302,
 /*   400 */   219,  228,  302,  302,  302,  302,  228,   84,  222,  223,
 /*   410 */   224,  229,   82,  222,  223,  224,  229,  259,  302,  228,
 /*   420 */   220,  225,  226,  227,  228,   83,  222,  223,  224,  229,
 /*   430 */    85,  222,  223,  224,  229,  253,  228,   19,   18,   15,
 /*   440 */    17,   16,   86,  222,  223,  224,  229,  302,  234,  228,
 /*   450 */    19,   18,   15,   17,   16,   76,  222,  223,  224,  229,
 /*   460 */   228,  302,  302,  302,  302,  228,   77,  222,  223,  224,
 /*   470 */   229,   78,  222,  223,  224,  229,  236,  228,   19,   18,
 /*   480 */    15,   17,   16,   79,  222,  223,  224,  229,  228,  302,
 /*   490 */   302,  302,  302,  228,   99,  222,  223,  224,  229,  100,
 /*   500 */   222,  223,  224,  229,  228,  302,  302,  302,  302,  228,
 /*   510 */   101,  222,  223,  224,  229,   89,  222,  223,  224,  229,
 /*   520 */   228,   19,   18,   15,   17,   16,   88,  222,  223,  224,
 /*   530 */   229,  302,  265,  302,  302,  265,  302,  302,   13,  222,
 /*   540 */   223,  224,  222,  223,  224,  265,   68,  267,  265,   69,
 /*   550 */   267,  302,  222,  223,  224,  222,  223,  224,   38,   70,
 /*   560 */   267,  265,   71,  267,  265,  222,  223,  224,  222,  223,
 /*   570 */   224,  222,  223,  224,  302,   72,  267,   65,   73,  267,
 /*   580 */   221,  219,  302,  302,  302,  302,  235,  222,  223,  224,
 /*   590 */   230,  288,  302,  222,  223,  224,  302,  281,  222,  223,
 /*   600 */   224,  220,  225,  226,  227,  288,  302,  302,   28,  288,
 /*   610 */   302,  302,  222,  223,  224,  302,  222,  223,  224,  302,
 /*   620 */   288,  302,  298,  288,  302,  302,  297,  222,  223,  224,
 /*   630 */   222,  223,  224,  302,  288,  302,  302,   31,  288,  302,
 /*   640 */   296,  222,  223,  224,  302,  222,  223,  224,  302,  302,
 /*   650 */   288,   33,  302,  288,  302,  290,  288,  222,  223,  224,
 /*   660 */   222,  223,  224,  222,  223,  224,  302,  294,  288,  302,
 /*   670 */    36,  302,  302,  293,  302,  222,  223,  224,  302,  265,
 /*   680 */   302,  302,  235,  302,  302,   29,  222,  223,  224,  222,
 /*   690 */   223,  224,  302,  280,  268,  235,  302,  302,  302,  302,
 /*   700 */   235,  302,  222,  223,  224,  235,  258,  222,  223,  224,
 /*   710 */   302,  256,  222,  223,  224,  235,  255,  302,  302,  302,
 /*   720 */   235,  302,  222,  223,  224,  302,  251,  222,  223,  224,
 /*   730 */   235,  250,  302,  302,  302,  235,  302,  222,  223,  224,
 /*   740 */   235,  249,  222,  223,  224,  235,  248,  222,  223,  224,
 /*   750 */   235,  247,  222,  223,  224,  302,  246,  222,  223,  224,
 /*   760 */   302,  245,  235,  302,  302,  302,  302,  235,  302,  222,
 /*   770 */   223,  224,  235,  244,  222,  223,  224,  235,  243,  222,
 /*   780 */   223,  224,  235,  242,  222,  223,  224,  302,  241,  222,
 /*   790 */   223,  224,  235,  240,  302,  302,  302,  235,  302,  222,
 /*   800 */   223,  224,  302,  239,  222,  223,  224,   63,  238,   19,
 /*   810 */    18,   15,   17,   16,  222,  223,  224,  221,  374,  374,
 /*   820 */   373,  373,  302,  302,  222,  223,  224,  302,  302,  302,
 /*   830 */   302,  302,  302,  302,  302,  302,  302,  302,  259,  302,
 /*   840 */   259,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,   92,    4,   79,   80,   81,    8,   72,   73,   74,
 /*    10 */    75,   76,   89,   78,   76,    5,    6,    7,   83,   84,
 /*    20 */    85,   83,   84,   85,   26,   87,   28,   29,   30,   31,
 /*    30 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*    40 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    50 */    52,   53,   54,   55,   56,   57,   58,   81,   60,   61,
 /*    60 */    62,   63,   64,   65,   66,   67,   68,   69,   70,    2,
 /*    70 */     1,    4,   76,   77,   94,    8,   23,   24,   82,   83,
 /*    80 */    84,   85,   86,   76,   88,   25,   26,   10,   92,    1,
 /*    90 */    83,   84,   85,   26,   87,   28,   29,   30,   31,   32,
 /*   100 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   110 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   120 */    53,   54,   55,   56,   57,   58,    1,   60,   61,   62,
 /*   130 */     2,    1,    4,   76,   77,    2,    8,   51,   52,   82,
 /*   140 */    83,   84,   85,   86,   76,   88,    2,    7,    1,   92,
 /*   150 */     0,   83,   84,   85,   26,   87,   28,   29,   30,   31,
 /*   160 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*   170 */    42,   43,   44,   45,   46,   47,   48,   49,   50,    2,
 /*   180 */    95,    4,    0,    1,   95,    8,   95,   59,   60,   61,
 /*   190 */    62,   95,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   200 */    95,   95,   20,   26,   95,   28,   29,   30,   31,   32,
 /*   210 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   220 */    43,   44,   45,   46,   47,   48,   49,   50,    4,    0,
 /*   230 */     1,   95,    8,   95,   95,   76,   77,   60,   61,   62,
 /*   240 */    95,   82,   83,   84,   85,   86,   95,   88,   95,   20,
 /*   250 */    26,   92,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   260 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   270 */    46,   47,   48,   49,   50,    1,   95,    3,    4,    5,
 /*   280 */     6,    7,   75,   76,   60,   61,   62,    2,   95,    4,
 /*   290 */    83,   84,   85,    8,   95,   10,   11,   12,   13,   14,
 /*   300 */    15,   16,   17,   18,   19,   20,   21,   22,   95,   95,
 /*   310 */    95,   95,   27,   28,   29,   30,   31,    2,   95,    4,
 /*   320 */    95,   95,   95,    8,    9,   10,   11,   12,   13,   14,
 /*   330 */    15,   16,   17,   18,   19,   20,   21,    3,    4,    5,
 /*   340 */     6,    7,   27,   28,   29,   30,   31,    2,   95,    4,
 /*   350 */     0,    1,   95,    8,   20,   10,   11,   12,   95,    2,
 /*   360 */    76,    4,   95,   95,   95,    8,   95,   83,   84,   85,
 /*   370 */    20,   95,   76,   28,   29,   30,   31,   93,   82,   83,
 /*   380 */    84,   85,   86,   76,   88,   28,   29,   30,   31,   82,
 /*   390 */    83,   84,   85,   86,    2,   88,    4,    0,    1,   95,
 /*   400 */     8,   76,   95,   95,   95,   95,   76,   82,   83,   84,
 /*   410 */    85,   86,   82,   83,   84,   85,   86,   20,   95,   76,
 /*   420 */    28,   29,   30,   31,   76,   82,   83,   84,   85,   86,
 /*   430 */    82,   83,   84,   85,   86,    1,   76,    3,    4,    5,
 /*   440 */     6,    7,   82,   83,   84,   85,   86,   95,    1,   76,
 /*   450 */     3,    4,    5,    6,    7,   82,   83,   84,   85,   86,
 /*   460 */    76,   95,   95,   95,   95,   76,   82,   83,   84,   85,
 /*   470 */    86,   82,   83,   84,   85,   86,    1,   76,    3,    4,
 /*   480 */     5,    6,    7,   82,   83,   84,   85,   86,   76,   95,
 /*   490 */    95,   95,   95,   76,   82,   83,   84,   85,   86,   82,
 /*   500 */    83,   84,   85,   86,   76,   95,   95,   95,   95,   76,
 /*   510 */    82,   83,   84,   85,   86,   82,   83,   84,   85,   86,
 /*   520 */    76,    3,    4,    5,    6,    7,   82,   83,   84,   85,
 /*   530 */    86,   95,   76,   95,   95,   76,   95,   95,   20,   83,
 /*   540 */    84,   85,   83,   84,   85,   76,   90,   91,   76,   90,
 /*   550 */    91,   95,   83,   84,   85,   83,   84,   85,   76,   90,
 /*   560 */    91,   76,   90,   91,   76,   83,   84,   85,   83,   84,
 /*   570 */    85,   83,   84,   85,   95,   90,   91,    4,   90,   91,
 /*   580 */    76,    8,   95,   95,   95,   95,   76,   83,   84,   85,
 /*   590 */    86,   76,   95,   83,   84,   85,   95,   87,   83,   84,
 /*   600 */    85,   28,   29,   30,   31,   76,   95,   95,   93,   76,
 /*   610 */    95,   95,   83,   84,   85,   95,   83,   84,   85,   95,
 /*   620 */    76,   95,   93,   76,   95,   95,   93,   83,   84,   85,
 /*   630 */    83,   84,   85,   95,   76,   95,   95,   93,   76,   95,
 /*   640 */    93,   83,   84,   85,   95,   83,   84,   85,   95,   95,
 /*   650 */    76,   93,   95,   76,   95,   93,   76,   83,   84,   85,
 /*   660 */    83,   84,   85,   83,   84,   85,   95,   93,   76,   95,
 /*   670 */    93,   95,   95,   93,   95,   83,   84,   85,   95,   76,
 /*   680 */    95,   95,   76,   95,   95,   93,   83,   84,   85,   83,
 /*   690 */    84,   85,   95,   87,   91,   76,   95,   95,   95,   95,
 /*   700 */    76,   95,   83,   84,   85,   76,   87,   83,   84,   85,
 /*   710 */    95,   87,   83,   84,   85,   76,   87,   95,   95,   95,
 /*   720 */    76,   95,   83,   84,   85,   95,   87,   83,   84,   85,
 /*   730 */    76,   87,   95,   95,   95,   76,   95,   83,   84,   85,
 /*   740 */    76,   87,   83,   84,   85,   76,   87,   83,   84,   85,
 /*   750 */    76,   87,   83,   84,   85,   95,   87,   83,   84,   85,
 /*   760 */    95,   87,   76,   95,   95,   95,   95,   76,   95,   83,
 /*   770 */    84,   85,   76,   87,   83,   84,   85,   76,   87,   83,
 /*   780 */    84,   85,   76,   87,   83,   84,   85,   95,   87,   83,
 /*   790 */    84,   85,   76,   87,   95,   95,   95,   76,   95,   83,
 /*   800 */    84,   85,   95,   87,   83,   84,   85,   76,   87,    3,
 /*   810 */     4,    5,    6,    7,   83,   84,   85,   76,    0,    1,
 /*   820 */     0,    1,   95,   95,   83,   84,   85,   95,   95,   95,
 /*   830 */    95,   95,   95,   95,   95,   95,   95,   95,   20,   95,
 /*   840 */    20,
};
#define YY_SHIFT_USE_DFLT (-3)
#define YY_SHIFT_COUNT (103)
#define YY_SHIFT_MIN   (-2)
#define YY_SHIFT_MAX   (820)
static const short yy_shift_ofst[] = {
 /*     0 */   285,   -2,   -2,   -2,   67,   67,  128,  177,  177,  177,
 /*    10 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*    20 */   357,  357,  357,  357,  357,  357,  224,  315,  345,  345,
 /*    30 */   345,  345,  345,  345,  345,  345,  345,  345,  345,  345,
 /*    40 */   392,  357,  392,  392,  392,  392,  392,  392,  392,  392,
 /*    50 */   392,  392,  392,  392,  392,  392,  392,  392,  392,  392,
 /*    60 */   392,  392,  392,  573,  573,  573,   53,  129,  182,  229,
 /*    70 */   350,  397,  818,  820,   53,   -3,  274,  334,  434,  518,
 /*    80 */   447,  475,  806,  806,  806,  806,  806,  806,   10,   10,
 /*    90 */    60,   86,   77,   69,   88,  125,  130,  133,  144,  140,
 /*   100 */   140,  140,  147,  150,
};
#define YY_REDUCE_USE_DFLT (-92)
#define YY_REDUCE_COUNT (75)
#define YY_REDUCE_MIN   (-91)
#define YY_REDUCE_MAX   (741)
static const short yy_reduce_ofst[] = {
 /*     0 */   -65,   -4,   57,  159,  296,  307,  325,  330,  343,  348,
 /*    10 */   360,  373,  384,  389,  401,  412,  417,  428,  433,  444,
 /*    20 */   456,  459,  469,  472,  485,  488,  504,  207,  284,  515,
 /*    30 */   529,  533,  544,  547,  558,  562,  574,  577,  580,  592,
 /*    40 */   -62,  603,    7,   68,  510,  606,  619,  624,  629,  639,
 /*    50 */   644,  654,  659,  664,  669,  674,  686,  691,  696,  701,
 /*    60 */   706,  716,  721,  482,  731,  741,  -76,  -91,  -77,  -77,
 /*    70 */   -77,  -77,  -77,  -77,  -24,  -20,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   301,  412,  412,  412,  412,  412,  412,  412,  412,  387,
 /*    10 */   385,  412,  412,  412,  412,  412,  412,  412,  412,  412,
 /*    20 */   412,  412,  412,  412,  412,  412,  412,  302,  412,  412,
 /*    30 */   412,  412,  412,  412,  412,  407,  412,  412,  412,  412,
 /*    40 */   412,  412,  412,  412,  412,  412,  412,  412,  412,  412,
 /*    50 */   412,  412,  412,  412,  412,  412,  412,  412,  412,  412,
 /*    60 */   412,  412,  412,  412,  412,  412,  322,  412,  372,  372,
 /*    70 */   372,  372,  372,  372,  323,  401,  412,  412,  412,  412,
 /*    80 */   412,  412,  411,  390,  391,  389,  388,  330,  343,  344,
 /*    90 */   412,  386,  412,  412,  412,  412,  412,  412,  412,  369,
 /*   100 */   364,  345,  412,  412,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
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
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
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
  ParseARG_SDECL                /* A place to hold %extra_argument */
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
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "DIV",           "GETTIME",       "GETVAL",        "XFILE",       
  "POWER",         "ALOG",          "ALOG10",        "CHANGE",      
  "SUBSTRING",     "DELETE",        "CONCAT",        "BEFORE",      
  "AFTER",         "REPLY",         "FTOA",          "error",       
  "main",          "pin",           "pbits_list",    "pbits",       
  "number",        "expr",          "evaluate",      "evaloptsp",   
  "evalopts",      "evalo",         "state",         "blackboard",  
  "header",        "key",           "func",          "num_or_pstate",
  "sac_math",      "commas",        "xlist",         "list_item",   
  "fstring",       "string",        "string_list", 
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
 /*  90 */ "sac_math ::= GETVAL XFILE num_or_pstate state",
 /*  91 */ "sac_math ::= GETVAL state",
 /*  92 */ "func ::= INTEGER num_or_pstate",
 /*  93 */ "func ::= POWER num_or_pstate",
 /*  94 */ "func ::= ALOG num_or_pstate",
 /*  95 */ "func ::= ALOG10 num_or_pstate",
 /*  96 */ "expr ::= fstring",
 /*  97 */ "string ::= STRING",
 /*  98 */ "string ::= QUOTED_STRING",
 /*  99 */ "string ::= ESCAPE_STRING",
 /* 100 */ "string ::= number",
 /* 101 */ "string_list ::=",
 /* 102 */ "string_list ::= string_list string",
 /* 103 */ "string ::= LPAREN fstring RPAREN",
 /* 104 */ "fstring ::= CHANGE string string string",
 /* 105 */ "fstring ::= SUBSTRING number number string",
 /* 106 */ "fstring ::= DELETE string string",
 /* 107 */ "fstring ::= CONCAT string_list",
 /* 108 */ "fstring ::= BEFORE string string",
 /* 109 */ "fstring ::= AFTER string string",
 /* 110 */ "fstring ::= REPLY string",
 /* 111 */ "fstring ::= FTOA state",
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

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
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
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
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

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
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
    case 58: /* GETVAL */
    case 59: /* XFILE */
    case 60: /* POWER */
    case 61: /* ALOG */
    case 62: /* ALOG10 */
    case 63: /* CHANGE */
    case 64: /* SUBSTRING */
    case 65: /* DELETE */
    case 66: /* CONCAT */
    case 67: /* BEFORE */
    case 68: /* AFTER */
    case 69: /* REPLY */
    case 70: /* FTOA */
{
#line 70 "expr_parse.y"

  yypParser->value = value;
  UNUSED(yypminor);

#line 842 "expr_parse.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
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
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
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
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
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
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
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
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
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
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
#line 55 "expr_parse.y"

  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
  UNUSED(yypMinor);
#line 1019 "expr_parse.c"
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

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
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 72, 1 },
  { 73, 0 },
  { 73, 1 },
  { 73, 2 },
  { 74, 1 },
  { 74, 2 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 3 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 73, 1 },
  { 78, 3 },
  { 79, 0 },
  { 79, 1 },
  { 80, 1 },
  { 80, 2 },
  { 81, 2 },
  { 81, 2 },
  { 81, 2 },
  { 75, 1 },
  { 77, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 2 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 83, 1 },
  { 84, 1 },
  { 85, 1 },
  { 82, 1 },
  { 82, 1 },
  { 82, 2 },
  { 82, 3 },
  { 82, 3 },
  { 82, 3 },
  { 82, 3 },
  { 87, 1 },
  { 87, 3 },
  { 87, 3 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 82, 3 },
  { 86, 6 },
  { 86, 6 },
  { 86, 2 },
  { 86, 2 },
  { 82, 3 },
  { 86, 2 },
  { 89, 1 },
  { 89, 0 },
  { 88, 2 },
  { 88, 2 },
  { 82, 3 },
  { 77, 1 },
  { 91, 1 },
  { 91, 3 },
  { 90, 1 },
  { 90, 3 },
  { 88, 2 },
  { 88, 2 },
  { 88, 2 },
  { 88, 2 },
  { 88, 2 },
  { 88, 1 },
  { 88, 2 },
  { 88, 3 },
  { 88, 3 },
  { 88, 4 },
  { 88, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 86, 2 },
  { 77, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 94, 0 },
  { 94, 2 },
  { 93, 3 },
  { 92, 4 },
  { 92, 4 },
  { 92, 3 },
  { 92, 2 },
  { 92, 3 },
  { 92, 3 },
  { 92, 2 },
  { 92, 2 },
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
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */
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
/********** Begin reduce actions **********************************************/
      case 0: /* main ::= pin */
#line 83 "expr_parse.y"
{ 
  if(value->error <=0){
    token_copy(value, &yymsp[0].minor.yy0);
  } 
}
#line 1246 "expr_parse.c"
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
#line 90 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);      }
#line 1264 "expr_parse.c"
        break;
      case 3: /* pin ::= pbits_list NEWLINE */
#line 91 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,9,&yymsp[0].minor);
}
#line 1270 "expr_parse.c"
        break;
      case 5: /* pbits_list ::= pbits_list pbits */
#line 94 "expr_parse.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0);  }
#line 1275 "expr_parse.c"
        break;
      case 6: /* pbits ::= number */
#line 96 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);     }
#line 1280 "expr_parse.c"
        break;
      case 10: /* pbits ::= LPAREN expr RPAREN */
#line 100 "expr_parse.y"
{ yymsp[-1].minor.yy0.col = yymsp[-2].minor.yy0.col; token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1286 "expr_parse.c"
        break;
      case 20: /* pin ::= evaluate */
#line 111 "expr_parse.y"
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
#line 1306 "expr_parse.c"
        break;
      case 21: /* evaluate ::= EVALUATE evaloptsp expr */
#line 127 "expr_parse.y"
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
#line 1328 "expr_parse.c"
        break;
      case 23: /* evaloptsp ::= evalopts */
#line 146 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);  }
#line 1333 "expr_parse.c"
        break;
      case 24: /* evalopts ::= evalo */
      case 34: /* number ::= blackboard */ yytestcase(yyruleno==34);
      case 35: /* number ::= header */ yytestcase(yyruleno==35);
      case 36: /* number ::= key */ yytestcase(yyruleno==36);
      case 76: /* expr ::= sac_math */ yytestcase(yyruleno==76);
      case 77: /* list_item ::= number */ yytestcase(yyruleno==77);
      case 79: /* xlist ::= list_item */ yytestcase(yyruleno==79);
      case 96: /* expr ::= fstring */ yytestcase(yyruleno==96);
#line 147 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0); }
#line 1345 "expr_parse.c"
        break;
      case 25: /* evalopts ::= evalopts evalo */
#line 148 "expr_parse.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1350 "expr_parse.c"
        break;
      case 26: /* evalo ::= TO STRING */
#line 149 "expr_parse.y"
{ 
   if(eval_out) {
     free(eval_out); 
     eval_out = NULL; 
   }
   eval_out = strdup(yymsp[0].minor.yy0.str); 
   yymsp[-1].minor.yy0.type = STRING;
   token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0);
}
#line 1363 "expr_parse.c"
        break;
      case 27: /* evalo ::= AS FLOAT */
#line 158 "expr_parse.y"
{ eval_asfloat = 1; yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1368 "expr_parse.c"
        break;
      case 28: /* evalo ::= AS INTEGER */
#line 159 "expr_parse.y"
{ eval_asfloat = 0;  yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1373 "expr_parse.c"
        break;
      case 29: /* pbits ::= DEBUG_PARSER */
#line 166 "expr_parse.y"
{
  if(!tdebug) {
    tdebug = 1;
                  
    ParseTrace(stdout, "expr: ");
      
                
                                     
      
  } else {
    tdebug = 0;
                  
    ParseTrace(NULL, "expr: ");
      
                
                                   
      
  }
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
#line 1397 "expr_parse.c"
        break;
      case 30: /* expr ::= state */
      case 31: /* number ::= NUM */ yytestcase(yyruleno==31);
#line 187 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);            }
#line 1403 "expr_parse.c"
        break;
      case 32: /* number ::= TOK_PI */
#line 189 "expr_parse.y"
{ token_value(&yygotominor.yy0, M_PI, yymsp[0].minor.yy0.col);      }
#line 1408 "expr_parse.c"
        break;
      case 33: /* number ::= MINUS number */
#line 191 "expr_parse.y"
{ token_value(&yygotominor.yy0, -yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1413 "expr_parse.c"
        break;
      case 37: /* blackboard ::= BLACKBOARD */
#line 196 "expr_parse.y"
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
#line 1430 "expr_parse.c"
        break;
      case 38: /* header ::= HEADER */
#line 209 "expr_parse.y"
{ 
  if(!token_var(&yygotominor.yy0, HEADER, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  } 
}
#line 1440 "expr_parse.c"
        break;
      case 39: /* key ::= VARIABLE */
#line 215 "expr_parse.y"
{ 
  if(!token_var(&yygotominor.yy0, VARIABLE, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  }
}
#line 1450 "expr_parse.c"
        break;
      case 40: /* state ::= number */
#line 222 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);        }
#line 1455 "expr_parse.c"
        break;
      case 41: /* state ::= func */
#line 223 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);    }
#line 1460 "expr_parse.c"
        break;
      case 42: /* state ::= MINUS func */
#line 224 "expr_parse.y"
{ token_value(&yygotominor.yy0,-yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1465 "expr_parse.c"
        break;
      case 43: /* state ::= state PLUS state */
#line 226 "expr_parse.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value + yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 1471 "expr_parse.c"
        break;
      case 44: /* state ::= state MINUS state */
#line 227 "expr_parse.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value - yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 1477 "expr_parse.c"
        break;
      case 45: /* state ::= state TIMES state */
#line 228 "expr_parse.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value * yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 1483 "expr_parse.c"
        break;
      case 46: /* state ::= LPAREN state RPAREN */
#line 230 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);         yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1490 "expr_parse.c"
        break;
      case 47: /* num_or_pstate ::= number */
#line 232 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);}
#line 1495 "expr_parse.c"
        break;
      case 48: /* num_or_pstate ::= LPAREN state RPAREN */
      case 49: /* num_or_pstate ::= LPAREN sac_math RPAREN */ yytestcase(yyruleno==49);
#line 233 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);  yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1503 "expr_parse.c"
        break;
      case 50: /* func ::= SIN num_or_pstate */
#line 236 "expr_parse.y"
{ token_value(&yygotominor.yy0, sin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1508 "expr_parse.c"
        break;
      case 51: /* func ::= COS num_or_pstate */
#line 237 "expr_parse.y"
{ token_value(&yygotominor.yy0, cos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1513 "expr_parse.c"
        break;
      case 52: /* func ::= TAN num_or_pstate */
#line 238 "expr_parse.y"
{ token_value(&yygotominor.yy0, tan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1518 "expr_parse.c"
        break;
      case 53: /* func ::= SINH num_or_pstate */
#line 240 "expr_parse.y"
{ token_value(&yygotominor.yy0, sinh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1523 "expr_parse.c"
        break;
      case 54: /* func ::= COSH num_or_pstate */
#line 241 "expr_parse.y"
{ token_value(&yygotominor.yy0, cosh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1528 "expr_parse.c"
        break;
      case 55: /* func ::= TANH num_or_pstate */
#line 242 "expr_parse.y"
{ token_value(&yygotominor.yy0, tanh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1533 "expr_parse.c"
        break;
      case 56: /* func ::= ASIN num_or_pstate */
#line 244 "expr_parse.y"
{ token_value(&yygotominor.yy0, asin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1538 "expr_parse.c"
        break;
      case 57: /* func ::= ACOS num_or_pstate */
#line 245 "expr_parse.y"
{ token_value(&yygotominor.yy0, acos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1543 "expr_parse.c"
        break;
      case 58: /* func ::= ATAN num_or_pstate */
#line 246 "expr_parse.y"
{ token_value(&yygotominor.yy0, atan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1548 "expr_parse.c"
        break;
      case 59: /* func ::= ABS num_or_pstate */
#line 248 "expr_parse.y"
{ token_value(&yygotominor.yy0, fabs(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1553 "expr_parse.c"
        break;
      case 60: /* func ::= FLOOR num_or_pstate */
#line 249 "expr_parse.y"
{ token_value(&yygotominor.yy0, floor(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1558 "expr_parse.c"
        break;
      case 61: /* func ::= CEIL num_or_pstate */
#line 250 "expr_parse.y"
{ token_value(&yygotominor.yy0, ceil(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1563 "expr_parse.c"
        break;
      case 62: /* func ::= EXPON num_or_pstate */
#line 251 "expr_parse.y"
{ token_value(&yygotominor.yy0, exp(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);    }
#line 1568 "expr_parse.c"
        break;
      case 63: /* func ::= ROUND num_or_pstate */
#line 252 "expr_parse.y"
{ token_value(&yygotominor.yy0, round(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1573 "expr_parse.c"
        break;
      case 64: /* state ::= state EXP state */
#line 254 "expr_parse.y"
{ 
  if(yymsp[-2].minor.yy0.value < 0.0 && fabs(round(yymsp[0].minor.yy0.value) - yymsp[0].minor.yy0.value) >= 1e-15) {
    parse_error(value, TOKEN_STATUS_ERROR_NEGATIVE_FRACTION_POWER);
  } else {
    token_value(&yygotominor.yy0, pow(yymsp[-2].minor.yy0.value, yymsp[0].minor.yy0.value), yymsp[-2].minor.yy0.col);
  }
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 1585 "expr_parse.c"
        break;
      case 65: /* func ::= ATAN2 LPAREN state COMMA state RPAREN */
#line 262 "expr_parse.y"
{ 
  token_value(&yygotominor.yy0, atan2( yymsp[-3].minor.yy0.value, yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1595 "expr_parse.c"
        break;
      case 66: /* func ::= LENGTH LPAREN state COMMA state RPAREN */
#line 265 "expr_parse.y"
{ 
  token_value(&yygotominor.yy0, sqrt( yymsp[-3].minor.yy0.value*yymsp[-3].minor.yy0.value + yymsp[-1].minor.yy0.value*yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1605 "expr_parse.c"
        break;
      case 67: /* func ::= LOGN num_or_pstate */
      case 94: /* func ::= ALOG num_or_pstate */ yytestcase(yyruleno==94);
#line 268 "expr_parse.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1617 "expr_parse.c"
        break;
      case 68: /* func ::= LOG10 num_or_pstate */
#line 275 "expr_parse.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1628 "expr_parse.c"
        break;
      case 69: /* state ::= state DIVIDE state */
#line 282 "expr_parse.y"
{ 
    if(yymsp[0].minor.yy0.value != 0.0) {
      token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value / yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);
    } else {
      parse_error(value, TOKEN_STATUS_ERROR_DIVIDE_BY_ZERO);
    } 
    yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 1640 "expr_parse.c"
        break;
      case 70: /* func ::= SQRT num_or_pstate */
#line 289 "expr_parse.y"
{ 
  if(yymsp[0].minor.yy0.value >= 0.0) {
    token_value(&yygotominor.yy0, sqrt ( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_SQRT_NEGATIVE);
  }
}
#line 1651 "expr_parse.c"
        break;
      case 71: /* commas ::= COMMA */
#line 298 "expr_parse.y"
{
  yy_destructor(yypParser,20,&yymsp[0].minor);
}
#line 1658 "expr_parse.c"
        break;
      case 73: /* sac_math ::= MINIMUM xlist */
#line 327 "expr_parse.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_min), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1666 "expr_parse.c"
        break;
      case 74: /* sac_math ::= MAXIMUM xlist */
#line 335 "expr_parse.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_max), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1674 "expr_parse.c"
        break;
      case 75: /* state ::= LPAREN sac_math RPAREN */
      case 78: /* list_item ::= LPAREN expr RPAREN */ yytestcase(yyruleno==78);
#line 341 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1682 "expr_parse.c"
        break;
      case 80: /* xlist ::= xlist commas list_item */
#line 347 "expr_parse.y"
{ token_append(&yygotominor.yy0,&yymsp[-2].minor.yy0,&yymsp[0].minor.yy0); }
#line 1687 "expr_parse.c"
        break;
      case 81: /* sac_math ::= ADD xlist */
#line 349 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_add), yymsp[-1].minor.yy0.col); }
#line 1692 "expr_parse.c"
        break;
      case 82: /* sac_math ::= SUB xlist */
#line 350 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_sub), yymsp[-1].minor.yy0.col); }
#line 1697 "expr_parse.c"
        break;
      case 83: /* sac_math ::= MUL xlist */
#line 351 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_mul), yymsp[-1].minor.yy0.col); }
#line 1702 "expr_parse.c"
        break;
      case 84: /* sac_math ::= DIV xlist */
#line 352 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_div), yymsp[-1].minor.yy0.col); }
#line 1707 "expr_parse.c"
        break;
      case 85: /* sac_math ::= GETTIME MINIMUM */
#line 354 "expr_parse.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
  }
  yy_destructor(yypParser,51,&yymsp[0].minor);
}
#line 1719 "expr_parse.c"
        break;
      case 86: /* sac_math ::= GETTIME */
#line 361 "expr_parse.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, FALSE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[0].minor.yy0.col;
  }
}
#line 1730 "expr_parse.c"
        break;
      case 87: /* sac_math ::= GETTIME MAXIMUM */
#line 368 "expr_parse.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, NULL, TRUE, FALSE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
  }
  yy_destructor(yypParser,52,&yymsp[0].minor);
}
#line 1742 "expr_parse.c"
        break;
      case 88: /* sac_math ::= GETTIME MINIMUM state */
#line 375 "expr_parse.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, FALSE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-2].minor.yy0.col;
  }
  yy_destructor(yypParser,51,&yymsp[-1].minor);
}
#line 1754 "expr_parse.c"
        break;
      case 89: /* sac_math ::= GETTIME MAXIMUM state */
#line 382 "expr_parse.y"
{ 
  if(!gettime_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, TRUE, TRUE)) {
    parse_error(value, TOKEN_STATUS_ERROR_GETTIME);
  } else {
    yygotominor.yy0.col = yymsp[-2].minor.yy0.col;
  }
  yy_destructor(yypParser,52,&yymsp[-1].minor);
}
#line 1766 "expr_parse.c"
        break;
      case 90: /* sac_math ::= GETVAL XFILE num_or_pstate state */
#line 389 "expr_parse.y"
{
    if(!getval_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, &yymsp[-1].minor.yy0, TRUE)) {
        parse_error(value, TOKEN_STATUS_ERROR_GETVAL);
    } else {
        yygotominor.yy0.col = yymsp[-3].minor.yy0.col;
    }
  yy_destructor(yypParser,59,&yymsp[-2].minor);
}
#line 1778 "expr_parse.c"
        break;
      case 91: /* sac_math ::= GETVAL state */
#line 396 "expr_parse.y"
{
    if(!getval_expr(&yygotominor.yy0, &yymsp[0].minor.yy0, NULL, FALSE)) {
        parse_error(value, TOKEN_STATUS_ERROR_GETVAL);
    } else {
        yygotominor.yy0.col = yymsp[-1].minor.yy0.col;
    }
}
#line 1789 "expr_parse.c"
        break;
      case 92: /* func ::= INTEGER num_or_pstate */
#line 404 "expr_parse.y"
{ token_value(&yygotominor.yy0, (int) yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);   }
#line 1794 "expr_parse.c"
        break;
      case 93: /* func ::= POWER num_or_pstate */
#line 405 "expr_parse.y"
{ token_value(&yygotominor.yy0, pow(10,yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); }
#line 1799 "expr_parse.c"
        break;
      case 95: /* func ::= ALOG10 num_or_pstate */
#line 413 "expr_parse.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0,log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1810 "expr_parse.c"
        break;
      case 97: /* string ::= STRING */
      case 98: /* string ::= QUOTED_STRING */ yytestcase(yyruleno==98);
      case 99: /* string ::= ESCAPE_STRING */ yytestcase(yyruleno==99);
#line 425 "expr_parse.y"
{ token_string(&yygotominor.yy0, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col); }
#line 1817 "expr_parse.c"
        break;
      case 100: /* string ::= number */
#line 428 "expr_parse.y"
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
#line 1834 "expr_parse.c"
        break;
      case 101: /* string_list ::= */
#line 443 "expr_parse.y"
{ token_string(&yygotominor.yy0, strdup(""), lexer_col()); }
#line 1839 "expr_parse.c"
        break;
      case 102: /* string_list ::= string_list string */
#line 444 "expr_parse.y"
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
#line 1855 "expr_parse.c"
        break;
      case 103: /* string ::= LPAREN fstring RPAREN */
#line 458 "expr_parse.y"
{  token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1862 "expr_parse.c"
        break;
      case 104: /* fstring ::= CHANGE string string string */
#line 460 "expr_parse.y"
{
  string *s = string_new( yymsp[0].minor.yy0.str );
  s = string_replace(s, yymsp[-2].minor.yy0.str, yymsp[-1].minor.yy0.str );
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-2].minor.yy0.str);
  FREE(yymsp[-1].minor.yy0.str);
}
#line 1874 "expr_parse.c"
        break;
      case 105: /* fstring ::= SUBSTRING number number string */
#line 468 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string *s2 = string_substr(s, (int)yymsp[-2].minor.yy0.value-1, (int)yymsp[-1].minor.yy0.value-(int)yymsp[-2].minor.yy0.value+1);
  token_string(&yygotominor.yy0, strdup(string_string(s2)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  string_free(&s2);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1886 "expr_parse.c"
        break;
      case 106: /* fstring ::= DELETE string string */
#line 476 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string_replace(s, yymsp[-1].minor.yy0.str, "");
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1898 "expr_parse.c"
        break;
      case 107: /* fstring ::= CONCAT string_list */
#line 485 "expr_parse.y"
{ yymsp[0].minor.yy0.col = yymsp[-1].minor.yy0.col; token_copy(&yygotominor.yy0, &yymsp[0].minor.yy0); }
#line 1903 "expr_parse.c"
        break;
      case 108: /* fstring ::= BEFORE string string */
#line 487 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, f-yymsp[0].minor.yy0.str, -1);
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1916 "expr_parse.c"
        break;
      case 109: /* fstring ::= AFTER string string */
#line 496 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, 0, (f-yymsp[0].minor.yy0.str)+strlen(yymsp[-1].minor.yy0.str));
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1929 "expr_parse.c"
        break;
      case 110: /* fstring ::= REPLY string */
#line 505 "expr_parse.y"
{
  char *a, *b, *def;
  char in[1024];
  char *prmt = NULL;
  memset(in, 0, sizeof(in));
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
  asprintf(&prmt, "%s$", yymsp[0].minor.yy0.str);
  zgpmsg(prmt, strlen(prmt), in, 1023);
  FREE(prmt);
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
  FREE(def);
}
#line 1967 "expr_parse.c"
        break;
      case 111: /* fstring ::= FTOA state */
#line 539 "expr_parse.y"
{ 
  char *s;
  asprintf(&s, "%g", yymsp[0].minor.yy0.value); 
  token_string(&yygotominor.yy0, s, yymsp[-1].minor.yy0.col);
}
#line 1976 "expr_parse.c"
        break;
      default:
      /* (1) pin ::= */ yytestcase(yyruleno==1);
      /* (22) evaloptsp ::= */ yytestcase(yyruleno==22);
      /* (72) commas ::= */ yytestcase(yyruleno==72);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno>=0 && yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    /* If the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
      yyTraceShift(yypParser, yyact);
    }else{
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
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
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
#line 60 "expr_parse.y"

  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
#line 2032 "expr_parse.c"
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
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
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
/************ Begin %syntax_error code ****************************************/
#line 75 "expr_parse.y"

  UNUSED(yymajor);
  UNUSED(yyminor);
  parse_error(value, TOKEN_STATUS_ERROR_SYNTAX);
#line 2054 "expr_parse.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
#line 64 "expr_parse.y"

  if(value->error <= 0) {
    value->error = TOKEN_STATUS_OK;
  }
#line 2080 "expr_parse.c"
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
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
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
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
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
  yyminorunion.yy0 = yyminor;
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
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
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
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
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}

