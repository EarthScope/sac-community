

program int_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real*4 :: data(nmax)
    real*4 :: beg, dt

    integer sac_compare_to_file

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    call int_trap(data, npts, dble(dt))

    call setnhv("npts", npts-1, nerr)
    call setfhv("b", beg + 0.5 * dt, nerr)
    call setihv("idep", "iunkn", nerr)

    ! write the seismogram with trend removed back to disk
    call wsac0('intf.sac', data, data, nerr)

    if(sac_compare_to_file('int_sac.sac', data, 1e-4, 0, 0) .ne. 0) then
       write(*,*)'data does not match file'
       call exit(1)
    endif

end program int_example
