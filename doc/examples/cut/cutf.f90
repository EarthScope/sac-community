
program cutf
  implicit none
  integer max

  include 'sacf.h'

  real*4 :: y(10000), out(10000)
  real*4 :: b, dt
  integer :: nerr, n, nout

  real*4 cutb, cute

  integer sac_compare

  max = 10000
  nout = max;
  cutb = 10.0
  cute = 15.0
  call rsac1("raw.sac", y, n, b, dt, max, nerr)

  call cut(y, n, b, dt, cutb, cute, CUT_FILLZ, out, nout)

  if(sac_compare("cut_sac.sac", out, nout, cutb, dt) .ne. 1) then
     write(*,*)'data does not match file'
  endif

  call wsac1("cutc.sac", out, nout, cutb, dt, nerr)

end program
