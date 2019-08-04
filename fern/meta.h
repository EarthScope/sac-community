
#ifndef _META_H_
#define _META_H_

#include <sacio/sacio.h>
#include <sacio/timespec.h>

#include "event.h"

int  sac_array_fill_meta_data(sac **files, int verbose);
void sac_array_fill_meta_data_from_event(sac **s, Event *ev, int verbose);
void sac_array_fill_meta_data_from_file(sac **files, int verbose, char *file);
void sac_fill_meta_data_from_event(sac *s, Event *ev, int verbose);
#endif /* _META_H_ */
