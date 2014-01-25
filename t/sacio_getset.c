
#include <string.h>
#include <math.h>

#include "unit.h"
#include "sacio.h"

#include "sacio_test.h"

#include <config.h>

#if HAVE_LIMITS_H
# include <limits.h>
#endif
 
#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif


void 
test_gethv() { 
  int err;
  int i;
  float y[1024];
  int lnpts, max;
  float bval, fdelta;
  float value;
  int  ival;
  char cval[9];
  char *file;

  file = FILE_SMALL;
  max = 1024;
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, 
	&err, -1);
  ok(err == SAC_OK, "rsac1 file <%s> data check err %d expected %d", 
     file, err, SAC_OK);
  ok(lnpts == 100, "rsac1 file <%s> data check npts %d exptected %d",
     file, lnpts, 100);
  test_rsac1_header_file();

  {
    char *names[] = { "delta", "b", "e", "depmin", "depmax", "depmen" };
    float values[] = { 1.0, 0.0, 99.0, 0.0, 1.0, 0.01 };
    char *bnames[] = {"a", "az", "baz", "cmpinc", "cmpaz", "dist", "evdp",
		      "evla", "evlo", "evel", "f", "fmt", "gcarc", "o",
		      "odelta", "resp0", "resp1", "resp2", "resp3", "resp4",
		      "resp5", "resp6", "resp7", "resp8", "resp9",
		      "sb", "scale", "sdelta", "stdp", "stel", "stla", "stlo",
		      "t0", "t1", "t2", "t3", "t4", 
		      "t5", "t6", "t7", "t8", "t9", 
		      "user0", "user1", "user2", "user3", "user4", "user5", 
		      "user6", "user7", "user8", "user9", 
		      "xmaximum", "xminimum", "ymaximum", "yminimum", };

    /* getfhv -- Defined */
    for(i = 0; i < (int)(sizeof(names)/sizeof(char*)); i++) {
      getfhv(names[i], &value, &err, -1);
      float_check(value, values[i], names[i]);
      int_check(err, SAC_OK, names[i]);
    }
    /* getfhv -- Undefined */
    for(i = 0; i < (int)(sizeof(bnames)/sizeof(char*)); i++) {
      getfhv(bnames[i], &value, &err, -1);
      float_undef(value, bnames[i]);
      int_check(err, 1336, bnames[i]);
    }
  }
  {
    char *inames[] = {"lcalda", "leven", "lhdr5", "lovrok", "lpspol" };
    int   ivalues[] = {1, 1, 0, 1, 0 };
    /* getlhv -- Defined */
    for(i = 0; i < (int)(sizeof(inames)/sizeof(char*)); i++) {
      getlhv(inames[i], &ival, &err, -1);
      int_check(ival, ivalues[i], inames[i]);
      int_check(err, SAC_OK, inames[i]);
    }
    /* getlhv -- Undefined  -- None */
  }
  {
    char *nnames[] = {"npts", "nvhdr" };
    int   nvalues[] = {100, 6 };
    char *nbnames[] = { "nevid", "norid", "nwfid", "nxsize", "nysize", 
			"nzjday", "nzmin", "nzmsec", "nzyear" };
    /* getnhv -- Defined */
    for(i = 0; i < (int)(sizeof(nnames)/sizeof(char*)); i++) {
      getnhv(nnames[i], &ival, &err, -1);
      int_check(ival, nvalues[i], nnames[i]);
      int_check(err, SAC_OK, nnames[i]);
    }
    /* getnhv -- Undefined */
    for(i = 0; i < (int)(sizeof(nbnames)/sizeof(char*)); i++) {
      getnhv(nbnames[i], &ival, &err, -1);
      int_undef(ival, nbnames[i]);
      int_check(err, 1336, nbnames[i]);
    }
  }
  {
    char *lnames[] = { "iftype" };
    char *lvalues[] = { "ITIME   " };
    char *lbnames[] = { "idep", "ievreg", "ievtyp", "imagtyp", "imagsrc",
			"iinst", "iqual", "isynth", "iztype", "istreg" };
    /* getihv -- Defined */
    for(i = 0; i < (int)(sizeof(lnames)/sizeof(char*)); i++) {
      getihv(lnames[i], &(cval[0]), &err, -1, 9);
      int_check(err, SAC_OK, lnames[i]);
      char_check(&(cval[0]), lvalues[i], lnames[i]);
    }
    /* getihv -- Undefined */
    for(i = 0; i < (int)(sizeof(lbnames)/sizeof(char*)); i++) {
      getihv(lbnames[i], &(cval[0]), &err, -1, 9);
      int_check(err, 1336, lbnames[i]);
      char_check(&(cval[0]), "UNDEFINE", lbnames[i]);
    }
  }
  getfhv("undef", &value, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getfhv(LONG_TEXT, &value, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getfhv("", &value, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getfhv("undef", &value, &err, 10000);
  int_check(err, 1337, "non-existant header value");
  getfhv("undef", &value, &err, 6);
  int_check(err, 1337, "non-existant header value");
  getfhv("undef", &value, &err, 2);
  int_check(err, 1337, "non-existant header value");
  getfhv("undef", &value, &err, 0);
  int_check(err, 1337, "non-existant header value");

  getlhv("undef", &ival, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getlhv(LONG_TEXT, &ival, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getlhv("", &ival, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getlhv("undef", &ival, &err, 1000);
  int_check(err, 1337, "non-existant header value");
  getlhv("undef", &ival, &err, 6);
  int_check(err, 1337, "non-existant header value");
  getlhv("undef", &ival, &err, 2);
  int_check(err, 1337, "non-existant header value");
  getlhv("undef", &ival, &err, 0);
  int_check(err, 1337, "non-existant header value");

  getnhv("undef", &ival, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getnhv(LONG_TEXT, &ival, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getnhv("", &ival, &err, -1);
  int_check(err, 1337, "non-existant header value");
  getnhv("undef", &ival, &err, 1000);
  int_check(err, 1337, "non-existant header value");
  getnhv("undef", &ival, &err, 6);
  int_check(err, 1337, "non-existant header value");
  getnhv("undef", &ival, &err, 2);
  int_check(err, 1337, "non-existant header value");
  getnhv("undef", &ival, &err, 0);
  int_check(err, 1337, "non-existant header value");

  cval[0] = '\0';
  getihv("undef", &(cval[0]), &err, -1, 8);
  int_check(err, 1337, "non-existant header value");
  cval[0] = '\0';
  getihv(LONG_TEXT, &(cval[0]), &err, -1, 8);
  int_check(err, 1337, "non-existant header value");
  cval[0] = '\0';
  getihv("", &(cval[0]), &err, -1, 8);
  int_check(err, 1337, "non-existant header value");
  cval[0] = '\0';
  getihv("undef", &(cval[0]), &err, 1000, 8);
  int_check(err, 1337, "non-existant header value");
  cval[0] = '\0';
  getihv("undef", &(cval[0]), &err, 6, 8);
  int_check(err, 1337, "non-existant header value");
  cval[0] = '\0';
  getihv("undef", &(cval[0]), &err, 2, 8);
  int_check(err, 1337, "non-existant header value");
  cval[0] = '\0';
  getihv("undef", &(cval[0]), &err, 0, 8);
  int_check(err, 1337, "non-existant header value");
  char_check(cval, "ILLEGAL", "illegal value for getihv");

  /* Check output arguements for getihv() */
  cval[0] = 0;
  getihv("iftype", &(cval[0]), &err, -1, 1);
  int_check(err, SAC_OK, "non-existant header value");
  char_check(cval, "", "empty iftype");
  cval[0] = 0;
  getihv("iftype", &(cval[0]), &err, -1, 2);
  int_check(err, SAC_OK, "non-existant header value");
  char_check(cval, "I", "empty iftype");
  cval[0] = 0;
  getihv("iftype", &(cval[0]), &err, -1, 9);
  int_check(err, SAC_OK, "non-existant header value");
  char_check(cval, "ITIME   ", "empty iftype");
  cval[0] = 0;
  getihv("iftype", &(cval[0]), &err, -1, 1);
  int_check(err, SAC_OK, "non-existant header value");
  char_check(cval, "", "empty iftype");
  cval[0] = 0;
  getihv("iftype", &(cval[0]), &err, -1, 0);
  int_check(err, SAC_OK, "non-existant header value");
  char_check(cval, "", "empty iftype");
  cval[0] = 0;
  getihv("iftype", &(cval[0]), &err, -1, -1);
  int_check(err, SAC_OK, "non-existant header value");
  char_check(cval, "", "empty iftype");
}

void 
test_sethv() { 
  int err;
  int i;
  float y[1024];
  int lnpts, max;
  float bval, fdelta;
  char *file;
  
  file = FILE_SMALL;
  max = 1024;
  rsac1(file,  &(y[0]), &lnpts, &bval, &fdelta, &max, 
	&err, -1);
  ok(err == SAC_OK, "rsac1 file <%s> data check err %d expected %d", 
     file, err, SAC_OK);
  ok(lnpts == 100, "rsac1 file <%s> data check npts %d exptected %d",
     file, lnpts, 100);
  test_rsac1_header_file();
  
  {
    float fin, fout;
    char *fnames[] = { "delta", "depmin", "depmax", "scale", "odelta", "b",
		       "e", "o", "a", "fmt", "t0","t1","t2","t3","t4","t5",
		       "t6","t7","t8","t9","f","resp0","resp1","resp2",
		       "resp3","resp4","resp5","resp6","resp7","resp8",
		       "resp9","stlo","stla","stel","stdp","evla","evlo",
		       "evel","evdp","mag","user0","user0","user1","user2",
		       "user3","user4","user5","user6","user7","user8",
		       "user9","dist","az","baz","gcarc","sb","sdelta",
		       "depmen","cmpaz","cmpinc","xminimum","xmaximum",
		       "yminimum","ymaximum", "adjtm","fhdr65","fhdr66",
		       "fhdr67","fhdr68","fhdr69","fhdr70",};
    fin = M_PI;
    for(i = 0; i < (int)(sizeof(fnames)/sizeof(char*)); i++) {
      fout = M_PI/2.0;
      setfhv(fnames[i], &fin, &err, -1);
      ok(err == SAC_OK, "setfhv() for varialble %s value %f",fnames[i],fin);
      getfhv(fnames[i], &fout, &err, -1);
      ok(err == SAC_OK, "setfhv()->getfhv() for variable %s err %d expected %d",
	 fnames[i], err, SAC_OK);
      ok(fin == fout, "setfhv()->getfhv() for varialble %s in %f out %f",
	 fnames[i],fin, fout);
    }
    setfhv("undef", &fin, &err, -1);
    ok(err == 1337, "setfhv() undefined header variable");
    setfhv("undef", &fin, &err, 1000);
    ok(err == 1337, "setfhv() undefined header variable");
    setfhv("undef", &fin, &err, 3);
    ok(err == 1337, "setfhv() undefined header variable");
    setfhv("delta", &fin, &err, 5);
    ok(err == SAC_OK, "setfhv for delta");
    setfhv("delta", &fin, &err, 50);
    ok(err == SAC_OK, "setfhv for delta");
    setfhv("delta", &fin, &err, 4);
    ok(err == 1337, "setfhv for delta");
    setfhv("delta", &fin, &err, 3);
    ok(err == 1337, "setfhv for delta");
  }
  {
    int iin, iout;
    char *inames[] = {"leven", "lpspol", "lovrok", "lcalda", "lhdr5" };
    iin = 1;
    for(i = 0; i < (int)(sizeof(inames)/sizeof(char*)); i++) {
      iout = 0;
      setlhv(inames[i], &iin, &err, -1);
      ok(err == SAC_OK, "setlhv() for variable %s value %d", inames[i], iin);
      getlhv(inames[i], &iout, &err, -1);
      ok(err == SAC_OK, "setlhv()->getlhv for variable %s err %d expected %d", 
	 inames[i], err, SAC_OK);
      ok(iin == iout, "setlhv()->getlhv for variable %s in %d out %d",
	 inames[i], iin, iout);
    }
    setlhv("undef", &iin, &err, -1);
    ok(err == 1337, "setlhv() undefine header variable");
    setlhv("undef", &iin, &err, 1000);
    ok(err == 1337, "setlhv() undefine header variable");
    setlhv("undef", &iin, &err, 3);
    ok(err == 1337, "setlhv() undefine header variable");
    setlhv("leven", &iin, &err, 5);
    ok(err == SAC_OK, "setlhv() for leven, just right");
    setlhv("leven", &iin, &err, -1);
    ok(err == SAC_OK, "setlhv() for leven < 0 ");
    setlhv("leven", &iin, &err, 1000);
    ok(err == SAC_OK, "setlhv() for leven too long");
    setlhv("leven", &iin, &err, 3);
    ok(err == 1337, "setlhv() for leven too short ");

    iin = 2;
    setlhv("leven", &iin, &err, -1);
    ok(err == SAC_OK, "setlhv() for leven, incorrect value err %d expected %d",
       err, SAC_OK);
    getlhv("leven", &iout, &err, -1);
    ok(err == SAC_OK, "getlhv() for leven, incorrect value err %d expected %d",
       err, SAC_OK);
    ok(iin == iout, "get/set lhv() for incorrect value in %d out %d", 
       iin, iout);
  }
  {
    int iin, iout;
    char *inames[] = { "nzyear", "nzjday", "nzhour", "nzmin", "nzsec", 
		       "nzmsec", "nvhdr", "norid", "nevid", "npts",
		       "nwfid", "nxsize", "nysize", };
    iin = 42;
    for(i = 0; i < (int)(sizeof(inames)/sizeof(char*)); i++) {
      iout = -34;
      setnhv(inames[i], &iin, &err, -1);
      ok(err == SAC_OK, "setnhv() for variable %s value %d", inames[i], iin);
      getnhv(inames[i], &iout, &err, -1);
      ok(err == SAC_OK, "set/get nhv() for variable %s err %d expected %d", 
	 inames[i], err, SAC_OK);
      ok(iin == iout, "set/get lhv() for variable %s in %d out %d",
	 inames[i], iin, iout);
    }
    setnhv("undef", &iin, &err, -1);
    ok(err == 1337, "setlhv() undefine header variable");
    setnhv("undef", &iin, &err, 1000);
    ok(err == 1337, "setnhv() undefine header variable");
    setnhv("undef", &iin, &err, 3);
    ok(err == 1337, "setnhv() undefine header variable");
    setnhv("nzyear", &iin, &err, 6);
    ok(err == SAC_OK, "setnhv() for nzyear, just right");
    setnhv("nzyear", &iin, &err, -1);
    ok(err == SAC_OK, "setnhv() for nzyear < 0 ");
    setnhv("nzyear", &iin, &err, 1000);
    ok(err == SAC_OK, "setnhv() for nzyear too long");
    setnhv("nzyear", &iin, &err, 3);
    ok(err == 1337, "setnhv() for nzyear too short ");
    
    iin = -34;
    setnhv("nzyear", &iin, &err, 1000);
    ok(err == SAC_OK, "setnhv() for nzyear negative");
    getnhv("nzyear", &iout, &err, -1);
    ok(err == SAC_OK, "set/get nhv() for variable %s err %d expected %d (neg)", 
       "nzyear", err, SAC_OK);
    ok(iin == iout, "set/get lhv() for variable %s in %d out %d (neg)",
       "nzyear", iin, iout);

    iin = INT32_MIN;
    setnhv("nzyear", &iin, &err, 1000);
    ok(err == SAC_OK, "setnhv() for nzyear negative");
    getnhv("nzyear", &iout, &err, -1);
    ok(err == SAC_OK, "set/get nhv() for variable %s err %d expected %d (neg)", 
       "nzyear", err, SAC_OK);
    ok(iin == iout, "set/get lhv() for variable %s in %d out %d (neg)",
       "nzyear", iin, iout);

    iin = INT32_MAX;
    setnhv("nzyear", &iin, &err, 1000);
    ok(err == SAC_OK, "setnhv() for nzyear negative");
    getnhv("nzyear", &iout, &err, -1);
    ok(err == SAC_OK, "set/get nhv() for variable %s err %d expected %d (neg)", 
       "nzyear", err, SAC_OK);
    ok(iin == iout, "set/get lhv() for variable %s in %d out %d (neg)",
       "nzyear", iin, iout);
  }
  /* Enumerated Fields */
  {
    char kin[9], kout[9];
    char *inames[] = {"iftype", "idep", "iztype", "ihdr4", "iinst", "istreg",
		      "ievreg", "ievtyp", "iqual", "isynth", "imagtyp", 
		      "imagsrc", "ihdr13","ihdr14","ihdr15","ihdr16",
		      "ihdr17","ihdr18","ihdr19","ihdr20", };
    kin[0] = 0;
    strncat(&(kin[0]), "ITIME   ", 8);
    for(i = 0; i < (int)(sizeof(inames)/sizeof(char*)); i++) {
      kout[0] = 0;
      setihv(inames[i], &(kin[0]), &err, -1, -1);
      ok(err == SAC_OK, "setihv() for variable %s value %s", inames[i], kin);
      getihv(inames[i], &(kout[0]), &err, -1, 9);
      ok(err == SAC_OK, "set/get ihv() for variable %s err %d expected %d", 
	 inames[i], err, SAC_OK);
      ok(strncmp(kin, kout,8) == 0,
	 "set/get ihv() for variable %s in <%s> out <%s>",
	 inames[i], kin, kout);
    }
    /* Incorect Key Value */
    setihv("undef", &kin[0], &err, -1, -1);
    ok(err == 1337, "setihv() undefine header variable");
    setihv("undef", &kin[0], &err, 1000, -1);
    ok(err == 1337, "setihv() undefine header variable");
    setihv("undef", &kin[0], &err, 3, -1);
    ok(err == 1337, "setihv() undefine header variable");

    /* Incorrect Key Length */
    setihv("iftype", &kin[0], &err, 6, -1);
    ok(err == SAC_OK, "setihv() for iftype, just right");
    setihv("iftype", &kin[0], &err, -1, -1);
    ok(err == SAC_OK, "setihv() for iftype < 0 ");
    setihv("iftype", &kin[0], &err, 1000, -1);
    ok(err == SAC_OK, "setihv() for iftype too long");
    setihv("iftype", &kin[0], &err, 3, -1);
    ok(err == 1337, "setihv() for iftype too short ");

    /* Incorrect Key Value Length */
    setihv("iftype", &kin[0], &err, -1, -1);
    int_check(err, SAC_OK, "setihv() for iftype, just right");
    setihv("iftype", &kin[0], &err, -1, 1000);
    int_check(err, SAC_OK, "setihv() for iftype < 0 ");
    setihv("iftype", &kin[0], &err, -1, 5);
    int_check(err, SAC_OK, "setihv() for iftype too long");
    setihv("iftype", &kin[0], &err, -1, 3);
    int_check(err, 1365, "setihv() for iftype, too short");

    /* Incorrect Key Value */
    kin[0] = 0;
    strncat(&(kin[0]), "ITIMEABC", 8);
    setihv("iftype", &kin[0], &err, 6, -1);
    int_check(err, 1365, "setihv() for iftype, incorrerct key value");
    setihv("iftype", &kin[0], &err, -1, -1);
    int_check(err, 1365, "setihv() for iftype, incorrect key value");
    setihv("iftype", &kin[0], &err, 1000, -1);
    int_check(err, 1365, "setihv() for iftype, incorrect key value");
    setihv("iftype", &kin[0], &err, 3, -1);
    int_check(err, 1337, "setihv() for iftype, incorrect key value ");
    
  }
  {
    char kin[17], kout[17];
    char *knames[] = { "kstnm", "kevnmc", "khole", "ko","ka",
		       "kt0","kt1","kt2","kt3","kt4","kt5","kt6","kt7",
		       "kt8","kt9","kf","kuser0","kuser1","kuser2",
		       "kcmpnm","knetwk","kdatrd","kinst"};
    kin[0] = 0;
    strncat(&kin[0], "12345678", 8);
    for(i = 0; i < (int)(sizeof(knames)/sizeof(char*)); i++) {
      kout[0] = 0;
      setkhv(knames[i], &(kin[0]), &err, -1, -1);
      ok(err == SAC_OK, "setkhv() for variable %s value %s", knames[i], kin);
      getkhv(knames[i], &(kout[0]), &err, -1, 9);
      ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
         knames[i], err, SAC_OK);
      ok(strncmp(kin, kout,8) == 0,
         "set/get khv() for variable %s in <%s> out <%s>",
         knames[i], kin, kout);
      ok(strlen(kin) == strlen(kout), "set/get khv() for variable %s in <%s> out <%s>",
         knames[i], kin, kout);
    }
    /* Incorrect Key Value */
    ok(err == SAC_OK, "before incorrect key value");
    setkhv("undef", &kin[0], &err, -1, -1);
    ok(err == 1337, "setkhv() undefine header variable");
    setkhv("undef", &kin[0], &err, 1000, -1);
    ok(err == 1337, "setkhv() undefine header variable");
    setkhv("undef", &kin[0], &err, 3, -1);
    ok(err == 1337, "setkhv() undefine header variable");
    
    /* Incorrect Key Length */
    setkhv("kstnm", &kin[0], &err, 5, -1);
    ok(err == SAC_OK, "setkhv() for kstnm, just right");
    setkhv("kstnm", &kin[0], &err, -1, -1);
    ok(err == SAC_OK, "setkhv() for kstnm < 0 ");
    setkhv("kstnm", &kin[0], &err, 1000, -1);
    ok(err == SAC_OK, "setkhv() for kstnm too long");
    setkhv("kstnm", &kin[0], &err, 3, -1);
    ok(err == 1337, "setkhv() for kstnm too short ");
    
    /* Incorrect Key Value Length */
    setkhv("kstnm", &kin[0], &err, -1, -1);
    int_check(err, SAC_OK, "setkhv() for kstnm, just right");
    getkhv("kstnm", &(kout[0]), &err, -1, 9);
    ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
       "kstnm", err, SAC_OK);
    ok(strncmp(kin, kout,8) == 0,
       "set/get khv() for variable %s in <%s> out <%s>",
       "kstnm", kin, kout);

    setkhv("kstnm", &kin[0], &err, -1, 1000);
    int_check(err, SAC_OK, "setkhv() for kstnm < 0 ");
    getkhv("kstnm", &(kout[0]), &err, -1, 9);
    ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
       "kstnm", err, SAC_OK);
    ok(strncmp(kin, kout,8) == 0,
       "set/get khv() for variable %s in <%s> out <%s>",
       "kstnm", kin, kout);

    setkhv("kstnm", &kin[0], &err, -1, 5);
    int_check(err, SAC_OK, "setkhv() for kstnm too long");
    getkhv("kstnm", &(kout[0]), &err, -1, 9);
    ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
       "kstnm", err, SAC_OK);
    ok(strncmp("12345   ", kout,8) == 0,
       "set/get khv() for variable %s in <%s> out <%s>",
       "kstnm", kin, kout);

    setkhv("kstnm", &kin[0], &err, -1, 3);
    int_check(err, SAC_OK, "setkhv() for kstnm, too short");
    getkhv("kstnm", &(kout[0]), &err, -1, 9);
    ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
       "kstnm", err, SAC_OK);
    ok(strncmp("123     ", kout,8) == 0,
       "set/get khv() for variable %s in <%s> out <%s>",
       "kstnm", kin, kout);

    /* Long Character Field */
    kin[0] = 0;
    strcat(&kin[0], "1234567890123456");
    kin[16] = 0;
    setkhv("kevnm", &kin[0], &err, -1, -1);
    int_check(err, SAC_OK, "setkhv() for kstnm, too short");
    getkhv("kevnm", &(kout[0]), &err, -1, 16);
    ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
       "kevnm", err, SAC_OK);
    ok(strncmp(kin, kout,16) == 0,
       "set/get khv() for variable %s in <%s> out <%s>",
       "kevnm", kin, kout);
    /* Set a short character string with a long character string */
    setkhv("kstnm", &kin[0], &err, -1, -1);
    int_check(err, SAC_OK, "setkhv() for kstnm, too short");
    getkhv("kstnm", &(kout[0]), &err, -1, 16);
    ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
       "kstnm", err, SAC_OK);
    ok(strncmp(kin, kout,16) != 0,
       "set/get khv() for variable %s in <%s> out <%s>",
       "kstnm", kin, kout);

    {
      char ktmp[18];
      int i;
      memset(kin,0,17);
      strncat(&kin[0], "1234567890123456", 16);
      kin[16] = 0;
      setkhv("kevnm", &kin[0], &err, -1, -1);
      int_check(err, SAC_OK, "setkhv() for kstnm, too short");
      getkhv("kevnm", &(kout[0]), &err, -1, 0);      
      ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
         "kevnm", err, SAC_OK);
      for(i = 1; i < 17; i++) {
        strcpy(&ktmp[0], &kin[0]);
        ktmp[i] = ' ';
        ktmp[i] = 0;
        memset(kout, 0, 17);
        /* Set a short character string with a long character string */
        getkhv("kevnm", &(kout[0]), &err, -1, i);
        ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
           "kevnm", err, SAC_OK);
        ok(strncmp(ktmp, kout,i) == 0,
           "set/get khv() for variable %s in <%s> out <%s> %d",
           "kevnm", ktmp, kout, i);
        ok(strlen(ktmp) == strlen(kout), "set/get khv() for variable %s in <%s> out <%s>",
           "kevnm", ktmp, kout);
      }
      
      memset(kin,0,17);
      strncat(&kin[0], "1234567890123456",16);
      kin[8] = 0;
      setkhv("kevnm", &kin[0], &err, -1, -1);
      int_check(err, SAC_OK, "setkhv() for kstnm, too short");
      for(i = 1; i < 9; i++) {
        strcpy(&ktmp[0], &kin[0]);
        ktmp[i] = ' ';
        ktmp[i] = 0;
        memset(kout, 0, 17);
        /* Set a short character string with a long character string */
        getkhv("kstnm", &(kout[0]), &err, -1, i);
        ok(err == SAC_OK, "set/get khv() for variable %s err %d expected %d", 
           "kstnm", err, SAC_OK);
        ok(strncmp(ktmp, kout,i) == 0,
           "set/get khv() for variable %s in <%s> out <%s> %d",
           "kstnm", ktmp, kout, i);
        ok(strlen(ktmp) == strlen(kout), "set/get khv() for variable %s in <%s> out <%s>",
           "kstnm", ktmp, kout);
      }
    }
    {
      int i, j;
      char ktmp[18];
      for(i = 1; i <= 8; i++) {
        kin[0] = 0;
        strncat(&kin[0], "1234567890", i);
        kin[i+1] = 0;
        setkhv("kstnm", &kin[0], &err, -1, -1);
        int_check(err, SAC_OK, "setkhv() for kstnm, variable length");
        for(j = 1; j < 16; j++) {
          memset(kout, 0, 17);
          memset(ktmp, ' ', 17);
          ktmp[17] = 0;
          strncpy(ktmp, kin, i);
          ktmp[j+1] = 0;
          if(j > 8) {
            ktmp[8] = 0;
          }
          getkhv("kstnm", &kout[0], &err, -1, j);
          int_check(err, SAC_OK, "getkhv() for kstnm, variable length");
          ok(strncmp(ktmp, kout, j) == 0,
             "set/get khv() for variable %s in[%d] <%s> out[%d] <%s> ",
             "kstnm", i, ktmp, j, kout);
        }
      }
    }
  }
}
