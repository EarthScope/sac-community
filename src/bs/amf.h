
#ifndef _AMF_H_
#define _AMF_H_

#include "debug.h"
#include <sacdata.h>
#include "strlist.h"
#include "errors.h"

void iniam();

sac *sacget(int i, int data, int *nerr);
void sacput(sac * s);
void sacpop();
void sacdel(int i);
int saclen();
void sacclear();
void sacsort(int (*compare) (const void *a, const void *b));
sac *sacget_current();
int sacget_current_id();
void sac_header_copy(sac * to, sac * from);

int sac_find_filename(char *file);

sac *bflget(string_list * list, int i);
void bflclear();

sac *sac_new();
void sac_free(sac * s);
void sac_alloc(sac * s);
int sac_comps(sac * s);
void sac_extrema(sac * s);

char *khdr(sac * s, int k);

void sac_buffer_free();
sac *sac_copy(sac *s);
void sac_meta_copy(sac *to, sac *from);
void sac_be(sac *s);

#endif /* _AMF_H_ */
