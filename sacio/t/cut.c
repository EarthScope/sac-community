
#include <stdio.h>
#include <assert.h>
#include <sacio.h>
#include <math.h>


#define assert_eq(a,b) assert(a == b)

int
main() {

    int nerr = 0;
    sac *s = NULL;
    // Too Wide
    s = sac_read_with_cut("t/test_line.sac", -100.0, 100.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_CUT_TIMES_BEYOND_DATA_LIMITS);
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
    
    // Within Data Range
    s = sac_read_with_cut("t/test_line.sac", 10.0, 90.0, CutUseBE, &nerr);
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
    sac_free(s);
    
    // Small length
    s = sac_read_with_cut("t/test_line.sac", 10.0, 11.0, CutUseBE, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 10.0, 10.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // start > stop
    s = sac_read_with_cut("t/test_line.sac", 10.0, 9.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // Too long on end, cut on start
    s = sac_read_with_cut("t/test_line.sac", 10.0, 110.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 90);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 10.0);
    assert_eq(s->z->_b, 10.0);
    assert_eq(s->h->_e, 99.0);
    assert_eq(s->z->_e, 99.0);
    printf("depmin depmax: %e %e\n", s->h->depmin, s->h->depmax);
    assert_eq(s->y[0], 10.0);
    assert_eq(s->h->depmin, 10.0);
    assert_eq(s->h->depmax, 99.0);
    sac_free(s);

    // Less than begin, cut at end
    s = sac_read_with_cut("t/test_line.sac", -10.0, 90.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 91);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 90.0);
    assert_eq(s->z->_e, 90.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 90.0);
    sac_free(s);
    
    // cut at begin, small within data range
    s = sac_read_with_cut("t/test_line.sac", -1.0, 1.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    printf("npts: %d\n", s->h->npts);
    assert_eq(s->h->npts, 2);
    printf("%e %e\n", s->h->_delta, s->z->_delta);
    assert_eq(s->h->_delta, 1.0);
    assert_eq(s->z->_delta, 1.0);
    assert_eq(s->h->_b, 0.0);
    assert_eq(s->z->_b, 0.0);
    assert_eq(s->h->_e, 1.0);
    assert_eq(s->z->_e, 1.0);
    assert_eq(s->h->depmin, 0.0);
    assert_eq(s->h->depmax, 1.0);
    sac_free(s);
    
    // Both < begin
    s = sac_read_with_cut("t/test_line.sac", -2.0, -1.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 102.0, 103.0, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 101.5, 101.8, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 100.1, 100.2, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 100.0, 100.2, CutUseBE, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start at end point
    s = sac_read_with_cut("t/test_line.sac", 99.0, 100.2, CutUseBE, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 99.49, 100.2, CutUseBE, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 99.50 - 1e-14, 100.2, CutUseBE, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -1e-14, 50.0, CutUseBE, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -0.49, 50.0, CutUseBE, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -0.49, 0.0, CutUseBE, &nerr);
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

    // Not floating point numbers
    s = sac_read_with_cut("t/test_line.sac", -1.0/0.0, 10.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_read_with_cut("t/test_line.sac", -1.0, 1.0/0.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_read_with_cut("t/test_line.sac", -1.0/0.0, 1.0/0.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_read_with_cut("t/test_line.sac", -1.0, sqrt(-1.0), CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);
    s = sac_read_with_cut("t/test_line.sac", sqrt(-1.0), 1.0, CutUseBE, &nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);


    // Too Wide
    s = sac_read_with_cut("t/test_line.sac", -100.0, 100.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_CUT_TIMES_BEYOND_DATA_LIMITS);
    assert_eq(s, NULL);
    sac_free(s);
    // Within Data Range
    s = sac_read_with_cut("t/test_line.sac", 10.0, 90.0, CutFatal, &nerr);
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
    
    // Small length
    s = sac_read_with_cut("t/test_line.sac", 10.0, 11.0, CutFatal, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 10.0, 10.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // start > stop
    s = sac_read_with_cut("t/test_line.sac", 10.0, 9.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // Too long on end, cut on start
    s = sac_read_with_cut("t/test_line.sac", 10.0, 110.0, CutFatal, &nerr);
    printf("nerr: %d too long on end %p\n", nerr, s);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);


    // Less than begin, cut at end
    s = sac_read_with_cut("t/test_line.sac", -10.0, 90.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    // cut at begin, small within data range
    s = sac_read_with_cut("t/test_line.sac", -1.0, 1.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    // Both < begin
    s = sac_read_with_cut("t/test_line.sac", -2.0, -1.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_LESS_THAN_BEGIN);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 102.0, 103.0, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 101.5, 101.8, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 100.1, 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Both > end
    s = sac_read_with_cut("t/test_line.sac", 100.0, 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start at end point
    s = sac_read_with_cut("t/test_line.sac", 99.0, 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start at end
    s = sac_read_with_cut("t/test_line.sac", 99.49, 100.2, CutFatal, &nerr);
    printf("nerr: %d start at end point %p\n", nerr, s);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);


    // Start at end
    s = sac_read_with_cut("t/test_line.sac", 99.50 - 1e-14, 100.2, CutFatal, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_STOP_TIME_GREATER_THAN_END);
    assert_eq(s, NULL);

    // Start slightly < begin
    s = sac_read_with_cut("t/test_line.sac", -1e-14, 50.0, CutFatal, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -0.49, 50.0, CutFatal, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -0.49, 0.0, CutFatal, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -100.0, 100.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 10.0, 90.0, CutFillZero, &nerr);
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
    sac_free(s);


    // Small length
    s = sac_read_with_cut("t/test_line.sac", 10.0, 11.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 10.0, 10.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // start > stop
    s = sac_read_with_cut("t/test_line.sac", 10.0, 9.0, CutFillZero, &nerr);
    printf("nerr: %d\n", nerr);
    assert_eq(nerr, ERROR_START_TIME_GREATER_THAN_STOP);
    assert_eq(s, NULL);

    // Too long on end, cut on start
    s = sac_read_with_cut("t/test_line.sac", 10.0, 110.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -10.0, 90.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -1.0, 1.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -2.0, -1.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 102.0, 103.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 101.5, 101.8, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 100.1, 100.2, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 100.0, 100.2, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 99.0, 100.2, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 99.49, 100.2, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", 99.50 - 1e-14, 100.2, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -1e-14, 50.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -0.49, 50.0, CutFillZero, &nerr);
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
    s = sac_read_with_cut("t/test_line.sac", -0.49, 0.0, CutFillZero, &nerr);
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




}
