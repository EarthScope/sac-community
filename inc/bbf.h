
#ifndef __BBF_H__
#define __BBF_H__

#include <stdarg.h>

struct Header {
  int delete;
  int readonly;
  int indirect;
  int shared;
  int reserved;
  int AppBit1;
  int AppBit2;
  int type;
  int namelength;
  int valuelength;
  int descriptionlength;
};

#define DELETEBIT      (1UL<<31)  /* 1st  */
#define READONLY       (1<<30)    /* 2nd  */
#define INDIRECT       (1<<29)    /* 3rd  */
#define SHARED         (1<<28)    /* 4th  */
#define RESERVED       (1<<27)    /* 5th  */
#define APPBIT1        (1<<21)    /* 11th */
#define APPBIT2        (1<<20)    /* 12th */

#define TYPESHIFT      22
#define TYPEBIT        037

#define NAMESHIFT      17
#define NAMEBIT        07

#define VALUEISLONGBIT (1<<16)    /* 16th */
#define VALUEBIT       (0177777)
#define VALUESHIFT     22

#define BBF_HEADER_IDENTITY  "VARS"
#define BBF_HEADER_VERSION   1
#define BBF_VARS_HEADER_SIZE sizeof(int)

#define TRUE   1
#define FALSE  0

#define MESSAGE_INFO       1
#define MESSAGE_DEBUG      2

typedef struct __vars vars;
struct __vars {
  int    hdr;
  char  *name;
  char  *value;
  double val;
  int    ival;
  int    namelength;
  int    valuelength;
  int    type;
};

typedef struct __bbf bbf;
struct __bbf {
  char id[5];
  int ver;
  int hdr;
  char *name;
  vars *v;
  int n;
  int namelength;
};

void  bbf_verbose    (int v);
int   bbf_error      (int errno, char *fmt, ...);

bbf * bbf_new        ();
void  bbf_free       (bbf *b);
int   bbf_write      (bbf *b, char *output, int doswap);
bbf * bbf_read       (char *input, int *doswap);
void  bbf_set_header (bbf           *b, 
                      struct Header *h,
                      char          *id, 
                      int            ver, 
                      int            hdr, 
                      char          *name);
void  bbf_set_var    (bbf           *b,
                      struct Header *h,
                      int            hdr,
                      char          *name,
                      ...);
unsigned int encodeHeader(struct Header *h);

#endif /* __BBF_H__ */
