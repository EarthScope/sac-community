#ifndef SM_MEMORY_H
#define SM_MEMORY_H

#include <stdlib.h>

void * smMalloc(size_t size);
void *smCalloc(size_t count, size_t size);
void *smRealloc(void *ptr, size_t size);
void smFree(void *ptr);
size_t smTotalPhysicalBytes(void);
size_t smAvailPhysicalBytes(void);
size_t smAllocatedBytes(void);
double smFracPhysMemUsed(void);
void smPrintContents(void);



#endif
