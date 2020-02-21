
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sacio.h"

#define MAX 2000
int
main() {
    char ty[17];
    int nlen = 0, max = MAX, nerr = 0, nvhdr = 6;
    float y[MAX], x[MAX];
    float beg = 0.0, del = 0.0, delta = 0.0;
    double delta8 = 0.0;
    rsac1("t/test_io_small.sac", y, &nlen, &beg, &del, &max, &nerr, -1);
    printf("%d\n", nerr);
    assert(nerr == 0);
    getfhv("delta", &delta, &nerr, -1);
    printf("%d %f\n", nerr, delta);
    assert(nerr == 0);
    assert(delta == 1.0);
    getdhv("delta", &delta8, &nerr, -1);
    assert(nerr == 0);
    assert(delta8 == 1.0);

    getfhv("b", &beg, &nerr, -1);
    assert(beg == 0.0);
    memset(ty, 0, sizeof(ty));
    getihv("iftype", ty, &nerr, -1, 16);
    printf("'%s'\n", ty);
    assert(strcmp(ty, "ITIME   ") == 0);

    delta8 = 1.0/3.0;
    printf("%.17g\n", delta8);
    setdhv("t0", &delta8, &nerr, -1);
    assert(nerr == 0);
    delta8 = 0.0;
    getdhv("t0", &delta8, &nerr, -1);
    printf("%.17g\n", delta8);
    assert(nerr == 0);
    assert(delta8 == (float)(1.0/3.0));

    nvhdr = 7;
    setnhv("nvhdr", &nvhdr, &nerr, -1);
    assert(nerr == 0);
    delta8 = 1.0/3.0;
    printf("%.17g\n", delta8);
    setdhv("t0", &delta8, &nerr, -1);
    assert(nerr == 0);
    delta8 = 0.0;
    getdhv("t0", &delta8, &nerr, -1);
    printf("%.17g\n", delta8);
    assert(nerr == 0);
    assert(delta8 == (1.0/3.0));

    nvhdr = 6;
    setnhv("nvhdr", &nvhdr, &nerr, -1);
    assert(nerr == 0);

    getkhv("kstnm", ty, &nerr, -1, 8);
    printf("'%s'\n", ty);
    assert(strcmp(ty, "sta    ") == 0);

    getkhv("kevnm", ty, &nerr, -1, 17);
    printf("'%s' %d\n", ty, nerr);
    assert(strcmp(ty, "FUNCGEN: IMPULSE") == 0);

    getnhv("npts", &nlen, &nerr, -1);
    //printf("npts: %d\n", nlen);
    assert(nlen == 100);

    getlhv("leven", &nlen, &nerr, -1);
    //printf("leven: %d\n", nlen);
    assert(nlen == 1);

    float v0 = (float) 10.1;
    float v = v0;
    setfhv("t0", &v, &nerr, -1);
    v = 0.0;
    getfhv("t0", &v, &nerr, -1);
    assert(v == v0);

    int n0 = 1999;
    int n = n0;
    setnhv("nzyear", &n, &nerr, -1);
    n = 0;
    getnhv("nzyear", &n, &nerr, -1);
    assert(n == n0);

    n0 = 0;
    n = n0;
    setlhv("lpspol", &n, &nerr, -1);
    n = 0;
    getlhv("lpspol", &n, &nerr, -1);
    assert(nerr == 0);
    assert(n == n0);

    setkhv("kcmpnm", "BHZ", &nerr, -1, 3);
    getkhv("kcmpnm", ty, &nerr, -1, 16);
    assert(strcmp(ty, "BHZ     ") == 0);

    setihv("iztype", "IO", &nerr, -1, 2);
    getihv("iztype", ty, &nerr, -1, 16);
    assert(strcmp(ty, "IO      ") == 0);

    setkhv("kcmpnm", "", &nerr, -1, -1);
    assert(nerr == 0);
    getkhv("kcmpnm", ty, &nerr, -1, 16);
    assert(nerr == 0);
    assert(strcmp(ty, "        ") == 0);

    rsac1("t/test_io_small.sac", y, &nlen, &beg, &del, &max, &nerr, -1);
    printf("nerr: %d\n", nerr);
    assert(nerr == 0);
    wsac1("t/test_io_small.sac.tmp", y, &nlen, &beg, &del, &nerr, -1);
    assert(nerr == 0);

    rsac1("t/test_io_big.sac", y, &nlen, &beg, &del, &max, &nerr, -1);
    assert(nerr == 0);
    wsac1("t/test_io_big.sac.tmp", y, &nlen, &beg, &del, &nerr, -1);
    assert(nerr == 0);

    rsac2("t/test_uneven_big.sac", y, &nlen, x, &max, &nerr, -1);
    assert(nerr == 0);
    wsac2("t/test_uneven_big.sac.tmp", y, &nlen, x, &nerr, -1);
    assert(nerr == 0);

    rsac2("t/test_uneven_small.sac", y, &nlen, x, &max, &nerr, -1);
    assert(nerr == 0);
    wsac2("t/test_uneven_small.sac.tmp", y, &nlen, x, &nerr, -1);
    assert(nerr == 0);

    rsac1("t/test_spec_small.sac", y, &nlen, &beg, &del, &max, &nerr, -1);
    assert(nerr == 0);
    wsac1("t/test_spec_small.sac.tmp", y, &nlen, &beg, &del, &nerr, -1);
    assert(nerr == 0);

    rsac1("t/test_spec_big.sac", y, &nlen, &beg, &del, &max, &nerr, -1);
    assert(nerr == 0);
    wsac1("t/test_spec_big.sac.tmp", y, &nlen, &beg, &del, &nerr, -1);
    assert(nerr == 0);

}
