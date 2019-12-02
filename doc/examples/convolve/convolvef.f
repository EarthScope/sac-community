      program convolvef

!     Reads in a (short) pulse that is convolved with (longer)
!         waveform.  Lengths are n_p and n_w.  Output (conv)
!         has length n_w + n_p + 1.  delta must be same for both.
!
!     Easiily expanded to read in multiple waveforms.
!
!     If disc_conv is "y", the output is not premultilied by delta
!         and the begin time for the pulse is treated as zero.  The
!         output for "y" is a a "discrete" convolution, if "n", the
!         output is a "time-series" convolution.

      implicit none

!     Define the Maximum length of waveform
      integer MAX
      parameter (MAX=10000)

      real waveform, pulse, ytmp, conv
      dimension waveform(MAX),pulse(MAX),ytmp(MAX),conv(MAX)
      character*16 kevnm

!     Declare Variables used in the rsac1() calls
      real b_w, delta, b_p, factor, b_p_in, deltaw
      integer n_w, n_p, nmarg, iargc
      character*80 wf_name, p_name, c_name
      character*1 disc_conv
      integer nerr

      nmarg = iargc()
      if (nmarg .eq. 0) then
        write(*,*) 'Usage: convolvef p_name wf_name c_name disc_conv'
        write(*,*) '  where the first three arguments are filenames'
        write(*,*) '  for pulse, waveform, and convolution output.'
        write(*,*) 'If disc_conv is y, it uses a discrete convolution'
        write(*,*) '  and the pulse begin time is set to zero.'
        write(*,*) 'If disc_conv is n, pulse begin time is unchanged'
        write(*,*) '  and the output is multiolied by delta, which is'
        write(*,*) '  what one has in a time-series convolution.'
        stop
      end if
      call getarg(1,p_name)
      call getarg(2,wf_name)
      call getarg(3,c_name)
      call getarg(4,disc_conv)

!   Read in pulse time series
      call rsac1(p_name, pulse, n_p, b_p, delta, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',p_name
         call exit(-1)
      endif

      !     Test if want to do a discrete convolution

      factor = delta
      b_p_in = b_p
      if (disc_conv .eq. 'y') then
         factor = 1.0
         b_p_in = 0.0
      endif

!     If wanted to do more than one waveform, do loop starts here

!     Read in waveform time series

      call rsac1(wf_name, waveform, n_w, b_w, deltaw, MAX, nerr)

      if(nerr .NE. 0) then
         write(*,*)'Error reading in file: ',wf_name
         call exit(-1)
      endif

      if(abs(delta-deltaw) .gt. 1.0e-4) then
         write(*,*)'Delta p .ne Delta w: ', delta, deltaw
         call exit(-1)
      endif



!     Do the convolution

      call td_conv(waveform,n_w,pulse,n_p,conv,delta,factor,b_p_in)

      call setnhv('npts',n_w+n_p-1,nerr)
      kevnm = 'CONVOLUTION'
      call setkhv ('kevnm', kevnm, nerr)

!     Write the SAC file
      call wsac0(c_name, ytmp, conv, nerr)
      if(nerr .NE. 0) then
         write(*,*)'Error writing out file: ',c_name,nerr
         call exit(-1)
      endif

!     If doing more than one waveform, do loop stops here

      call exit(0)

      end program convolvef


      subroutine td_conv(waveform,n_w,pulse,n_p,conv,delta,factor,b_p)
!
!     waveform of length n_w is the waveform time series against which pulse
!         of length n_p is convolved.  Output: conv of length n_w+n_w+1.
!         waveform and pulse are unchanged.
!     If a time-series convolution, b_p is the input value and factor
!         is delta, if a discrete cnovolution, b+p is zero and factor=1.
!     The convoluton is done as an inner product in the time
!         domain.
!     Stops if n_w < n_p.
!
!     Arthur Snoke

      implicit none
      real*4 waveform(*), pulse(*), conv(*)
      real*4 delta,b_p,temp,factor
      integer n_p, n_w, j_1, i, j

      if (n_p .ge. n_w) then
         write(*,*) 'Need more points in waveform than pulse'
         write(*,*) 'n_w and n_p:',n_w, n_p
         stop
      end if

      j_1 = -nint(b_p/delta)+1
      do i=1,n_w+n_p-1
         temp = 0.0
         do j=1,n_w
            if (i.ge.(j-j_1) .and. n_p.ge.(i-j+j_1)) then
               temp = temp + waveform(j)*pulse(i-j+j_1)
            endif
         end do
         conv(i) = factor*temp
      end do
      return
      end
