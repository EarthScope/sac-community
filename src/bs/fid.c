/**
 * @file
 * @brief File Indentification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "../../libmseed/libmseed.h"

#include "fid.h"

/**
 * File Identification
 * @private
 */
struct FID {
    int    id; /**< @private Unique file type*/
    off_t  min; /**< @private Minimum length used for identification */
    int  (*try)(char *buf, int n); /**< @private Function for identification */
    char  *name; /**< @private File type description*/
};


#ifdef __DEBUG__
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M , __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

#define TRUE  1
#define FALSE 0

static int
little_endian() {
  int x = 1;
  return *(char*)&x;
}

/*
static int
big_endian(){
  return !little_endian();
}
*/

static int
is_binary(char *p, size_t n) {
  return (memchr(p, 0, n) != NULL);
}

static void
byteswap_fid(void *ptr, int n) {
  int i;
  char t, *p;
  p = ptr;
  for(i = 0; i < n/2; i++) {
    t        = p[i];
    p[i]     = p[n-1-i];
    p[n-1-i] = t;
  }
  return;
}

static void
to_big_endian(void *p, size_t len) {
  if(little_endian()) {
    byteswap_fid(p, len);
  }
}

static int
int_at(char *v, int off, int big) {
  int n;
  n = *(int *)(v + off);
  if(big) {
    to_big_endian(&n, sizeof(n));
  }
  return n;
}
short
short_at(char *v, int off, int big) {
  short n;
  n = *(short *)(v + off);
  if(big) {
    to_big_endian(&n, sizeof(n));
  }
  return n;
}
unsigned short
ushort_at(char *v, int off, int big) {
    unsigned short n;
    n = *(unsigned short *)(v + off);
    if(big) {
        to_big_endian(&n, sizeof(n));
    }
    return n;
}

char
char_at(char *v, int off) {
    return v[off];
}

off_t
fsize(const char *filename) {
  struct stat st;
  if (stat(filename, &st) == 0) {
    return st.st_size;
  }
  return -1;
}

int
fid_ah2(char *buf, int len) {
  int magic;
  if(!is_binary(buf, len)) {
    return 0;
  }
  magic = int_at(buf, 0, TRUE);
  if(magic != 1100) { /* AH2.0 Magic Number */
    return 0;
  }
  return 1;
}

int
fid_ah1(char *buf, int len) {
  int i,n,v;
  /* All Big Endian */
  int off[] = {0, 12, 24, 572, 700, 784, 992};
  int val[] = {6,  6,  8,  80,  80, 202,  21};
  n = sizeof(val) / sizeof(val[0]);

  if(!is_binary(buf,len)){
    return 0;
  }
  for(i = 0; i < n; i++) {
    v = int_at(buf, off[i], TRUE);
    if(v != val[i]) {
      return 0;
    }
  }
  return 1;
}

int
fid_ah0(char *buf, int len) {
  short t;

  if(!is_binary(buf, len)) {
    return 0;
  }
  t = short_at(buf, 632, FALSE);
  if(t < 1 || t > 6) {
    return 0;
  }
  return 1;
}

int
fid_gse(char *buf, int len) {

  if(is_binary(buf,len)) {
    return 0;
  }
  debug( "FID: checking for 'GSE' [%s]\n", "GSE2.0");
  if(!strstr(buf, "GSE")) {
    return 0;
  }
  debug( "FID: checking for 'MSG_TYPE' [%s]\n", "GSE2.0");
  if(!strstr(buf, "MSG_TYPE")) {
    return 0;
  }
  return 1;
}

int
fid_cssxx(char *buf, int len, int version) {
  char w[33];
  int n;
  int nh,noff,woff;
  char *nfmt30 = "%4d", *wfmt30 = "%32s";//, *type30 = "CSS3.0";
  char *nfmt28 = "%8d", *wfmt28 = "%20s";//, *type28 = "CSS2.8";
  char *nfmt,*wfmt;
  if(version == 30) {
    nh   = 284;
    noff =  79;
    woff = 213;
    nfmt = nfmt30;
    wfmt = wfmt30;
  } else { /* Version == 28 */
    nh   = 208;
    noff =  35;
    woff = 137;
    nfmt = nfmt28;
    wfmt = wfmt28;
  }

  if(is_binary(buf, len)) {
    return 0;
  }
  if(buf[nh-1] != '\n') {
    return 0;
  }

  if(sscanf(buf+noff, nfmt, &n) != 1) {
    return 0;
  }
  debug( "FID: checking wfdisc npts %d [%s]\n", n, type);
  if(n <= 0) {
    return 0;
  }

  if(sscanf(buf+woff, wfmt, w) != 1) {
    return 0;
  }
  debug( "FID: checking wfdisc file '%s' [%s]\n", w, type);
  if(fsize(w) < 0) {
    return 0;
  }
  return 1;
}

int
fid_css28(char *buf, int n) {
  return fid_cssxx(buf, n, 28);
}

int
fid_css30(char *buf, int n) {
  return fid_cssxx(buf, n, 30);
}

int
fid_mseed(char *buf, int len) {
  int reclen;
  uint8_t formatversion;

  /* Returns:
   * -1 Data record not detected or error
   * 0  Data record detected but could not determine length
   * >0 Size of the record in bytes
   */
  reclen = ms3_detect (buf, len, &formatversion);

  return (reclen > 0) ? 1 : 0;
}


#define SAC_HEADER_SIZE   632
#define SAC_VERSION_LOC   304
#define SAC_NPTS_LOC      316
#define SAC_HEADER_NUMBER_6  6
#define SAC_HEADER_NUMBER_7  7

int
fid_sac_alpha(char *buf, int len) {
    char *p = NULL;
    int v[5] = {0,0,0,0,0};
    int line = 1;
    if(is_binary(buf, len)) {
        return 0;
    }
    p = buf;
    debug("%s\n", p);
    while(p && *p) {
        debug("LINE: '%d'\n", line);
        if(line == 16) {
            debug("LINE: '%d' CHECKING\n", line);
            if(sscanf(p, "%d %d %d %d %d", &v[0],&v[1],&v[2],&v[3],&v[4]) != 5) {
                return 0;
            }
            debug("%d %d %d %d %d\n", v[0],v[1],v[2],v[3],v[4]);
            if(v[1] != SAC_HEADER_NUMBER_6 && v[1] != SAC_HEADER_NUMBER_7) { // Header Version
                return 0;
            }
            if(v[4] < 0) { // Number of Points
                return 0;
            }
            return 1;
        }
        if(!(p = strchr(p, '\n'))) {
            debug("NO NEWLINE FOUND\n");
            break;
        }
        p++;
        line++;
    }
    return 0;
}

int
fid_sac(char *buf, int len) {
  int v,n;

  if(!is_binary(buf, len)) {
    return 0;
  }
  /* Check Header */
  v = int_at(buf, SAC_VERSION_LOC, FALSE);
  n = int_at(buf, SAC_NPTS_LOC, FALSE);
  if(v != SAC_HEADER_NUMBER_6 && v != SAC_HEADER_NUMBER_7) {
    byteswap_fid(&v, sizeof(v));
    byteswap_fid(&n, sizeof(n));
    if(v != SAC_HEADER_NUMBER_6 && v != SAC_HEADER_NUMBER_7) {
      return 0;
    }
  }
  debug( "FID: header version: %5d\n", v);
  debug( "FID: sample_length:  %5d\n", n);

  /* Number of points must be positive */
  if(n <= 0) {
    return 0;
  }
  return 1;
}

int
fid_segy(char *buf, int n) {
  short n16;
  int n32;
#define SEGY_FILE_HEADER_SIZE       400
#define SEGY_FILE_TEXT_HEADER_SIZE 3200
#define SEGY_TRACE_HEADER_SIZE      240
#define SEGY_NSAMP_LOC              114
#define SEGY_NSAMP_PASSCAL_LOC      228
#define SEGY_NSAMP_MAX            32767

  if((strncmp(buf, "@@@",3) == 0 ||
      strncmp(buf, "C1 ",3) == 0 ||
      strncmp(buf, "C 1",3) == 0 ||
      (*buf == -1 && *(buf+1) == -1 && *(buf+2) == -1))) {
    debug( "FID: checking for file header, found[SEG-Y]\n");
    return 1;
  }
  if(!is_binary(buf, n)) {
    return 0;
  }
  if(!is_binary(buf,SEGY_TRACE_HEADER_SIZE)) {
    return 0;
  }
  /* Sample Rate */
  n16 = short_at(buf, 116, TRUE);
  debug("FID: samp rate: %d [SEG-Y]\n", n16);
  if(n16 < 0) {
    return 0;
  }

  /* Trace ID Number - Very Strict */
  n16 = short_at(buf, 28, TRUE);
  debug( "FID: traceid %d \n", n16);
  if(n16 < -1 || n16 > 30) {
    return 0;
  }

  /* Sample Length */
  n16 = short_at(buf,SEGY_NSAMP_LOC, TRUE);
  debug( "FID: sample_length: %d [0x%02hhx,0x%02hhx]\n", n16, buf[114],buf[115]);
  n32 = (int)n16;
  if(n16 == 0 || n16 == SEGY_NSAMP_MAX) {
    n32 = int_at(buf,SEGY_NSAMP_PASSCAL_LOC, TRUE);
  }
  debug( "FID: sample_length: %d \n", n32);
  /* Number of points must be positive */
  if(n32 < 0) {
    return 0;
  }
  /* Data Format */
  n16 = short_at(buf, 204, TRUE);
  debug("FID: dataform: %hd [SEG-Y]\n", n16);
  if(n16 != 0 && n16 != 1) {
    return 0;
  }

  return 1;
}

struct FID fids[] = {
  {FID_SAC,    632,  fid_sac,   "Seismic Analysis Code"},
  {FID_CSS30,  284,  fid_css30, "CSS3.0"},
  {FID_CSS28,  284,  fid_css28, "CSS2.8"},
  {FID_GSE20,   10,  fid_gse,   "GSE2.0"},
  {FID_AH00,   632,  fid_ah0,   "AH0.0"},
  {FID_AH10,  1017,  fid_ah1,   "AH1.0"},
  {FID_AH20,     4,  fid_ah2,   "AH2.0"},
  {FID_SEGY,   240,  fid_segy,  "SEG-Y"},
  {FID_MSEED,   48,  fid_mseed, "miniSEED"},
  {FID_SACA,  1673,  fid_sac_alpha, "Seismic Analysis Code Alpha"},
  {FID_NULL,     0, NULL, NULL},
};

/**
 * @brief File Identification
 *
 * @param file   filename to attempt identification
 *
 * @return Bits set for matching file types
 */
int
fid(char *file) {
  off_t size;
  FILE *fp;
  int n;
  char buf[2048];
  struct FID *f;
  int out;
  char *ext;

  f = fids;

  out = FID_NULL;
  debug("OUT: %d\n", out);
  /* Find last dot */
  if((ext = rindex(file, '.'))) {
    ext++;
  }


  debug( "FID: examing file: %s\n", file);
  if((size = fsize(file)) < 0) {
      return out;
  }

  debug( "FID: opening file\n");
  if(!(fp = fopen(file, "rb"))) {
      return out;
  }

  /* Read in Beginning of File */
  memset(buf, 0, sizeof(buf));
  n = fread(&buf, 1, sizeof(buf), fp);
  fclose(fp);
  while(f && f->name && f->id > 0) {
    debug( "FID: trying file type %s\n", f->name);
    if(size >= f->min && f->try(buf, n)) {
        debug("\tFOUND %s\n", f->name);
        out |= f->id;
    }
    f++;
  }
  return out;
}
