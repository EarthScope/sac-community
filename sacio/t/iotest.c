
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "sacio.h"

void
check_sac_file_even(char *file) {
    int i = 0;
    int nerr;
    sac *s = sac_read(file, &nerr);
    assert(nerr == 0);
    assert(s->h->npts == 100);
    assert(s->h->_b == 0.0);
    assert(s->h->_e == 99.0);
    assert(s->h->_delta == 1.0);
    assert(s->z->_b == 0.0);
    assert(s->z->_e == 99.0);
    assert(s->z->_delta == 1.0);
    
    assert(s->h->depmin == 0.0);
    assert(s->h->depmax == 1.0);
    assert(s->h->depmen == 0.009999999776482582);
    assert(strcmp(s->h->kstnm,  "sta     ") == 0);
    assert(strcmp(s->h->kcmpnm, "Q       ") == 0);
    assert(strcmp(s->h->kevnm, "FUNCGEN: IMPULSE") == 0);
    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_STA || i == SAC_CHA ||
           i == SAC_EVENT || i == SAC_EVENT2) {
            continue;
        }
        char tmp[20] = {0};
        sac_get_string(s, i, tmp, sizeof(tmp));
        assert(strcmp(tmp, "-12345  ") == 0);
    }
    for(i = SAC_DELTA; i <= SAC_UN70; i++) {
        double v = 0.0;
        sac_get_float(s, i, &v);
        fprintf(stderr, "%d %f\n", i, v);
        if(i == SAC_DELTA) { assert(v == 1.0); }
        else if(i == SAC_DEPMIN) { assert(v == 0.0); }
        else if(i == SAC_DEPMAX) { assert(v == 1.0); }
        else if(i == SAC_B) { assert(v == 0.0); }
        else if(i == SAC_E) { assert(v == 99.0); }
        else if(i == SAC_DEPMEN) { assert(v == 0.009999999776482582); }
        else { assert(v == SAC_FLOAT_UNDEFINED); }
    }
    for(i = SAC_YEAR; i <= SAC_UN110; i++) {
        int v = 0;
        sac_get_int(s, i, &v);
        if(i == SAC_HDR) { assert(v == 6); }
        else if(i == SAC_NPTS)         { assert(v == 100); }
        else if(i == SAC_FILE_TYPE)    { assert(v == 1); }
        else if(i == SAC_EVEN)         { assert(v == 1); }
        else if(i == SAC_POLARITY)     { assert(v == 0); }
        else if(i == SAC_OVERWRITE)    { assert(v == 1); }
        else if(i == SAC_CALC_DIST_AZ) { assert(v == 1); }
        else if(i == SAC_UN110)        { assert(v == 0); }
        else { assert(v == SAC_INT_UNDEFINED); }
    }
}

void
check_sac_file_uneven(char *file) {
    int i = 0;
    int nerr = 0;
    sac *s = sac_read(file, &nerr);
    assert(nerr == 0);
    assert(strcmp(s->h->kstnm,  "sta     ") == 0);
    assert(strcmp(s->h->kcmpnm, "Q       ") == 0);

    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_STA || i == SAC_CHA){
            continue;
        }
        char tmp[20] = {0};
        sac_get_string(s, i, tmp, sizeof(tmp));
        assert(strcmp(tmp, "-12345  ") == 0);
    }
    for(i = SAC_DELTA; i <= SAC_UN70; i++) {
        double v = 0.0;
        sac_get_float(s, i, &v);
        if(i == SAC_DEPMIN) { assert(v == 0.0); }
        else if(i == SAC_DEPMAX) { assert(v == 0.904837429523468018); }
        else if(i == SAC_B) { assert(v == 0.100000001490116119); }
        else if(i == SAC_E) { assert(v == 876778944.000); }
        else if(i == SAC_DEPMEN) { assert(v == 0.080867879092693329); }
        else { assert(v == SAC_FLOAT_UNDEFINED); }
    }
    for(i = SAC_YEAR; i <= SAC_UN110; i++) {
        int v = 0;
        sac_get_int(s, i, &v);
        if(i == SAC_HDR) { assert(v == 6); }
        else if(i == SAC_NPTS)         { assert(v == 300); }
        else if(i == SAC_FILE_TYPE)    { assert(v == 1); }
        else if(i == SAC_EVEN)         { assert(v == 0); }
        else if(i == SAC_POLARITY)     { assert(v == 0); }
        else if(i == SAC_OVERWRITE)    { assert(v == 1); }
        else if(i == SAC_CALC_DIST_AZ) { assert(v == 1); }
        else if(i == SAC_UN110)        { assert(v == 0); }
        else { assert(v == SAC_INT_UNDEFINED); }
    }
}

void
check_sac_file_spec(char *file) {
    int i = 0;
    int nerr = 0;
    sac *s = sac_read(file, &nerr);
    assert(nerr == 0);
    assert(strcmp(s->h->kstnm,  "CDV     ") == 0);
    assert(strcmp(s->h->kcmpnm, "Q       ") == 0);
    assert(strcmp(s->h->kuser0, "ABKD    ") == 0);
    assert(strcmp(s->h->kuser1, "USER0   ") == 0);
    assert(strcmp(s->h->kt2,    "KT1     ") == 0);
    assert(strcmp(s->h->kt0,    "XYZ     ") == 0);
    assert(strcmp(s->h->ka,     "IPD0    ") == 0);
    assert(strcmp(s->h->ko,     "HOLE    ") == 0);
    assert(strcmp(s->h->kevnm,  "K8108838        ") == 0);

    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_STA || i == SAC_CHA || i == SAC_EVENT || i == SAC_EVENT2 ||
           i == SAC_KO || i == SAC_KA || i == SAC_KT0 || i == SAC_KT2 ||
           i == SAC_KUSER1 || i == SAC_KUSER0 ){
            continue;
        }
        char tmp[20] = {0};
        sac_get_string(s, i, tmp, sizeof(tmp));
        assert(strcmp(tmp, "-12345  ") == 0);
    }
    for(i = SAC_DELTA; i <= SAC_UN70; i++) {
        double v = 0.0;

        sac_get_float(s, i, &v);
        if(i == SAC_DEPMIN)      { assert(v == 0.00003626910620369017); }
        else if(i == SAC_DEPMAX) { assert(v == 0.98547208309173583984); }
        else if(i == SAC_DELTA)  { assert(v == 0.097656250000000000); }
        else if(i == SAC_B)      { assert(v == 0.0); }
        else if(i == SAC_E)      { assert(v == 50.0); }
        else if(i == SAC_DEPMEN) { assert(v == 0.05418542400002479553); }
        else if(i == SAC_CMPAZ)  { assert(v == 0.0); }
        else if(i == SAC_CMPINC) { assert(v == 0.0); }
        else if(i == SAC_SDELTA) { assert(v == 0.00999999977648258209); }
        else if(i == SAC_SB)     { assert(v == 9.45999908447265625000); }
        else if(i == SAC_GCARC)  { assert(v == 40.09692764282226562500); }
        else if(i == SAC_BAZ)    { assert(v == 185.20507812500000000000); }
        else if(i == SAC_AZ)     { assert(v == 0.27151772379875183105); }
        else if(i == SAC_DIST)   { assert(v == 4461.03564453125000000000); }
        else if(i == SAC_USER0)  { assert(v == 123.45600128173828125000); }

        else if(i == SAC_EVLA)   { assert(v == 47.99996948242187500000); }
        else if(i == SAC_EVLO)   { assert(v == -1.25000E+02); }
        else if(i == SAC_STLA)   { assert(v == 87.99996948242187500000); }
        else if(i == SAC_STLO)   { assert(v == -1.20000E+02); }
        else if(i == SAC_F)      { assert(v == 17.78000068664550781250); }
        else if(i == SAC_T1)     { assert(v == 2.00000E+01); }
        else if(i == SAC_FMT)    { assert(v == 2.0); }
        else if(i == SAC_A)      { assert(v == 10.47000026702880859375); }
        else if(i == SAC_O)      { assert(v == 0.0); }
        else                     { assert(v == SAC_FLOAT_UNDEFINED); }
    }
    for(i = SAC_YEAR; i <= SAC_UN110; i++) {
        int v = 0;
        sac_get_int(s, i, &v);
        if(i == SAC_HDR) { assert(v == 6); }
        else if(i == SAC_NPTS)         { assert(v == 1024); }
        else if(i == SAC_NSNPTS)       { assert(v == 1000); }
        else if(i == SAC_EVID)         { assert(v == 0); }
        else if(i == SAC_ORID)         { assert(v == 0); }
        else if(i == SAC_EVENT_TYPE)   { assert(v == 42); }

        else if(i == SAC_YEAR)         { assert(v == 1981); }
        else if(i == SAC_DAY)          { assert(v == 88); }
        else if(i == SAC_HOUR)         { assert(v == 10); }
        else if(i == SAC_MIN)          { assert(v == 38); }
        else if(i == SAC_SEC)          { assert(v == 14); }
        else if(i == SAC_MSEC)         { assert(v == 0); }

        else if(i == SAC_FILE_TYPE)    { assert(v == 3); }
        else if(i == SAC_DEP_TYPE)     { assert(v == 50); }
        else if(i == SAC_ZERO_TIME)    { assert(v == 9); }
        else if(i == SAC_EVEN)         { assert(v == 1); }
        else if(i == SAC_POLARITY)     { assert(v == 1); }
        else if(i == SAC_OVERWRITE)    { assert(v == 1); }
        else if(i == SAC_CALC_DIST_AZ) { assert(v == 1); }
        else if(i == SAC_UN110)        { assert(v == 0); }
        else { assert(v == SAC_INT_UNDEFINED); }
    }
}

int
main() {
    int i = 0;
    check_sac_file_even("t/test_io_small.sac");
    check_sac_file_even("t/test_io_big.sac");
    check_sac_file_uneven("t/test_uneven_small.sac");
    check_sac_file_uneven("t/test_uneven_big.sac");
    check_sac_file_spec("t/test_spec_small.sac");
    check_sac_file_spec("t/test_spec_big.sac");

    int nerr = 0;
    char tmp[128] = {0};
    sac *s = sac_read("t/test_spec_small.sac", &nerr);;
    sac_get_string(s, SAC_DATE, tmp, sizeof(tmp));
    printf("%s\n", tmp);
    sac_get_string(s, SAC_TIME, tmp, sizeof(tmp));
    printf("%s\n", tmp);
    sac_get_string(s, SAC_STCMP, tmp, sizeof(tmp));
    printf("%s\n", tmp);
    sac_set_string(s, SAC_CHA, SAC_CHAR_UNDEFINED);
    sac_set_float(s, SAC_CMPINC, SAC_FLOAT_UNDEFINED);
    sac_set_float(s, SAC_CMPAZ, SAC_FLOAT_UNDEFINED);
    sac_get_string(s, SAC_STCMP, tmp, sizeof(tmp));
    printf("%s\n", tmp);
    for(i = -360*2; i <= 360.0; i += 90) {
        sac_set_float(s, SAC_CMPINC, 90.0);
        sac_set_float(s, SAC_CMPAZ, (float)i);
        sac_get_string(s, SAC_STCMP, tmp, sizeof(tmp));
        printf("%s\n", tmp);
    }
    sac_set_float(s, SAC_CMPINC, 45.0);
    sac_set_float(s, SAC_CMPAZ, 45.0);
    sac_get_string(s, SAC_STCMP, tmp, sizeof(tmp));
    printf("%s\n", tmp);

    sac_get_string(s, SAC_AMARKER, tmp, sizeof(tmp));
    printf("%s\n", tmp);

    sac_set_string(s, SAC_KA, SAC_CHAR_UNDEFINED);
    sac_get_string(s, SAC_AMARKER, tmp, sizeof(tmp));
    printf("%s\n", tmp);

    memset(tmp, 0, sizeof(tmp));
    sac_set_float(s, SAC_A, SAC_FLOAT_UNDEFINED);
    sac_get_string(s, SAC_AMARKER, tmp, sizeof(tmp));
    printf("%s\n", tmp);

    return 0;
}
