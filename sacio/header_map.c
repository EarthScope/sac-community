/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t -T --output-file=header_map.c -N sac_keyword_to_header ../../../git/sacio/header_map.txt  */
/* Computed positions: -k'1-3,5,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "../../../git/sacio/header_map.txt"

#include <stdio.h>
#include "sacio.h"
#include <string.h>

#define TOTAL_KEYWORDS 151
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 443
/* maximum key range = 443, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
static unsigned char gperf_downcase[256] =
  {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255
  };
#endif

#ifndef GPERF_CASE_STRCMP
#define GPERF_CASE_STRCMP 1
static int
gperf_case_strcmp (register const char *s1, register const char *s2)
{
  for (;;)
    {
      unsigned char c1 = gperf_downcase[(unsigned char)*s1++];
      unsigned char c2 = gperf_downcase[(unsigned char)*s2++];
      if (c1 != 0 && c1 == c2)
        continue;
      return (int)c1 - (int)c2;
    }
}
#endif

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444,  35,  15,
       50, 115, 100,  10,  20,  25, 105,  45, 444, 444,
      444, 444, 444, 444, 444,   5,  80,  10,  10,   0,
       45, 215,  65,  10,   0,   0,  60,   5,   0,  40,
       35,   0,  80,   0,  25,  95,  35,   0, 255, 230,
        0, 444, 444, 444, 444, 444, 444,   5,  80,  10,
       10,   0,  45, 215,  65,  10,   0,   0,  60,   5,
        0,  40,  35,   0,  80,   0,  25,  95,  35,   0,
      255, 230,   0, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444, 444, 444, 444, 444,
      444, 444, 444, 444, 444, 444
    };
  register unsigned int hval = (unsigned int) len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct hid *
sac_keyword_to_header (register const char *str, register size_t len)
{
  static struct hid wordlist[] =
    {
      {"",-1,-1},
#line 18 "../../../git/sacio/header_map.txt"
      {"e",	    SAC_FLOAT_TYPE,	SAC_E},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 143 "../../../git/sacio/header_map.txt"
      {"knetwk",	SAC_STRING_TYPE,	SAC_NET},
#line 63 "../../../git/sacio/header_map.txt"
      {"az",	    SAC_FLOAT_TYPE,	SAC_AZ},
      {"",-1,-1}, {"",-1,-1},
#line 85 "../../../git/sacio/header_map.txt"
      {"nzmin",	SAC_INT_TYPE,	SAC_MIN},
#line 20 "../../../git/sacio/header_map.txt"
      {"a",	    SAC_FLOAT_TYPE,	SAC_A},
#line 127 "../../../git/sacio/header_map.txt"
      {"ka",	    SAC_STRING_TYPE,	SAC_KA},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 15 "../../../git/sacio/header_map.txt"
      {"scale",	SAC_FLOAT_TYPE, SAC_SCALE},
#line 87 "../../../git/sacio/header_map.txt"
      {"nzmsec",	SAC_INT_TYPE,	SAC_MSEC},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 86 "../../../git/sacio/header_map.txt"
      {"nzsec",	SAC_INT_TYPE,	SAC_SEC},
#line 142 "../../../git/sacio/header_map.txt"
      {"kcmpnm",	SAC_STRING_TYPE,	SAC_CHA},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 75 "../../../git/sacio/header_map.txt"
      {"adjtm", SAC_FLOAT_TYPE, SAC_UN64},
#line 92 "../../../git/sacio/header_map.txt"
      {"nsnpts",	SAC_INT_TYPE,	SAC_NSNPTS},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 147 "../../../git/sacio/header_map.txt"
      {"kztime",	SAC_AUX_TYPE,	SAC_TIME},
#line 108 "../../../git/sacio/header_map.txt"
      {"imagsrc",	SAC_ENUM_TYPE,	SAC_MAG_SRC},
      {"",-1,-1}, {"",-1,-1},
#line 122 "../../../git/sacio/header_map.txt"
      {"kstnm",	SAC_STRING_TYPE,	SAC_STA},
#line 146 "../../../git/sacio/header_map.txt"
      {"kzdate",	SAC_AUX_TYPE,	SAC_DATE},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 67 "../../../git/sacio/header_map.txt"
      {"sdelta", SAC_FLOAT_TYPE, SAC_SDELTA},
#line 27 "../../../git/sacio/header_map.txt"
      {"t5",	    SAC_FLOAT_TYPE,	SAC_T5},
#line 133 "../../../git/sacio/header_map.txt"
      {"kt5",	  SAC_STRING_TYPE,	SAC_KT5},
#line 62 "../../../git/sacio/header_map.txt"
      {"dist",	  SAC_FLOAT_TYPE,	SAC_DIST},
#line 123 "../../../git/sacio/header_map.txt"
      {"kevnm",	SAC_LONG_STRING_TYPE,	SAC_EVENT},
#line 68 "../../../git/sacio/header_map.txt"
      {"depmen", SAC_FLOAT_TYPE, SAC_DEPMEN},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 69 "../../../git/sacio/header_map.txt"
      {"cmpaz",	SAC_FLOAT_TYPE,	SAC_CMPAZ},
#line 124 "../../../git/sacio/header_map.txt"
      {"kevnmc",	SAC_LONG_STRING_TYPE,	SAC_EVENT},
#line 23 "../../../git/sacio/header_map.txt"
      {"t1",	    SAC_FLOAT_TYPE,	SAC_T1},
#line 129 "../../../git/sacio/header_map.txt"
      {"kt1",	  SAC_STRING_TYPE,	SAC_KT1},
#line 98 "../../../git/sacio/header_map.txt"
      {"idep",	  SAC_ENUM_TYPE,	SAC_DEP_TYPE},
#line 90 "../../../git/sacio/header_map.txt"
      {"nevid",	SAC_INT_TYPE,	SAC_EVID},
#line 13 "../../../git/sacio/header_map.txt"
      {"depmin",	SAC_FLOAT_TYPE, SAC_DEPMIN},
      {"",-1,-1}, {"",-1,-1},
#line 91 "../../../git/sacio/header_map.txt"
      {"npts",	  SAC_INT_TYPE,	SAC_NPTS},
#line 145 "../../../git/sacio/header_map.txt"
      {"kinst",	SAC_STRING_TYPE,	SAC_INST},
#line 70 "../../../git/sacio/header_map.txt"
      {"cmpinc",	SAC_FLOAT_TYPE,	SAC_CMPINC},
#line 28 "../../../git/sacio/header_map.txt"
      {"t6",	    SAC_FLOAT_TYPE,	SAC_T6},
#line 134 "../../../git/sacio/header_map.txt"
      {"kt6",	  SAC_STRING_TYPE,	SAC_KT6},
      {"",-1,-1},
#line 93 "../../../git/sacio/header_map.txt"
      {"nwfid",	SAC_INT_TYPE,	SAC_WFID},
#line 148 "../../../git/sacio/header_map.txt"
      {"kstcmp",	SAC_AUX_TYPE,	SAC_STCMP},
      {"",-1,-1}, {"",-1,-1},
#line 46 "../../../git/sacio/header_map.txt"
      {"stdp",	  SAC_FLOAT_TYPE,	SAC_STDP},
#line 101 "../../../git/sacio/header_map.txt"
      {"iinst",	SAC_ENUM_TYPE,	SAC_INST_TYPE},
#line 99 "../../../git/sacio/header_map.txt"
      {"iztype",	SAC_ENUM_TYPE,	SAC_ZERO_TIME},
#line 29 "../../../git/sacio/header_map.txt"
      {"t7",	    SAC_FLOAT_TYPE,	SAC_T7},
#line 135 "../../../git/sacio/header_map.txt"
      {"kt7",	  SAC_STRING_TYPE,	SAC_KT7},
      {"",-1,-1}, {"",-1,-1},
#line 19 "../../../git/sacio/header_map.txt"
      {"o",	    SAC_FLOAT_TYPE,	SAC_O},
#line 126 "../../../git/sacio/header_map.txt"
      {"ko",	    SAC_STRING_TYPE,	SAC_KO},
      {"",-1,-1},
#line 50 "../../../git/sacio/header_map.txt"
      {"evdp",	  SAC_FLOAT_TYPE,	SAC_EVDP},
#line 12 "../../../git/sacio/header_map.txt"
      {"delta",	SAC_FLOAT_TYPE, SAC_DELTA},
#line 16 "../../../git/sacio/header_map.txt"
      {"odelta",	SAC_FLOAT_TYPE,	SAC_ODELTA},
#line 107 "../../../git/sacio/header_map.txt"
      {"imagtyp",	SAC_ENUM_TYPE,	SAC_MAG_TYPE},
#line 64 "../../../git/sacio/header_map.txt"
      {"baz",	  SAC_FLOAT_TYPE,	SAC_BAZ},
#line 45 "../../../git/sacio/header_map.txt"
      {"stel",	  SAC_FLOAT_TYPE,	SAC_STEL},
      {"",-1,-1},
#line 32 "../../../git/sacio/header_map.txt"
      {"f",	    SAC_FLOAT_TYPE,	SAC_F},
#line 138 "../../../git/sacio/header_map.txt"
      {"kf",	    SAC_STRING_TYPE,	SAC_KF},
      {"",-1,-1},
#line 43 "../../../git/sacio/header_map.txt"
      {"stla",	  SAC_FLOAT_TYPE,	SAC_STLA},
      {"",-1,-1},
#line 120 "../../../git/sacio/header_map.txt"
      {"lcalda",	SAC_BOOL_TYPE,	SAC_CALC_DIST_AZ},
#line 22 "../../../git/sacio/header_map.txt"
      {"t0",	    SAC_FLOAT_TYPE,	SAC_T0},
#line 128 "../../../git/sacio/header_map.txt"
      {"kt0",	  SAC_STRING_TYPE,	SAC_KT0},
#line 49 "../../../git/sacio/header_map.txt"
      {"evel",	  SAC_FLOAT_TYPE,	SAC_EVEL},
#line 117 "../../../git/sacio/header_map.txt"
      {"leven",	SAC_BOOL_TYPE,	SAC_EVEN},
      {"",-1,-1},
#line 150 "../../../git/sacio/header_map.txt"
      {"amarker",	SAC_AUX_TYPE,	SAC_AMARKER},
#line 21 "../../../git/sacio/header_map.txt"
      {"fmt",	  SAC_FLOAT_TYPE,	SAC_FMT},
#line 47 "../../../git/sacio/header_map.txt"
      {"evla",	  SAC_FLOAT_TYPE,	SAC_EVLA},
#line 38 "../../../git/sacio/header_map.txt"
      {"resp5",	SAC_FLOAT_TYPE,	SAC_RESP5},
#line 96 "../../../git/sacio/header_map.txt"
      {"nhdr15", SAC_INT_TYPE, SAC_UN85},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 125 "../../../git/sacio/header_map.txt"
      {"khole",	SAC_STRING_TYPE,	SAC_HOLE},
#line 144 "../../../git/sacio/header_map.txt"
      {"kdatrd",	SAC_STRING_TYPE,	SAC_DATRD},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 34 "../../../git/sacio/header_map.txt"
      {"resp1",	SAC_FLOAT_TYPE,	SAC_RESP1},
#line 111 "../../../git/sacio/header_map.txt"
      {"ihdr15", SAC_ENUM_TYPE, SAC_UN100},
#line 31 "../../../git/sacio/header_map.txt"
      {"t9",	    SAC_FLOAT_TYPE,	SAC_T9},
#line 137 "../../../git/sacio/header_map.txt"
      {"kt9",	  SAC_STRING_TYPE,	SAC_KT9},
      {"",-1,-1},
#line 57 "../../../git/sacio/header_map.txt"
      {"user5",	SAC_FLOAT_TYPE,	SAC_USER5},
#line 97 "../../../git/sacio/header_map.txt"
      {"iftype",	SAC_ENUM_TYPE,	SAC_FILE_TYPE},
      {"",-1,-1},
#line 149 "../../../git/sacio/header_map.txt"
      {"filename",	SAC_AUX_TYPE,	SAC_FILENAME},
      {"",-1,-1},
#line 39 "../../../git/sacio/header_map.txt"
      {"resp6",	SAC_FLOAT_TYPE,	SAC_RESP6},
#line 112 "../../../git/sacio/header_map.txt"
      {"ihdr16", SAC_ENUM_TYPE, SAC_UN101},
#line 24 "../../../git/sacio/header_map.txt"
      {"t2",	    SAC_FLOAT_TYPE,	SAC_T2},
#line 130 "../../../git/sacio/header_map.txt"
      {"kt2",	  SAC_STRING_TYPE,	SAC_KT2},
#line 44 "../../../git/sacio/header_map.txt"
      {"stlo",	  SAC_FLOAT_TYPE,	SAC_STLO},
#line 53 "../../../git/sacio/header_map.txt"
      {"user1",	SAC_FLOAT_TYPE,	SAC_USER1},
#line 113 "../../../git/sacio/header_map.txt"
      {"ihdr17", SAC_ENUM_TYPE, SAC_UN102},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 40 "../../../git/sacio/header_map.txt"
      {"resp7",	SAC_FLOAT_TYPE,	SAC_RESP7},
      {"",-1,-1},
#line 151 "../../../git/sacio/header_map.txt"
      {"omarker",	SAC_AUX_TYPE,	SAC_OMARKER},
      {"",-1,-1},
#line 48 "../../../git/sacio/header_map.txt"
      {"evlo",	  SAC_FLOAT_TYPE,	SAC_EVLO},
#line 58 "../../../git/sacio/header_map.txt"
      {"user6",	SAC_FLOAT_TYPE,	SAC_USER6},
      {"",-1,-1},
#line 152 "../../../git/sacio/header_map.txt"
      {"fmarker",	SAC_AUX_TYPE,	SAC_FMARKER},
      {"",-1,-1}, {"",-1,-1},
#line 89 "../../../git/sacio/header_map.txt"
      {"norid",	SAC_INT_TYPE,	SAC_ORID},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 59 "../../../git/sacio/header_map.txt"
      {"user7",	SAC_FLOAT_TYPE,	SAC_USER7},
#line 115 "../../../git/sacio/header_map.txt"
      {"ihdr19", SAC_ENUM_TYPE, SAC_UN104},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 33 "../../../git/sacio/header_map.txt"
      {"resp0",	SAC_FLOAT_TYPE,	SAC_RESP0},
#line 76 "../../../git/sacio/header_map.txt"
      {"fhdr65", SAC_FLOAT_TYPE, SAC_UN65},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 121 "../../../git/sacio/header_map.txt"
      {"lhdr5", SAC_BOOL_TYPE,  SAC_UN110},
#line 17 "../../../git/sacio/header_map.txt"
      {"b",	    SAC_FLOAT_TYPE,	SAC_B},
#line 66 "../../../git/sacio/header_map.txt"
      {"sb",     SAC_FLOAT_TYPE, SAC_SB},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 77 "../../../git/sacio/header_map.txt"
      {"fhdr66", SAC_FLOAT_TYPE, SAC_UN66},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 52 "../../../git/sacio/header_map.txt"
      {"user0",	SAC_FLOAT_TYPE,	SAC_USER0},
#line 78 "../../../git/sacio/header_map.txt"
      {"fhdr67", SAC_FLOAT_TYPE, SAC_UN67},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 42 "../../../git/sacio/header_map.txt"
      {"resp9",	SAC_FLOAT_TYPE,	SAC_RESP9},
#line 116 "../../../git/sacio/header_map.txt"
      {"ihdr20", SAC_ENUM_TYPE, SAC_UN105},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 119 "../../../git/sacio/header_map.txt"
      {"lovrok",	SAC_BOOL_TYPE,	SAC_OVERWRITE},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 35 "../../../git/sacio/header_map.txt"
      {"resp2",	SAC_FLOAT_TYPE,	SAC_RESP2},
#line 81 "../../../git/sacio/header_map.txt"
      {"fhdr70", SAC_FLOAT_TYPE, SAC_UN70},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 61 "../../../git/sacio/header_map.txt"
      {"user9",	SAC_FLOAT_TYPE,	SAC_USER9},
#line 80 "../../../git/sacio/header_map.txt"
      {"fhdr69", SAC_FLOAT_TYPE, SAC_UN69},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 140 "../../../git/sacio/header_map.txt"
      {"kuser1",	SAC_STRING_TYPE,	SAC_KUSER1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 54 "../../../git/sacio/header_map.txt"
      {"user2",	SAC_FLOAT_TYPE,	SAC_USER2},
#line 118 "../../../git/sacio/header_map.txt"
      {"lpspol",	SAC_BOOL_TYPE,	SAC_POLARITY},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 110 "../../../git/sacio/header_map.txt"
      {"ihdr14", SAC_ENUM_TYPE, SAC_UN99},
      {"",-1,-1},
#line 158 "../../../git/sacio/header_map.txt"
      {"t5marker",	SAC_AUX_TYPE,	SAC_T5MARKER},
      {"",-1,-1}, {"",-1,-1},
#line 114 "../../../git/sacio/header_map.txt"
      {"ihdr18", SAC_ENUM_TYPE, SAC_UN103},
      {"",-1,-1},
#line 154 "../../../git/sacio/header_map.txt"
      {"t1marker",	SAC_AUX_TYPE,	SAC_T1MARKER},
      {"",-1,-1}, {"",-1,-1},
#line 139 "../../../git/sacio/header_map.txt"
      {"kuser0",	SAC_STRING_TYPE,	SAC_KUSER0},
      {"",-1,-1},
#line 159 "../../../git/sacio/header_map.txt"
      {"t6marker",	SAC_AUX_TYPE,	SAC_T6MARKER},
      {"",-1,-1}, {"",-1,-1},
#line 109 "../../../git/sacio/header_map.txt"
      {"ihdr13", SAC_ENUM_TYPE, SAC_UN98},
      {"",-1,-1},
#line 160 "../../../git/sacio/header_map.txt"
      {"t7marker",	SAC_AUX_TYPE,	SAC_T7MARKER},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 26 "../../../git/sacio/header_map.txt"
      {"t4",	    SAC_FLOAT_TYPE,	SAC_T4},
#line 132 "../../../git/sacio/header_map.txt"
      {"kt4",	  SAC_STRING_TYPE,	SAC_KT4},
      {"",-1,-1},
#line 105 "../../../git/sacio/header_map.txt"
      {"iqual",	SAC_ENUM_TYPE,	SAC_QUAL},
#line 141 "../../../git/sacio/header_map.txt"
      {"kuser2",	SAC_STRING_TYPE,	SAC_KUSER2},
      {"",-1,-1},
#line 153 "../../../git/sacio/header_map.txt"
      {"t0marker",	SAC_AUX_TYPE,	SAC_T0MARKER},
      {"",-1,-1}, {"",-1,-1},
#line 95 "../../../git/sacio/header_map.txt"
      {"nysize",	SAC_INT_TYPE,	SAC_NY},
#line 30 "../../../git/sacio/header_map.txt"
      {"t8",	    SAC_FLOAT_TYPE,	SAC_T8},
#line 136 "../../../git/sacio/header_map.txt"
      {"kt8",	  SAC_STRING_TYPE,	SAC_KT8},
      {"",-1,-1}, {"",-1,-1},
#line 83 "../../../git/sacio/header_map.txt"
      {"nzjday",	SAC_INT_TYPE,	SAC_DAY},
      {"",-1,-1},
#line 162 "../../../git/sacio/header_map.txt"
      {"t9marker",	SAC_AUX_TYPE,	SAC_T9MARKER},
      {"",-1,-1}, {"",-1,-1},
#line 84 "../../../git/sacio/header_map.txt"
      {"nzhour",	SAC_INT_TYPE,	SAC_HOUR},
      {"",-1,-1},
#line 155 "../../../git/sacio/header_map.txt"
      {"t2marker",	SAC_AUX_TYPE,	SAC_T2MARKER},
      {"",-1,-1}, {"",-1,-1},
#line 79 "../../../git/sacio/header_map.txt"
      {"fhdr68", SAC_FLOAT_TYPE, SAC_UN68},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 65 "../../../git/sacio/header_map.txt"
      {"gcarc",	SAC_FLOAT_TYPE,	SAC_GCARC},
#line 102 "../../../git/sacio/header_map.txt"
      {"istreg",	SAC_ENUM_TYPE,	SAC_STREG},
#line 25 "../../../git/sacio/header_map.txt"
      {"t3",	    SAC_FLOAT_TYPE,	SAC_T3},
#line 131 "../../../git/sacio/header_map.txt"
      {"kt3",	  SAC_STRING_TYPE,	SAC_KT3},
      {"",-1,-1}, {"",-1,-1},
#line 94 "../../../git/sacio/header_map.txt"
      {"nxsize",	SAC_INT_TYPE,	SAC_NX},
      {"",-1,-1},
#line 74 "../../../git/sacio/header_map.txt"
      {"ymaximum",	SAC_FLOAT_TYPE,	SAC_YMAX},
      {"",-1,-1},
#line 88 "../../../git/sacio/header_map.txt"
      {"nvhdr",	SAC_INT_TYPE,	SAC_HDR},
#line 103 "../../../git/sacio/header_map.txt"
      {"ievreg",	SAC_ENUM_TYPE,	SAC_EVREG},
      {"",-1,-1},
#line 73 "../../../git/sacio/header_map.txt"
      {"yminimum",	SAC_FLOAT_TYPE,	SAC_YMIN},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 37 "../../../git/sacio/header_map.txt"
      {"resp4",	SAC_FLOAT_TYPE,	SAC_RESP4},
      {"",-1,-1}, {"",-1,-1},
#line 72 "../../../git/sacio/header_map.txt"
      {"xmaximum",	SAC_FLOAT_TYPE,	SAC_XMAX},
      {"",-1,-1},
#line 100 "../../../git/sacio/header_map.txt"
      {"ihdr4",  SAC_ENUM_TYPE,  SAC_UN89},
      {"",-1,-1}, {"",-1,-1},
#line 71 "../../../git/sacio/header_map.txt"
      {"xminimum",	SAC_FLOAT_TYPE,	SAC_XMIN},
      {"",-1,-1},
#line 41 "../../../git/sacio/header_map.txt"
      {"resp8",	SAC_FLOAT_TYPE,	SAC_RESP8},
      {"",-1,-1}, {"",-1,-1},
#line 157 "../../../git/sacio/header_map.txt"
      {"t4marker",	SAC_AUX_TYPE,	SAC_T4MARKER},
      {"",-1,-1},
#line 56 "../../../git/sacio/header_map.txt"
      {"user4",	SAC_FLOAT_TYPE,	SAC_USER4},
      {"",-1,-1}, {"",-1,-1},
#line 161 "../../../git/sacio/header_map.txt"
      {"t8marker",	SAC_AUX_TYPE,	SAC_T8MARKER},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 60 "../../../git/sacio/header_map.txt"
      {"user8",	SAC_FLOAT_TYPE,	SAC_USER8},
#line 14 "../../../git/sacio/header_map.txt"
      {"depmax",	SAC_FLOAT_TYPE, SAC_DEPMAX},
      {"",-1,-1},
#line 156 "../../../git/sacio/header_map.txt"
      {"t3marker",	SAC_AUX_TYPE,	SAC_T3MARKER},
      {"",-1,-1},
#line 36 "../../../git/sacio/header_map.txt"
      {"resp3",	SAC_FLOAT_TYPE,	SAC_RESP3},
#line 104 "../../../git/sacio/header_map.txt"
      {"ievtyp", SAC_ENUM_TYPE,  SAC_EVENT_TYPE},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 82 "../../../git/sacio/header_map.txt"
      {"nzyear",	SAC_INT_TYPE,	SAC_YEAR},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 55 "../../../git/sacio/header_map.txt"
      {"user3",	SAC_FLOAT_TYPE,	SAC_USER3},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1},
#line 106 "../../../git/sacio/header_map.txt"
      {"isynth",	SAC_ENUM_TYPE,	SAC_SYNTH},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 51 "../../../git/sacio/header_map.txt"
      {"mag",	  SAC_FLOAT_TYPE,	SAC_MAG}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 163 "../../../git/sacio/header_map.txt"


#ifdef __TESTING__
//struct hid { char *name; int  type; int  id; };
int
main() {
       struct hid *h = NULL;
       h = sac_keyword_to_header("delta", 5);
       printf("%s %d %d\n", h->name, h->type, h->id);
       h = sac_keyword_to_header("delt", 4);
       if(h) {
          printf("%s %d %d\n", h->name, h->type, h->id);
       }
}
#endif
