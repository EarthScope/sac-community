
! Demonstrate removing the trend from a seismogram using the SAC library
program rtrend_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: nlen, npts, nerr
    real*4 :: data(nmax)
    real*4 :: beg, dt, yint, slope

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

!   Call rtrend ( Removes the trend )
!    - data   - Original Data
!    - npts   - Number of points in data
!    - yint   - y-intercept of best-fitting line
!    - slope  - Slope of best-fitting line
!    - beg    - Beginning time of original data
!    - dt     - Sample rate of original data
    call rtrend(data, npts, yint, slope, beg, dt)

    ! write the seismogram with trend removed back to disk
    call wsac0('rtrend.sac', data, data, nerr)

end program rtrend_example
