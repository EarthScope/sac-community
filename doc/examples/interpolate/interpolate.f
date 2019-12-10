      program interpolate
!
!
!     Pure cubic-spline interpolation, so extremes are not restricted to the
!     amplitude at digitized points.  Source:  Forsythe, G.E., M.A. Malcolm,
!     and C.B. Moler, 1977. Computer Methods for Mathematical Computations,
!     Prentice-Hall, Inc.
!     Read in a sac file name and delta_new
!     Output filename is unchanged except a letter i'
!         precedes last char; e.g., bla.bhz => bla.bhiz'
!     Interpolation keeps start and stop times unchanged.

      parameter (max=524288)
      real*4 signal(max), signal_out(max),time_in(max)
      real*4 a(max),b(max),c(max)
      character filename*80, name*20

      nmarg = iargc()
      if (nmarg .eq. 0) then
         write(*,*)'Usage: interpolate filename delta_new'
         write(*,*)'  filename: the input filename,'
         write(*,*)'  delta_new: the new digitizing interval,'
         write(*,*)'Output filename is unchanged except a letter i'
         write(*,*)'  precedes last char; e.g., bla.bhz => bla.bhiz'
         stop
      end if

      call getarg(1,filename)
      call getarg(2,name)
      read(name,'(f10.0)') delta_new

      nf = lenc(filename)
      call rsac1(filename(1:nf),signal,npts,secs,delta,max,nerr)
      if (nerr .ne. 0) then
         write(*,*)
     1        'Error reading sac file ',
     2        filename(1:nf),'  nerr =',nerr
         stop
      endif

      e = secs + (npts-1)*delta

!     In this version, the "x" vales are equally spaced, but don't need to be

      do j=1,npts
         time_in(j) = secs + (j-1)*delta
      enddo
      npts_new = int( (e-secs)/delta_new ) + 1
      call setfhv('delta',delta_new,ierr)
      call setnhv('npts',npts_new,nerr)
      write(*,'(a,f8.4,i10)') 'delta_new: npts_new',delta_new,npts_new

!     Forsythe Interpolation

      call interp_f(npts,time_in,signal,npts_new,delta_new,
     1     signal_out,a,b,c)
      filename = filename(1:nf-1)//'i'//filename(nf:nf)

      call wsac0(filename(1:nf+1),dummy,signal_out,nerr)
      end
