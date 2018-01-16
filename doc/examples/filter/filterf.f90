
subroutine test_bp()
  implicit none
  integer nmax, n, nerr, sac_compare
  real*4 :: y(10000), b, dt
  nmax = 10000
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  call bandpass(y, n, dt, 0.10, 1.00)
  if(sac_compare("bandpass_sac.sac", y, n, b, dt) .ne. 1) then
     write(*,*)'data does not match file'
  endif
  call wsac0("bandpass.sac", y, y, nerr)
end subroutine test_bp

subroutine test_lp()
  implicit none
  integer nmax, n, nerr, sac_compare
  real*4 :: y(10000), b, dt
  nmax = 10000
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  call lowpass(y, n, dt, 2.0)
  if(sac_compare("lowpass_sac.sac", y, n, b, dt) .ne. 1) then
     write(*,*)'data does not match file'
  endif
  call wsac0("lowpass.sac", y, y, nerr)
end subroutine test_lp

subroutine test_hp()
  implicit none
  integer nmax, n, nerr, sac_compare
  real*4 :: y(10000), b, dt
  nmax = 10000
  call rsac1("raw.sac", y, n, b, dt, nmax, nerr)
  call highpass(y, n, dt, 10.0)
  if(sac_compare("highpass_sac.sac", y, n, b, dt) .ne. 1) then
     write(*,*)'data does not match file'
  endif
  call wsac0("highpass.sac", y, y, nerr)
end subroutine test_hp


program filter
  implicit none
  call test_bp()
  call test_lp()
  call test_hp()
end program filter
