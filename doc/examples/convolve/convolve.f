      subroutine convolve(trace1,n1,trace2,n2,conv12,delta,b2)
C
C       trace1 of length n1 is the time series against which trace2
C         of length n2 is convolved.  Output is conv of length n1.
C         trace1 and trace2 are unchanged.
C       In principle, could be n1 + n2 output points, but assume
C         trace1 goes to zero at point n1 so output still is n1.
C         (of computed values), and the convoluton is done as an
C         inner product in the time domain normalizing sotrace2
C         has unit amplitude.
C       Stops if n1 < n2.
C
C       Arthur Snoke December 2014
C-
      real*4 trace1(*), trace2(*), conv12(*)
C
      if (n2 .ge. n1) then
         write(*,*) 'Numbers for long and short:',n1, n2
         stop
      end if
      sum2 = 0.0
      kshift = nint(b2/delta)
      if (kshift .lt. 0) then
         do k=1,-kshift
            conv12(k) = 0.0
         enddo
         kstart = -kshift+1
      else
         kstart = 1
      endif
      do k = 1,n2
         sum2 = sum2 + trace2(k)**2
      end do
      do k=kstart,n1
         c1c2 = 0.0
         do kk=kstart,n1
            if (k.ge.(kk-kstart+1) .and. n2.ge.(k-kk+kstart)) then
               c1c2 = c1c2 + trace1(kk)*trace2(k-kk+kstart)
            endif
         end do
c     write(*,*) k,c1c2
c     conv12(k) = c1c2/sqrt(sum2*sum1)
         conv12(k) = delta*c1c2/sqrt(sum2)
      end do
      return
      end
