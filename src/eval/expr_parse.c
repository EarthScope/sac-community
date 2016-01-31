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
#define YYNOCODE 94
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
#define YYNSTATE             99
#define YYNRULE              110
#define YY_MAX_SHIFT         98
#define YY_MIN_SHIFTREDUCE   183
#define YY_MAX_SHIFTREDUCE   292
#define YY_MIN_REDUCE        293
#define YY_MAX_REDUCE        402
#define YY_ERROR_ACTION      403
#define YY_ACCEPT_ACTION     404
#define YY_NO_ACTION         405
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
#define YY_ACTTAB_COUNT (788)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     4,   90,   24,    1,   71,  207,  214,  404,   98,   25,
 /*    10 */   187,  189,   38,  203,  230,   13,   15,   14,  217,  218,
 /*    20 */   219,  217,  218,  219,   42,  276,  215,  220,  221,  222,
 /*    30 */    59,   58,   57,   56,   55,   54,   53,   52,   51,   50,
 /*    40 */    49,   48,   47,   46,   93,   92,   45,   44,   43,   23,
 /*    50 */    22,   21,   20,   19,   18,   86,   41,   40,   39,   37,
 /*    60 */    61,   35,   72,   32,   30,   28,    6,    4,  208,   24,
 /*    70 */   223,  204,   33,  214,   87,   85,   82,  217,  218,  219,
 /*    80 */   224,  230,  259,  210,  211,  209,  277,  232,  217,  218,
 /*    90 */   219,   42,  275,  215,  220,  221,  222,   59,   58,   57,
 /*   100 */    56,   55,   54,   53,   52,   51,   50,   49,   48,   47,
 /*   110 */    46,   93,   92,   45,   44,   43,   23,   22,   21,   20,
 /*   120 */    19,   18,   86,   41,   40,   39,    4,  258,   24,  223,
 /*   130 */    91,  284,  214,    8,    7,   82,  217,  218,  219,  224,
 /*   140 */   230,  259,  261,   14,   10,  277,  193,  217,  218,  219,
 /*   150 */    42,  274,  215,  220,  221,  222,   59,   58,   57,   56,
 /*   160 */    55,   54,   53,   52,   51,   50,   49,   48,   47,   46,
 /*   170 */    93,   92,   45,   44,   43,   62,   12,  293,  295,  214,
 /*   180 */   223,   97,   41,   40,   39,  295,   82,  217,  218,  219,
 /*   190 */   224,  295,  259,  295,  295,  295,  277,   42,  295,  215,
 /*   200 */   220,  221,  222,   59,   58,   57,   56,   55,   54,   53,
 /*   210 */    52,   51,   50,   49,   48,   47,   46,   93,   92,   45,
 /*   220 */    44,   43,  249,  295,   17,   16,   13,   15,   14,   41,
 /*   230 */    40,   39,    3,  295,   62,  377,  377,  295,  214,  295,
 /*   240 */   190,  191,  192,  194,  195,  196,  197,  198,  199,  200,
 /*   250 */   201,  202,   63,  295,  295,  254,  295,  212,  215,  220,
 /*   260 */   221,  222,    3,  295,   62,  295,  295,  295,  214,  186,
 /*   270 */   190,  191,  192,  194,  195,  196,  197,  198,  199,  200,
 /*   280 */   201,  202,   17,   16,   13,   15,   14,  212,  215,  220,
 /*   290 */   221,  222,   64,  295,   62,  295,  295,  295,  214,    9,
 /*   300 */   278,  279,  280,   37,   61,   35,   72,   32,   30,   28,
 /*   310 */     6,   17,   16,   13,   15,   14,  295,  223,  215,  220,
 /*   320 */   221,  222,  295,   77,  217,  218,  219,  224,    2,   89,
 /*   330 */    62,  295,  223,  295,  214,  295,  295,  295,   78,  217,
 /*   340 */   218,  219,  224,    5,   88,   62,  376,  376,  295,  214,
 /*   350 */   295,  295,  223,  295,  215,  220,  221,  222,   79,  217,
 /*   360 */   218,  219,  224,  295,  295,  295,  254,  223,  295,  215,
 /*   370 */   220,  221,  222,   80,  217,  218,  219,  224,  223,  295,
 /*   380 */   295,  295,  295,  223,   81,  217,  218,  219,  224,   73,
 /*   390 */   217,  218,  219,  224,  223,  375,  375,  374,  374,  223,
 /*   400 */    74,  217,  218,  219,  224,   75,  217,  218,  219,  224,
 /*   410 */   367,  367,  295,  223,  295,  254,  295,  254,  223,   76,
 /*   420 */   217,  218,  219,  224,   94,  217,  218,  219,  224,  223,
 /*   430 */   254,  295,  295,  295,  223,   95,  217,  218,  219,  224,
 /*   440 */    96,  217,  218,  219,  224,  223,  366,  366,  295,  295,
 /*   450 */   223,   84,  217,  218,  219,  224,   83,  217,  218,  219,
 /*   460 */   224,  260,  295,  295,  260,  295,  254,  295,  217,  218,
 /*   470 */   219,  217,  218,  219,  260,   65,  262,  260,   66,  262,
 /*   480 */   295,  217,  218,  219,  217,  218,  219,  295,   67,  262,
 /*   490 */    62,   68,  262,  260,  214,  295,  260,  295,  295,  295,
 /*   500 */   217,  218,  219,  217,  218,  219,  295,   69,  262,  295,
 /*   510 */    70,  262,  295,  295,  215,  220,  221,  222,  248,  295,
 /*   520 */    17,   16,   13,   15,   14,   17,   16,   13,   15,   14,
 /*   530 */   229,  216,   17,   16,   13,   15,   14,  295,  217,  218,
 /*   540 */   219,  225,   11,  295,  231,  295,   17,   16,   13,   15,
 /*   550 */    14,  188,  189,  295,  295,  295,  281,  295,  295,  217,
 /*   560 */   218,  219,  295,  217,  218,  219,  295,  281,  295,  295,
 /*   570 */   295,  281,  295,  285,  217,  218,  219,  295,  217,  218,
 /*   580 */   219,  295,  281,  295,   26,  281,  295,  295,  291,  217,
 /*   590 */   218,  219,  217,  218,  219,  295,  295,  281,  295,  290,
 /*   600 */   281,  295,   29,  295,  217,  218,  219,  217,  218,  219,
 /*   610 */   295,  281,  295,  295,  289,  281,  295,   31,  217,  218,
 /*   620 */   219,  295,  217,  218,  219,  295,  281,  295,  283,  295,
 /*   630 */   295,  281,  287,  217,  218,  219,  295,  295,  217,  218,
 /*   640 */   219,  295,  281,   34,  295,  295,  260,  295,  286,  217,
 /*   650 */   218,  219,  295,  217,  218,  219,  295,  230,  295,   27,
 /*   660 */   295,  263,  295,  230,  217,  218,  219,  295,  273,  230,
 /*   670 */   217,  218,  219,  295,  253,  230,  217,  218,  219,  295,
 /*   680 */   251,  230,  217,  218,  219,  295,  250,  295,  217,  218,
 /*   690 */   219,  295,  246,  230,  295,  295,  295,  295,  230,  295,
 /*   700 */   217,  218,  219,  230,  245,  217,  218,  219,  230,  244,
 /*   710 */   217,  218,  219,  230,  243,  217,  218,  219,  230,  242,
 /*   720 */   217,  218,  219,  230,  241,  217,  218,  219,  230,  240,
 /*   730 */   217,  218,  219,  295,  239,  217,  218,  219,  230,  238,
 /*   740 */   295,  295,  295,  230,  295,  217,  218,  219,  295,  237,
 /*   750 */   217,  218,  219,  230,  236,  295,  295,  295,  230,  295,
 /*   760 */   217,  218,  219,  230,  235,  217,  218,  219,   36,  234,
 /*   770 */   217,  218,  219,   60,  233,  217,  218,  219,  216,  295,
 /*   780 */   217,  218,  219,  295,  295,  217,  218,  219,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,   90,    4,   77,   78,   79,    8,   70,   71,   72,
 /*    10 */    73,   74,   87,   76,   74,    5,    6,    7,   81,   82,
 /*    20 */    83,   81,   82,   83,   26,   85,   28,   29,   30,   31,
 /*    30 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*    40 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    50 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    60 */    62,   63,   64,   65,   66,   67,   68,    2,   79,    4,
 /*    70 */    74,   75,   92,    8,   23,   24,   80,   81,   82,   83,
 /*    80 */    84,   74,   86,   25,   26,   10,   90,    1,   81,   82,
 /*    90 */    83,   26,   85,   28,   29,   30,   31,   32,   33,   34,
 /*   100 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*   110 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   120 */    55,   56,   57,   58,   59,   60,    2,    1,    4,   74,
 /*   130 */    75,    1,    8,   51,   52,   80,   81,   82,   83,   84,
 /*   140 */    74,   86,    1,    7,    2,   90,    1,   81,   82,   83,
 /*   150 */    26,   85,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   160 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   170 */    46,   47,   48,   49,   50,    4,    2,    0,   93,    8,
 /*   180 */    74,   75,   58,   59,   60,   93,   80,   81,   82,   83,
 /*   190 */    84,   93,   86,   93,   93,   93,   90,   26,   93,   28,
 /*   200 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   210 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*   220 */    49,   50,    1,   93,    3,    4,    5,    6,    7,   58,
 /*   230 */    59,   60,    2,   93,    4,    0,    1,   93,    8,   93,
 /*   240 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*   250 */    20,   21,   22,   93,   93,   20,   93,   27,   28,   29,
 /*   260 */    30,   31,    2,   93,    4,   93,   93,   93,    8,    9,
 /*   270 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*   280 */    20,   21,    3,    4,    5,    6,    7,   27,   28,   29,
 /*   290 */    30,   31,    2,   93,    4,   93,   93,   93,    8,   20,
 /*   300 */    10,   11,   12,   61,   62,   63,   64,   65,   66,   67,
 /*   310 */    68,    3,    4,    5,    6,    7,   93,   74,   28,   29,
 /*   320 */    30,   31,   93,   80,   81,   82,   83,   84,    2,   86,
 /*   330 */     4,   93,   74,   93,    8,   93,   93,   93,   80,   81,
 /*   340 */    82,   83,   84,    2,   86,    4,    0,    1,   93,    8,
 /*   350 */    93,   93,   74,   93,   28,   29,   30,   31,   80,   81,
 /*   360 */    82,   83,   84,   93,   93,   93,   20,   74,   93,   28,
 /*   370 */    29,   30,   31,   80,   81,   82,   83,   84,   74,   93,
 /*   380 */    93,   93,   93,   74,   80,   81,   82,   83,   84,   80,
 /*   390 */    81,   82,   83,   84,   74,    0,    1,    0,    1,   74,
 /*   400 */    80,   81,   82,   83,   84,   80,   81,   82,   83,   84,
 /*   410 */     0,    1,   93,   74,   93,   20,   93,   20,   74,   80,
 /*   420 */    81,   82,   83,   84,   80,   81,   82,   83,   84,   74,
 /*   430 */    20,   93,   93,   93,   74,   80,   81,   82,   83,   84,
 /*   440 */    80,   81,   82,   83,   84,   74,    0,    1,   93,   93,
 /*   450 */    74,   80,   81,   82,   83,   84,   80,   81,   82,   83,
 /*   460 */    84,   74,   93,   93,   74,   93,   20,   93,   81,   82,
 /*   470 */    83,   81,   82,   83,   74,   88,   89,   74,   88,   89,
 /*   480 */    93,   81,   82,   83,   81,   82,   83,   93,   88,   89,
 /*   490 */     4,   88,   89,   74,    8,   93,   74,   93,   93,   93,
 /*   500 */    81,   82,   83,   81,   82,   83,   93,   88,   89,   93,
 /*   510 */    88,   89,   93,   93,   28,   29,   30,   31,    1,   93,
 /*   520 */     3,    4,    5,    6,    7,    3,    4,    5,    6,    7,
 /*   530 */     1,   74,    3,    4,    5,    6,    7,   93,   81,   82,
 /*   540 */    83,   84,   20,   93,    1,   93,    3,    4,    5,    6,
 /*   550 */     7,   73,   74,   93,   93,   93,   74,   93,   93,   81,
 /*   560 */    82,   83,   93,   81,   82,   83,   93,   74,   93,   93,
 /*   570 */    93,   74,   93,   91,   81,   82,   83,   93,   81,   82,
 /*   580 */    83,   93,   74,   93,   91,   74,   93,   93,   91,   81,
 /*   590 */    82,   83,   81,   82,   83,   93,   93,   74,   93,   91,
 /*   600 */    74,   93,   91,   93,   81,   82,   83,   81,   82,   83,
 /*   610 */    93,   74,   93,   93,   91,   74,   93,   91,   81,   82,
 /*   620 */    83,   93,   81,   82,   83,   93,   74,   93,   91,   93,
 /*   630 */    93,   74,   91,   81,   82,   83,   93,   93,   81,   82,
 /*   640 */    83,   93,   74,   91,   93,   93,   74,   93,   91,   81,
 /*   650 */    82,   83,   93,   81,   82,   83,   93,   74,   93,   91,
 /*   660 */    93,   89,   93,   74,   81,   82,   83,   93,   85,   74,
 /*   670 */    81,   82,   83,   93,   85,   74,   81,   82,   83,   93,
 /*   680 */    85,   74,   81,   82,   83,   93,   85,   93,   81,   82,
 /*   690 */    83,   93,   85,   74,   93,   93,   93,   93,   74,   93,
 /*   700 */    81,   82,   83,   74,   85,   81,   82,   83,   74,   85,
 /*   710 */    81,   82,   83,   74,   85,   81,   82,   83,   74,   85,
 /*   720 */    81,   82,   83,   74,   85,   81,   82,   83,   74,   85,
 /*   730 */    81,   82,   83,   93,   85,   81,   82,   83,   74,   85,
 /*   740 */    93,   93,   93,   74,   93,   81,   82,   83,   93,   85,
 /*   750 */    81,   82,   83,   74,   85,   93,   93,   93,   74,   93,
 /*   760 */    81,   82,   83,   74,   85,   81,   82,   83,   74,   85,
 /*   770 */    81,   82,   83,   74,   85,   81,   82,   83,   74,   93,
 /*   780 */    81,   82,   83,   93,   93,   81,   82,   83,
};
#define YY_SHIFT_USE_DFLT (-3)
#define YY_SHIFT_COUNT (98)
#define YY_SHIFT_MIN   (-2)
#define YY_SHIFT_MAX   (543)
static const short yy_shift_ofst[] = {
 /*     0 */   230,   -2,   -2,   -2,   65,   65,  124,  124,  124,  124,
 /*    10 */   124,  124,  124,  124,  124,  124,  124,  124,  326,  326,
 /*    20 */   326,  326,  326,  326,  171,  260,  290,  290,  290,  290,
 /*    30 */   290,  290,  290,  290,  290,  290,  290,  290,  326,  341,
 /*    40 */   341,  341,  341,  341,  341,  341,  341,  341,  341,  341,
 /*    50 */   341,  341,  341,  341,  341,  341,  341,  341,  341,  341,
 /*    60 */   486,  486,  486,   51,  242,  235,  346,  395,  397,  410,
 /*    70 */   446,   51,   -3,  221,  279,  517,  522,  529,  543,  308,
 /*    80 */   308,  308,  308,   10,   10,   58,   82,   75,   86,  126,
 /*    90 */   130,  141,  142,  174,  136,  136,  136,  145,  177,
};
#define YY_REDUCE_USE_DFLT (-90)
#define YY_REDUCE_COUNT (72)
#define YY_REDUCE_MIN   (-89)
#define YY_REDUCE_MAX   (704)
static const short yy_reduce_ofst[] = {
 /*     0 */   -63,   -4,   55,  106,  243,  258,  278,  293,  304,  309,
 /*    10 */   320,  325,  339,  344,  355,  360,  371,  376,  387,  390,
 /*    20 */   400,  403,  419,  422,  457,  478,  482,  493,  497,  508,
 /*    30 */   511,  523,  526,  537,  541,  552,  557,  568,  572,  -60,
 /*    40 */     7,   66,  583,  589,  595,  601,  607,  619,  624,  629,
 /*    50 */   634,  639,  644,  649,  654,  664,  669,  679,  684,  689,
 /*    60 */   694,  699,  704,  -74,  -89,  -75,  -75,  -75,  -75,  -75,
 /*    70 */   -75,  -11,  -20,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   294,  403,  403,  403,  403,  403,  403,  380,  378,  403,
 /*    10 */   403,  403,  403,  403,  403,  403,  403,  403,  403,  403,
 /*    20 */   403,  403,  403,  403,  403,  295,  403,  403,  403,  403,
 /*    30 */   403,  403,  403,  398,  403,  403,  403,  403,  403,  403,
 /*    40 */   403,  403,  403,  403,  403,  403,  403,  403,  403,  403,
 /*    50 */   403,  403,  403,  403,  403,  403,  403,  403,  403,  403,
 /*    60 */   403,  403,  403,  315,  403,  365,  365,  365,  365,  365,
 /*    70 */   365,  316,  392,  403,  403,  403,  403,  403,  403,  402,
 /*    80 */   382,  381,  323,  336,  337,  403,  379,  403,  403,  403,
 /*    90 */   403,  403,  403,  403,  362,  357,  338,  403,  403,
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
#line 70 "expr_parse.y"

  yypParser->value = value;
  UNUSED(yypminor);

#line 824 "expr_parse.c"
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
#line 1001 "expr_parse.c"
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
#line 1226 "expr_parse.c"
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
#line 1244 "expr_parse.c"
        break;
      case 3: /* pin ::= pbits_list NEWLINE */
#line 91 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,9,&yymsp[0].minor);
}
#line 1250 "expr_parse.c"
        break;
      case 5: /* pbits_list ::= pbits_list pbits */
#line 94 "expr_parse.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0);  }
#line 1255 "expr_parse.c"
        break;
      case 6: /* pbits ::= number */
#line 96 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);     }
#line 1260 "expr_parse.c"
        break;
      case 10: /* pbits ::= LPAREN expr RPAREN */
#line 100 "expr_parse.y"
{ yymsp[-1].minor.yy0.col = yymsp[-2].minor.yy0.col; token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);        yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1266 "expr_parse.c"
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
#line 1286 "expr_parse.c"
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
#line 1308 "expr_parse.c"
        break;
      case 23: /* evaloptsp ::= evalopts */
#line 146 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);  }
#line 1313 "expr_parse.c"
        break;
      case 24: /* evalopts ::= evalo */
      case 34: /* number ::= blackboard */ yytestcase(yyruleno==34);
      case 35: /* number ::= header */ yytestcase(yyruleno==35);
      case 36: /* number ::= key */ yytestcase(yyruleno==36);
      case 76: /* expr ::= sac_math */ yytestcase(yyruleno==76);
      case 77: /* list_item ::= number */ yytestcase(yyruleno==77);
      case 79: /* xlist ::= list_item */ yytestcase(yyruleno==79);
      case 94: /* expr ::= fstring */ yytestcase(yyruleno==94);
#line 147 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0); }
#line 1325 "expr_parse.c"
        break;
      case 25: /* evalopts ::= evalopts evalo */
#line 148 "expr_parse.y"
{ token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1330 "expr_parse.c"
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
#line 1343 "expr_parse.c"
        break;
      case 27: /* evalo ::= AS FLOAT */
#line 158 "expr_parse.y"
{ eval_asfloat = 1; yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1348 "expr_parse.c"
        break;
      case 28: /* evalo ::= AS INTEGER */
#line 159 "expr_parse.y"
{ eval_asfloat = 0;  yymsp[-1].minor.yy0.type = yymsp[0].minor.yy0.type = STRING; token_append(&yygotominor.yy0,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy0); }
#line 1353 "expr_parse.c"
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
#line 1377 "expr_parse.c"
        break;
      case 30: /* expr ::= state */
      case 31: /* number ::= NUM */ yytestcase(yyruleno==31);
#line 187 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);            }
#line 1383 "expr_parse.c"
        break;
      case 32: /* number ::= TOK_PI */
#line 189 "expr_parse.y"
{ token_value(&yygotominor.yy0, M_PI, yymsp[0].minor.yy0.col);      }
#line 1388 "expr_parse.c"
        break;
      case 33: /* number ::= MINUS number */
#line 191 "expr_parse.y"
{ token_value(&yygotominor.yy0, -yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1393 "expr_parse.c"
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
#line 1410 "expr_parse.c"
        break;
      case 38: /* header ::= HEADER */
#line 209 "expr_parse.y"
{ 
  if(!token_var(&yygotominor.yy0, HEADER, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  } 
}
#line 1420 "expr_parse.c"
        break;
      case 39: /* key ::= VARIABLE */
#line 215 "expr_parse.y"
{ 
  if(!token_var(&yygotominor.yy0, VARIABLE, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col)) {
    parse_error(value, TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE);
    value->str = yymsp[0].minor.yy0.str;
  }
}
#line 1430 "expr_parse.c"
        break;
      case 40: /* state ::= number */
#line 222 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);        }
#line 1435 "expr_parse.c"
        break;
      case 41: /* state ::= func */
#line 223 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);    }
#line 1440 "expr_parse.c"
        break;
      case 42: /* state ::= MINUS func */
#line 224 "expr_parse.y"
{ token_value(&yygotominor.yy0,-yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);  }
#line 1445 "expr_parse.c"
        break;
      case 43: /* state ::= state PLUS state */
#line 226 "expr_parse.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value + yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 1451 "expr_parse.c"
        break;
      case 44: /* state ::= state MINUS state */
#line 227 "expr_parse.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value - yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 1457 "expr_parse.c"
        break;
      case 45: /* state ::= state TIMES state */
#line 228 "expr_parse.y"
{ token_value(&yygotominor.yy0, yymsp[-2].minor.yy0.value * yymsp[0].minor.yy0.value, yymsp[-2].minor.yy0.col);   yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 1463 "expr_parse.c"
        break;
      case 46: /* state ::= LPAREN state RPAREN */
#line 230 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);         yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1470 "expr_parse.c"
        break;
      case 47: /* num_or_pstate ::= number */
#line 232 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[0].minor.yy0);}
#line 1475 "expr_parse.c"
        break;
      case 48: /* num_or_pstate ::= LPAREN state RPAREN */
      case 49: /* num_or_pstate ::= LPAREN sac_math RPAREN */ yytestcase(yyruleno==49);
#line 233 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);  yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1483 "expr_parse.c"
        break;
      case 50: /* func ::= SIN num_or_pstate */
#line 236 "expr_parse.y"
{ token_value(&yygotominor.yy0, sin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1488 "expr_parse.c"
        break;
      case 51: /* func ::= COS num_or_pstate */
#line 237 "expr_parse.y"
{ token_value(&yygotominor.yy0, cos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1493 "expr_parse.c"
        break;
      case 52: /* func ::= TAN num_or_pstate */
#line 238 "expr_parse.y"
{ token_value(&yygotominor.yy0, tan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col);  }
#line 1498 "expr_parse.c"
        break;
      case 53: /* func ::= SINH num_or_pstate */
#line 240 "expr_parse.y"
{ token_value(&yygotominor.yy0, sinh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1503 "expr_parse.c"
        break;
      case 54: /* func ::= COSH num_or_pstate */
#line 241 "expr_parse.y"
{ token_value(&yygotominor.yy0, cosh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1508 "expr_parse.c"
        break;
      case 55: /* func ::= TANH num_or_pstate */
#line 242 "expr_parse.y"
{ token_value(&yygotominor.yy0, tanh( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1513 "expr_parse.c"
        break;
      case 56: /* func ::= ASIN num_or_pstate */
#line 244 "expr_parse.y"
{ token_value(&yygotominor.yy0, asin( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1518 "expr_parse.c"
        break;
      case 57: /* func ::= ACOS num_or_pstate */
#line 245 "expr_parse.y"
{ token_value(&yygotominor.yy0, acos( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1523 "expr_parse.c"
        break;
      case 58: /* func ::= ATAN num_or_pstate */
#line 246 "expr_parse.y"
{ token_value(&yygotominor.yy0, atan( yymsp[0].minor.yy0.value ), yymsp[-1].minor.yy0.col); }
#line 1528 "expr_parse.c"
        break;
      case 59: /* func ::= ABS num_or_pstate */
#line 248 "expr_parse.y"
{ token_value(&yygotominor.yy0, fabs(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1533 "expr_parse.c"
        break;
      case 60: /* func ::= FLOOR num_or_pstate */
#line 249 "expr_parse.y"
{ token_value(&yygotominor.yy0, floor(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1538 "expr_parse.c"
        break;
      case 61: /* func ::= CEIL num_or_pstate */
#line 250 "expr_parse.y"
{ token_value(&yygotominor.yy0, ceil(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);   }
#line 1543 "expr_parse.c"
        break;
      case 62: /* func ::= EXPON num_or_pstate */
#line 251 "expr_parse.y"
{ token_value(&yygotominor.yy0, exp(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);    }
#line 1548 "expr_parse.c"
        break;
      case 63: /* func ::= ROUND num_or_pstate */
#line 252 "expr_parse.y"
{ token_value(&yygotominor.yy0, round(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col);  }
#line 1553 "expr_parse.c"
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
#line 1565 "expr_parse.c"
        break;
      case 65: /* func ::= ATAN2 LPAREN state COMMA state RPAREN */
#line 262 "expr_parse.y"
{ 
  token_value(&yygotominor.yy0, atan2( yymsp[-3].minor.yy0.value, yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1575 "expr_parse.c"
        break;
      case 66: /* func ::= LENGTH LPAREN state COMMA state RPAREN */
#line 265 "expr_parse.y"
{ 
  token_value(&yygotominor.yy0, sqrt( yymsp[-3].minor.yy0.value*yymsp[-3].minor.yy0.value + yymsp[-1].minor.yy0.value*yymsp[-1].minor.yy0.value ), yymsp[-5].minor.yy0.col); 
  yy_destructor(yypParser,2,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1585 "expr_parse.c"
        break;
      case 67: /* func ::= LOGN num_or_pstate */
      case 92: /* func ::= ALOG num_or_pstate */ yytestcase(yyruleno==92);
#line 268 "expr_parse.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0, log(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1597 "expr_parse.c"
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
#line 1608 "expr_parse.c"
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
#line 1620 "expr_parse.c"
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
#line 1631 "expr_parse.c"
        break;
      case 71: /* commas ::= COMMA */
#line 298 "expr_parse.y"
{
  yy_destructor(yypParser,20,&yymsp[0].minor);
}
#line 1638 "expr_parse.c"
        break;
      case 73: /* sac_math ::= MINIMUM xlist */
#line 327 "expr_parse.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_min), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1646 "expr_parse.c"
        break;
      case 74: /* sac_math ::= MAXIMUM xlist */
#line 335 "expr_parse.y"
{
  token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_max), yymsp[-1].minor.yy0.col);
  token_free(yymsp[0].minor.yy0.next);
}
#line 1654 "expr_parse.c"
        break;
      case 75: /* state ::= LPAREN sac_math RPAREN */
      case 78: /* list_item ::= LPAREN expr RPAREN */ yytestcase(yyruleno==78);
#line 341 "expr_parse.y"
{ token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1662 "expr_parse.c"
        break;
      case 80: /* xlist ::= xlist commas list_item */
#line 347 "expr_parse.y"
{ token_append(&yygotominor.yy0,&yymsp[-2].minor.yy0,&yymsp[0].minor.yy0); }
#line 1667 "expr_parse.c"
        break;
      case 81: /* sac_math ::= ADD xlist */
#line 349 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_add), yymsp[-1].minor.yy0.col); }
#line 1672 "expr_parse.c"
        break;
      case 82: /* sac_math ::= SUB xlist */
#line 350 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_sub), yymsp[-1].minor.yy0.col); }
#line 1677 "expr_parse.c"
        break;
      case 83: /* sac_math ::= MUL xlist */
#line 351 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_mul), yymsp[-1].minor.yy0.col); }
#line 1682 "expr_parse.c"
        break;
      case 84: /* sac_math ::= DIV xlist */
#line 352 "expr_parse.y"
{token_value(&yygotominor.yy0, token_foreach(&yymsp[0].minor.yy0, token_div), yymsp[-1].minor.yy0.col); }
#line 1687 "expr_parse.c"
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
#line 1699 "expr_parse.c"
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
#line 1710 "expr_parse.c"
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
#line 1722 "expr_parse.c"
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
#line 1734 "expr_parse.c"
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
#line 1746 "expr_parse.c"
        break;
      case 90: /* func ::= INTEGER num_or_pstate */
#line 391 "expr_parse.y"
{ token_value(&yygotominor.yy0, (int) yymsp[0].minor.yy0.value, yymsp[-1].minor.yy0.col);   }
#line 1751 "expr_parse.c"
        break;
      case 91: /* func ::= POWER num_or_pstate */
#line 392 "expr_parse.y"
{ token_value(&yygotominor.yy0, pow(10,yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); }
#line 1756 "expr_parse.c"
        break;
      case 93: /* func ::= ALOG10 num_or_pstate */
#line 400 "expr_parse.y"
{ 
  if(yymsp[0].minor.yy0.value > 0.0) {
    token_value(&yygotominor.yy0,log10(yymsp[0].minor.yy0.value), yymsp[-1].minor.yy0.col); 
  } else {
    parse_error(value, TOKEN_STATUS_ERROR_LOG_NEGATIVE);
  }
}
#line 1767 "expr_parse.c"
        break;
      case 95: /* string ::= STRING */
      case 96: /* string ::= QUOTED_STRING */ yytestcase(yyruleno==96);
      case 97: /* string ::= ESCAPE_STRING */ yytestcase(yyruleno==97);
#line 412 "expr_parse.y"
{ token_string(&yygotominor.yy0, yymsp[0].minor.yy0.str, yymsp[0].minor.yy0.col); }
#line 1774 "expr_parse.c"
        break;
      case 98: /* string ::= number */
#line 415 "expr_parse.y"
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
#line 1791 "expr_parse.c"
        break;
      case 99: /* string_list ::= */
#line 430 "expr_parse.y"
{ token_string(&yygotominor.yy0, strdup(""), lexer_col()); }
#line 1796 "expr_parse.c"
        break;
      case 100: /* string_list ::= string_list string */
#line 431 "expr_parse.y"
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
#line 1812 "expr_parse.c"
        break;
      case 101: /* string ::= LPAREN fstring RPAREN */
#line 445 "expr_parse.y"
{  token_copy(&yygotominor.yy0,&yymsp[-1].minor.yy0);   yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 1819 "expr_parse.c"
        break;
      case 102: /* fstring ::= CHANGE string string string */
#line 447 "expr_parse.y"
{
  string *s = string_new( yymsp[0].minor.yy0.str );
  s = string_replace(s, yymsp[-2].minor.yy0.str, yymsp[-1].minor.yy0.str );
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-2].minor.yy0.str);
  FREE(yymsp[-1].minor.yy0.str);
}
#line 1831 "expr_parse.c"
        break;
      case 103: /* fstring ::= SUBSTRING number number string */
#line 455 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string *s2 = string_substr(s, (int)yymsp[-2].minor.yy0.value-1, (int)yymsp[-1].minor.yy0.value-(int)yymsp[-2].minor.yy0.value+1);
  token_string(&yygotominor.yy0, strdup(string_string(s2)), yymsp[-3].minor.yy0.col);
  string_free(&s);
  string_free(&s2);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1843 "expr_parse.c"
        break;
      case 104: /* fstring ::= DELETE string string */
#line 463 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  string_replace(s, yymsp[-1].minor.yy0.str, "");
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1855 "expr_parse.c"
        break;
      case 105: /* fstring ::= CONCAT string_list */
#line 472 "expr_parse.y"
{ yymsp[0].minor.yy0.col = yymsp[-1].minor.yy0.col; token_copy(&yygotominor.yy0, &yymsp[0].minor.yy0); }
#line 1860 "expr_parse.c"
        break;
      case 106: /* fstring ::= BEFORE string string */
#line 474 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, f-yymsp[0].minor.yy0.str, -1);
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1873 "expr_parse.c"
        break;
      case 107: /* fstring ::= AFTER string string */
#line 483 "expr_parse.y"
{
  string *s = string_new(yymsp[0].minor.yy0.str);
  char *f = strstr(yymsp[0].minor.yy0.str, yymsp[-1].minor.yy0.str);
  string_remove(s, 0, (f-yymsp[0].minor.yy0.str)+strlen(yymsp[-1].minor.yy0.str));
  token_string(&yygotominor.yy0, strdup(string_string(s)), yymsp[-2].minor.yy0.col);
  string_free(&s);
  FREE(yymsp[-1].minor.yy0.str);
  FREE(yymsp[0].minor.yy0.str);
}
#line 1886 "expr_parse.c"
        break;
      case 108: /* fstring ::= REPLY string */
#line 492 "expr_parse.y"
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
#line 1924 "expr_parse.c"
        break;
      case 109: /* fstring ::= FTOA state */
#line 526 "expr_parse.y"
{ 
  char *s;
  asprintf(&s, "%g", yymsp[0].minor.yy0.value); 
  token_string(&yygotominor.yy0, s, yymsp[-1].minor.yy0.col);
}
#line 1933 "expr_parse.c"
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
#line 1989 "expr_parse.c"
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
#line 2011 "expr_parse.c"
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
#line 2037 "expr_parse.c"
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

