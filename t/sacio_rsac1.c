#include <string.h>
#include <math.h>

#include "amf.h"
#include "unit.h"
#include "hdr.h"
#include "dff.h"
#include "sacio_test.h"
static sac *s;

void
test_rsac1_header_file() {
  s = sacget_current();

  float_check(s->h->_b, 0.0, "b");
  float_check(s->z->_b, 0.0, "b");
  //float_check(s->h->begin, 0.0, "begin");
  float_check(s->h->_e, 99.0, "e");
  float_check(s->z->_e, 99.0, "e");
  //float_check(s->h->ennd, 99.0, "e");
  float_check(s->h->_delta, 1.0, "delta");
  float_check(s->z->_delta, 1.0, "delta");
  float_check(s->h->depmax, 1.0, "depmax");
  float_check(s->h->depmin, 0.0, "cmpinc");
  //float_check(s->h->depmx, 1.0, "depmax");
  //float_check(s->h->depmn, 0.0, "depmn");
  ok(fabs(s->h->depmen - 0.01) < 1e-4, 
     "depmen value (value %f, expected %f diff %e)", 
     s->h->depmen, 0.01, fabs(s->h->depmen - 0.01));
  //ok(fabs(s->h->fmean - 0.01) < 1e-4, 
  //   "fmean value (value %f, expected %f diff %e)", 
  //   s->h->fmean, 0.01, fabs(s->h->fmean - 0.01));

  float_undef(s->h->_a, "a");
  float_undef(s->z->_a, "a");
  float_undef(s->h->az, "az");
  float_undef(s->h->baz, "baz");
  float_undef(s->h->cmpinc, "cmpinc");
  float_undef(s->h->cmpaz, "cmpaz");
  float_undef(s->h->dist, "dist");
  float_undef(s->h->evdp, "evdp");
  float_undef(s->h->_evla, "evla");
  float_undef(s->z->_evla, "evla");
  float_undef(s->h->_evlo, "evlo");
  float_undef(s->z->_evlo, "evlo");
  float_undef(s->h->evel, "evel");
  float_undef(s->h->_f,    "f");
  float_undef(s->z->_f,    "f");
  //float_undef(s->h->fini, "fini");
  float_undef(s->h->unused6, "fhdr64");
  float_undef(s->h->unused7, "fhdr65");
  float_undef(s->h->unused8, "fhdr66");
  float_undef(s->h->unused9, "fhdr67");
  float_undef(s->h->unused10, "fhdr68");
  float_undef(s->h->unused11, "fhdr69");
  float_undef(s->h->unused12, "fhdr70");
  float_undef(s->h->fmt,    "fmt");
  float_undef(s->h->gcarc, "gcarc");

  int_undef(s->h->idep, "idep");
  int_undef(s->h->ievreg, "ievreg");
  int_undef(s->h->ievtyp, "ievtyp");
  int_check(s->h->iftype, 1, "file type");
  int_undef(s->h->imagtyp, "imagtyp");
  int_undef(s->h->imagsrc, "imagsrc");
  int_undef(s->h->unused19, "ihdr13");
  int_undef(s->h->unused20, "ihdr14");
  int_undef(s->h->unused21, "ihdr15");
  int_undef(s->h->unused22, "ihdr16");
  int_undef(s->h->unused23, "ihdr17");
  int_undef(s->h->unused24, "ihdr18");
  int_undef(s->h->unused25, "ihdr19");
  int_undef(s->h->unused26, "ihdr20");
  int_undef(s->h->unused16, "ihdr4");
  int_undef(s->h->iinst, "iinst");
  int_undef(s->h->iqual, "iqual");
  int_undef(s->h->istreg, "istreg");
  int_undef(s->h->isynth, "isynth");
  int_undef(s->h->iztype, "iztype");

  char_check(s->h->kstnm, "sta     ", "kstnm"); 
  char_check16(s->h->kevnm,"FUNCGEN: IMPULSE", "kevnm"); 

  char_undef(s->h->khole, "khole");
  char_undef(s->h->ka, "ka");
  char_undef(s->h->ko, "ko");
  char_undef(s->h->kt0, "kt0");
  char_undef(s->h->kt1, "kt1");
  char_undef(s->h->kt2, "kt2");
  char_undef(s->h->kt3, "kt3");
  char_undef(s->h->kt4, "kt4");
  char_undef(s->h->kt5, "kt5");
  char_undef(s->h->kt6, "kt6");
  char_undef(s->h->kt7, "kt7");
  char_undef(s->h->kt8, "kt8");
  char_undef(s->h->kt9, "kt9");
  char_undef(s->h->kf, "kf");
  char_undef(s->h->kuser0, "kuser0");
  char_undef(s->h->kuser1, "kuser1");
  char_undef(s->h->kuser2, "kuser2");
  char_check(s->h->kcmpnm, "Q       ", "kcmpnm");
  char_undef(s->h->knetwk, "knetwk");
  char_undef(s->h->kdatrd, "kdatrd");
  char_undef(s->h->kinst, "kinst");

  int_check(s->h->lcalda, 1, "lcalda");
  int_check(s->h->leven,  1, "leven");
  int_check(s->h->unused27,  0, "lhdr5");
  int_check(s->h->lovrok, 1, "lovrok");
  int_check(s->h->lpspol, 0, "lpspol");

  int_undef(s->h->nevid, "nevid");
  int_undef(s->h->unused15, "nhdr15");
  int_undef(s->h->norid, "norid");
  int_check(s->h->npts, 100, "npts");
  int_check(s->h->nvhdr, 6, "nvhdr");
  int_undef(s->h->nwfid, "nwfid");
  int_undef(s->h->nxsize, "nxsize");
  int_undef(s->h->nysize, "nysize");
  //int_undef(s->h->nzdttm, "nzdttm");
  int_undef(s->h->nzjday, "nzjday");
  int_undef(s->h->nzmin, "nzmin");
  int_undef(s->h->nzmsec, "nzmsec");
  int_undef(s->h->nzsec, "nzsec");
  int_undef(s->h->nzyear, "nzyear");

  float_undef(s->h->_o, "o");
  float_undef(s->z->_o, "o");
  float_undef(s->h->odelta, "odelta");
  //float_undef(s->h->origin, "origin");
  float_undef(s->h->resp0, "resp0");
  float_undef(s->h->resp1, "resp1");
  float_undef(s->h->resp2, "resp2");
  float_undef(s->h->resp3, "resp3");
  float_undef(s->h->resp4, "resp4");
  float_undef(s->h->resp5, "resp5");
  float_undef(s->h->resp6, "resp6");
  float_undef(s->h->resp7, "resp7");
  float_undef(s->h->resp8, "resp8");
  float_undef(s->h->resp9, "resp9");

  float_undef(s->h->_sb, "sb");
  float_undef(s->z->_sb, "sb");
  float_undef(s->h->scale, "scale");
  float_undef(s->h->_sdelta, "sdelta");
  float_undef(s->z->_sdelta, "sdelta");
  float_undef(s->h->stdp, "stdp");
  float_undef(s->h->stel, "stel");
  float_undef(s->h->_stla, "stla");
  float_undef(s->z->_stla, "stla");
  float_undef(s->h->_stlo, "stlo");
  float_undef(s->z->_stlo, "stlo");
  float_undef(s->h->_t0, "t0");
  float_undef(s->h->_t1, "t1");
  float_undef(s->h->_t2, "t2");
  float_undef(s->h->_t3, "t3");
  float_undef(s->h->_t4, "t4");
  float_undef(s->h->_t5, "t5");
  float_undef(s->h->_t6, "t6");
  float_undef(s->h->_t7, "t7");
  float_undef(s->h->_t8, "t8");
  float_undef(s->h->_t9, "t9");
  float_undef(s->z->_t0, "t0");
  float_undef(s->z->_t1, "t1");
  float_undef(s->z->_t2, "t2");
  float_undef(s->z->_t3, "t3");
  float_undef(s->z->_t4, "t4");
  float_undef(s->z->_t5, "t5");
  float_undef(s->z->_t6, "t6");
  float_undef(s->z->_t7, "t7");
  float_undef(s->z->_t8, "t8");
  float_undef(s->z->_t9, "t9");
  //float_undef(s->h->time0, "time0");
  //float_undef(s->h->time1, "time1");
  //float_undef(s->h->time2, "time2");
  //float_undef(s->h->time3, "time3");
  //float_undef(s->h->time4, "time4");
  //float_undef(s->h->time5, "time5");
  //float_undef(s->h->time6, "time6");
  //float_undef(s->h->time7, "time7");
  //float_undef(s->h->time8, "time8");
  //float_undef(s->h->time9, "time9");
  float_undef(s->h->user0, "user0");
  float_undef(s->h->user1, "user1");
  float_undef(s->h->user2, "user2");
  float_undef(s->h->user3, "user3");
  float_undef(s->h->user4, "user4");
  float_undef(s->h->user5, "user5");
  float_undef(s->h->user6, "user6");
  float_undef(s->h->user7, "user7");
  float_undef(s->h->user8, "user8");
  float_undef(s->h->user9, "user9");

  float_undef(s->h->xmaximum, "xmaximum");
  float_undef(s->h->xminimum, "xminimum");
  float_undef(s->h->ymaximum, "ymaximum");
  float_undef(s->h->yminimum, "yminimum");
}


void
test_rsac1_file(char *file) {
  int i;
  int err;
  float y[1024];
  int lnpts, max;
  float bval, fdelta;
  
  err = SAC_OK;
  max = 1024;
  /* Correct Length */
  rsac1(file, &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(file));
  ok(err == SAC_OK, "rsac1 file <%s> err %d expected %d", file, err, SAC_OK);
  
  err = SAC_OK;
  /* Length too Short */
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(file)-1);
  ok(err == 108, "rsac1 file <%s> too short err %d expected %d", file, err, 108);

  /* Length too Long */
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(file)*10);
  ok(err == SAC_OK, "rsac1 file <%s> too long err %d expected %d", file, err, SAC_OK);

  /* Length < 0 */
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, -1);
  ok(err == SAC_OK, "rsac1 file <%s> < 0 err %d expected %d", file, err, SAC_OK);
  ok(lnpts == 100, "rsac1 file <%s> truncated npts %d exptected %d", file, lnpts, 100);
  test_rsac1_header_file();

  /* Maximum number of points = 100 */
  max = 100;
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, -1);
  ok(err == SAC_OK, "rsac1 file <%s> err %d expected %d", file, err, SAC_OK);
  ok(lnpts == 100, "rsac1 file <%s> npts %d exptected %d", file, lnpts, 100);
  test_rsac1_header_file();

  /* Maximum number of points = 99 */
  max = 99;
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, -1);
  ok(err == -803, "rsac1 file <%s> truncated err %d expected %d", file, err, -803);
  ok(lnpts == 99, "rsac1 file <%s> truncated npts %d exptected %d", file, lnpts, 99);
  test_rsac1_header_file();

  /* Maximum number of points = 50 */
  max = 50;
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, -1);
  ok(err == -803, "rsac1 file <%s> truncated err %d expected %d", file, err, -803);
  ok(lnpts == 50, "rsac1 file <%s> truncated npts %d exptected %d", file, lnpts, 50);
  test_rsac1_header_file();

  /* Length < 0 */
  max = 1024;
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, &err, -1);
  ok(err == SAC_OK, "rsac1 file <%s> data check err %d expected %d", file, err, SAC_OK);
  ok(lnpts == 100, "rsac1 file <%s> data check npts %d exptected %d", file, lnpts, 100);
  test_rsac1_header_file();
  for(i = 0; i < lnpts; i++) {
    if(i != 49) {
      ok(y[i] == 0.0, "rsac1 file <%s> data point %d value %f expected %f", file, i, y[i], 0.0);
    } else {
      ok(y[i] == 1.0, "rsac1 file <%s> data point %d value %f expected %f", file, i, y[i], 1.0);
    }
  }
}

void 
test_rsac1() { 
  int err;
  float y[1024];
  int lnpts, max;
  float bval, fdelta;

  err = SAC_OK;
  
  ok(err == SAC_OK, "error_code");

  max = 1024;

  /* Unknown file */
  rsac1(FILE_UNKNOWN, &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(FILE_UNKNOWN));
  int_check(err, 108, "rsac1 unknown file");
  
  /* Length too short */
  rsac1(FILE_UNKNOWN, &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(FILE_UNKNOWN)-1);
  int_check(err, 108, "rsac1 unknown file");

  /* Length too long */
  rsac1(FILE_UNKNOWN, &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(FILE_UNKNOWN) * 10);
  int_check(err, 108, "rsac1 unknown file");
  
  /* Length < 0 */
  rsac1(FILE_UNKNOWN, &(y[0]), &lnpts, &bval, &fdelta, &max, &err, strlen(FILE_UNKNOWN));
  int_check(err, 108, "rsac1 unknown file");

  test_rsac1_file(FILE_SMALL);
  test_rsac1_file(FILE_BIG);

}

void
test_rsach_file(char *file) {
  int err;

  err = 0;
  rsach(file, &err, strlen(file));
  ok(err == SAC_OK, "reading file %s, err = %d should be %d", file, err, SAC_OK);

  /* Length too short */
  rsach(file, &err, strlen(file)-2);
  ok(err == 108, "read file %s (too short) (error %d expected %d)",  file, err, 108);

  /* Length too long */
  rsach(file, &err, strlen(file)  * 10);
  ok(err == SAC_OK, "read file %s (too long) (error %d expected %d)", file, err, SAC_OK);

  /* Length < 0 */
  rsach(file, &err, -1);
  ok(err == SAC_OK, "reading file %s (error %d expected %d)", file, err, SAC_OK);

  /* Read the file correctly */
  rsach(file, &err, -1);
  ok(err == SAC_OK, "reading file %s (error %d expected %d)", file, err, SAC_OK);
  
  test_rsac1_header_file(); 
  
}

void
test_rsach() {
  int err;

  err = SAC_OK;
  ok(err == SAC_OK, "error code");

  /* Unknown file */
  rsach(FILE_UNKNOWN, &err, strlen(FILE_UNKNOWN) );
  ok(err != SAC_OK, "unknown file, not ok");
  ok(err == 108, "rsach file does not exist"); /* File does not exist */

  /* Length too short */
  rsach(FILE_UNKNOWN, &err, strlen(FILE_UNKNOWN) - 1);
  ok(err != SAC_OK, "unknown file, not ok");
  ok(err == 108, "rsach file does not exist, length too short");

  /* Length too long */
  rsach(FILE_UNKNOWN, &err, strlen(FILE_UNKNOWN)  * 10);
  ok(err != SAC_OK, "unknown file, not ok");
  ok(err == 108, "rsach file does not exist, length too short");

  /* Length < 0 */
  rsach(FILE_UNKNOWN, &err, -1);
  ok(err != SAC_OK, "unknown file, not ok");
  ok(err == 108, "rsach file does not exist, length < 0");

  /* Test small and large endian files */
  test_rsach_file(FILE_SMALL);
  test_rsach_file(FILE_BIG);

}

