
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <sacio.h>

#define assert_eq(a,b) assert(a == b)
#define assert_aeq(a,b) assert(fabs(a - b) <= (1e-8 + 1e-5 * fabs(b)))
#define assert_ne(a,b) assert(a != b)

void
compare(sac *a, sac *b) {
    int i;
    float *fa, *fb;
    int *ia, *ib;
    char *ca, *cb;

    printf("FILES: '%s' '%s'\n", a->m->filename, b->m->filename);
    fa = &(a->h->_delta);
    fb = &(b->h->_delta);
    for(i = 0; i < 70; i++) {
        printf("b,a[%d]: %.15f %.15f %.15f %.15f\n", i, *fa, *fb, fabs(*fa-*fb), 1e-8+1e-5*fabs(*fb));
        if(isfinite(*fa) && isfinite(*fb)) {
            assert_aeq(*fa, *fb);
        } else {
            assert_eq(*fa, *fb);
        }
        fa++;
        fb++;
    }

    printf("ints\n");
    ia = &(a->h->nzyear);
    ib = &(b->h->nzyear);
    for(i = 0; i < 40; i++) {
        printf("b,a[%d]: %d %d (%d) %d %d\n", i, *ia, *ib, *ia-*ib, b->h->npts, a->h->npts);
        assert_eq(*ia, *ib);
        ia++;
        ib++;
    }

    ca = a->h->kstnm;
    cb = b->h->kstnm;
    printf("KEVNM '%s' '%s'\n", a->h->kevnm, b->h->kevnm);
    for(i = 0; i < 24; i++) {
        if(i != 2) {
            printf("b,a[%d]: '%s' '%s' \n", i, ca, cb);
            assert_eq(strcmp(ca, cb), 0);
        }
        ca += 9;
        cb += 9;

    }
    for(i = 0; i < b->h->npts; i++) {
        printf("y %d %.15f %.15f\n", i, a->y[i], b->y[i]);
        assert_aeq(b->y[i], a->y[i]);
    }
    if(sac_comps(b) == 2) {
        for(i = 0; i < b->h->npts; i++) {
            printf("x %d %.15f %.15f\n", i, a->y[i], b->y[i]);
            assert_aeq(b->x[i], a->x[i]);
        }
    } else {
        assert_eq(a->x, b->x);
    }
}


int
main() {
    int i = 0;
    int nerr = 0;
    sac *a, *b;
    b = sac_read("t/imp.sac", &nerr);
    assert_eq(nerr, 0);
    assert_ne(b, 0);

    a = sac_read_alpha("t/imp.alpha", &nerr);
    assert_eq(nerr, 0);
    assert_ne(a, 0);
    compare(a, b);
    sac_free(a);
    sac_free(b);


    b = sac_read("t/test_spec_small.sac", &nerr);
    assert_eq(nerr, 0);
    a = sac_read_alpha("t/test_spec_alpha.sac", &nerr);
    assert_eq(nerr, 0);
    compare(a, b);
    sac_free(a);
    sac_free(b);

    a = sac_read("t/imp.sac", &nerr);
    assert_eq(nerr, 0);
    sac_write_alpha(a, "t/test_imp_alpha.tmp", &nerr);
    assert_eq(nerr, 0);
    b = sac_read_alpha("t/test_imp_alpha.tmp", &nerr);
    assert_eq(nerr, 0);
    compare(a,b);
    sac_free(a);
    sac_free(b);

    a = sac_read("t/test_spec_small.sac", &nerr);
    assert_eq(nerr, 0);
    sac_write_alpha(a, "t/test_spec_alpha.tmp", &nerr);
    assert_eq(nerr, 0);
    b = sac_read_alpha("t/test_spec_alpha.tmp", &nerr);
    assert_eq(nerr, 0);
    compare(a,b);
    sac_free(a);
    sac_free(b);

    printf("TEST V7 ALPHA\n");
    a = sac_read("t/imp.sac", &nerr);
    assert_eq(nerr, 0);
    a->h->nvhdr = 7;
    sac_set_float(a, SAC_T0, 1.0/3.0);
    sac_set_float(a, SAC_T1, 1.0/7.0);
    sac_set_float(a, SAC_F, 1.0/13.0);
    for(i = 2; i <= 9; i++) {
        sac_set_float(a, SAC_T0 + i, (double)i*100);
    }
    sac_set_float(a, SAC_EVLO, 1.0);
    sac_set_float(a, SAC_EVLA, 2.0);
    sac_set_float(a, SAC_STLO, 3.0);
    sac_set_float(a, SAC_STLA, 4.0);
    sac_set_float(a, SAC_SB, -0.434839483e55);
    sac_set_float(a, SAC_SDELTA, -1.2408584395843954);
    sac_write_alpha(a, "t/test_imp_alpha7.tmp", &nerr);
    assert_eq(nerr, 0);
    printf("VAL: %.17g\n", a->z->_sb);
    b = sac_read_alpha("t/test_imp_alpha7.tmp", &nerr);
    assert_eq(nerr, 0);
    printf("VAL: %.17g\n", b->z->_sb);
    compare(a,b);
    sac_free(a);
    sac_free(b);

    return 0;
}
