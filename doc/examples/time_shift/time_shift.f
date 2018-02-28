      program time_shift

!     Time-shifts a SAC file.  One is prompted for the input & output
!     filenames and tshift (new-old).
!     The program calls SAC library routines for forward and inverse
!        Fourier transforms.

!   To compile:
!     gfortran -o time_shift time_shift.f ‘sac-config --cflags --libs libsac libsacio

!   Sample run format: (input.sac and a time-shift (new-old) of -0.05 are input.)
!     time_shift input.sac output.sac -0.05    -

      implicit none
      integer nmax
      parameter(nmax = 131072)

      real*4 signal(nmax),re(nmax),im(nmax)
      character filename_in*80, filename_out*80, name*20
      real*4 dt,tshift,secs,b
      integer npts, nmarg, nerr, nfn
      integer lenc
C
C
      nmarg = iargc()
      if (nmarg .eq. 0) then
         write(*,*)'Usage: time_shift filename_in filename_out tshift'
         write(*,*)'    tshift is new-old'
         stop
      end if
      call getarg(1,filename_in)
      call getarg(2,filename_out)
      call getarg(3,name)
      read(name,'(f10.0)') tshift
C
      nfn = lenc(filename_in)
      call rsac1(filename_in(1:nfn),signal,npts,secs,dt,nmax,nerr)
      if (nerr .ne. 0) then
         write(*,*) 'Error opening file ',filename_in(1:nfn)
         stop
      end if

      ! Take out mean and trend and then taper the data
      call getfhv('b',b,nerr)
      call remove_trend(signal, npts, dt, b)
      call taper_width(signal, npts, 2, 0.05)

      call timeshift(signal,re,im,npts,nmax,dt,tshift)

      write(*,'(a,f10.3)') 'Time shift (new-old) of',tshift

      call setfhv('user9',tshift,nerr)

      ! Write out timeshifted file
      nfn = lenc(filename_out)
      call wsac0(filename_out(1:nfn),signal,signal,nerr)
      stop
      end

      subroutine timeshift(signal,re,im,npts,nmax,dt,tshift)

!     time shifts signal by tshift (new-old)

      implicit none

      ! Input Parameters
      real*4 signal(*),re(*),im(*)
      integer npts, nf      ! Length of signal
      integer nmax
      real dt, tshift

      !  real*4 array signal is changed.

      integer k, ntran
      real ttot, df,fnyq

      ttot  = dt*(npts-1)
      ntran = min(4*npts,nmax)
      write(*,*) "dt, npts, ntran", dt, npts, ntran

      ! Pad Input with Zeros
      if (npts < ntran) then
         do k=npts+1,ntran
            signal(k) = 0.0
         end do
      end if

! nf must be a power of 2 that is >= ntran

      nf = 4
      do while (nf < ntran)
        nf = nf * 2
      enddo

      ! Forward FFT  signal, ntran, and nf input; re and im are output
      call fft(signal, ntran, re, im, nf)   !  SAC library routine
      df    = 1./(nf*dt)
      fnyq = (nf/2)*df
      write(*,*) "nf, df, fnyq", nf,df,fnyq

      ! Time shift
      if (abs(tshift) .ge. 1e-8) then
         call shiftt(nf,re,im,df,tshift)
      end if

      ! Inverse FFT  nf, re, im, nf input;  signal output
      call ifft(signal, ntran, re, im, nf)
      write(*,*) 'ntran nf',ntran, nf
      return
      end

      SUBROUTINE SHIFTT(nf,re,im,DF,TSHIFT)

      COMPLEX FSIG
!      complex  now
      real*4 re(*),im(*)
      TWOPI = 8.0*ATAN(1.0)
      N = nf/2 + 1
      DO J=1,N
         fsig = cmplx(re(j),im(j))
!         now = fsig
!        write(*,*) df*(j-1),real(fsig(j)),aimag(fsig(j))
         WT = -TWOPI*(J-1)*DF*TSHIFT    !SAC default sign convention
         FSIG = FSIG*CMPLX(COS(WT),SIN(WT))
!        write(*,*) j, df*(j-1), now, fsig
         re(j) = real(fsig)
         im(j) = aimag(fsig)
      END DO
!      if (ntran.gt.0) stop
      do j=N+1,nf
        re(j) = re(nf-j+2)
        im(j) = -im(nf-j+2)
      enddo
      RETURN
      END

      function lenc(string)

C     Returns length of character variable STRING excluding right-hand
C     most blanks or nulls

      character*(*) string
      length = len(string)      ! total length
      if (length .eq. 0) then
         lenc = 0
         return
      end if
      if(ichar(string(length:length)).eq.0)string(length:length) = ' '
      do j=length,1,-1
         lenc = j
         if (string(j:j).ne.' ' .and. ichar(string(j:j)).ne.0) return
      end do
      lenc = 0
      return
      end
