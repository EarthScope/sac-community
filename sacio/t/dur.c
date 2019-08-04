
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "timespec.h"

struct DurationTest {
    char *txt;
    TimeType type;
    int64_t n;
};

int
main() {
    size_t i;
    struct DurationTest test[] =  {
        {"+10m", Duration_Minutes, 10},
        {"-10m", Duration_Minutes, -10},
        {"+1m", Duration_Minutes, 1},
        {"-1m", Duration_Minutes, -1},
        {"-0001m", Duration_Minutes, -1},
        {"+1minute", Duration_Minutes, 1},
        {"+1minutes", Duration_Minutes, 1},
        {"+1mins", Duration_Minutes, 1},

        {"+1s", Duration_Seconds, 1},
        {"+1sec", Duration_Seconds, 1},
        {"+1secs", Duration_Seconds, 1},
        {"+1seconds", Duration_Seconds, 1},

        {"+1d", Duration_Days, 1},
        {"+1days", Duration_Days, 1},

        {"+1h", Duration_Hours, 1},
        {"+1hrs", Duration_Hours, 1},
        {"+1hours", Duration_Hours, 1},

        {"+1w", Duration_Weeks, 1},
        {"+1wk", Duration_Weeks, 1},
        {"+1wks", Duration_Weeks, 1},
        {"+1weeks", Duration_Weeks, 1},

        {"+1mon", Duration_Months, 1},
        {"+1months", Duration_Months, 1},

        {"+1y", Duration_Years, 1},
        {"+1yr", Duration_Years, 1},
        {"+1yrs", Duration_Years, 1},
        {"+1years", Duration_Years, 1},

        {"+1dec", Duration_Decades, 1},
        {"+1decades", Duration_Decades, 1},
        {"+1cent", Duration_Centuries, 1},
        {"+1centuries", Duration_Centuries, 1},

        {"+123443543sec", Duration_Seconds, 123443543},
        {"+1234435431234567890sec", Duration_Seconds, 1234435431234567890},
        {"+123443543123456789000000sec", Duration_None,0},
        {"+9223372036854775806sec", Duration_Seconds,  9223372036854775806},
        {"9223372036854775807sec", Duration_None, 0},
        {"9223372036854775808sec", Duration_None, 0},
        {"-9223372036854775808sec", Duration_None, 0},
        {"-9223372036854775807sec", Duration_Seconds, -9223372036854775807},
        {"-9223372036854775806sec", Duration_Seconds, -9223372036854775806},
        {"sec", Duration_None, 0},
};

    duration *d = duration_new();
    assert(d->type == Duration_None);
    assert(d->n == 0);

    duration_parse("+10m", d);
    assert(d->type == Duration_Minutes );
    assert(d->n == 10);
    duration_parse("-10m", d);
    assert(d->type == Duration_Minutes );
    assert(d->n == -10);

    for(i = 0; i < sizeof(test) / sizeof(struct DurationTest); i++) {
        duration_init(d);
        duration_parse(test[i].txt, d);
        printf("txt %10s type %d n %" PRId64 "\n", test[i].txt, test[i].type, test[i].n);
        printf("    %10s type %d n %" PRId64 "\n", "", d->type, d->n);
        assert(d->type == test[i].type);
        assert(d->n == test[i].n);
    }
    printf("%zu %zu\n", sizeof(int64_t), sizeof(long long));

    return 0;
}
