
#include <string.h>
#include <math.h>

#include "unit.h"
#include "hdr.h"
#include "dff.h"

#include "sacio_test.h"


void
test_rsac2_header_file(int vnpts, float vend) {
  float_check(*b, 0.10, "b");
  float_check(*begin, 0.10, "begin");
  float_check(*e, vend, "e");
  float_check(*ennd, vend, "ennd");
  float_undef(*delta, "delta");
  float_check(*depmax, 0.904837, "depmax");
  float_check(*depmin, 0.0, "cmpinc");
  float_check(*depmx, 0.904837, "depmax");
  float_check(*depmn, 0.0, "depmn");
  ok(fabs(*depmen - 0.080868) < 1e-4, 
     "depmen value (value %f, expected %f diff %e)", 
     *depmen, 0.01, fabs(*depmen - 0.01));
  ok(fabs(*fmean - 0.080868) < 1e-4, 
     "fmean value (value %f, expected %f diff %e)", 
     *fmean, 0.01, fabs(*fmean - 0.01));

  float_undef(*a, "a");
  float_undef(*az, "az");
  float_undef(*baz, "baz");
  float_undef(*cmpinc, "cmpinc");
  float_undef(*cmpaz, "cmpaz");
  float_undef(*dist, "dist");
  float_undef(*evdp, "evdp");
  float_undef(*evla, "evla");
  float_undef(*evlo, "evlo");
  float_undef(*evel, "evel");
  float_undef(*f,    "f");
  float_undef(*fini, "fini");
  float_undef(*fhdr64, "fhdr64");
  float_undef(*fhdr65, "fhdr65");
  float_undef(*fhdr66, "fhdr66");
  float_undef(*fhdr67, "fhdr67");
  float_undef(*fhdr68, "fhdr68");
  float_undef(*fhdr69, "fhdr69");
  float_undef(*fhdr70, "fhdr70");
  float_undef(*fmt,    "fmt");
  float_undef(*gcarc, "gcarc");

  int_undef(*idep, "idep");
  int_undef(*ievreg, "ievreg");
  int_undef(*ievtyp, "ievtyp");
  int_check(*iftype, 1, "file type");
  int_undef(*imagtyp, "imagtyp");
  int_undef(*imagsrc, "imagsrc");
  int_undef(*ihdr13, "ihdr13");
  int_undef(*ihdr14, "ihdr14");
  int_undef(*ihdr15, "ihdr15");
  int_undef(*ihdr16, "ihdr16");
  int_undef(*ihdr17, "ihdr17");
  int_undef(*ihdr18, "ihdr18");
  int_undef(*ihdr19, "ihdr19");
  int_undef(*ihdr20, "ihdr20");
  int_undef(*ihdr4, "ihdr4");
  int_undef(*iinst, "iinst");
  int_undef(*iqual, "iqual");
  int_undef(*istreg, "istreg");
  int_undef(*isynth, "isynth");
  int_undef(*iztype, "iztype");

  char_check(kstnm, "sta     ", kstnm); 
  char_undef(kevnm,"kevnm"); 

  char_undef(khole, "khole");
  char_undef(ka, "ka");
  char_undef(ko, "ko");
  char_undef(kt0, "kt0");
  char_undef(kt1, "kt1");
  char_undef(kt2, "kt2");
  char_undef(kt3, "kt3");
  char_undef(kt4, "kt4");
  char_undef(kt5, "kt5");
  char_undef(kt6, "kt6");
  char_undef(kt7, "kt7");
  char_undef(kt8, "kt8");
  char_undef(kt9, "kt9");
  char_undef(kf, "kf");
  char_undef(kuser0, "kuser0");
  char_undef(kuser1, "kuser1");
  char_undef(kuser2, "kuser2");
  char_check(kcmpnm, "Q       ", "kcmpnm");
  char_undef(knetwk, "knetwk");
  char_undef(kdatrd, "kdatrd");
  char_undef(kinst, "kinst");

  int_check(*lcalda, 1, "lcalda");
  int_check(*leven,  0, "leven");
  int_check(*lhdr5,  0, "lhdr5");
  int_check(*lovrok, 1, "lovrok");
  int_check(*lpspol, 0, "lpspol");

  int_undef(*nevid, "nevid");
  int_undef(*nhdr15, "nhdr15");
  int_undef(*norid, "norid");
  int_check(*npts, vnpts, "npts");
  int_check(*nvhdr, 6, "nvhdr");
  int_undef(*nwfid, "nwfid");
  int_undef(*nxsize, "nxsize");
  int_undef(*nysize, "nysize");
  int_undef(*nzdttm, "nzdttm");
  int_undef(*nzjday, "nzjday");
  int_undef(*nzmin, "nzmin");
  int_undef(*nzmsec, "nzmsec");
  int_undef(*nzsec, "nzsec");
  int_undef(*nzyear, "nzyear");

  float_undef(*o, "o");
  float_undef(*odelta, "odelta");
  float_undef(*origin, "origin");
  float_undef(*resp0, "resp0");
  float_undef(*resp1, "resp1");
  float_undef(*resp2, "resp2");
  float_undef(*resp3, "resp3");
  float_undef(*resp4, "resp4");
  float_undef(*resp5, "resp5");
  float_undef(*resp6, "resp6");
  float_undef(*resp7, "resp7");
  float_undef(*resp8, "resp8");
  float_undef(*resp9, "resp9");

  float_undef(*sb, "sb");
  float_undef(*scale, "scale");
  float_undef(*sdelta, "sdelta");
  float_undef(*stdp, "stdp");
  float_undef(*stel, "stel");
  float_undef(*stla, "stla");
  float_undef(*stlo, "stlo");
  float_undef(*t0, "t0");
  float_undef(*t1, "t1");
  float_undef(*t2, "t2");
  float_undef(*t3, "t3");
  float_undef(*t4, "t4");
  float_undef(*t5, "t5");
  float_undef(*t6, "t6");
  float_undef(*t7, "t7");
  float_undef(*t8, "t8");
  float_undef(*t9, "t9");
  float_undef(*time0, "time0");
  float_undef(*time1, "time1");
  float_undef(*time2, "time2");
  float_undef(*time3, "time3");
  float_undef(*time4, "time4");
  float_undef(*time5, "time5");
  float_undef(*time6, "time6");
  float_undef(*time7, "time7");
  float_undef(*time8, "time8");
  float_undef(*time9, "time9");
  float_undef(*user0, "user0");
  float_undef(*user1, "user1");
  float_undef(*user2, "user2");
  float_undef(*user3, "user3");
  float_undef(*user4, "user4");
  float_undef(*user5, "user5");
  float_undef(*user6, "user6");
  float_undef(*user7, "user7");
  float_undef(*user8, "user8");
  float_undef(*user9, "user9");

  float_undef(*xmaximum, "xmaximum");
  float_undef(*xminimum, "xminimum");
  float_undef(*ymaximum, "ymaximum");
  float_undef(*yminimum, "yminimum");
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
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == -803, "rsac2 file <%s> truncated [299] err %d expected %d", file, err, -803);
  ok(lnpts == 299, "rsac2 file <%s> truncated [299] npts %d exptected %d", file, lnpts, 299);
  test_rsac2_header_file(299, 752285120.0);

  /* Maximum number of points = 50 */
  max = 50;
  rsac2(file,  &(y[0]), &lnpts, &(x[0]), &max, &err, -1);
  ok(err == -803, "rsac2 file <%s> truncated [50] err %d expected %d", file, err, -803);
  ok(lnpts == 50, "rsac2 file <%s> truncated [50] npts %d exptected %d", file, lnpts, 50);
  test_rsac2_header_file(50, 0.0);

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
  ok(cmhdr.fundef == SAC_FLOAT_UNDEFINED, " Header Float is undefined");
  

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
