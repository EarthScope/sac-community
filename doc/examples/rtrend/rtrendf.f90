
! Demonstrate removing the trend from a seismogram using the SAC library
program rtrend_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real*4 :: data(nmax)
    real*4 :: beg, dt

    integer sac_compare_to_file

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    call remove_trend(data, npts, dt, beg)

    ! write the seismogram with trend removed back to disk
    call wsac0('rtrendf.sac', data, data, nerr)

    if(sac_compare_to_file('rtrend_sac.sac', data, 1e-4, 0, 0) .ne. 0) then
       write(*,*)'data does not match file'
       call exit(1)
    endif

end program rtrend_example
