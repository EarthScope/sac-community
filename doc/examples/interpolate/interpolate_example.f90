
! Demonstrate interpolating a seismogram using the SAC library
program interpolate_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, newlen, nerr
    real*4 :: data(nmax)
    real*4 :: beg, eval, dt, tstart, dtnew
    real*4 :: eps = 0.01

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    ! Set up interpolation parameters
    eval = beg + float(npts - 1)*dt ! Ending time of original data
    newlen = npts/2 ! Number of point in interpolated data
    dtnew  = dt/2 ! New sample rate (half of current sample rate) 

!   Call interp ( Interpolates the seismogram to a new sample rate )
!    - data   - Original Data
!    - npts   - Number of points in data
!    - interpolated_data   - Interpolated Data
!    - newlen  - Number of points in interpolated data
!    - beg     - Beginning time of original data
!    - eval    - Ending time of original data
!    - dt      - Sample rate of original data
!    - tstart  - Start time of interpolated data
!    - dtnew   - Sample rate of interpolated data
!    - eps     - Machine epsilon precision
    call interp(data, npts, data, newlen, beg, eval, dt, beg, dtnew, eps)

    ! write the interpolated seismogram back to disk
    call wsac1('interp.sac', data(1:newlen), newlen, beg, dtnew, nerr)

end program interpolate_example
