       program time_shift

!   Time shifts a SAC file One is prompted for the input and output
!     filenames and tshift (new-old),

      DIMENSION SIGNAL(131072)
      character filename_in*80, filename_out*80, name*20

      MAX = 131072

      nmarg = iargc()
      if (nmarg .eq. 0) then
              write(*,*)'Usage: time_shift file_in file_out tshift'
        stop
      end if
      call getarg(1,filename_in)
      call getarg(2,filename_out)
      call getarg(3,name)
      read(name,'(f10.0)') tshift

       nf = lenc(filename_in)
       call rsac1(filename_in(1:nf),signal,npts,secs,dt,max,nerr)
       if (nerr .ne. 0) then
            write(*,*) 'Error opening file ',filename_in(1:nf)
            stop
       end if
       call timeshift(signal,npts,max,dt,tshift)
       WRITE(*,'(a,f10.3)') 'Time shift (new-old) of',tshift
       call setfhv('user9',tshift,nerr)
       nf = lenc(filename_out)
       call wsac0(filename_out(1:nf),signal,signal,nerr)
       stop
       end
!
      subroutine timeshift(signal,npts,max,dt,tshift)

!     time shifts signal by tshift (new-old)

      dimension signal(*)
      ttot = dt*(npts-1)
      ntran = min0(4*npts,max)
      npts = min0(npts,ntran)
      if (npts .lt. ntran) then
        do k=npts+1,ntran
          signal(k) = 0.0
        end do
      end if
      call forwft(ntran,signal,dt,1.0)
      df = 1./(2.0*(ntran-1)*dt)
      if (abs(tshift - 0.0) >= 1e-7 ) then
        call shiftt(ntran,signal,df,tshift)
      end if
      call invrft(ntran,signal,df,-1.0)
      return
      end
!
      subroutine shiftt(ntran,fsig,df,tshift)

      complex fsig(ntran)
      twopi = 8.0*atan(1.0)
      do j=1,ntran
        wt = twopi*(j-1)*df*tshift
        fsig(j) = fsig(j)*cmplx(cos(wt),sin(wt))
      end do
      return
      end
