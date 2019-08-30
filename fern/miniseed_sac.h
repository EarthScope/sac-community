
#ifndef _MINISEED_SAC_H_
#define _MINISEED_SAC_H_

#include <sacio/sacio.h>
#include <libmseed/libmseed.h>

#include "event.h"

int64_t read_miniseed_memory(MS3TraceList *mst3k, char *buffer, uint64_t len);
sac ** miniseed_trace_list_to_sac(MS3TraceList *mst3k);
int read_miniseed_file(MS3TraceList *mst3k, char *file);



#endif /* _MINISEED_SAC_H_ */
