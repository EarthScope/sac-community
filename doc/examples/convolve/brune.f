      program brune

!     Choose tau = 0.1 for sac examples/convolve
!     Could modify program to input N and delta too.

      real*4 pulse(100)
      character*40 file_out,input
      nmarg = iargc()
      if (nmarg .eq. 0) then
         write(*,*)'Usage: brune tau file_out'
         write(*,*) 'N=64, delta=0.02' 
         stop
      end if
      call getarg(1,input)
      read(input,'(f10.0)') tau
      call getarg(2,file_out)
      write(*,*) tau,file_out
      N = 64
      delta = 2e-2
      do j=1,N
         t = float(j-1)*delta/tau
         pulse(j) = t*exp(-t)
      enddo
      call wsac1(file_out,pulse,64,0.0,delta,nerr)
      stop
      end
