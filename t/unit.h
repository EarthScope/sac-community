
#ifndef __UNIT_H__
#define __UNIT_H__

#include <stdio.h>
#include <stdarg.h>

void ok(int value, char *fmt, ...);
void not_ok(int value, char *fmt, ...);
long long int number_failed();
void report();
void set_show_checks ();

#define TEST_SUCCESS  0
#define TEST_FAILURE -1

#define TEST_FINISH \
    do { \
	report(); \
	return (number_failed() == 0) ? TEST_SUCCESS : TEST_FAILURE; \
    } while (0)

#endif /* __UNIT_H__ */
