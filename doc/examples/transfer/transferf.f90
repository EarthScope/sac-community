
subroutine compare_to_sac_file(file, y, n, b, dt)
  implicit none
  character(len=*) file
  real*4 y(*)
  integer n
  real*4 b, dt
  integer nerr
  real*4 y1(10000)
  integer n1
  real*4 dt1, b1
  integer nmax
  integer i

  integer isclosef
  
  nmax = 10000
  call rsac1(file, y1, n1, b1, dt1, nmax, nerr)
  if (nerr .ne. 0) then
     write(*,*) "Error reading file: ", file(1:len_trim(file)), "nerr: ", nerr
     return
  endif
  if(n .ne. n1) then
     write(*,*) "n: ", n, n1
  endif
  if(abs(b - b1) > 1e-7) then
     write(*,*) "b: ", b, b1
  endif
  if(abs(dt - dt1) > 1e-7) then
     write(*,*) "dt: ", dt, dt1
  endif
  do i = 1, n
     if(isclosef(y(i),y1(i)) .ne. 1) then
        write(*,*) "y[", i, "]: ", y(i),y1(i)
        call exit(-1)
     endif
  enddo
end subroutine compare_to_sac_file

subroutine test_revalresp()
  implicit none
  include "sacf.h"
  real*4 y(10000), b, dt
  integer n, nerr
  integer nmax
  real*8 limits(4)

  integer remove_evalresp_simple

  nmax = 10000
  limits(1) = 0.002
  limits(2) = 0.005
  limits(3) = 12.0
  limits(4) = 20.0
  ! Read in Raw Sac file

  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  if(nerr .ne. 0) then
     write(*,*) "Error reading file: raw.sac nerr: ", nerr
     return
  endif

  if(remove_evalresp_simple(y, n, dt, limits) .ne. 0) then
     write(*,*) "Error removing instrument with evalresp"
     return
  endif

  ! write the deconvolved seismogram back to disk
  call wsac0("deconvolved_evr.sac", y, y, nerr)

  ! Check input/output
  call compare_to_sac_file("deconvolved_from_evr.sac",y,n,b,dt)

end subroutine test_revalresp


subroutine test_rpolezero()
  implicit none
  include 'sacf.h'
  
  real*4 y(10000), b, dt
  integer n, nerr
  integer nmax
  real*8 limits(4)

  integer remove_polezero_simple
  
  nmax = 10000
  limits(1) = 0.002
  limits(2) = 0.005
  limits(3) = 12.0
  limits(4) = 20.0

  ! Read in Raw Sac file 
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  if(nerr .ne. 0) then
     write(*,*) "Error reading file: raw.sac nerr: ", nerr
     return
  endif
  if(remove_polezero_simple(y, n, dt, limits) .ne. 0) then
     write(*,*) "Error removing instrument with polezero"
     return
  endif
  ! write the deconvolved seismogram back to disk
  call wsac0("deconvolved_pz.sac", y, y, nerr)

  ! Check input/output
  call compare_to_sac_file("deconvolved_from_pz.sac", y,n,b,dt)

end subroutine test_rpolezero

program main
  implicit none
  write(*,*) "test evalresp"
  call test_revalresp()
  write(*,*) "test polezero"
  call test_rpolezero()
end program main
