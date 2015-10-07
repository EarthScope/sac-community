      program convolvef
      
!       Reads in a short time series that is convolved with the
!         second (longer) time series.  Easiily expanded to read
!         in multiple long time series.  Output has same length
!         and time parameters as longer series.  (Assumes longer
!         goes to zero at start and finish.)
!       gfortran -o convolvef convolvef.f -I/usr/local/sac/include
!           -L/usr/local/sac/lib -lsacio -lsac

      implicit none

      include "sacf.h"

      integer i,j
!     Define the Maximum size of the data Array
      integer MAX
      parameter (MAX=4000)

      real yarray, yarrays, ytmp, yconv
      dimension yarray(MAX), yarrays(MAX), ytmp(MAX), yconv(MAX)
      character*16 kevnm

!     Declare Variables used in the rsac1() calls
      real beg, delta, begs
      integer nlen, nlens
      character*80 KNAME
      integer nerr

!   Read in short time series
      kname = 'brune_pulse.sac'
      call rsac1(kname, yarrays, nlens, begs, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',kname
         call exit(-1)
       endif

!    Read in long series against which short series is convolved

      kname = 'synthetic.sac'

      call rsac1(kname, yarray, nlen, beg, delta, MAX, nerr)

      if(nerr .NE. 0) then
      	  write(*,*)'Error reading in file: ',kname
	  call exit(-1)
       endif

!     Do the convolution

        call td_conv(yarray,nlen,yarrays,nlens,yconv,delta,begs)

      kevnm = 'Convolution'
      call setkhv ('kevnm', kevnm, nerr)
!     Write the SAC file
      kname='convolvef_out.sac'
      call wsac0(kname, ytmp, yconv, nerr)
      if(nerr .NE. 0) then
      	  write(*,*)'Error writing out file: ',kname,nerr
	  call exit(-1)
      endif

      call exit(0)

      end program convolvef
      
c+
      subroutine td_conv(trace,n,traces,ns,conv,delta,begs)
C
C       trace of length n is the time series against which traces
C         of length ns is convolved.  Output is conv of length n.
C         trace and traces are unchanged.
C       In principle, could be n + ns output points, but assume
C         trace goes to zero at points 1 amd n so output is n.
C         The convoluton is done as an inner product in the time
C         domain.  Normalizing so traces has unit amplitude.
C       Stops if n < ns.
C
C       Arthur Snoke 2015
C-
        real*4 trace(*), traces(*), conv(*)
C
        if (ns .ge. n) then
                write(*,*) 'Numbers for long and short:',n, ns
                stop
        end if
        sum2 = 0.0
        kshift = nint(begs/delta)
        if (kshift .lt. 0) then
          do k=1,-kshift
            conv(k) = 0.0
          enddo
          kstart = -kshift+1
        else
          kstart = 1
        endif
        do k = 1,ns
          sum2 = sum2 + traces(k)**2
        end do
        do k=kstart,n
          temp = 0.0
          do kk=kstart,n
            if (k.ge.(kk-kstart+1) .and. ns.ge.(k-kk+kstart))
     1          temp = temp + trace(kk)*traces(k-kk+kstart)
          end do
          conv(k) = delta*temp/sqrt(sum2)
        end do
        return
        end
