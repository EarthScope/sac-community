      program envelopef
      implicit none

      include "sacf.h"

!     Define the Maximum size of the data Array
      integer MAX
      parameter (MAX=4000)

!     Define the Data Array of size MAX
      real*4 :: ya(MAX), yb(MAX), yc(MAX)

!     Declare Variables used in the rsac1() subroutine
      real beg, delta
      integer na, nb, nc
      character*64 KNAME
      integer nerr

      integer sac_compare

!     Read in the first data file
      kname = 'convolvef_in1.sac'
      call rsac1(kname, ya, na, beg, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',kname
         call exit(-1)
      endif

!     Read in the second data file
      kname = 'convolvef_in2.sac'
      call rsac1(kname, yb, nb, beg, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',kname
         call exit(-1)
      endif

      nc = na + nb - 1
      call convolve(ya, na, yb, nb, yc, nc)

      if(sac_compare("convolvef_out_sac1.sac", yc, nc, 0.0, delta) .ne. 1) then
         write(*,*)'data does not match file'
      endif

      call wsac1("convolvef_out.sac", yc, nc, beg, delta, nerr);

      end program envelopef
