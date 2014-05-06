
! Demonstrate removing the mean from a seismogram using the SAC library
program rmean_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: nlen, npts, nerr
    real*4 :: data(nmax)
    real*4 :: beg, dt, mean

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

!   Call rmean ( Removes the mean )
!    - data   - Original Data
!    - npts   - Number of points in data
!    - mean   - Mean value of the Original Data
    call rmean(data, npts, mean)

    ! write the seismogram with mean removed back to disk
    call wsac0('rmean.sac', data, data, nerr)

end program rmean_example
