#include <string.h>
#include <unistd.h>

#include "unit.h"
#include "hdr.h"
#include "bool.h"
#include "dff.h"

#include "sacio_test.h"

void 
test_wsac0() { 
  float x[1024], y[1024];
  int err, max, leven;

  x[0] = 0;
  y[0] = 0;
  leven = TRUE;
  max = 1;

  /* File with one data point */
  newhdr();
  setnhv("npts",   &max,    &err, -1);
  setlhv("leven",  &leven,  &err, -1);
  setfhv("b",      &(x[0]), &err, -1);
  setfhv("e",      &(x[0]), &err, -1);
  setihv("iftype", "itime", &err, -1, -1);

  /* Filename Length Special < 0 */
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink(FILE_WRITE);

  /* Filename Correct Length */
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, strlen(FILE_WRITE));
  ok(err == SAC_OK, "wsac0 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink(FILE_WRITE);

  /* Filename Length too long */
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, strlen(FILE_WRITE) * 3);
  ok(err == SAC_OK, "wsac0 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink(FILE_WRITE);  

  /* Filename Length too short*/
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, strlen(FILE_WRITE) - 4);
  ok(err == SAC_OK, "wsac0 writing sac file with 1 point");
  file_check("test_write", SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink("test_write");  

  /* Filename Length = 0 */
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, 0);
  ok(err == 101, "wsac0 writing sac file with 1 point value %d expected %d", 
     err, 101);
  
  /* Increase the number of data points to 1024 */
  {
    int i;
    for(i = 0; i < 1024; i++) {
      x[i] = y[i] = i;
    }
  }

  /* But do not tell the sacio library about the increased size*/
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file with 1 / 1024 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + (1 * sizeof(float)), "1 data point");
  unlink(FILE_WRITE);  

  /* Inform sacio that the file is now longer */
  max = 1024;
  setnhv("npts",   &max,    &err, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file with 1024 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + (1024 * sizeof(float)), 
	     "1024 data points");
  unlink(FILE_WRITE);  

  /* Write a file with < 0 points */
  max = -1;
  setnhv("npts",   &max,    &err, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == 115, "wsac0 writing sac file with -1 data points value %d expected %d", err, 115);
  ok(!file_exists(FILE_WRITE), "-1 data points");

  /* Write the other types of sac files */
  max = 2;
  setnhv("npts",   &max,    &err, -1);  

  /* Spectral Real / Imaginary */
  setihv("iftype", "irlim", &err, -1, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file (real/imag) with 2 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + max * 2 * sizeof(float),
	     "real/imag");
  unlink(FILE_WRITE);  

  /* Spectral Amplitude / Phase */
  setihv("iftype", "iamph", &err, -1, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file (amp/phase)with 2 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + max * 2 * sizeof(float),
	     "amp/phase");
  unlink(FILE_WRITE);  

  /* IXY spaced file */
  setihv("iftype", "ixy", &err, -1, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file (ixy) with 2 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + (max * 1 * sizeof(float)),
	     "ixy");
  unlink(FILE_WRITE);  


  leven = 0;
  setlhv("leven",  &leven,  &err, -1);  
  setihv("iftype", "ixy", &err, -1, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file (ixy) with 2 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + (max * 2 * sizeof(float)),
	     "ixy");
  unlink(FILE_WRITE);  

  /* unevenly spaced file */
  leven = 0;
  setlhv("leven",  &leven,  &err, -1);  
  setihv("iftype", "itime", &err, -1, -1);  
  wsac0(FILE_WRITE, &(x[0]), &(y[0]), &err, -1);
  ok(err == SAC_OK, "wsac0 writing sac file (uneven/time) with 2 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + max * 2 * sizeof(float),
	     "uneven/time");
  unlink(FILE_WRITE);  

}
