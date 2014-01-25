/** 
 * Copyright (c) 2008, Brian Savage < savage _AT_ uri.edu >
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the distribution.
 * * Neither the name of the author nor the names of its contributors may be 
 *   used to endorse or promote products derived from this software without 
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * The views and conclusions contained in the software and documentation are 
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "vars.h"
#include "bbf.h"

static int verbose       = 0;
static int library_use = 0;

#define FREE(x) do { \
    if(x) {          \
      free(x);       \
      x = NULL;      \
    }                \
  }  while(0);

void
bbf_verbose(int v) {
  verbose = v;
}

void
bbf_set_library_use(int value) {
  library_use = value;
}

int
bbf_error(int errno, char *fmt, ...) {
  va_list ap;
  
  if(library_use) {
    va_start(ap, fmt);
    fprintf(stderr, "bbf_error: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(-1);
  } else {
    return errno;
  }
}


void
bbf_message(int type, char *fmt, ...) {
  va_list ap;
  if(verbose >= type) {
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
  }
}

void
swap(char *p, int size) {
  int i;
  char tmp;

  for(i = 0; i < size/2; i++) {
    tmp         = p[i];
    p[i]        = p[size-i-1];
    p[size-i-1] = tmp;
  }
}

bbf *
bbf_new() {
  bbf *b;
  bbf_message(MESSAGE_DEBUG, "Creating Internal BBF Storage\n");
  b = (bbf *) malloc(sizeof(bbf));
  b->id[0] = 0;
  b->ver   = 0;
  b->hdr   = 0;
  b->name  = NULL;
  b->v     = NULL;
  b->n     = 0;
  return b;
}

void
bbf_set_header(bbf           *b, 
	       struct Header *h,
	       char          *id, 
	       int       ver, 
	       int       hdr, 
	       char          *name) {
  int n;
  bbf_message(MESSAGE_DEBUG, "Setting BBF Header Values\n");
  if(!b) 
    return;
  b->ver = ver;
  b->hdr = hdr;
  bbf_message(MESSAGE_DEBUG, "Setting name\n");
  n = (h->namelength * 4);
  b->name = (char *) malloc(sizeof(char) * (n+1));
  memset(b->name, ' ', n);
  strncpy(b->name, name, strlen(name));
  b->name[n] = 0;
  b->namelength = n;

  bbf_message(MESSAGE_DEBUG, "Setting id '%s'\n", id);
  strcpy(b->id, id);
  b->id[strlen(id)] = 0;

  return;
}

void
bbf_set_var(bbf           *b,
            struct Header *h,
            int            hdr,
            char          *name,
            ...) {
  int k;
  va_list ap;
  char *s;
  int n = b->n;
  
  (b->n)++;
  b->v = (vars *) realloc(b->v, sizeof(vars) * b->n);
  b->v[n].name  = NULL;
  b->v[n].value = NULL;
  bbf_message(MESSAGE_DEBUG, "set_var: hdr %lu\n", hdr);
  (b->v[n]).hdr   = hdr;
  k = (h->namelength * 4) ;

  b->v[n].name  = (char *) malloc(sizeof(char) * (k+1));
  memset(b->v[n].name, ' ', k);
  strncpy(b->v[n].name, name, strlen(name));
  b->v[n].name[k] = 0;
  b->v[n].namelength = k;
  b->v[n].type = h->type;

  va_start(ap, name);
  switch(h->type) {
  case VALUESTRING:
    s = va_arg(ap, char *);
    k = (h->valuelength * 4);
    b->v[n].value  = (char *) malloc(sizeof(char) * (k+1));
    memset(b->v[n].value, ' ', k);
    strncpy(b->v[n].value, s, strlen(s));
    b->v[n].value[k] = 0;
    b->v[n].valuelength = k;
    break;
  case VALUEDOUBLE:
    b->v[n].val = va_arg(ap, double);
    b->v[n].valuelength = 2;
    break;
  case VALUEINTEGER:
    b->v[n].ival = va_arg(ap, int);
    b->v[n].valuelength = 1;
    break;
  case VALUENIL:
    b->v[n].valuelength = 0;
  }
  return;
}

void
bbf_free(bbf *b) {
  int i;
  if(b) {
    for(i = 0; i < b->n; i++) {
      vars *v = &(b->v[i]);
      FREE(v->name);
      FREE(v->value);
    }
    FREE(b->v);
    FREE(b->name);
    FREE(b);
  }
}

int
bbf_write(bbf *b, char *output, int doswap) {
  int i;
  FILE *fp;

  bbf_message(MESSAGE_INFO, "Writing out bbf: %s\n", output);
  if((fp = fopen(output, "w")) == NULL) {
    return bbf_error(101, "opening file for writing: %s\n", output);
  }

  bbf_message(MESSAGE_DEBUG, "id:  >%s< %d\n", b->id, (int)strlen(BBF_HEADER_IDENTITY));
  if(fwrite(b->id,  sizeof(char), strlen(b->id), fp) != strlen(b->id)) {
    return bbf_error(115, "error writing identify\n");
  }

  bbf_message(MESSAGE_DEBUG, "ver: %lu\n", b->ver);
  if(doswap) 
    swap((char *)&(b->ver), sizeof(int));
  if(fwrite(&(b->ver), sizeof(b->ver), 1, fp) != 1) {
    return bbf_error(115, "error writing version\n");
  }

  bbf_message(MESSAGE_DEBUG, "hdr: %lu\n", b->hdr);
  if(doswap) 
    swap((char *)&(b->hdr), sizeof(int));
  if(fwrite(&(b->hdr), BBF_VARS_HEADER_SIZE, 1, fp) != 1) {
    return bbf_error(115, "error writing header\n");
  }

  bbf_message(MESSAGE_DEBUG, "name: >%s< %d %d\n", b->name, b->namelength, strlen(b->name));
  if((int)fwrite(b->name, sizeof(char), b->namelength, fp) != b->namelength) {
    return bbf_error(115, "error writing variable group name\n");
  }
  
  for(i = 0; i < b->n; i++) {
    bbf_message(MESSAGE_DEBUG, "   hdr:   %lu\n", b->v[i].hdr);
    if(doswap) 
      swap((char *)&(b->v[i].hdr), sizeof(int));

    if(fwrite(&(b->v[i].hdr), BBF_VARS_HEADER_SIZE, 1, fp) != 1) {
      return bbf_error(115, "writing variable header\n");
    }
    bbf_message(MESSAGE_DEBUG, "   name[%d]:  >%s<\n", b->v[i].namelength, b->v[i].name);
    
    if(fwrite(b->v[i].name, sizeof(char), b->v[i].namelength, fp) != 
       (size_t) b->v[i].namelength) {
      return bbf_error(115, "writing variable name\n");
    }
    bbf_message(MESSAGE_DEBUG, "   value[%d]: %d\n", b->v[i].valuelength, 0);
    bbf_message(MESSAGE_DEBUG, "   value[%d]: %d\n", 0, b->v[i].type);
    switch(b->v[i].type) {
    case VALUESTRING:
      bbf_message(MESSAGE_DEBUG, "   value[%d]: >%s<\n", b->v[i].valuelength, b->v[i].value);
      if(fwrite(b->v[i].value, sizeof(char), b->v[i].valuelength, fp) != 
         (size_t)b->v[i].valuelength) {
        return bbf_error(115, "writing variable value\n");
      }     
      break;
    case VALUEDOUBLE:
      if(fwrite(&b->v[i].val, sizeof(double), 1, fp) != 1) {
        return bbf_error(115, "writing variable value\n");
      }
      break;
    case VALUEINTEGER:
      if(fwrite(&b->v[i].ival, sizeof(int), 1, fp) != 1) {
        return bbf_error(115, "writing variable value\n");
      }
      break;
    case VALUENIL:
      break;
    }
  }
  bbf_message(MESSAGE_DEBUG, "closing file\n");
  fclose(fp);
  return 0;
}


unsigned int 
encodeHeader(struct Header *h) {
  unsigned int s;
  s = 0L;
  /* Turn on all the correct Bits */
  if(h->delete)
    s |= DELETEBIT;
  if(h->readonly)
    s |= (READONLY);
  if(h->indirect)
    s |= (INDIRECT);
  if(h->shared)
    s |= (SHARED);
  if(h->reserved)
    s |= (RESERVED);
  if(h->AppBit1)
    s |= (APPBIT1);
  if(h->AppBit2)
    s |= (APPBIT2);

  /* Set the Type */
  s |= ( h->type  & TYPEBIT ) << TYPESHIFT;
  /* Set the Name Length */
  s |= (( h->namelength - 1 ) & NAMEBIT ) << NAMESHIFT;

  /* Set the Value Length */
  if(h->valuelength -1 > VALUEBIT) {
    s |= VALUEISLONGBIT;
  } else {
    s |= ((h->valuelength - 1L) & VALUEBIT);
  }

  return s;
}

struct Header *
decodeHeader(int z) {
  struct Header *h;
  h = (struct Header *) malloc(sizeof(struct Header));
  /* Are the specific Bits ON ? */
  h->delete   = ( (z & DELETEBIT)   != 0);
  h->readonly = ( (z & READONLY) != 0);
  h->indirect = ( (z & INDIRECT) != 0);
  h->shared   = ( (z & SHARED)   != 0);
  h->reserved = ( (z & RESERVED) != 0);
  h->AppBit1  = ( (z & APPBIT1)  != 0);
  h->AppBit2  = ( (z & APPBIT2)  != 0);
 
  /* Get the Type */
  h->type              = (z >> TYPESHIFT) & TYPEBIT;
  /* Get the Length of the Name in 4 byte words */
  h->namelength        = ((z >> NAMESHIFT) & NAMEBIT) + 1 ;
  /* Get the Description Length */
  h->descriptionlength = (z & VALUEISLONGBIT) ? 2 : 1;
  /* Get the Value Length */
  h->valuelength       = (z & VALUEISLONGBIT) ? (z+1) : (z & VALUEBIT) + 1;
  return h;
}

char *
int2bin(unsigned int a) {
  char *str,*tmp;
  int cnt = 31;
  str = (char *) malloc(sizeof(char) * 33); /*32 + 1 , becoz its a 32 bit bin number*/
  memset(str,0,33);
  tmp = str;
  while ( cnt > -1 ){
    str[cnt]= '0';
    cnt --;
  }
  cnt = 31;
  while (a > 0){
    if (a%2==1){
      str[cnt] = '1';
    }
    cnt--;
    a = a/2 ;
  }
  return tmp;   
  
}

void
dumpHeader(struct Header *h) {
  bbf_message(MESSAGE_DEBUG, "Delete:      %d\n", h->delete);
  bbf_message(MESSAGE_DEBUG, "Readonly:    %d\n", h->readonly);
  bbf_message(MESSAGE_DEBUG, "Indirect:    %d\n", h->indirect);
  bbf_message(MESSAGE_DEBUG, "Shared:      %d\n", h->shared);
  bbf_message(MESSAGE_DEBUG, "Reserved:    %d\n", h->reserved);
  bbf_message(MESSAGE_DEBUG, "AppBit1:     %d\n", h->AppBit1);
  bbf_message(MESSAGE_DEBUG, "AppBit2:     %d\n", h->AppBit2);
  bbf_message(MESSAGE_DEBUG, "Type:        %d\n", h->type);
  bbf_message(MESSAGE_DEBUG, "Description: %d\n", h->descriptionlength);
  bbf_message(MESSAGE_DEBUG, "NameLength:  %d 4-byte words\n", h->namelength);
  bbf_message(MESSAGE_DEBUG, "ValueLength: %d 4-byte words\n", h->valuelength);
}

bbf *
bbf_read(char *input, int *doswap) {
  FILE *fp;
  bbf *b;
  int ver;
  unsigned int hdr;
  char id[5];
  char *name, *value;
  double dvalue;
  int ivalue;
  struct Header *h;

  fp = NULL;
  b  = NULL;

  bbf_message(MESSAGE_INFO, "Reading in bbf: %s\n", input);

  /* Open the file for reading */
  if((fp = fopen(input, "r")) == NULL) {
    *doswap = bbf_error(101, "reading in file: %s\n", input);  
    goto ERROR;
  }
  /* Read in the Identifier */
  fread(&id[0], sizeof(char), strlen(BBF_HEADER_IDENTITY), fp);
  if(strncasecmp(BBF_HEADER_IDENTITY, id, strlen(BBF_HEADER_IDENTITY)) != 0) {
    *doswap = bbf_error(1208, "Input is not a Blackboard Variable File: %s\n", input);
    goto ERROR;
  }
  id[strlen(BBF_HEADER_IDENTITY)] = 0;
  bbf_message(MESSAGE_INFO, "Identify:  >%s<\n", id);
  
  /* Read in the Version Number */
  fread(&ver, sizeof(BBF_HEADER_VERSION), 1, fp);
  bbf_message(MESSAGE_INFO, "Version:     %u \n", ver);
  *doswap = FALSE;
  if(ver != BBF_HEADER_VERSION) {
    bbf_message(MESSAGE_INFO, "Header Version is unrecognized\n");
    swap((char *) &ver, sizeof(int));
    if(ver != BBF_HEADER_VERSION) {
      *doswap = bbf_error(1208, "Blackboard variable File has incorrect version number: %s\n", input);
    }
    *doswap = TRUE;
    bbf_message(MESSAGE_INFO, "Version:     %lu  (After Swap)\n", ver);
  }
  /* Read in the Header for the Entire file */
  fread(&hdr, sizeof(int), 1, fp);
  bbf_message(MESSAGE_DEBUG, "Header:      %u\n", hdr);
  if(*doswap) {
    swap((char*)&hdr, sizeof(int));
  }  

  /* Decode the Header */
  h = decodeHeader(hdr);
  
  bbf_message(MESSAGE_DEBUG, "Header:      %u\n", hdr);
  bbf_message(MESSAGE_DEBUG,
	  "         %s\n         %s\n", int2bin(hdr), int2bin(encodeHeader(h)));
  
  dumpHeader(h);
  
  /* Make space for the name and read it in */
  name = (char *)malloc(sizeof(char) * ((h->namelength * 4) + 1));
  fread(&name[0], sizeof(char), (h->namelength * 4), fp);
  name[h->namelength * 4] = 0;
  bbf_message(MESSAGE_INFO, "Filename: >%s<\n", name);
  
  b = bbf_new();
  /* Write Identifies, Version Number, Header, and Name */
  bbf_set_header(b, h, id, ver, hdr, name);
  
  free(h);
  free(name);
  name  = NULL;
  value = NULL;
  h     = NULL;
  
  /* Read in a Header for each Value */
  while(fread(&hdr, BBF_VARS_HEADER_SIZE, 1, fp) == 1) {
    bbf_message(MESSAGE_DEBUG, "------------------------------------\n");
    bbf_message(MESSAGE_DEBUG, "Header:      %u\n", hdr);
    if(*doswap) {
      swap((char*)&hdr, sizeof(int));
    }
    /* Decode the Header */
    h = decodeHeader(hdr);

    bbf_message(MESSAGE_DEBUG, "Header:      %u\n", hdr);
    bbf_message(MESSAGE_DEBUG, 
            "         %s\n         %s\n", int2bin(hdr), int2bin(encodeHeader(h)));
    
    dumpHeader(h);
    
    /* Read in the Name */
    name = (char *)realloc(name, sizeof(char) * ((h->namelength * 4) + 1));
    if(fread(&name[0], sizeof(char), (h->namelength * 4), fp) != (size_t)(h->namelength * 4)) {
      *doswap = bbf_error(114, "reading in variable name\n");
      goto ERROR;
    }
    name[h->namelength * 4] = 0;
    bbf_message(MESSAGE_INFO, "Name:       \"%s\"\n", name);
    
    /* Read in the Value */
    if(h->type == VALUESTRING) {
      value = (char *)realloc(value, sizeof(char) * ((h->valuelength * 4) + 1));
      if(fread(&value[0], sizeof(char), (h->valuelength * 4), fp) != (size_t)(h->valuelength * 4)) {
        *doswap = bbf_error(114, "reading in variable value\n");
        goto ERROR;
      }
      value[h->valuelength * 4] = 0;
      bbf_message(MESSAGE_INFO, "Value:      \"%s\"\n", value);
      /* Write the variable out */
      bbf_set_var(b, h, hdr, name, value);
    }
    if(h->type == VALUEDOUBLE) {
      if(fread(&dvalue, sizeof(double), 1, fp) != 1) {
        *doswap = bbf_error(114, "reading in variable double value\n");
        goto ERROR;
      }
      bbf_message(MESSAGE_INFO, "Value:      %lf\n", dvalue);      
      bbf_set_var(b, h, hdr, name, dvalue);
    }
    if(h->type == VALUEINTEGER) {
      if(fread(&ivalue, sizeof(int), 1, fp) != 1) {
        *doswap = bbf_error(114, "reading in variable integer value\n");
        goto ERROR;
      }
      bbf_message(MESSAGE_INFO, "Value:      %d\n", ivalue);      
      bbf_set_var(b, h, hdr, name, ivalue);
    }
    if(h->type == VALUENIL) {
      bbf_set_var(b, h, hdr, name, 0);
    }
    /* Free Header memory */
    free(h);
    h = NULL;

    /* Break when we have com to the end */
    if(strncmp(name, "NIL", 3) == 0) {
      break;
    }
  }
  
  bbf_message(MESSAGE_DEBUG, "closing file, moving to next file\n");
  fclose(fp);
  return b;
 ERROR:
  if(fp) {
    fclose(fp);
  }
  if(b) {
    bbf_free(b);
    b = NULL;
  }
  return NULL;
}

