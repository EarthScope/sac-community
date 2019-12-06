      program envelopef
      implicit none

!     Define the Maximum size of the data Array
      integer MAX
      parameter (MAX=10000)

!     Define the Data Array of size MAX
      real*4 :: yarray(MAX), yenv(MAX)

      integer sac_compare_to_file

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

      call wsac0("envf.sac",yenv,yenv,nerr);

      if(sac_compare_to_file("env_sac.sac", yenv, 1e-4, 0, 0) .ne. 0) then
         write(*,*) 'data does not match file'
         call exit(1)
      endif


      end program envelopef
