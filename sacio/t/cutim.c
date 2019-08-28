
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sacio.h>

#define assert_eq(a,b) assert(a == b)

int
main() {
    sac *s = NULL;
    sac *c = NULL;
    int i = 0;
    int nerr = 0;

    c = sac_read("t/test_line.sac", &nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", c->h->npts);
    assert_eq(c->h->npts, 100);

    s = sac_cut(c, "Z", 10.0, "Z", 90.0, CutNone, &nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 100);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);

    sac_free(s);

    for(i = 1; i < 3; i++) {
        s = sac_cut(c, "Z", 10.0, "Z", 90.0, (enum CutAction) i, &nerr);
        assert_eq(nerr, 0);
        printf("npts: %d\n", s->h->npts);
        assert_eq(s->h->npts, 81);
        assert_eq(s->h->_b, 10.0);
        assert_eq(s->z->_b, 10.0);
        assert_eq(s->h->_e, 90.0);
        assert_eq(s->z->_e, 90.0);
        assert_eq(s->h->depmin, 10.0);
        assert_eq(s->h->depmax, 90.0);
        sac_free(s);
    }
    for(i = 1; i < 3; i++) {
        s = sac_cut(c, "B", 10.0, "B", 90.0, (enum CutAction) i, &nerr);
        assert_eq(nerr, 0);
        printf("npts: %d\n", s->h->npts);
        assert_eq(s->h->npts, 81);
        assert_eq(s->h->_b, 10.0);
        assert_eq(s->z->_b, 10.0);
        assert_eq(s->h->_e, 90.0);
        assert_eq(s->z->_e, 90.0);
        assert_eq(s->h->depmin, 10.0);
        assert_eq(s->h->depmax, 90.0);
        sac_free(s);
    }

    s = sac_cut(c, "B", -10.0, "B", 90.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 91);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 90.0);
    assert_eq(s->z->_e, 90.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 90.0);
    sac_free(s);

    s = sac_cut(c, "B", 10.0, "B", 110.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 90);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    assert_eq(s->h->depmin, 10.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    s = sac_cut(c, "B", -10.0, "B", 110.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_CUT_TIMES_BEYOND_DATA_LIMITS);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 100);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    s = sac_cut(c, "B", -10.0, "B", 90.0, CutFatal, &nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);
    s = sac_cut(c, "B",  10.0, "B", 110.0, CutFatal, &nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);
    s = sac_cut(c, "B", -10.0, "B", 110.0, CutFatal, &nerr);
    assert_eq(nerr, ERROR_CUT_TIMES_BEYOND_DATA_LIMITS);
    assert_eq(s, NULL);

    s = sac_cut(c, "B", -10.0, "B", 90.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 101);
    assert_eq(s->h->_b, -10.0);
    assert_eq(s->z->_b, -10.0);
    assert_eq(s->h->_e, 90.0);
    assert_eq(s->z->_e, 90.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 90.0);
    sac_free(s);

    s = sac_cut(c, "B", 10.0, "B", 110.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 101);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 110.0);
    assert_eq(s->z->_e, 110.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    s = sac_cut(c, "B", -10.0, "B", 110.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 121);
    assert_eq(s->h->_b, -10.0);
    assert_eq(s->z->_b, -10.0);
    assert_eq(s->h->_e, 110.0);
    assert_eq(s->z->_e, 110.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    s = sac_cut(c, "Z", 10.0, "Z", 10.0, CutFillZero, &nerr);
    assert_eq(s, NULL);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);

    s = sac_cut(c, "Z", -100.0, "Z", -10.0, CutUseBE, &nerr);
    assert_eq(s, NULL);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_LESS_THAN_BEGIN);

    s = sac_cut(c, "Z", 110.0, "Z", 130.0, CutUseBE, &nerr);
    assert_eq(s, NULL);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);

    s = sac_cut(c, "Z", -100.0, "Z", -10.0, CutFatal, &nerr);
    assert_eq(s, NULL);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_LESS_THAN_BEGIN);

    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 110.0, "Z", 130.0, CutFatal, &nerr);
    assert_eq(s, NULL);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);

    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 10.0, "Z", 11.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 11.0);
    assert_eq(s->z->_e, 11.0);
    sac_free(s);

    // Start at end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 99.49, "Z", 100.2, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 99.0);
    assert_eq(s->z->_b, 99.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    assert_eq(s->h->depmin, 99.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    // Start at end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 99.50 - 1e-14, "Z", 100.2, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 99.0);
    assert_eq(s->z->_b, 99.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    assert_eq(s->h->depmin, 99.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);


        // Start slightly < begin
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -1e-14, "Z", 50.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 51);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 50.0);
    assert_eq(s->z->_e, 50.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 50.0);
    sac_free(s);

    // Start slightly < begin
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -0.49, "Z", 50.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 51);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 50.0);
    assert_eq(s->z->_e, 50.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 50.0);
    sac_free(s);

    // Start slightly < begin
    s = sac_cut(c, "Z", -0.49, "Z", 0.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 0.0);
    assert_eq(s->z->_e, 0.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);

    printf("LINE %d\n", __LINE__);
    // Not floating point numbers
    s = sac_cut(c, "Z", -1.0/0.0, "Z", 10.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_cut(c, "Z", -1.0, "Z", 1.0/0.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_cut(c, "Z", -1.0/0.0, "Z", 1.0/0.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_cut(c, "Z", -1.0, "Z", sqrt(-1.0), CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_cut(c, "Z", sqrt(-1.0), "Z", 1.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // Too Wide
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -100.0, "Z", 100.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_CUT_TIMES_BEYOND_DATA_LIMITS);
    assert_eq(s, NULL);
    sac_free(s);
    
    // Within Data Range
    s = sac_cut(c, "Z", 10.0, "Z", 90.0, CutFatal, &nerr);
    printf("nerr: %d within data range %p\n", nerr, s);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 81);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 90.0);
    assert_eq(s->z->_e, 90.0);
    sac_free(s);

    printf("LINE %d\n", __LINE__);
    // Small length
    s = sac_cut(c, "Z", 10.0, "Z", 11.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 11.0);
    assert_eq(s->z->_e, 11.0);
    sac_free(s);

    printf("LINE %d\n", __LINE__);
    // No Window
    s = sac_cut(c, "Z", 10.0, "Z", 10.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    printf("LINE %d\n", __LINE__);
    // start > stop
    s = sac_cut(c, "Z", 10.0, "Z", 9.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    printf("LINE %d\n", __LINE__);
    // Too long on end, cut on start
    s = sac_cut(c, "Z", 10.0, "Z", 110.0, CutFatal, &nerr);
    printf("nerr: %d too long on end %p\n", nerr, s);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    printf("LINE %d\n", __LINE__);
    // Less than begin, cut at end
    s = sac_cut(c, "Z", -10.0, "Z", 90.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    printf("LINE %d\n", __LINE__);
    // cut at begin, small within data range
    s = sac_cut(c, "Z", -1.0, "Z", 1.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    printf("LINE %d\n", __LINE__);
    // Both < begin
    s = sac_cut(c, "Z", -2.0, "Z", -1.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    // Both > end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 102.0, "Z", 103.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 101.5, "Z", 101.8, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 100.1, "Z", 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 100.0, "Z", 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start at end point
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 99.0, "Z", 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start at end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 99.49, "Z", 100.2, CutFatal, &nerr);
    printf("nerr: %d start at end point %p\n", nerr, s);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);


    // Start at end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 99.50 - 1e-14, "Z", 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start slightly < begin
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -1e-14, "Z", 50.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 51);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 50.0);
    assert_eq(s->z->_e, 50.0);
    printf("min,max: %e %e\n", s->h->depmin, s->h->depmax);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 50.0);
    sac_free(s);


    // Start slightly < begin
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -0.49, "Z", 50.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 51);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 50.0);
    assert_eq(s->z->_e, 50.0);
    printf("min,max: %e %e\n", s->h->depmin, s->h->depmax);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 50.0);
    sac_free(s);


    // Start slightly < begin
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -0.49, "Z", 0.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 0.0);
    assert_eq(s->z->_e, 0.0);
    printf("min,max: %e %e\n", s->h->depmin, s->h->depmax);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);


    // Too Wide
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -100.0, "Z", 100.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 201);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, -100.0);
    assert_eq(s->z->_b, -100.0);
    assert_eq(s->h->_e, 100.0);
    assert_eq(s->z->_e, 100.0);
    printf("min,max: %e %e\n", s->h->depmin, s->h->depmax);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    // Within Data Range
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 10.0, "Z", 90.0, CutFillZero, &nerr);
    printf("LINE %d %p\n", __LINE__, s);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 81);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 90.0);
    assert_eq(s->z->_e, 90.0);
    printf("LINE %d %p\n", __LINE__, s);
    sac_free(s);
    printf("LINE %d\n", __LINE__);

    // Small length
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 10.0, "Z", 11.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 11.0);
    assert_eq(s->z->_e, 11.0);
    sac_free(s);


    // No Window
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 10.0, "Z", 10.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // start > stop
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 10.0, "Z", 9.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // Too long on end, cut on start
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 10.0, "Z", 110.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 101);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 110.0);
    assert_eq(s->z->_e, 110.0);
    printf("s->y[0]: %e\n", s->y[0]);
    assert_eq(s->y[0], 10.0);
    printf("depmin depmax: %e %e\n", s->h->depmin, s->h->depmax);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);



    // Less than begin, cut at end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -10.0, "Z", 90.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 101);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, -10.0);
    assert_eq(s->z->_b, -10.0);
    assert_eq(s->h->_e, 90.0);
    assert_eq(s->z->_e, 90.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 90.0);
    sac_free(s);


    // cut at begin, small within data range
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -1.0, "Z", 1.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 3);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, -1.0);
    assert_eq(s->z->_b, -1.0);
    assert_eq(s->h->_e, 1.0);
    assert_eq(s->z->_e, 1.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 1.0);
    sac_free(s);


    // Both < begin
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", -2.0, "Z", -1.0, CutFillZero, &nerr);
    printf("nerr: %d both < begin %p\n", nerr, s);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, -2.0);
    assert_eq(s->z->_b, -2.0);
    assert_eq(s->h->_e, -1.0);
    assert_eq(s->z->_e, -1.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);



    // Both > end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 102.0, "Z", 103.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 102.0);
    assert_eq(s->z->_b, 102.0);
    assert_eq(s->h->_e, 103.0);
    assert_eq(s->z->_e, 103.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);



    // Both > end
    printf("LINE %d\n", __LINE__);
    s = sac_cut(c, "Z", 101.5, "Z", 101.8, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    printf("%f %f\n", s->h->_b, s->h->_e);
    assert_eq(s->h->_b, 102.);
    assert_eq(s->z->_b, 102.);
    assert_eq(s->h->_e, 102.);
    assert_eq(s->z->_e, 102.);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);


    // Both > end
    s = sac_cut(c, "Z", 100.1, "Z", 100.2, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    printf("%f %f\n", s->h->_b, s->h->_e);
    assert_eq(s->h->_b, 100.);
    assert_eq(s->z->_b, 100.);
    assert_eq(s->h->_e, 100.);
    assert_eq(s->z->_e, 100.);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);



    // Both > end
    s = sac_cut(c, "Z", 100.0, "Z", 100.2, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    printf("%f %f\n", s->h->_b, s->h->_e);
    assert_eq(s->h->_b, 100.);
    assert_eq(s->z->_b, 100.);
    assert_eq(s->h->_e, 100.);
    assert_eq(s->z->_e, 100.);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);



    // Start at end point
    s = sac_cut(c, "Z", 99.0, "Z", 100.2, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 99.0);
    assert_eq(s->z->_b, 99.0);
    assert_eq(s->h->_e, 100.0);
    assert_eq(s->z->_e, 100.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);


    // Start at end
    s = sac_cut(c, "Z", 99.49, "Z", 100.2, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 99.0);
    assert_eq(s->z->_b, 99.0);
    assert_eq(s->h->_e, 100.0);
    assert_eq(s->z->_e, 100.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);



    // Start at end
    s = sac_cut(c, "Z", 99.50 - 1e-14, "Z", 100.2, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 99.0);
    assert_eq(s->z->_b, 99.0);
    assert_eq(s->h->_e, 100.0);
    assert_eq(s->z->_e, 100.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);


    // Start slightly < begin
    s = sac_cut(c, "Z", -1e-14, "Z", 50.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 51);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 50.0);
    assert_eq(s->z->_e, 50.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 50.0);
    sac_free(s);


    // Start slightly < begin
    s = sac_cut(c, "Z", -0.49, "Z", 50.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 51);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 50.0);
    assert_eq(s->z->_e, 50.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 50.0);
    sac_free(s);


    // Start slightly < begin
    s = sac_cut(c, "Z", -0.49, "Z", 0.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 1);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 0.0);
    assert_eq(s->z->_e, 0.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 0.0);
    sac_free(s);

    printf("-------------------------\n");
    s = sac_cut(c, "B", 0.0, "E", 0.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 100);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    sac_free(s);

    printf("-------------------------\n");
    s = sac_cut(c, "B", 10.0, "E", -10.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 80);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 89.0);
    assert_eq(s->z->_e, 89.0);
    assert_eq(s->h->depmin, 10.0);
    assert_eq(s->h->depmax, 89.0);
    sac_free(s);

    s = sac_cut(c, "B", -1.0, "E", -10.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);

    s = sac_cut(c, "B", 0.0, "E", 1.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);

    printf("-------------------------\n");
    s = sac_cut(c, "T9", 10.0, "T8", -10.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, 0);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 100);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    sac_free(c);

    return 0;
}
