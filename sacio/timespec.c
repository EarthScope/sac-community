/**
 * @file
 * @brief Long duration, high precision timing
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <inttypes.h>

#include <errno.h>
#include <limits.h>

#include "timespec.h"
#include "strip.h"
#include "time64.h"


char * strptime64(const char *buf, const char *fmt, struct TM *tm, int64_t *ns);
size_t strftime64(char *dst, size_t n, const char *fmt, struct TM *tm, int64_t ns);

/**
 * @defgroup time time
 * @brief Long duration, high precision time manipulation
 */

/**
 * @private
 */
char *months_abbrev[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                         "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

/**
 * @brief Convert year-month-day to year-day-of-the-year
 * @private
 */
void ymd_from_yd(struct TM *tm);
/**
 * @brief Convert year-day-of-the-year to year-month-day
 * @private
 */
void yd_from_ymd(struct TM *tm);

static const char *fmts[] = {
    "%Y/%m/%dT%H:%M:%S.%fZ",
    "%Y/%m/%dT%H:%M:%S.%f",
    "%Y/%m/%dT%H:%M:%SZ",
    "%Y/%m/%dT%H:%M:%S",

    "%Y/%m/%d/%H:%M:%S.%fZ",
    "%Y/%m/%d/%H:%M:%S.%f",
    "%Y/%m/%d/%H:%M:%SZ",
    "%Y/%m/%d/%H:%M:%S",

    "%Y,%m,%dT%H:%M:%S.%fZ",
    "%Y,%m,%dT%H:%M:%S.%f",
    "%Y,%m,%dT%H:%M:%SZ",
    "%Y,%m,%dT%H:%M:%S",

    "%Y,%m,%d,%H:%M:%S.%fZ",
    "%Y,%m,%d,%H:%M:%S.%f",
    "%Y,%m,%d,%H:%M:%SZ",
    "%Y,%m,%d,%H:%M:%S",

    "%Y-%m-%dT%H:%M:%S.%fZ",
    "%Y-%m-%dT%H:%M:%S.%f",
    "%Y-%m-%dT%H:%M:%SZ",
    "%Y-%m-%dT%H:%M:%S",

    "%Y-%m-%d-%H:%M:%S.%fZ",
    "%Y-%m-%d-%H:%M:%S.%f",
    "%Y-%m-%d-%H:%M:%SZ",
    "%Y-%m-%d-%H:%M:%S",

    "%Y,%jT%H:%M:%S.%fZ",
    "%Y,%jT%H:%M:%S.%f",
    "%Y,%jT%H:%M:%SZ",
    "%Y,%jT%H:%M:%S",

    "%Y,%j,%H:%M:%S.%fZ",
    "%Y,%j,%H:%M:%S.%f",
    "%Y,%j,%H:%M:%SZ",
    "%Y,%j,%H:%M:%S",

    "%Y/%jT%H:%M:%S.%fZ",
    "%Y/%jT%H:%M:%S.%f",
    "%Y/%jT%H:%M:%SZ",
    "%Y/%jT%H:%M:%S",

    "%Y/%j/%H:%M:%S.%fZ",
    "%Y/%j/%H:%M:%S.%f",
    "%Y/%j/%H:%M:%SZ",
    "%Y/%j/%H:%M:%S",

    "%Y-%jT%H:%M:%S.%fZ",
    "%Y-%jT%H:%M:%S.%f",
    "%Y-%jT%H:%M:%SZ",
    "%Y-%jT%H:%M:%S",

    "%Y-%j-%H:%M:%S.%fZ",
    "%Y-%j-%H:%M:%S.%f",
    "%Y-%j-%H:%M:%SZ",
    "%Y-%j-%H:%M:%S",

    "%Y,%m,%d",
    "%Y-%m-%d",
    "%Y/%m/%d",

    "%Y-%j",
    "%Y,%j",
    "%Y/%j",
};

/**
 * @brief Get now as a timespec64 value
 *
 * @memberof timespec64
 * @ingroup time
 *
 * @return timepsec64 value filled with the current time
 *
 * @note Nanoseconds are always set to 0
 */
timespec64
timespec64_now() {
    timespec64 t = {0,0};
    t.tv_sec = time(NULL);
    t.tv_nsec = 0.0;
    return t;
}
/**
 * @brief Crate a timespec64 value from Year, Day of Year, Hour, Minute, Second, Nanosecond
 *
 * @memberof timespec64
 * @ingroup time
 *
 * @param year   Year
 * @param jday   Day of the year
 * @param hour   Hour
 * @param min    Minute
 * @param sec    Second
 * @param ns     Nanosecond
 *
 * @return new timespec64 value
 *
 */
timespec64
timespec64_from_yjhmsf(int64_t year, int jday, int hour, int min, int sec, int64_t ns) {
    struct TM tm = {0};
    timespec64 t = {0,0};
    while(ns < 0) {
        ns += 1000000000;
        sec -= 1;
    }
    while(ns >= 1000000000) {
        ns -= 1000000000;
        sec += 1;
    }

    tm.tm_sec  = sec;
    tm.tm_min  = min;
    tm.tm_hour = hour;
    tm.tm_year = year - 1900;
    tm.tm_yday = jday - 1;

    ymd_from_yd(&tm);
    t.tv_sec  = timegm64(&tm);
    t.tv_nsec = ns;
    return t;
}

/**
 * @brief Crate a timespec64 value from Year, Month, Day, Hour, Minute, Second, Nanosecond
 *
 * @memberof timespec64
 * @ingroup time
 *
 * @param year   Year
 * @param jday   Month
 * @param jday   Month Day
 * @param hour   Hour
 * @param min    Minute
 * @param sec    Second
 * @param ns     Nanosecond
 *
 * @return new timespec64 value
 *
 */
timespec64
timespec64_from_ymdhmsf(int64_t year, int month, int day, int hour, int min, int sec, int64_t ns) {
    struct TM tm = {0};
    timespec64 t = {0,0};
    while(ns < 0) {
        ns += 1000000000;
        sec -= 1;
    }
    while(ns >= 1000000000) {
        ns -= 1000000000;
        sec += 1;
    }
    tm.tm_sec  = sec;
    tm.tm_min  = min;
    tm.tm_hour = hour;
    tm.tm_year = year - 1900;
    tm.tm_mon  = month - 1;
    tm.tm_mday = day;
    t.tv_sec  = timegm64(&tm);
    t.tv_nsec = ns;
    return t;
}

/**
 * @brief Parse a string into a timespec64 value
 *
 * @memberof timespec64
 * @ingroup time
 *
 * @param buf   Input character string
 * @param t     timespec64 value to parse into
 *
 * @return 1 on success, 0 on failure
 */
int
timespec64_parse(const char *buf, timespec64 *t) {
    size_t i = 0;
    char *p = NULL;
    for(i = 0; i < sizeof(fmts) / sizeof(char *); i++) {
        if((p = strptime64t(buf, fmts[i], t)) && *p == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Print out a timespec64 value
 *
 * @memberof timespec64
 * @ingroup time
 * @param t  \ref timespec64
 *
 */
void
timespec64_print(timespec64 *t) {
    char dst[128] = {0};
    strftime64t(dst, sizeof(dst), "%FT%T.%3fZ", t);
    printf("%s\n", dst);
}

/**
 * @brief Add a duration to a timespec64
 *
 * @memberof timespec64
 * @ingroup time
 *
 * @param a  timespec64 value
 * @param d  duration to add to to timespec64
 *
 * @return new timespec64 value (a + d)
 */
timespec64
timespec64_add_duration(timespec64 a, duration *d) {
    struct TM tm;
    timespec64 b = { 0, 0 };
    if(!d) {
        return b;
    }
    gmtime64_r(&a.tv_sec, &tm);
    switch(d->type) {
    case Duration_None:      break;
    case Duration_Seconds:   tm.tm_sec    += d->n;       break;
    case Duration_Minutes:   tm.tm_min    += d->n;       break;
    case Duration_Hours:     tm.tm_hour   += d->n;       break;
    case Duration_Days:      tm.tm_mday   += d->n;       break;
    case Duration_Weeks:     tm.tm_mday   += d->n * 7;   break;
    case Duration_Months:    tm.tm_mon    += d->n;       break;
    case Duration_Years:     tm.tm_year   += d->n;       break;
    case Duration_Decades:   tm.tm_year   += d->n * 10;  break;
    case Duration_Centuries: tm.tm_year   += d->n * 100; break;
    }
    b.tv_sec  = timegm64(&tm);
    b.tv_nsec = a.tv_sec;
    return b;
}

/**
 * @brief Compare two timespec64 values
 *
 * @memberof timespec64
 * @ingroup time
 *
 * @param a  First timespec64 value
 * @param b  Second timespec64 value
 *
 * @return
 *     -  0 if a == b
 *     - -1 if a < b
 *     - +1 if a > b
 *
 */
int
timespec64_cmp(timespec64 *a, timespec64 *b) {
    if(a->tv_sec < b->tv_sec) {
        return -1;
    }
    if(a->tv_sec > b->tv_sec) {
        return 1;
    }
    if(a->tv_nsec < b->tv_nsec) {
        return -1;
    }
    if(a->tv_nsec > b->tv_nsec) {
        return 1;
    }
    return 0;
}



/**
 * @brief Parse an integer with digits between upper and lower, applying a scale
 * @private
 *
 * @ingroup time
 *
 * @param p     pointer to a character string
 * @param pval  output int64_t value
 * @param lower minimum number of digits in integer
 * @param upper maximum number of digits in integer
 * @param scale whether to apply a scale or not
 *
 * @return 1 on success, 0 on failure
 *
 * @note If a scale is applied the interpreted value is multipled by 10**(upper-n)
 *    where n is the number of digits in the parsed value
 */
static int
atoi64v(char **p, int64_t *pval, int lower, int upper, int scale) {
    int64_t val;
    int n;

    if (**p < '0' || **p > '9') {
        return 0;
    }
    n = 0;
    val = 0;
    do {
        val *= 10;
        val += **p - '0';
        (*p)++;
        n++;
    } while (**p != 0 && **p >= '0' && **p <= '9' && n < upper);
    if (n < lower) {
        return 0;
    }
    if(!scale) {
        *pval = val;
    } else {
        *pval = val * pow(10, upper-n);
    }
    return 1;
}
/**
 * @brief Parse an integer
 *
 * @private
 * @ingroup time
 *
 * @param p     pointer to a character string
 * @param pval  output int64_t value
 * @param lower minimum number of digits in integer
 * @param upper maximum number of digits in integer
 *
 * @return 1 on success, 0 on failure
 *
 */
static int
atoi64(char **p, int64_t *pval, int lower, int upper) {
    return atoi64v(p, pval, lower, upper, 0);
}


/** \cond NO_DOCS */
#define YEAR_SET 1<<0 /**< @brief year bit @private */
#define YDAY_SET 1<<1 /**< @brief day of the year bit @private */
#define MON_SET  1<<2 /**< @brief month bit @private */
#define MDAY_SET 1<<3 /**< @brief month day bit @private */
#define HOUR_SET 1<<4 /**< @brief hour bit @private */
#define MIN_SET  1<<5 /**< @brief minute bit @private */
#define SEC_SET  1<<6 /**< @brief second bit @private */
/** \endcond */

/**
 * @brief Parse a string into a TM value
 *
 * @private
 * @ingroup  time
 *
 * @details Parse a timespec64 according to the format specifiers
 *          - `%%` -- `%` character
 *          - `%%Y` -- Year
 *          - `%%j` -- Day of the year, 1-366
 *          - `%%m` -- Month number, 1-12
 *          - `%%d` -- Day of the month, 1-31
 *          - `%%H` -- Hour, 0-23
 *          - `%%M` -- Minute, 0-59
 *          - `%%S` -- Seconds, 0-60
 *          - `%%f` -- partial seconds as nanoseconds
 *
 * @param buf   Input character string
 * @param fmt   Format to interpret the charater string
 * @param tm    struct TM to place values into
 * @param ns    output for nanoseconds
 *
 * @return first character not read based on the format during success,
 *    NULL on failure
 *
 */
char *
strptime64(const char *buf, const char *fmt, struct TM *tm, int64_t *ns) {
    int set = 0;
    unsigned char c;
    int64_t v = 0;
    char *b = NULL;
    if(!buf || !fmt) {
        return NULL;
    }
    b = (char *) buf;
    while((c = *fmt) != 0) {
        if(isspace(c)) {
            while(isspace(*b)) {
                b++;
            }
            fmt++;
            continue;
        }
        if((c = *fmt++) != '%') {
            if(c != *b++) {
                return NULL;
            }
            continue;
        }
        switch(c = *fmt++) {
        case '%':
            if(c != *b++) {
                return NULL;
            }
            break;
        case 'f':
            if(!atoi64v(&b, &v, 1,9, 1)) { return NULL; }
            *ns = v;
            break;
        case 'Y':
            if(!atoi64(&b, &v, 1, 4)) { return NULL; }
            tm->tm_year = v - 1900;
            set |= YEAR_SET;
            break;
        case 'j':
            if(!atoi64(&b, &v, 1, 3)) { return NULL; }
            tm->tm_yday = v - 1 ;
            set |= YDAY_SET;
            break;
        case 'm':
            if(!atoi64(&b, &v, 1, 2)) { return NULL; }
            tm->tm_mon = v - 1;
            set |= MON_SET;
            break;
        case 'd':
            if(!atoi64(&b, &v, 1, 2)) { return NULL; }
            tm->tm_mday = v ;
            set |= MDAY_SET;
            break;
        case 'H':
            if(!atoi64(&b, &v, 1, 2)) { return NULL; }
            tm->tm_hour = v ;
            set |= HOUR_SET;
            break;
        case 'M':
            if(!atoi64(&b, &v, 1, 2)) { return NULL; }
            tm->tm_min = v ;
            set |= MIN_SET;
            break;
        case 'S':
            if(!atoi64(&b, &v, 1, 2)) { return NULL; }
            tm->tm_sec = v ;
            set |= SEC_SET;
            break;
        default:
            return NULL;
            break;
        }
    }
    if(set & YEAR_SET) {
        if(!(set & YDAY_SET) && (set & MON_SET) && (set & MDAY_SET)) {
            yd_from_ymd(tm);
            set |= YDAY_SET;
        }
        if(set & YDAY_SET) {
            ymd_from_yd(tm);
            set |= MON_SET;
            set |= MDAY_SET;
        }
        if(!(set & MON_SET) && !(set & MDAY_SET) && !(set & YDAY_SET)) {
            // No Date Set
            return NULL;
        }
    } else {
        return NULL;
    }
    return b;
}

/**
 * @brief Parse a string into a timespec64 value
 *
 * @memberof timespec64
 * @ingroup  time
 *
 * @details Parse a timespec64 according to the format specifiers
 *          - `%%` -- `%` characters
 *          - `%%Y` -- Year
 *          - `%%j` -- Day of the year, 1-366
 *          - `%%m` -- Month number, 1-12
 *          - `%%d` -- Day of the month, 1-31
 *          - `%%H` -- Hour, 0-23
 *          - `%%M` -- Minute, 0-59
 *          - `%%S` -- Seconds, 0-60
 *          - `%%f` -- partial seconds as nanoseconds
 *
 * @param buf  Input character string
 * @param fmt  Format to interpret the charater string
 * @param t    output timespec64 value to place values into
 *
 * @return first character not read based on format during success,
 *    NULL on failure
 *
 */
char *
strptime64t(const char *buf, const char *fmt, timespec64 *t) {
    char *p = NULL;
    struct TM tm = {0};
    if(!(p = strptime64(buf, fmt, &tm, &t->tv_nsec))) {
        return NULL;
    }
    t->tv_sec = timegm64(&tm);
    return p;
}

/**
 * @brief Take the power of an interger to an integer
 *
 * @private
 *
 * @param a   Base integer
 * @param b   Exponent integer
 *
 * @return a**b
 *
 */
static int64_t
powi(int64_t a, int64_t b) {
    int i = 0;
    int64_t v = 1;
    if(b == 0) {
        return 1;
    }
    if(b < 0) {
        return 1;
    }
    for(i = 0; i < labs(b); i++) {
        v *= a;
    }
    return v;
}

/**
 * @brief Format a struct TM value
 *
 * @private
 * @ingroup  time
 *
 * @details Format a timespec64 according to the format specifiers
 *          - `%%` -- `%` character
 *          - `%%Y` -- Year, 4 digits, zero padded
 *          - `%%j` -- Day of the year, 3 digits, zero padded
 *          - `%%m` -- Month number, 2 digits, zero padded
 *          - `%%d` -- Day of the month, 2 digits, zero padded
 *          - `%%H` -- Hour, 2 digits, zero padded
 *          - `%%M` -- Minute, 2 digits, zero padded
 *          - `%%S` -- Seconds, 2 digits, zero padded
 *          - `%%f` -- partial seconds as nanoseconds, prefix digit defines
 *                    the number of leading digits to show
 *          - `%%F` -- alias for `%Y-%m-%d`
 *          - `%%T` -- alias for `%H:%M:%S`
 *          - `%%b` -- Abbreviated Month Name
 *
 * @param dst   Output character string
 * @param n     Length of dst
 * @param fmt   Format for character string
 * @param tm    input struct TM
 * @param ns    input nanoseconds 
 *
 * @return length of output character string, 0 on failure
 *
 */
size_t
strftime64(char *dst, size_t n, const char *fmt, struct TM *tm, int64_t ns) {
    int len = 0;
    size_t i = 0;
    unsigned char c = 0;

    if(!dst || !fmt || n == 0) {
        return 0;
    }
    memset(dst, 0, n);
    while((c = *fmt) != 0) {
        len = 0;
        // Regular Character
        if((c = *fmt++) != '%') {
            dst[i++] = c;
            continue;
        }
        // Format Character
        while(*fmt >= '0' && *fmt <= '9') {
            len = (len * 10) + *fmt - '0';
            fmt++;
        }
        switch(c = *fmt++) {
        case '%':
            dst[i++] = c;
            break;
        case 'Y':
            snprintf(dst+i, n-i, "%04" PRId64, tm->tm_year + 1900);
            break;
        case 'f':
            if(len == 0) {
                snprintf(dst+i, n-i, "%09" PRId64, ns);
            } else {
                snprintf(dst+i, n-i, "%0*" PRId64, len, ns/powi(10, 9-len));
            }
            break;
        case 'j':
            snprintf(dst+i, n-i, "%03d", tm->tm_yday + 1);
            break;
        case 'm':
            snprintf(dst+i, n-i, "%02d", tm->tm_mon + 1);
            break;
        case 'd':
            snprintf(dst+i, n-i, "%02d", tm->tm_mday);
            break;
        case 'H':
            snprintf(dst+i, n-i, "%02d", tm->tm_hour);
            break;
        case 'M':
            snprintf(dst+i, n-i, "%02d", tm->tm_min);
            break;
        case 'S':
            snprintf(dst+i, n-i, "%02d", tm->tm_sec);
            break;
        case 'F':
            strftime64(dst+i, n-i, "%Y-%m-%d", tm, ns);
            break;
        case 'T':
            strftime64(dst+i, n-i, "%H:%M:%S", tm, ns);
            break;
        case 'b':
            i = sacio_strlcat(dst, months_abbrev[tm->tm_mon], n);
            break;
        }
        i = strlen(dst);
    }
    dst[i] = 0;
    return i;
}

/**
 * @brief Format a struct TM value
 *
 * @memberof timespec64
 * @ingroup  time
 *
 * @details Format a timespec64 according to the format specifiers
 *          - `%%` -- `%` character
 *          - `%%Y` -- Year, 4 digits, zero padded
 *          - `%%j` -- Day of the year, 3 digits, zero padded
 *          - `%%m` -- Month number, 2 digits, zero padded
 *          - `%%d` -- Day of the month, 2 digits, zero padded
 *          - `%%H` -- Hour, 2 digits, zero padded
 *          - `%%M` -- Minute, 2 digits, zero padded
 *          - `%%S` -- Seconds, 2 digits, zero padded
 *          - `%%f` -- partial seconds as nanoseconds, prefix digit defines
 *                    the number of leading digits to show
 *          - `%%F` -- alias for `%%Y-%%m-%%d`
 *          - `%%T` -- alias for `%%H:%%M:%%S`
 *          - `%%b` -- Abbreviated Month Name
 *
 * @param dst   Output character string
 * @param n     Length of dst
 * @param fmt   Format for character string
 * @param t     input timespec64 value
 *
 * @return length of output character string, 0 on failure
 *
 */

size_t
strftime64t(char *dst, size_t n, const char *fmt, timespec64 *t) {
    struct TM tm;
    gmtime64_r(&t->tv_sec, &tm);
    return strftime64(dst, n, fmt, &tm, t->tv_nsec);
}

/**
 * @brief Create a new duration
 *
 * @memberof duration
 *
 * @return new duration
 *
 * @warning User owns the duration and is responsibile for freeing the
 *    underlying memory
 */
duration *
duration_new() {
    duration *d = calloc(1, sizeof(duration));
    duration_init(d);
    return d;
}

/**
 * @brief Initialize a duration
 *
 * @memberof duration
 * @ingroup time
 *
 * @param d  duration to initialize
 *
 */
void
duration_init(duration *d) {
    d->type = Duration_None;
    d->n    = 0;
}

/**
 * @brief Parse a string into a duration
 *
 * @memberof duration
 * @ingroup time
 *
 * @param in  character string to parse
 * @param d   duration output from string
 *
 * @return 1 on success, 0 in failure
 *
 * Format for a duration is `[-+]###unit` where:
 *    - `###` is a integer
 *    - `unit` is one of the following
 *      - s, sec, secs, seconds
 *      - m, min, mins, minutes
 *      - h, hrs, hours
 *      - d, days
 *      - w, wk, wks, weeks
 *      - mon, months
 *      - y, yr, yrs, years
 *      - dec, decades
 *      - cent, centuries
 *    - durations are allowed to be positive or negative
 */
int
duration_parse(char *in, duration *d) {
    size_t i = 0;
    char *p = in;
    int64_t n = 0;
    char *endptr = NULL;
    if(!d) {
        return 0;
    }
    n = strtoll(p, &endptr, 10);
    if(endptr == p ||
       ((n == LLONG_MIN || n == LLONG_MAX) && errno == ERANGE)) {
        return 0;
    }
    p = endptr;
    if(!p) {
        return 0;
    }
    char *key[] = {"s", "sec", "secs",  "seconds",
                   "m", "min", "mins" , "minutes",
                   "d",  "days",
                   "h", "hrs", "hours",
                   "w" , "wk" ,"wks" ,"weeks",
                   "mon", "months",
                   "y", "yr", "yrs", "years",
                   "dec", "decades",
                   "cent", "centuries"
    };
    TimeType T[] = {Duration_Seconds, Duration_Seconds,
                    Duration_Seconds, Duration_Seconds,
                    Duration_Minutes, Duration_Minutes,
                    Duration_Minutes, Duration_Minutes,
                    Duration_Days, Duration_Days,
                    Duration_Hours, Duration_Hours, Duration_Hours,
                    Duration_Weeks, Duration_Weeks, Duration_Weeks, Duration_Weeks,
                    Duration_Months, Duration_Months,
                    Duration_Years, Duration_Years, Duration_Years, Duration_Years,
                    Duration_Decades, Duration_Decades,
                    Duration_Centuries, Duration_Centuries};

    size_t m = strlen(p);
    size_t nkeys =  sizeof(key)/sizeof(char*);
    for(i = 0; i < nkeys; i++) {
        if(strncasecmp(p, key[i], m) == 0) {
            d->n = n;
            d->type = T[i];
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Create a duration from a character stirng
 *
 * @memberof duration
 * @ingroup time
 *
 * @param in character string to parse into a duration
 *
 * @return parsed duration, or NULL on error
 *
 * @warning User owns the duration and is responsibile for freeing the 
 *     underlying memory
 */
duration *
duration_from_string(char *in) {
    duration *d = NULL;
    if(!(d = duration_new())) {
        return NULL;
    }
    if(!duration_parse(in, d)) {
        free(d);
        d = NULL;
    }
    return d;
}


#ifdef _TEST_

int
main() {
    char dst[128] = {0};
    timespec64 t = {0};
    timespec64_parse("1994/160",&t);
    strftime64t(dst, sizeof(dst), "%FT%T.%3fZ", &t);
    printf("%s\n", dst);
    return 0;
}

#endif
