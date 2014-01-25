typedef union {
  int    i;   /* Integer */
  float  d;   /* Double, floating point */
  char*  s;   /* String */
} YYSTYPE;
#define	INTEGER	257
#define	STRING	258
#define	NUMBER	259
#define	BOOL_TRUE	260
#define	BOOL_FALSE	261
#define	EQUALS	262


extern YYSTYPE yylval;
