
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <timespec.h>

struct year_sec {
    int64_t year;
    int64_t sec;
    char *yd;
};

int
main() {
    size_t i = 0;
    char dst[128] = {0};
    timespec64 t = {0,0};
    t = timespec64_from_yjhmsf(1970, 1, 0, 0, 0, 0);
    assert(t.tv_sec == 0);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1970, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec == 0);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1970, 1, 0, 0, -1, 0);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1970, 1, 1, 0, 0, -1, 0);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1970, 1, 0, 0, 1, -1);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec ==  0);
    assert(t.tv_nsec == 999999999);
    t = timespec64_from_ymdhmsf(1970, 1, 1, 0, 0, 1, -1);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec ==  0);
    assert(t.tv_nsec == 999999999);

    t = timespec64_from_yjhmsf(1970, 1, 0, 0, 0, -1);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 999999999);
    strftime64t(dst, sizeof(dst), "%FT%T.%3fZ", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1969-12-31T23:59:59.999Z") == 0);

    t = timespec64_from_ymdhmsf(1970, 1, 1, 0, 0, 0, -1);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 999999999);
    strftime64t(dst, sizeof(dst), "%FT%T.%3fZ", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1969-12-31T23:59:59.999Z") == 0);

    t = timespec64_from_yjhmsf(1971, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  1*365*24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1971, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  1*365*24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1972, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  2*365*24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1972, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  2*365*24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1973, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  3*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1973, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  3*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1974, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  4*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1974, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  4*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1975, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  5*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1975, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  5*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1976, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  6*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1976, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  6*365*24*60*60 + 24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(1977, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  7*365*24*60*60 + 2*24*60*60);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(1977, 1, 1, 0, 0, 0, 0);
    assert(t.tv_sec ==  7*365*24*60*60 + 2*24*60*60);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(2001, 252, 1, 46, 40, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  1000000000);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(2001, 9,9, 1, 46, 40, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  1000000000);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(2005, 77, 1, 58, 31, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  1111111111);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(2005, 3,18, 1, 58, 31, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  1111111111);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(2009, 44, 23, 31, 30, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  1234567890);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(2009, 2,13, 23, 31, 30, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  1234567890);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(2033, 138, 3, 33, 20, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  2000000000);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(2033, 5, 18, 3, 33, 20, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  2000000000);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(2038, 19, 3, 14, 7, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  2147483647);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(2038, 1, 19, 3, 14, 7, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  2147483647);
    assert(t.tv_nsec == 0);

    t = timespec64_from_yjhmsf(2038, 19, 3, 14, 8, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  2147483648);
    assert(t.tv_nsec == 0);
    t = timespec64_from_ymdhmsf(2038, 1, 19, 3, 14, 8, 0);
    //printf("%lld\n", t.tv_sec);
    assert(t.tv_sec ==  2147483648);
    assert(t.tv_nsec == 0);

    struct year_sec ys[] = {
        {1900,  -2208988800, "1900-01"},
        {1800,  -5364662400, "1800-01"},
        {1700,  -8520336000, "1700-01"},
        {1600, -11676096000, "1600-01"},
        {1000, -30610224000, "1000-01"},
        {100,  -59011459200,  "100-01"},
        {1,    -62135596800,    "1-01"},
        {2050,   2524608000, "2050-01"},
        {2100,   4102444800, "2100-01"},
        {3000,  32503680000, "3000-01"},
    };
    for(i = 0; i < sizeof(ys)/sizeof(struct year_sec); i++) {
        int64_t year = ys[i].year;
        int64_t sec = ys[i].sec;
        t = timespec64_from_yjhmsf(year, 1, 0, 0, 0, 0);
        //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
        assert(t.tv_sec ==  sec);
        assert(t.tv_nsec == 0);
        t = timespec64_from_ymdhmsf(year, 1, 1, 0, 0, 0, 0);
        //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
        assert(t.tv_sec ==  sec);
        assert(t.tv_nsec == 0);
    }

    char *p = NULL;
    p = strptime64t("1970-1", "%Y-%j", &t);
    assert(p != NULL);
    assert(*p == 0);
    assert(t.tv_sec == 0);
    assert(t.tv_nsec == 0);

    p = strptime64t("1970-1T00:00:01", "%Y-%jT%H:%M:%S", &t);
    assert(p != NULL);
    assert(*p == 0);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 0);

    p = strptime64t("1970-1T00:00:01.1", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 100000000);

    p = strptime64t("1970-1T00:00:01.12", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 120000000);

    p = strptime64t("1970-1T00:00:01.123", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 123000000);

    p = strptime64t("1970-1T00:00:01.01", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 10000000);

    p = strptime64t("1970-1T00:00:01.001", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 1000000);

    p = strptime64t("1970-1T00:00:01.0001", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 100000);

    p = strptime64t("1970-1T00:00:01.00000001", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 10);

    p = strptime64t("1970-1T00:00:01.000000001", "%Y-%jT%H:%M:%S.%f", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 1);

    // Nanoseconds is only up to 9 digits
    p = strptime64t("1970-1T00:00:01.0000000001", "%Y-%jT%H:%M:%S.%f1", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 1);
    assert(t.tv_nsec == 0);

    p = strptime64t("1969-12-31T23:59:59.999Z", "%Y-%m-%dT%H:%M:%S.%fZ", &t);
    assert(p != NULL);
    assert(*p == 0);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 999000000);

    p = strptime64t("1970", "%Y", &t);
    assert(p == NULL);
    p = strptime64t("1971-1", "%Y-%j", &t);
    assert(t.tv_sec == 365*24*60*60);
    p = strptime64t("1972-1", "%Y-%j", &t);
    assert(t.tv_sec == 2*365*24*60*60);
    p = strptime64t("1973-1", "%Y-%j", &t);
    assert(t.tv_sec == (3*365 + 1) * 24*60*60);
    p = strptime64t("1974-1", "%Y-%j", &t);
    assert(t.tv_sec == (4*365 + 1) * 24*60*60);
    p = strptime64t("1975-1", "%Y-%j", &t);
    assert(t.tv_sec == (5*365 + 1) * 24*60*60);
    p = strptime64t("1976-1", "%Y-%j", &t);
    assert(t.tv_sec == (6*365 + 1) * 24*60*60);
    p = strptime64t("1977-1", "%Y-%j", &t);
    assert(t.tv_sec == (7*365 + 2) * 24*60*60);


    t.tv_sec = 0;
    t.tv_nsec = 0;
    p = strptime64t("1970 2 hi there end of times", "%Y %j", &t);
    //printf("%p\n", p);
    //printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    assert(t.tv_sec == 24*60*60);
    assert(t.tv_nsec == 0);
    assert(strcmp(p, " hi there end of times") == 0);

    p = strptime64t("2001 252 1 46 40 000", "%Y %j %H %M %S %f", &t);
    assert(t.tv_sec == 1000000000);
    assert(t.tv_nsec == 0);

    p = strptime64t("2001 09 09 1 46 40 000", "%Y %m %d %H %M %S %f", &t);
    assert(t.tv_sec == 1000000000);
    assert(t.tv_nsec == 0);

    p = strptime64t("2005 077 1 58 31 000", "%Y %j %H %M %S %f", &t);
    assert(t.tv_sec == 1111111111);
    assert(t.tv_nsec == 0);

    p = strptime64t("2005 3 18 1 58 31 000", "%Y %m %d %H %M %S %f", &t);
    assert(t.tv_sec == 1111111111);
    assert(t.tv_nsec == 0);

    p = strptime64t("2009 44 23 31 30 000", "%Y %j %H %M %S %f", &t);
    assert(t.tv_sec == 1234567890);
    assert(t.tv_nsec == 0);

    p = strptime64t("2009 02 13 23 31 30 000", "%Y %m %d %H %M %S %f", &t);
    //printf("%lld %lld %p\n", t.tv_sec, t.tv_nsec, p);
    assert(t.tv_sec == 1234567890);
    assert(t.tv_nsec == 0);

    p = strptime64t("2033 138 3 33 20 000", "%Y %j %H %M %S %f", &t);
    assert(t.tv_sec == 2000000000);
    assert(t.tv_nsec == 0);

    p = strptime64t("2033 5 18 3 33 20 000", "%Y %m %d %H %M %S %f", &t);
    //printf("%lld %lld %p\n", t.tv_sec, t.tv_nsec, p);
    assert(t.tv_sec == 2000000000);
    assert(t.tv_nsec == 0);

    p = strptime64t("2038 19 3 14 7 000", "%Y %j %H %M %S %f", &t);
    assert(t.tv_sec == 2147483647);
    assert(t.tv_nsec == 0);

    p = strptime64t("2038 1 19 3 14 7 000", "%Y %m %d %H %M %S %f", &t);
    //printf("%lld %lld %p\n", t.tv_sec, t.tv_nsec, p);
    assert(t.tv_sec == 2147483647);
    assert(t.tv_nsec == 0);

    p = strptime64t("2038 19 3 14 8 000", "%Y %j %H %M %S %f", &t);
    assert(t.tv_sec == 2147483648);
    assert(t.tv_nsec == 0);

    p = strptime64t("2038 1 19 3 14 8 000", "%Y %m %d %H %M %S %f", &t);
    //printf("%lld %lld %p\n", t.tv_sec, t.tv_nsec, p);
    assert(t.tv_sec == 2147483648);
    assert(t.tv_nsec == 0);

    for(i = 0; i < sizeof(ys)/sizeof(struct year_sec); i++) {
        p = strptime64t(ys[i].yd, "%Y-%j", &t);
        assert(t.tv_sec == ys[i].sec);
        assert(t.tv_nsec == 0);
    }

    timespec64 a = { 1,0};
    timespec64 b = {-1,0};
    assert(timespec64_cmp(&a,&b) > 0);
    assert(timespec64_cmp(&a,&a) == 0);
    assert(timespec64_cmp(&b,&a) < 0);

    a.tv_sec = 1;
    a.tv_nsec = 2;
    b.tv_sec = a.tv_sec;
    a.tv_nsec = 1;
    assert(timespec64_cmp(&a,&b) > 0);
    assert(timespec64_cmp(&a,&a) == 0);
    assert(timespec64_cmp(&b,&a) < 0);

    t = timespec64_from_yjhmsf(1970, 1, 0, 0, -1, 0);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 0);

    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1969 365 12 31 23 59 59 000 000000 000000000 000000000 DEC 1969-12-31 23:59:59") == 0);

    t = timespec64_from_yjhmsf(1970, 1, 0, 0, 0, -1);
    assert(t.tv_sec == -1);
    assert(t.tv_nsec == 999999999);

    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1969 365 12 31 23 59 59 999 999999 999999999 999999999 DEC 1969-12-31 23:59:59") == 0);

    t.tv_sec = -62135596800;
    t.tv_nsec = 0;

    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "0001 001 01 01 00 00 00 000 000000 000000000 000000000 JAN 0001-01-01 00:00:00") == 0);

    strptime64t("1996-02-29", "%Y-%m-%d", &t);
    assert(t.tv_sec == 825552000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1996 060 02 29 00 00 00 000 000000 000000000 000000000 FEB 1996-02-29 00:00:00") == 0);

    strptime64t("1995-02-29", "%Y-%m-%d", &t);
    assert(t.tv_sec == 794016000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1995 060 03 01 00 00 00 000 000000 000000000 000000000 MAR 1995-03-01 00:00:00") == 0);

    strptime64t("1995-03-01", "%Y-%m-%d", &t);
    assert(t.tv_sec == 794016000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T %%", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1995 060 03 01 00 00 00 000 000000 000000000 000000000 MAR 1995-03-01 00:00:00 %") == 0);

    timespec64_parse("1995-03-01", &t);
    assert(t.tv_sec == 794016000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T %%", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1995 060 03 01 00 00 00 000 000000 000000000 000000000 MAR 1995-03-01 00:00:00 %") == 0);

    timespec64_parse("1995-02-29", &t);
    assert(t.tv_sec == 794016000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T %%", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1995 060 03 01 00 00 00 000 000000 000000000 000000000 MAR 1995-03-01 00:00:00 %") == 0);

    timespec64_parse("1996-02-29", &t);
    assert(t.tv_sec == 825552000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1996 060 02 29 00 00 00 000 000000 000000000 000000000 FEB 1996-02-29 00:00:00") == 0);

    timespec64_parse("199660000", &t);
    assert(t.tv_sec == 825552000);
    strftime64t(dst, sizeof(dst), "%Y %j %m %d %H %M %S %3f %6f %9f %f %b %F %T", &t);
    //printf("%s\n", dst);
    assert(strcmp(dst, "1996 060 02 29 00 00 00 000 000000 000000000 000000000 FEB 1996-02-29 00:00:00") == 0);


    return 0;
}
