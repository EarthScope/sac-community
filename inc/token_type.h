
#ifndef __TOKEN_TYPE_H__
#define __TOKEN_TYPE_H__

typedef struct _Token Token;
struct _Token {
  int    type;
  double value;
  char  *str;
  Token *next;
  int    col;
  int    line;
  int    error;
};

#endif /* __TOKEN_TYPE_H__ */
