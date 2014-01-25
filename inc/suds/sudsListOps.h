#ifndef SUDS_LIST_OPS_H
#define SUDS_LIST_OPS_H

#include "suds.h"



void *AddSudsListElement(SUDS *S, short StructID); 
void FreeSuds(SUDS *S);
SUDS *NewSudsList(void);
char *Phasecode(short obs_phase);
char *Instcode(short code);
char *GetCharAuth(short authority);
char *GetCharProg(char program);
char *GetCharMagType(short mag_type);
#endif
