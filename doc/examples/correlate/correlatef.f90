
subroutine correlate_files(filea, fileb, amp_max, t_max)
  implicit none
  character(len=*) filea, fileb
  real*4 :: amp_max, t_max
  real*4 :: a(10000), b(10000), ba,bb,dt,bc
  real*4 :: c(10000)
  integer :: nmax, nerr,na, nb, nc, i

  real*4 correlate_time_begin
  real*4 correlate_time
  integer correlate_max
  integer isclose

  nmax = 10000

  c(:) = 0.0

  call rsac1(filea, a, na, ba, dt, nmax, nerr)
  call rsac1(fileb, b, nb, bb, dt, nmax, nerr)

  nc = na + nb - 1
  call correlate(a, na, b, nb, c, nc)

  bc = correlate_time_begin(dt, na, nb, ba, bb)

  i = correlate_max(c, nc)

  if(isclose(correlate_time(dt, bc, i), t_max) .ne. 1) then
     write(*,*)'max timing mismatch', t_max, correlate_time(dt,bc,i)
     !call exit(1)
  endif

  if(isclose(c(i), amp_max) .ne. 1) then
     write(*,*)'max amplitude mismatch', amp_max, c(i)
     call exit(1)
  endif
  
end subroutine correlate_files

program correlate_ex
  implicit none
  call correlate_files("imp02.sac", "imp06.sac", 1.0,  0.4)
  call correlate_files("imp02.sac", "imp12.sac", 1.0,  1.0)
  call correlate_files("imp02.sac", "imp02n.sac",1.0, -1.0)
  call correlate_files("imp02n.sac", "imp06.sac",1.0,  1.4)
  call correlate_files("imp02n.sac", "imp12.sac",1.0,  2.0)

  call correlate_files("imp06.sac", "imp02.sac", 1.0, -0.4)
  call correlate_files("imp12.sac", "imp02.sac", 1.0, -1.0)
  call correlate_files("imp02n.sac", "imp02.sac",1.0,  1.0)
  call correlate_files("imp06.sac", "imp02n.sac",1.0, -1.4)
  call correlate_files("imp12.sac", "imp02n.sac",1.0, -2.0)

end program correlate_ex

