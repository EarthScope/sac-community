
#include <stdio.h>
#include <stdarg.h>

#include "unit.h"

static long long int ntests        = 0;
static long long int tests_failed  = 0;
static long long int tests_succeed = 0;
static long long int show_checks = 0;

long long int
number_failed() {
  return tests_failed;
}

void
set_show_checks ()
{
    show_checks = 1;
}

void
report() {
  fprintf(stderr, "Failures: %lld / %lld [ %5.1f %% ]\n", 
	  tests_failed, ntests, 100.0 * tests_failed / ntests);
  fprintf(stderr, "Success:  %lld / %lld [ %5.1f %% ]\n", 
	  tests_succeed, ntests, 100.0 * tests_succeed / ntests);
}

void
ok(int value, char *fmt, ...) {
  va_list ap;

  ntests++;

  if (show_checks)
    {
      fprintf (stderr, "Test[%lld]: CHECK ", ntests);
      va_start(ap, fmt);
      vfprintf(stderr, fmt, ap);
      va_end(ap);
      fprintf(stderr, "\n");
    }

  if(!value) {
    tests_failed ++;
    fprintf(stderr, "Test[%lld]: FAIL ", ntests);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
  } else {
    tests_succeed ++;
  }
}

void 
not_ok(int value, char *fmt, ...) {
  va_list ap;
  
  ntests++;
  if(show_checks) {
    fprintf(stderr, "Test[%lld]: CHECK (Should fail) ", ntests);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
  }
  if(value) {
    tests_failed++;
    fprintf(stderr, "Test[%lld]: PASS (Should fail) ", ntests);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
  } else {
    fprintf(stderr, "Test[%lld]: FAIL (Should fail) ", ntests);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    tests_succeed ++;
  }
}
