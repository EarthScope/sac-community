      program convolvef
      
!     gfortran -o convolvef convolvef.f -I/usr/local/sac/include
!     -L/usr/local/sac/lib -lsacio -lsac

      implicit none

      include "sacf.h"

!     Define the Maximum size of the data Array
      integer MAX
      parameter (MAX=4000)

!     Define the Data Array of size MAX
      real yarray1, yarray2, ytmp, y12conv
      dimension yarray1(MAX), yarray2(MAX), ytmp(MAX), y12conv(MAX)

!     Declare Variables used in the rsac1() subroutine
      real beg, delta, beg2
      integer nlen1, nlen2
      character*30 KNAME
      integer nerr
      
      character *16  kevnm

!     Read in the first data file
      kname = 'convolvef2_in1.sac'
      call rsac1(kname, yarray1, nlen1, beg, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',kname
         call exit(-1)
      endif

!     Read in the second data file
      kname = 'convolvef2_in2.sac'
      call rsac1(kname, yarray2, nlen2, beg2, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',kname
         call exit(-1)
      endif

!     do the convolution

      call convolve(yarray1, nlen1,
     +              yarray2, nlen2,
     +              y12conv, delta, beg2)

      kname = 'convolvef2_in1.sac'
      call rsac1(kname, yarray1, nlen1, beg, delta, MAX, nerr)
c     write(*,*) (y12conv(j),j=1,nlen1)
      kevnm = 'Convolution'
      call setkhv ('kevnm', kevnm, nerr)
!     Write the SAC file
      kname='convolvef2_out1.sac'
      call wsac0(kname, ytmp, y12conv, nerr)
      if(nerr .NE. 0) then
         write(*,*)'Error writing out file: ',kname,nerr
         call exit(-1)
      endif

      call exit(0)

      end program convolvef
      
