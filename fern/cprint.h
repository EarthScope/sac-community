
#ifndef _CPRINT_H_
#define _CPRINT_H_

#include <stdio.h>

#define BLACK      "\x1B[30m"
#define RED        "\x1B[31m"
#define GREEN      "\x1B[32m"
#define YELLOW     "\x1B[33m"
#define BLUE       "\x1B[34m"
#define MAGENTA    "\x1B[35m"
#define CYAN       "\x1B[36m"
#define WHITE      "\x1B[37m"
#define RESET      "\x1B[0m"
#define BOLD       "\x1B[1m"

int cfprintf(FILE *fp, const char *color, const char *fmt, ...);
int cprintf(const char *color, const char *fmt, ...);

#endif /* _CPRINT_H_ */
