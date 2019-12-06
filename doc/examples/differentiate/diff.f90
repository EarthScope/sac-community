

program dif_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real*4 :: data(nmax), out(nmax)
    real*4 :: beg, dt

    integer sac_compare_to_file

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    call dif2(data, npts, dble(dt), out)

    call setnhv("npts", npts-1, nerr);
    call setfhv("b", beg+0.5*dt, nerr);
    call setihv("idep", "IUNKN", nerr);

    ! write the seismogram with trend removed back to disk
    call wsac0('diff.sac', out, out, nerr)

    if(sac_compare_to_file('dif_sac.sac', out, 1e-4, 0, 0) .ne. 0) then
       write(*,*)'data does not match file'
       call exit(1)
    endif

end program dif_example
