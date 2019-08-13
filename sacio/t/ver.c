
#include <stdio.h>
#include <assert.h>
#include <sacio.h>

int
main() {

    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert(nerr == 0);
    assert(s->h->nvhdr == 6);

    sac_set_v7(s);
    assert(s->h->nvhdr == 7);

    sac_set_v6(s);
    assert(s->h->nvhdr == 6);

    sac_set_v7(s);
    assert(s->h->nvhdr == 7);

    sac_write(s, "t/test_io_small_v7.sac.tmp", &nerr);

    sac *s7 = sac_read("t/test_io_small_v7.sac.tmp", &nerr);
    assert(nerr == 0);
    assert(s->h->nvhdr == 7);

    assert(s->h->_delta == 1.0);
    assert(s->z->_delta == 1.0);

    sac_set_float(s, SAC_T0, 1.0/3.0);
    assert(s->z->_t0 == 1.0/3.0);
    assert(s->h->_t0 == (float)(1.0/3.0));
    sac_write(s, "t/test_io_small_v7.sac.tmp", &nerr);
    s7 = sac_read("t/test_io_small_v7.sac.tmp", &nerr);
    assert(s->z->_t0 == 1.0/3.0);
    assert(s->h->_t0 == (float)(1.0/3.0));
    assert(s->h->_delta == 1.0);
    assert(s->z->_delta == 1.0);

    sac_set_float(s, SAC_DELTA, 1.0/3.0);
    assert(s->h->_delta != s->z->_delta);
    sac_set_float(s, SAC_B, 0.0);
    sac_be(s);
    sac_write(s,"t/test_io_small_v7.sac.tmp", &nerr);
    s7 = sac_read("t/test_io_small_v7.sac.tmp", &nerr);
    assert(s->z->_e == 0.0 + (1.0/3.0) * (double)(s->h->npts - 1));
    assert(s->h->_e == 0.0 + (1.0/3.0) * (double)(s->h->npts - 1));
    printf("%.15e %.15e\n", s->h->_e, s->z->_e);

    sac_set_v6(s7);
    assert(s7->h->nvhdr == 6);
    sac_be(s);
    assert(s->h->_e == 0.0 + (1.0/3.0) * (float)(s->h->npts - 1));
    sac_write(s7,"t/test_io_small_v6.sac.tmp", &nerr);
    sac *s6 = sac_read("t/test_io_small_v6.sac.tmp", &nerr);
    assert(nerr == 0);
    assert(s6->z->_delta = (1.0/3.0));
    printf("ver: %d\n", s6->h->nvhdr);
    assert(s6->h->nvhdr == 6);
    assert(s6->h->_e == 0.0 + (1.0/3.0) * (float)(s6->h->npts - 1));
    assert(s6->h->_e == 0.0 + s6->h->_delta * (float)(s6->h->npts - 1));

    // Setting a value and computation of End Value: Version 7
    double e;
    printf("Version 6\n");
    sac_set_v6(s6);
    sac_set_float(s6, SAC_B, 1.0/3.0);
    sac_be(s6);
    printf("t: %.15e %.15e\n", s6->h->_delta, s6->z->_delta);
    printf("b: %.15e %.15e\n", s6->h->_b, s6->z->_b);
    printf("e: %.15e %.15e (h, z)\n", s6->h->_e, s6->z->_e);
    /// Simplistic manner of computing end time, mising floats
    e = (float) (1.0/3.0) + s6->h->_delta * (float) (s6->h->npts - 1);
    printf("e: %.15e %.15e (mix)\n", s6->h->_e, e);
    assert(s6->h->_e != e);
    // Values are promoted to double then back to float
    e = (float)((double)s6->h->_b  + (double)s6->h->_delta * (double)(s6->h->npts - 1));
    printf("e: %.15e %.15e (64->32)\n", s6->h->_e, e);
    assert(s6->h->_e ==  e);
    // Values are promoted to double
    e = (double)s6->h->_b  + (double)s6->h->_delta * (double)(s6->h->npts - 1);
    printf("e: %.15e %.15e (64)\n", s6->z->_e, e);
    assert(s6->z->_e == e);

    // Setting a value and computation of End Value: Version 7
    printf("Version 7\n");
    sac_set_v7(s6);
    sac_set_float(s6, SAC_B, 1.0/3.0);
    sac_be(s6);
    printf("t: %.15e %.15e\n", s6->h->_delta, s6->z->_delta);
    printf("b: %.15e %.15e\n", s6->h->_b, s6->z->_b);
    printf("e: %.15e %.15e (h, z)\n", s6->h->_e, s6->z->_e);
    // Simplistic manner of computing end time, mixing floats
    e =  (1.0/3.0) + s6->h->_delta * (float) (s6->h->npts - 1);
    printf("e: %.15e %.15e (mix)\n", s6->h->_e, e);
    assert(s6->h->_e != e);
    // Values are promoted to double then back to float
    e = (float)((double)s6->z->_b  + (double)s6->z->_delta * (double)(s6->h->npts - 1));
    printf("e: %.15e %.15e (64->32)\n", s6->h->_e, e);
    assert(s6->h->_e == e);
    // Values are promoted to double
    e = s6->z->_b + s6->z->_delta * (double)(s6->h->npts - 1);
    printf("e: %.15e %.15e (64)\n", s6->z->_e, e);
    assert(s6->z->_e == e);

    double v;
    // Setting and getting a value: Version 6
    sac_set_v6(s6);
    sac_set_float(s6, SAC_T0, 1.0/3.0);
    sac_get_float(s6, SAC_T0, &v);
    assert(s6->h->_t0 == v);
    assert(s6->h->_t0 == (float)(1.0/3.0));
    assert(s6->z->_t0 ==  1.0/3.0);

    // Setting and getting a value: Version 7
    sac_set_v7(s6);
    sac_set_float(s6, SAC_T0, 1.0/3.0);
    sac_get_float(s6, SAC_T0, &v);
    assert(s6->h->_t0 == (float)(1.0/3.0));
    assert(s6->h->_t0 != 1.0/3.0);
    assert(s6->z->_t0 == v);

    return 0;
}
