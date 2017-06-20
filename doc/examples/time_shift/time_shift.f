C     +
      program time_shift
C
C     Time shifts a SAC file One is prompted for the input and output
C     filenames and tshift (new-old),
!
!     gfortran -Wall -Wextra -o time_shift time_shift.f time_shift_subs.f $(sac-config --libs sacio)
!
!     time_shift input.sac output.sac -0.05
!
C     -

      integer nmax
      parameter(nmax = 131072)

      DIMENSION SIGNAL(nmax)
      character kstnm*8,kcmpnm*8,kevnm*16
      character filename_in*80, filename_out*80, name*20
C
C
      nmarg = iargc()
      if (nmarg .eq. 0) then
         write(*,*)'Usage: time_shift filename_in filename_out tshift'
         stop
      end if
      call getarg(1,filename_in)
      call getarg(2,filename_out)
      call getarg(3,name)
      read(name,'(f10.0)') tshift
C
      nf = lenc(filename_in)
      call rsac1(filename_in(1:nf),signal,npts,secs,dt,nmax,nerr)
      if (nerr .ne. 0) then
         write(*,*) 'Error opening file ',filename_in(1:nf)
         stop
      end if

      call getkhv('KSTNM',kstnm,nerr)
      if (nerr .ne. 0) then
         kstnm = ' '
      endif
      call getkhv('KCMPNM',kcmpnm,nerr)
      if (nerr .ne. 0) then
         kcmpnm = ' '
      endif
      call getkhv('KEVNM',kevnm,nerr)
      if (nerr .ne. 0) then
         kevnm = ' '
      endif

      call timeshift(signal,npts,max,dt,tshift)

      write(*,'(a,f10.3)') 'Time shift (new-old) of',tshift

      call setkhv('kuser2','tshift',nerr)
      call setfhv('user2',tshift,nerr)

      ! Write out timeshifted file
      nf = lenc(filename_out)
      call wsac0(filename_out(1:nf),signal,signal,nerr)
      stop
      end
C


      subroutine timeshift(signal,npts,nmax,dt,tshift)
C
C     time shifts signal by tshift (new-old)
C     -
      implicit none

      ! Input Parameters
      real signal(*)
      integer npts      ! Length of signal
      integer nmax
      real dt, tshift

      integer k, ntran
      real ttot, df

      ttot  = dt*(npts-1)
      ntran = min(4*npts,nmax)
      npts  = min(npts,ntran)
      df    = 1./(2.0*(ntran-1)*dt)

      ! Pad Input with Zeros
      if (npts < ntran) then
         do k=npts+1,ntran
            signal(k) = 0.0
         end do
      end if

      ! Forward FFT
      !  Input - Real
      !  Output - Complex
      call forwft(ntran,signal,dt,1.0)

      ! Time shift
      ! Input - Complex
      ! Output - Complex
      if (abs(tshift) .ge. 1e-8) then
         call shiftt(ntran,signal,df,tshift)
      end if

      ! Inverse FFT
      !  Input - Complex
      !  Output - Real
      call invrft(ntran,signal,df,-1.0)
      write(*,*)signal(1:4)
      return
      end

