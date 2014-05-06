
! Demonstrate applying a taper to a seismogram using the SAC library
program taper_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: nlen, npts, nerr, ipts, taper_type
    real*4 :: data(nmax)
    real*4 :: beg, dt, width

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)
    
    ! Set up taper parameters
    width = .05     ! Width to taper original data
    taper_type = 2  ! HANNING taper

!   Call taper_width_to_points
!    - width    - Width to taper original data
!    - npts     - Number of points in data
!    - ipts     - Number of points that will be tapered in data
    call taper_width_to_points(width, npts, ipts)

!   Call taper ( Applies a Taper )
!    - data   - Original Data
!    - npts   - Number of points in data
!    - taper_type - Typer of taper: 1 = cosine; 2 = hanning; 3 = hamming
!    - ipts   - Number of points tapered
    call taper(data, npts, taper_type, ipts)

    ! write the seismogram with taper applied back to disk
    call wsac0('taper.sac', data, data, nerr)

end program taper_example
