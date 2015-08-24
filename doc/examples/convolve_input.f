
      program convolve_input
      implicit none

      call brune()
      call impulse()

      end program

      subroutine brune
      implicit none

      real*4 pulse(100), tau, delta, t 
      character*80 file_out
      integer j, nerr, N

      file_out = 'convolvef2_in2.sac'
      tau = 0.1
      N = 64
      delta = 0.02
      do j=1,N
         t = float(j-1)*delta/tau
         pulse(j) = t*exp(-t)
      enddo
      call wsac1(file_out,pulse,64,0.0,delta,nerr)

      end subroutine

      subroutine impulse
      implicit none
      integer i, k, nerr, N
      real*4 y(2048), delta, A, t
      character*80 file_out

      delta = 0.02
      N = 2048
      A = 1.0
      k = 1

      do i = 1,N
         y(i) = 0.0
         if(mod(i, 100) .eq. 0 ) then
            t = float(i-1) * delta / 10
            y(i) = A * exp(-t) * k
            k = -k
         endif
      enddo

      file_out = 'convolvef2_in1.sac'
      call wsac1(file_out, y, N, 0.0, delta, nerr)

      end subroutine

