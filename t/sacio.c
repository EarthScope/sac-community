#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "unit.h"
#include "hdr.h"
#include "dff.h"

#include "sacio_test.h"

/* tests used here */

void sac_error_stdout();
void sac_warning_stdout();

int
file_exists(char *file) {
  struct stat sb;
  return stat(file, &sb) == 0;
}

void
file_check(char *file, int size, char *name) {
  struct stat sb;
  ok(stat(file, &sb) == 0, "checking for existance of file <%s>", file);
  ok(sb.st_size == size, "checking for size of file <%s> <%s> "
     "value %d expected %d",
     name, file, (int) sb.st_size, (int) size);
}

void
file_check_not_ok(char *file, int size, char *name) {
  struct stat sb;
  ok(stat(file, &sb) == 0, "checking for existance of file <%s>", file);
  ok(sb.st_size != size, "checking for size of file <%s> <%s> "
     "value %d expected %d",
     name, file, sb.st_size, size);
}

void
float_check(float a, float b, char *name) {
  ok( fabs(a - b) < 1e-4,  "%s value (value %f (%e), expected %f (%e))",  
      name, a, a, b, b);
}
void
float_undef(float a, char *name) {
  float_check(a, SAC_FLOAT_UNDEFINED, name);
}

void
int_check(int a, int b, char *name) {
  ok(a == b,  "%s value (value %d, expected %d)", name, a, b);
}

void
int_undef(int a, char *name) {
  int_check(a, SAC_INT_UNDEFINED, name);
}

void
char_check(char *a, char *b, char *name) {
  ok(strncmp(a, b, 8) == 0, 
     "%s value (value <%s>[%d], expected <%s>[%d])", 
     name, a, strlen(a), b, strlen(b));  
}
void
char_check16(char *a, char *b, char *name) {
  ok(strncmp(a, b, 16) == 0, 
     "%s value (value <%s>[%d], expected <%s>[%d])", 
     name, a, strlen(a), b, strlen(b));  
}

void
char_undef(char *a, char *name) {
  char_check(a, SAC_CHAR_UNDEFINED, name);
}

int
main(int argc, char *argv[])
{
  ok(1 == 1, "initial test");
  sacio_initialize_common();

  sac_error_stdout();
  sac_warning_stdout();
  
  if (argc == 1) {
    test_rsach();
    test_rsac1();
    test_rsac2();
    test_gethv();
    test_sethv();
    test_wsac0();
    test_wsac1();
    test_wsac2();
    test_wsac3();
  } else {
    int i;
    for (i = 1; i < argc; i++) {
      char *item = argv[i];
      if (strcmp (item, "rsach") == 0) {
        test_rsach();
      }
    }
  }
  
  TEST_FINISH;
}


