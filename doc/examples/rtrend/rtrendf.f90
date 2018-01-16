
! Demonstrate removing the trend from a seismogram using the SAC library
program rtrend_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real*4 :: data(nmax)
    real*4 :: beg, dt

    integer sac_compare

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    call remove_trend(data, npts, dt, beg)

    ! write the seismogram with trend removed back to disk
    call wsac0('rtrend.sac', data, data, nerr)

    if(sac_compare('rtrend_sac.sac', data, npts, beg, dt) .ne. 1) then
       write(*,*)'data does not match file'
    endif

end program rtrend_example
