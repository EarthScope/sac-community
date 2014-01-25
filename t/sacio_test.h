
#include "errors.h"

int  file_exists  (char *file);
void file_check   (char *file, int size, char *name);
void file_check_not_ok(char *file, int size, char *name);
void float_check  (float a, float b, char *name);
void float_undef  (float a, char *name);
void int_check    (int a,   int   b, char *name);
void int_undef    (int a,   char *name);
void char_check   (char *a, char *b, char *name);
void char_check16 (char *a, char *b, char *name);
void char_undef   (char *a, char *name);
void test_rsac1_header_file();
void test_rsac1();
void test_rsach();
void test_rsac2();
void test_gethv();
void test_sethv();
void test_wsac0();
void test_wsac1();
void test_wsac2();
void test_wsac3();

#define FILE_UNKNOWN      "file-unknown"
#define FILE_SMALL        TOP_SRCDIR "/t/test_io_small.sac"
#define FILE_BIG          TOP_SRCDIR "/t/test_io_big.sac"
#define FILE_SMALL_SPEC   TOP_SRCDIR "/t/test_spec_small.sac"
#define FILE_BIG_SPEC     TOP_SRCDIR "/t/test_spec_big.sac"
#define FILE_SMALL_UNEVEN TOP_SRCDIR "/t/test_uneven_small.sac"
#define FILE_BIG_UNEVEN   TOP_SRCDIR "/t/test_uneven_big.sac"
#define LONG_TEXT    "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."

#define FILE_WRITE        "test_write.sac"

/*
  Things which are odd
     - rsac2 will not read spectral files
     - rsac2 will not read ixy files 
     - rsac2 will truncate a file and set the nlen and e values to 
           the truncated length
     - rsac1 will truncate a file but not set nlen and e values to
           the truncated length
     - wsac0 does not explicitly test for length less than 0.0
           and the half written file is not cleared
     - wsac0 does not write spectral files
     - wsac0 does not write ixy files
 */
#define SAC_HEADER_FLOATS 70
#define SAC_HEADER_INTS   40
#define SAC_HEADER_CHARS  24
#define SAC_HEADER_FLOAT_SIZE sizeof(float)
#define SAC_HEADER_INT_SIZE   sizeof(int)
#define SAC_HEADER_CHAR_SIZE  8
#define SAC_HEADER_SIZE ( (SAC_HEADER_FLOATS * SAC_HEADER_FLOAT_SIZE) + \
			  (SAC_HEADER_INTS * SAC_HEADER_INT_SIZE) +     \
			  (SAC_HEADER_CHARS * SAC_HEADER_CHAR_SIZE) )
