#include <string.h>
#include <unistd.h>

#include "unit.h"
#include "hdr.h"
#include "dff.h"

#include "sacio_test.h"


void 
test_wsac1() { 
  float y[1024];
  int err, max;
  float delta, b;

  /* Increase the number of data points to 1024 */
  {
    int i;
    for(i = 0; i < 1024; i++) {
      y[i] = i;
    }
  }
  b = 0;
  delta = 1.0;
  max = 1;

  /* Filename Length Special < 0 */
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, -1);
  ok(err == SAC_OK, "wsac1 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink(FILE_WRITE);

  /* Filename Correct Length */
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, 
	strlen(FILE_WRITE));
  ok(err == SAC_OK, "wsac1 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink(FILE_WRITE);

  /* Filename Length too long */
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, 
	strlen(FILE_WRITE) * 3);
  ok(err == SAC_OK, "wsac1 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink(FILE_WRITE);

  /* Filename Length too short*/
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, 
	strlen(FILE_WRITE) - 4);
  ok(err == SAC_OK, "wsac1 writing sac file with 1 point");
  file_check("test_write", SAC_HEADER_SIZE + 1 * sizeof(float), "1 data point");
  unlink("test_write");

  /* Filename Length = 0 */
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, 0);
  ok(err == 101, "wsac1 writing sac file with 1 point value %d expected %d", 
     err, 101);
  
  /* Inform sacio that the file is now longer */
  max = 1024;
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, -1);
  ok(err == SAC_OK, "wsac1 writing sac file with 1024 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + (1024 * sizeof(float)), 
	     "1024 data points");
  unlink(FILE_WRITE);  

  /* Write a file with < 0 points */
  max = -1;
  wsac1(FILE_WRITE, &(y[0]), &max, &b, &delta, &err, -1);
  ok(err == 115, "wsac1 writing sac file with -1 data points "
     "value %d expected %d", err, 115);
  ok(!file_exists(FILE_WRITE), "-1 data points");

}
