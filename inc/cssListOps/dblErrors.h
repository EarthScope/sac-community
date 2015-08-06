#ifndef _DBLERRORS_H
#define _DBLERRORS_H
#include <setjmp.h>

void dblClearErrorList(void);
void dblSetError(int Severe, const char *string);
char *dblGetErrorMessage(void);
int dblGetNumErrors(void);

#endif /* _DBLERRORS_H */
