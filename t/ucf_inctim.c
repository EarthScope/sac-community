
#include <math.h>

#include "unit.h"
#include "ucf.h"

void
test_inctim() {
  int i, j, k, l, q, qq;
  float pre;
  int hour, min, sec, msec;
  int seconds;
  int nhour, nmin, nsec, nmsec, nextday;
  
  hour = 0;
  min  = 0;
  sec  = 0;
  msec = 0;
  nhour = 0;
  nmin = 0;
  nsec = 0;
  nmsec = 0;
  nextday = 0;
  for(l = 0; l < 1; l++) {
    for(k = 0; k < 24; k++) { 
      for(j = 0; j < 60; j++) {
        for(i = 0; i < 60; i++) {
          seconds = i + (j * 60) + (k * 60 * 60);
          inctimf(hour, min, sec, msec, seconds, 0.0,
                  &nhour, &nmin, &nsec, &nmsec, &nextday);
          ok(nextday == l, "inctimf nextday");
          ok(nhour   == k, "inctimf hour");
          ok(nmin    == j, "inctimf min");
          ok(nsec    == i, "inctimf sec");
          ok(nmsec   == 0, "inctimf msec");
        }
      }
    }
  }
  i = 10;
  j = 0;
  k = 0;
  l = 0;
  for(q = 0; q < 10*1000; q++) {
    inctimf(hour, min, sec, msec, 0.0, q,
            &nhour, &nmin, &nsec, &nmsec, &nextday);
    i = floor(q / 1000);
    qq = q - i*1000;
    ok(nextday == l, "inctimf nextday");
    ok(nhour   == k, "inctimf hour");
    ok(nmin    == j, "inctimf min");
    ok(nsec    == i, "inctimf sec: %d should be %d", nsec, i);
    ok(nmsec   == qq, "inctim msec: %d should be %d (%d => sec %d/%d)", nmsec, qq, q, i,nsec);
  }
}
