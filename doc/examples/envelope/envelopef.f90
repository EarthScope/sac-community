      program envelopef
      implicit none

!     Define the Maximum size of the data Array
      integer MAX
      parameter (MAX=10000)

!     Define the Data Array of size MAX
      real*4 :: yarray(MAX), yenv(MAX)

      integer sac_compare

!     Declare Variables used in the rsac1() subroutine
      real beg, delta
      integer nlen, nerr
      character*64 KNAME

      kname = 'raw.sac'
      call rsac1(kname, yarray, nlen, beg, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',kname
         call exit(-1)
      endif

      call envelope(nlen, yarray, yenv)

      if(sac_compare("env_sac.sac", yenv, nlen, beg, delta) .ne. 1) then
         write(*,*) 'data does not match file'
      endif

      call wsac1("envf.sac",yenv,nlen,beg,delta,nerr);

      end program envelopef
