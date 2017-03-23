
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdio.h>

#define TOKEN_INT_PRECISION_NON_ARGUMENT 0.0

typedef struct _Token Token;
struct _Token {
    int type;
    double value;
    char *str;
    Token *next;
    int col;
    int line;
    int error;
};

typedef struct _eval_t eval;
struct _eval_t {
    int type;
    double result;
    char *str;
    int status;
    char *error_message;
    char *input;
    int processed;
};

enum {
    TOKEN_STATUS_UNKNOWN = -1,
    TOKEN_STATUS_OK = 0,
    TOKEN_STATUS_ERROR,
    TOKEN_STATUS_ERROR_SYNTAX,
    TOKEN_STATUS_ERROR_DIVIDE_BY_ZERO,
    TOKEN_STATUS_ERROR_SQRT_NEGATIVE,
    TOKEN_STATUS_ERROR_LOG_NEGATIVE,
    TOKEN_STATUS_ERROR_NEGATIVE_FRACTION_POWER,
    TOKEN_STATUS_ERROR_UNEXPECTED_CHARACTER,
    TOKEN_STATUS_ERROR_UNKNOWN_BLACKBOARD_VARIABLE,
    TOKEN_STATUS_ERROR_UNKNOWN_MACRO_VARIABLE,
    TOKEN_STATUS_ERROR_UNKNOWN_HEADER_VARIABLE,
    TOKEN_STATUS_ERROR_GETTIME,
    TOKEN_STATUS_ERROR_GETVAL,
};

enum {
    EVAL_STATUS_EMPTY = -1,
    EVAL_STATUS_OK = 0,
    EVAL_STATUS_SYNTAX_ERROR,
    EVAL_STATUS_PARSING,
};

enum {
    EVAL_OUTPUT_TYPE_UNKNOWN = -1,
    EVAL_OUTPUT_TYPE_NUMBER,
    EVAL_OUTPUT_TYPE_STRING,
};

eval *eval_new();
void eval_input(eval * e, char *input);
void eval_syntax_error(eval * e, Token * t);
void eval_eval(eval * e);
eval *eval_compute(char *input);
void eval_clear(eval * e);
void eval_free(eval * e);

void token_free(Token * t);
Token *token_new(int type, double value, char *str, int line, int col);
Token *token_last(Token * t);

int token_is_string(Token * t);
Token *parse(eval * e);
char *token_to_string(Token * t);       /* Token List, commas between */
char *token_as_string(Token * t);       /* Single Token */
char *token_to_line(Token * t); /* Token List, spaces between */
int token_is_number(Token * t);
int token_is_int(Token * t);
int token_is_int_precision(Token * t, double precision);
int token_as_int(Token * t);
int token_is_quoted_string(Token * t);
int token_is_escape_string(Token * t);
int token_is_equals(Token * t);
int token_is_comma(Token * t);
Token *token_new_int(int i);
Token *token_new_string(char *s);

char *replace(char *t, char k);
int token_var(Token * a, int type, char *key, int col);
void token_string(Token * t, char *str, int col);
void token_value(Token * t, double v, int col);
void token_int(Token * t, int v, int col);
void token_copy(Token * a, Token * b);
void token_append(Token * a, Token * b, Token * c);
Token *token_last(Token * t);
Token *token_remove(Token * head, Token * t);
Token *token_insert_before(Token * head, Token * t, Token * new);
Token *token_deep_copy(Token * t);

int isnum(char *s);
int isnumx(char *s);
int is_string_like(int type);
char *lexer_input();
int gettime_expr(Token * A, Token * B, int lmax, int lvalue);

int token_is_eq(Token * t);
int token_is_ne(Token * t);
int token_is_gt(Token * t);
int token_is_lt(Token * t);
int token_is_ge(Token * t);
int token_is_le(Token * t);

double round_expr(double x);
double copysign_expr(double x, double y);

Token *string_to_token_list(char *str);
Token *token_to_token_list(Token * t);
int token_strncasecmp(Token * t, char *s, size_t n);

#endif /* __TOKEN_H__ */
