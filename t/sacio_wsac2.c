#include <string.h>
#include <unistd.h>

#include "unit.h"
#include "hdr.h"
#include "dff.h"

#include "sacio_test.h"

void 
test_wsac2() { 
  float x[1024], y[1024];
  int err, max;

  /* Increase the number of data points to 1024 */
  {
    int i;
    for(i = 0; i < 1024; i++) {
      x[i] = y[i] = i;
    }
  }
  max = 1;

  /* Filename Length Special < 0 */
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, -1);
  ok(err == SAC_OK, "wsac2 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 2 * sizeof(float), 
	     "1 data point wsac2 < 0");
  unlink(FILE_WRITE);

  /* Filename Correct Length */
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, 
	strlen(FILE_WRITE));
  ok(err == SAC_OK, "wsac2 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 2 * sizeof(float), 
	     "1 data point wsac2 correct ");
  unlink(FILE_WRITE);

  /* Filename Length too long */
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, 
	strlen(FILE_WRITE) * 3);
  ok(err == SAC_OK, "wsac2 writing sac file with 1 point");
  file_check(FILE_WRITE, SAC_HEADER_SIZE + 2 * sizeof(float), 
	     "1 data point wsac2 too long");
  unlink(FILE_WRITE);

  /* Filename Length too short*/
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, 
	strlen(FILE_WRITE) - 4);
  ok(err == SAC_OK, "wsac2 writing sac file with 1 point");
  file_check("test_write", SAC_HEADER_SIZE + 2 * sizeof(float), 
	     "1 data point wsac2 too short");
  unlink("test_write");

  /* Filename Length = 0 */
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, 0);
  ok(err == 101, "wsac2 writing sac file with 1 point value %d expected %d", 
     err, 101);
  
  /* Inform sacio that the file is now longer */
  max = 1024;
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, -1);
  ok(err == SAC_OK, "wsac2 writing sac file with 1024 data points "
     "value %d expected %d", err, SAC_OK);
  file_check(FILE_WRITE, SAC_HEADER_SIZE + (2 * 1024 * sizeof(float)), 
	     "1024 data points wsac2");
  unlink(FILE_WRITE);  

  /* Write a file with < 0 points */
  max = -1;
  wsac2(FILE_WRITE, &(y[0]), &max, &(x[0]), &err, -1);
  ok(err == 115, "wsac2 writing sac file with -1 data points "
     "value %d expected %d", err, 115);
  ok(!file_exists(FILE_WRITE), " -1 data points");

}


