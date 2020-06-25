/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf enums.txt  */
/* Computed positions: -k'1-3,$' */

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
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "enums.txt"

#include <stdio.h>
#include "sacio.h"
#include <string.h>

#define TOTAL_KEYWORDS 146
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 17
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 312
/* maximum key range = 311, duplicates = 0 */

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
hash (register const char *str, register unsigned int len)
{
  static unsigned short asso_values[] =
    {
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313,  95, 100,
      155,  30,  70,  90,  40,  15, 120, 105,  35, 313,
      313, 313, 313, 313, 313,   5,  25,   0,  10,  20,
       55, 100, 140,   0,  85,  85,  60,   5,  15,  80,
        0,  40,  45,   0,   0,  75,  65,   5,  35,  55,
       75,  15, 313, 313, 313,  20,  15,   5,  25,   0,
       10,  20,  55, 100, 140,   0,  85,  85,  60,   5,
       15,  80,   0,  40,  45,   0,   0,  75,  65,   5,
       35,  55,  75,  15, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313, 313, 313, 313,
      313, 313, 313, 313, 313, 313, 313
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]+1];
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

struct eid *
sac_enum_to_id (register const char *str, register unsigned int len)
{
  static struct eid wordlist[] =
    {
      {"",-1,-1}, {"",-1,-1},
#line 154 "enums.txt"
      {"it", SAC_EVENT_TYPE, IT},
      {"",-1,-1},
#line 109 "enums.txt"
      {"iisc", SAC_MAG_SRC, IISC},
      {"",-1,-1}, {"",-1,-1},
#line 92 "enums.txt"
      {"ms", SAC_MAG_TYPE, IMS},
#line 91 "enums.txt"
      {"ims", SAC_MAG_TYPE, IMS},
      {"",-1,-1},
#line 79 "enums.txt"
      {"ichem", SAC_EVENT_TYPE, ICHEM},
#line 47 "enums.txt"
      {"a", SAC_ZERO_TIME, IA},
#line 46 "enums.txt"
      {"ia", SAC_ZERO_TIME, IA},
#line 110 "enums.txt"
      {"isc", SAC_MAG_SRC, IISC},
      {"",-1,-1},
#line 85 "enums.txt"
      {"idrop", SAC_QUAL, IDROP},
      {"",-1,-1},
#line 96 "enums.txt"
      {"mw", SAC_MAG_TYPE, IMW},
      {"",-1,-1},
#line 32 "enums.txt"
      {"iacc",  SAC_DEP_TYPE, IACC},
#line 71 "enums.txt"
      {"ipren", SAC_EVENT_TYPE, IPREN},
#line 73 "enums.txt"
      {"ipostn", SAC_EVENT_TYPE, IPOSTN},
#line 29 "enums.txt"
      {"displacement",  SAC_DEP_TYPE, IDISP},
      {"",-1,-1}, {"",-1,-1},
#line 133 "enums.txt"
      {"imoon", SAC_BODY_TYPE, IMOON},
      {"",-1,-1},
#line 98 "enums.txt"
      {"md", SAC_MAG_TYPE, IMD},
#line 149 "enums.txt"
      {"inc", SAC_EVENT_TYPE, INC},
      {"",-1,-1},
#line 105 "enums.txt"
      {"ipdew", SAC_MAG_SRC, IPDEW},
#line 121 "enums.txt"
      {"ievloc", SAC_MAG_SRC, IEVLOC},
#line 64 "enums.txt"
      {"t7", SAC_ZERO_TIME, IT7},
#line 89 "enums.txt"
      {"imb", SAC_MAG_TYPE, IMB},
      {"",-1,-1},
#line 134 "enums.txt"
      {"imars", SAC_BODY_TYPE, IMARS},
#line 155 "enums.txt"
      {"teleseismic", SAC_EVENT_TYPE, IT},
      {"",-1,-1},
#line 97 "enums.txt"
      {"imd", SAC_MAG_TYPE, IMD},
#line 18 "enums.txt"
      {"time", SAC_FILE_TYPE, ITIME},
      {"",-1,-1},
#line 145 "enums.txt"
      {"marine-explosion", SAC_EVENT_TYPE, IME},
#line 33 "enums.txt"
      {"acceleration",  SAC_DEP_TYPE, IACC},
      {"",-1,-1},
#line 107 "enums.txt"
      {"ipde", SAC_MAG_SRC, IPDE},
#line 77 "enums.txt"
      {"ipreq", SAC_EVENT_TYPE, IPREQ},
#line 78 "enums.txt"
      {"ipostq", SAC_EVENT_TYPE, IPOSTQ},
      {"",-1,-1},
#line 95 "enums.txt"
      {"imw", SAC_MAG_TYPE, IMW},
      {"",-1,-1}, {"",-1,-1},
#line 39 "enums.txt"
      {"b", SAC_ZERO_TIME, IB},
#line 38 "enums.txt"
      {"ib", SAC_ZERO_TIME, IB},
#line 160 "enums.txt"
      {"iqc", SAC_EVENT_TYPE, IQC},
      {"",-1,-1},
#line 141 "enums.txt"
      {"earthquake", SAC_EVENT_TYPE, IEQ},
      {"",-1,-1},
#line 90 "enums.txt"
      {"mb", SAC_MAG_TYPE, IMB},
#line 61 "enums.txt"
      {"it6", SAC_ZERO_TIME, IT6},
#line 139 "enums.txt"
      {"iqmt", SAC_EVENT_TYPE, IQMT},
#line 19 "enums.txt"
      {"irlim",  SAC_FILE_TYPE, IRLIM},
#line 34 "enums.txt"
      {"irldta",  SAC_DEP_TYPE, IRLDTA},
#line 56 "enums.txt"
      {"t3", SAC_ZERO_TIME, IT3},
#line 86 "enums.txt"
      {"dropouts", SAC_QUAL, IDROP},
#line 165 "enums.txt"
      {"imet", SAC_EVENT_TYPE, IMET},
#line 103 "enums.txt"
      {"ipdeq", SAC_MAG_SRC, IPDEQ},
      {"",-1,-1}, {"",-1,-1},
#line 135 "enums.txt"
      {"iqb", SAC_EVENT_TYPE, IQB},
      {"",-1,-1}, {"",-1,-1},
#line 36 "enums.txt"
      {"ivolts",  SAC_DEP_TYPE, IVOLTS},
      {"",-1,-1},
#line 93 "enums.txt"
      {"iml", SAC_MAG_TYPE, IML},
#line 106 "enums.txt"
      {"pdew", SAC_MAG_SRC, IPDEW},
#line 101 "enums.txt"
      {"ineic", SAC_MAG_SRC, INEIC},
      {"",-1,-1},
#line 100 "enums.txt"
      {"mx", SAC_MAG_TYPE, IMX},
      {"",-1,-1},
#line 138 "enums.txt"
      {"iqbx", SAC_EVENT_TYPE, IQBX},
#line 113 "enums.txt"
      {"iusgs", SAC_MAG_SRC, IUSGS},
#line 87 "enums.txt"
      {"ilowsn", SAC_QUAL, ILOWSN},
#line 62 "enums.txt"
      {"t6", SAC_ZERO_TIME, IT6},
#line 144 "enums.txt"
      {"ime", SAC_EVENT_TYPE, IME},
#line 129 "enums.txt"
      {"isun", SAC_BODY_TYPE, ISUN},
#line 22 "enums.txt"
      {"amplitude-phase",  SAC_FILE_TYPE, IAMPH},
      {"",-1,-1}, {"",-1,-1},
#line 108 "enums.txt"
      {"pde", SAC_MAG_SRC, IPDE},
#line 124 "enums.txt"
      {"jsop", SAC_MAG_SRC, IJSOP},
#line 123 "enums.txt"
      {"ijsop", SAC_MAG_SRC, IJSOP},
      {"",-1,-1},
#line 152 "enums.txt"
      {"ir", SAC_EVENT_TYPE, IR},
#line 112 "enums.txt"
      {"reb", SAC_MAG_SRC, IREB},
#line 41 "enums.txt"
      {"iday", SAC_ZERO_TIME, IDAY},
#line 122 "enums.txt"
      {"evloc", SAC_MAG_SRC, IEVLOC},
      {"",-1,-1}, {"",-1,-1},
#line 99 "enums.txt"
      {"imx", SAC_MAG_TYPE, IMX},
#line 157 "enums.txt"
      {"ieq3", SAC_EVENT_TYPE, IEQ3},
#line 28 "enums.txt"
      {"idisp",  SAC_DEP_TYPE, IDISP},
      {"",-1,-1}, {"",-1,-1},
#line 55 "enums.txt"
      {"it3", SAC_ZERO_TIME, IT3},
#line 35 "enums.txt"
      {"real-data",  SAC_DEP_TYPE, IRLDTA},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 140 "enums.txt"
      {"ieq", SAC_EVENT_TYPE, IEQ},
#line 104 "enums.txt"
      {"pdeq", SAC_MAG_SRC, IPDEQ},
#line 17 "enums.txt"
      {"itime",  SAC_FILE_TYPE, ITIME},
      {"",-1,-1}, {"",-1,-1},
#line 146 "enums.txt"
      {"iex", SAC_EVENT_TYPE, IEX},
#line 115 "enums.txt"
      {"ibrk", SAC_MAG_SRC, IBRK},
#line 81 "enums.txt"
      {"igood", SAC_QUAL, IGOOD},
      {"",-1,-1}, {"",-1,-1},
#line 150 "enums.txt"
      {"io_", SAC_EVENT_TYPE, IO_},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 151 "enums.txt"
      {"il", SAC_EVENT_TYPE, IL},
#line 130 "enums.txt"
      {"imercury", SAC_BODY_TYPE, IMERCURY},
#line 102 "enums.txt"
      {"neic", SAC_MAG_SRC, INEIC},
#line 125 "enums.txt"
      {"iuser", SAC_MAG_SRC, IUSER},
#line 131 "enums.txt"
      {"ivenus", SAC_BODY_TYPE, IVENUS},
#line 94 "enums.txt"
      {"ml", SAC_MAG_TYPE, IML},
#line 48 "enums.txt"
      {"first-arrival", SAC_ZERO_TIME, IA},
#line 111 "enums.txt"
      {"ireb", SAC_MAG_SRC, IREB},
#line 119 "enums.txt"
      {"illnl", SAC_MAG_SRC, ILLNL},
#line 75 "enums.txt"
      {"iquake", SAC_EVENT_TYPE, IQUAKE},
#line 148 "enums.txt"
      {"nuclear-explosion", SAC_EVENT_TYPE, INU},
#line 59 "enums.txt"
      {"it5", SAC_ZERO_TIME, IT5},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 63 "enums.txt"
      {"it7", SAC_ZERO_TIME, IT7},
#line 161 "enums.txt"
      {"iqb0", SAC_EVENT_TYPE, IQB0},
      {"",-1,-1}, {"",-1,-1},
#line 58 "enums.txt"
      {"t4", SAC_ZERO_TIME, IT4},
#line 67 "enums.txt"
      {"it9", SAC_ZERO_TIME, IT9},
#line 136 "enums.txt"
      {"iqb1", SAC_EVENT_TYPE, IQB1},
#line 69 "enums.txt"
      {"inucl", SAC_EVENT_TYPE, INUCL},
      {"",-1,-1},
#line 24 "enums.txt"
      {"xy",    SAC_FILE_TYPE, IXY},
#line 42 "enums.txt"
      {"day", SAC_ZERO_TIME, IDAY},
#line 164 "enums.txt"
      {"ilit", SAC_EVENT_TYPE, ILIT},
#line 166 "enums.txt"
      {"iodor", SAC_EVENT_TYPE, IODOR},
      {"",-1,-1},
#line 156 "enums.txt"
      {"iu", SAC_EVENT_TYPE, IU},
#line 31 "enums.txt"
      {"velocity",  SAC_DEP_TYPE, IVEL},
      {"",-1,-1},
#line 37 "enums.txt"
      {"volts",  SAC_DEP_TYPE, IVOLTS},
      {"",-1,-1},
#line 118 "enums.txt"
      {"caltech", SAC_MAG_SRC, ICALTECH},
#line 147 "enums.txt"
      {"inu", SAC_EVENT_TYPE, INU},
#line 20 "enums.txt"
      {"real-imaginary",  SAC_FILE_TYPE, IRLIM},
      {"",-1,-1},
#line 44 "enums.txt"
      {"o", SAC_ZERO_TIME, IO},
#line 43 "enums.txt"
      {"io", SAC_ZERO_TIME, IO},
#line 57 "enums.txt"
      {"it4", SAC_ZERO_TIME, IT4},
#line 158 "enums.txt"
      {"ieq0", SAC_EVENT_TYPE, IEQ0},
#line 21 "enums.txt"
      {"iamph",  SAC_FILE_TYPE, IAMPH},
      {"",-1,-1}, {"",-1,-1},
#line 23 "enums.txt"
      {"ixy",    SAC_FILE_TYPE, IXY},
#line 142 "enums.txt"
      {"ieq1", SAC_EVENT_TYPE, IEQ1},
      {"",-1,-1}, {"",-1,-1},
#line 128 "enums.txt"
      {"unknown", SAC_MAG_SRC, IUNKNOWN},
#line 117 "enums.txt"
      {"icaltech", SAC_MAG_SRC, ICALTECH},
#line 159 "enums.txt"
      {"iex0", SAC_EVENT_TYPE, IEX0},
#line 27 "enums.txt"
      {"iunkn",  SAC_DEP_TYPE, IUNKN},
      {"",-1,-1}, {"",-1,-1},
#line 127 "enums.txt"
      {"iunknown", SAC_MAG_SRC, IUNKNOWN},
#line 126 "enums.txt"
      {"user", SAC_MAG_SRC, IUSER},
      {"",-1,-1}, {"",-1,-1},
#line 60 "enums.txt"
      {"t5", SAC_ZERO_TIME, IT5},
#line 26 "enums.txt"
      {"xyz",   SAC_FILE_TYPE, IXYZ},
#line 30 "enums.txt"
      {"ivel",  SAC_DEP_TYPE, IVEL},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 53 "enums.txt"
      {"it2", SAC_ZERO_TIME, IT2},
#line 25 "enums.txt"
      {"ixyz",   SAC_FILE_TYPE, IXYZ},
      {"",-1,-1},
#line 132 "enums.txt"
      {"iearth", SAC_BODY_TYPE, IEARTH},
#line 50 "enums.txt"
      {"t0", SAC_ZERO_TIME, IT0},
      {"",-1,-1},
#line 82 "enums.txt"
      {"good", SAC_QUAL, IGOOD},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 49 "enums.txt"
      {"it0", SAC_ZERO_TIME, IT0},
#line 137 "enums.txt"
      {"iqb2", SAC_EVENT_TYPE, IQB2},
      {"",-1,-1}, {"",-1,-1},
#line 52 "enums.txt"
      {"t1", SAC_ZERO_TIME, IT1},
      {"",-1,-1}, {"",-1,-1},
#line 40 "enums.txt"
      {"begin", SAC_ZERO_TIME, IB},
#line 80 "enums.txt"
      {"iother", SAC_EVENT_TYPE, IOTHER},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1},
#line 68 "enums.txt"
      {"t9", SAC_ZERO_TIME, IT9},
      {"",-1,-1},
#line 162 "enums.txt"
      {"igey", SAC_EVENT_TYPE, IGEY},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 116 "enums.txt"
      {"brk", SAC_MAG_SRC, IBRK},
#line 114 "enums.txt"
      {"usgs", SAC_MAG_SRC, IUSGS},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 143 "enums.txt"
      {"ieq2", SAC_EVENT_TYPE, IEQ2},
      {"",-1,-1}, {"",-1,-1},
#line 88 "enums.txt"
      {"low-snr", SAC_QUAL, ILOWSN},
#line 65 "enums.txt"
      {"it8", SAC_ZERO_TIME, IT8},
      {"",-1,-1}, {"",-1,-1},
#line 45 "enums.txt"
      {"origin", SAC_ZERO_TIME, IO},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 66 "enums.txt"
      {"t8", SAC_ZERO_TIME, IT8},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 163 "enums.txt"
      {"geyser", SAC_EVENT_TYPE, IGEY},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 83 "enums.txt"
      {"iglch", SAC_QUAL, IGLCH},
      {"",-1,-1}, {"",-1,-1},
#line 84 "enums.txt"
      {"glitches", SAC_QUAL, IGLCH},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 51 "enums.txt"
      {"it1", SAC_ZERO_TIME, IT1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1},
#line 120 "enums.txt"
      {"llnl", SAC_MAG_SRC, ILLNL},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 153 "enums.txt"
      {"regional", SAC_EVENT_TYPE, IR},
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
#line 54 "enums.txt"
      {"t2", SAC_ZERO_TIME, IT2}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 167 "enums.txt"


#ifdef __TESTING__

int
main() {
       struct eid *e = NULL;
       char *itime = "itime";
       e = sac_enum_to_id(itime, strlen(itime));
       printf("%s %d %d\n", e->name, e->type, e->id);
       return 0;
}

#endif
