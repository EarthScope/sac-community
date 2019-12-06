
program cutf
  implicit none
  integer max

  include 'sacf.h'

  real*4 :: y(10000), out(10000)
  real*4 :: b, dt
  integer :: nerr, n, nout

  real*4 cutb, cute

  integer sac_compare_to_file

  max = 10000
  nout = max;
  cutb = 10.0
  cute = 15.0
  call rsac1("raw.sac", y, n, b, dt, max, nerr)

  call cut(y, n, b, dt, cutb, cute, CUT_FILLZ, out, nout)

  call setnhv("npts", nout, nerr);
  call setfhv("b", cutb, nerr);
  call setfhv("e", cute, nerr);

  call wsac0("cutf.sac", out, out, nerr, -1)
  if(sac_compare_to_file("cut_sac.sac", out, 1e-4, 0, 0) .ne. 0) then
     write(*,*)'data does not match file'
     call exit(1)
  endif


end program
