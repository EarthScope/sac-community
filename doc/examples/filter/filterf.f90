
subroutine test_bp()
  implicit none
  integer nmax, n, nerr, sac_compare_to_file
  real*4 :: y(10000), b, dt
  nmax = 10000
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  call remove_trend(y, n, dt, b)
  call bandpass(y, n, dt, 0.10, 1.00)
  call wsac0("bandpassf.sac", y, y, nerr)
  if(sac_compare_to_file("bandpass_sac.sac", y, 1e-4, 0, 0) .ne. 0) then
     write(*,*)'data does not match file for bandpass'
     call exit(1)
  endif
end subroutine test_bp

subroutine test_lp()
  implicit none
  integer nmax, n, nerr, sac_compare_to_file
  real*4 :: y(10000), b, dt
  nmax = 10000
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  call remove_trend(y, n, dt, b)
  call lowpass(y, n, dt, 2.0)
  call wsac0("lowpassf.sac", y, y, nerr)
  if(sac_compare_to_file("lowpass_sac.sac", y, 1e-4, 0, 0) .ne. 0) then
     write(*,*)'data does not match file for lowpass'
     call exit(1)
  endif
end subroutine test_lp

subroutine test_hp()
  implicit none
  integer nmax, n, nerr, sac_compare_to_file
  real*4 :: y(10000), b, dt
  nmax = 10000
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  call remove_trend(y, n, dt, b)
  call highpass(y, n, dt, 10.0)
  call wsac0("highpassf.sac", y, y, nerr)
  if(sac_compare_to_file("highpass_sac.sac", y, 1e-4, 0, 0) .ne. 0) then
     write(*,*)'data does not match file for highpass'
     call exit(1)
  endif
end subroutine test_hp


program filter
  implicit none
  call test_bp()
  call test_lp()
  call test_hp()
end program filter
