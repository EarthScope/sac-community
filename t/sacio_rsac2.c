
#include <string.h>
#include <math.h>

#include "amf.h"
#include "unit.h"
#include "hdr.h"
#include "dff.h"

#include "sacio_test.h"

static sac *s;

void
test_rsac2_header_file(int vnpts, float vend) {
  s = sacget_current();
  float_check(s->h->_b, 0.10, "b");
  float_check(s->z->_b, 0.10, "b");
  //float_check(s->h->begin, 0.10, "begin");
  float_check(s->h->_e, vend, "e");
  float_check(s->z->_e, vend, "e");
  //float_check(s->h->ennd, vend, "ennd");
  float_undef(s->h->_delta, "delta");
  float_undef(s->z->_delta, "delta");
  float_check(s->h->depmax, 0.904837, "depmax");
  float_check(s->h->depmin, 0.0, "cmpinc");
  //float_check(s->h->depmx, 0.904837, "depmax");
  //float_check(s->h->depmn, 0.0, "depmn");
  ok(fabs(s->h->depmen - 0.080868) < 1e-4, 
     "depmen value (value %f, expected %f diff %e)", 
     s->h->depmen, 0.01, fabs(s->h->depmen - 0.01));
  //ok(fabs(s->h->fmean - 0.080868) < 1e-4, 
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
  char_undef(s->h->kevnm,"kevnm"); 

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
  int_check(s->h->leven,  0, "leven");
  int_check(s->h->unused27,  0, "lhdr5");
  int_check(s->h->lovrok, 1, "lovrok");
  int_check(s->h->lpspol, 0, "lpspol");

  int_undef(s->h->nevid, "nevid");
  int_undef(s->h->unused15, "nhdr15");
  int_undef(s->h->norid, "norid");
  int_check(s->h->npts, vnpts, "npts");
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
  float_undef(s->h->_t1, "t1");
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
test_rsac2_file(char *file) {
  int i;
  int err;
  float y[1024], x[1024];
  int lnpts, max;
  
  err = SAC_OK;
  max = 1024;
  /* Filename Correct Length */
  fprintf(stderr, "file: <%s>\n", file);
  rsac2(file, &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(file));
  ok(err == SAC_OK, "rsac2 file <%s> correct length err %d expected %d", file, err, SAC_OK);
  
  err = SAC_OK;
  /* Filename Length too Short */
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(file)-1);
  ok(err == 108, "rsac2 file <%s> too short err %d expected %d", file, err, 108);

  /* Filename Length too Long */
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(file)*10);
  ok(err == SAC_OK, "rsac2 file <%s> too long err %d expected %d", file, err, SAC_OK);

  /* Filename Length < 0 */
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == SAC_OK, "rsac2 file <%s> < 0 err %d expected %d", file, err, SAC_OK);
  ok(lnpts == 300, "rsac2 file <%s> truncated npts %d exptected %d", file, lnpts, 300);
  test_rsac2_header_file(300, 876778944.0);

  /* Maximum number of points = 100 */
  max = 300;
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == SAC_OK, "rsac2 file <%s> err %d expected %d", file, err, SAC_OK);
  ok(lnpts == 300, "rsac2 file <%s> npts %d exptected %d", file, lnpts, 300);
  test_rsac2_header_file(300, 876778944.0);

  /* Maximum number of points = 299 */
  max = 299;
  fprintf(stderr, "truncated to 299\n");
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == -803, "rsac2 file <%s> truncated [299] err %d expected %d", file, err, -803);
  ok(lnpts == 299, "rsac2 file <%s> truncated [299] npts %d exptected %d", file, lnpts, 299);
  test_rsac2_header_file(299, 8.121501e+08);

  /* Maximum number of points = 50 */
  max = 50;
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == -803, "rsac2 file <%s> truncated [50] err %d expected %d", file, err, -803);
  ok(lnpts == 50, "rsac2 file <%s> truncated [50] npts %d exptected %d", file, lnpts, 50);
  test_rsac2_header_file(50, 4.260268);

  /* Length < 0 */
  max = 1024;
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == SAC_OK, "rsac2 file <%s> data check err %d expected %d", file, err, SAC_OK);
  ok(lnpts == 300, "rsac2 file <%s> data check npts %d exptected %d", file, lnpts, 300);
  test_rsac2_header_file(300, 876778944.0);
  
  {
    float xx, yy;
    xx = 0.1;
    yy = exp(-xx);
    for(i = 0; i < lnpts; i++) {
      ok( fabs(x[i] - xx) < 1e-4, 
	  "rsac2 file data point x [%d] "
	  "value %f (%e) expected %f (%e)",
	  i,x[i],x[i], xx, xx);
      ok( fabs(y[i] - yy) < 1e-4, 
	  "rsac2 file data point y [%d] "
	  "value %f (%e) expected %f (%e) (%e)",
	  i,y[i],y[i], yy, yy, fabs(y[i] - yy) );
      xx = xx + xx * 1.0/(4.0 * M_PI);
      yy = exp(-xx);
    }
  }
}

void 
test_rsac2() { 
  int err;
  float y[1024], x[1024];
  int lnpts, max;

  err = SAC_OK;
  
  ok(err == SAC_OK, "error_code");

  max = 1024;

  /* Unknown file */
  rsac2(FILE_UNKNOWN, &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(FILE_UNKNOWN));
  int_check(err, 108, "rsac2 unknown file");
  
  /* Length too short */
  rsac2(FILE_UNKNOWN, &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(FILE_UNKNOWN)-1);
  int_check(err, 108, "rsac2 unknown file");

  /* Length too long */
  rsac2(FILE_UNKNOWN, &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(FILE_UNKNOWN) * 10);
  int_check(err, 108, "rsac2 unknown file");
  
  /* Length < 0 */
  rsac2(FILE_UNKNOWN, &(y[0]), &lnpts, &(x[0]), &max, &err, strlen(FILE_UNKNOWN));
  int_check(err, 108, "rsac2 unknown file");

  test_rsac2_file(FILE_SMALL_UNEVEN);
  test_rsac2_file(FILE_BIG_UNEVEN);
  
}
