
#ifndef __RC_H__
#define __RC_H__

#define SAC_RC_FILE ".sacrc"

typedef int rc_function(char *key, int type, void *value);

typedef void set_option(void *value);

struct rc_options {
  char *key;
  int   type;
  set_option *set;
};

int   rc_set_option(char *key, int type, void *value);

int   rc_read(char *file, rc_function *func);
void  rc_set_function(rc_function *func);
int   rc_debug(int verbose_level, int output_lines, int parse_debug);
int   rc_linenumber();
char *rc_line();

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE  0
#endif 

enum {
  RC_NULL = 0,
  RC_BOOL,
  RC_INTEGER,
  RC_STRING,
  RC_REAL
};

#endif /* __RC_H__ */
