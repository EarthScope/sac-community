
#ifndef _TIMESPEC_H_
#define _TIMESPEC_H_

#include <stdint.h>
#include <time.h>
//#include "time64.h"

typedef struct timespec64 timespec64;
typedef struct duration duration;

duration * duration_new();
void       duration_init(duration *d);
duration * duration_from_string(char *in);
int        duration_parse(char *in, duration *d);

timespec64 timespec64_add_duration(timespec64 a, duration *d);
timespec64 timespec64_now();
timespec64 timespec64_undef();
int        timespec64_cmp(timespec64 *a, timespec64 *b);
int        timespec64_parse(const char *buf, timespec64 *t);
void       timespec64_print(timespec64 *t);

size_t strftime64t(char *dst, size_t n, const char *fmt, timespec64 *t);
char * strptime64t(const char *buf, const char *fmt, timespec64 *t);

timespec64 timespec64_from_yjhmsf(int64_t year, int jday,
                                  int hour, int min, int sec, int64_t ns);
timespec64 timespec64_from_ymdhmsf(int64_t year, int month, int day,
                                   int hour, int min, int sec, int64_t ns);
void timespec64_to_ymd(timespec64 *t, int64_t *year, int *month, int *day, int *oday);

/**
 * @brief High-precision, long duration time value
 * @ingroup time
 */
struct timespec64 {
    int64_t tv_sec; /**< Number of seconds, epoch 1970/1/1 00:00:00 */
    int64_t tv_nsec; /**< Number of nanoseconds */
};


typedef enum TimeType TimeType;
/**
 * @brief duration units
 * @ingroup time
 *
 */
enum TimeType {
    Duration_None    = 0,
    Duration_Seconds = 1,
    Duration_Minutes = 2,
    Duration_Hours   = 3,
    Duration_Days    = 4,
    Duration_Weeks   = 5,
    Duration_Months  = 6,
    Duration_Years   = 7,
    Duration_Decades = 8,
    Duration_Centuries = 9,
};
/**
 * @brief time duration with units
 * @ingroup time
 *
 */
struct duration {
    TimeType type; /**<  duration unit */
    int64_t n;     /**<  duration length */
};



#endif /* _TIMESPEC_H_ */
