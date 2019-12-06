
! Demonstrate applying a taper to a seismogram using the SAC library
program taper_example
    implicit none

    include 'sacf.h'
    
    integer,parameter :: nmax = 1000000
    integer :: npts, nerr, taper_type
    real*4 :: data(nmax)
    real*4 :: beg, dt, width

    integer sac_compare_to_file

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    ! Set up taper parameters
    width = .05     ! Width to taper original data
    taper_type = 2  ! HANNING taper

    call taper_width(data, npts, taper_type, width)

    call wsac0('taperf.sac', data, data, nerr)

    if(sac_compare_to_file("taper_sac.sac", data, 1e-4, 0, 0) .ne. 0) then
       write(*,*) 'data does not match file'
       call exit(1)
    endif

    ! write the seismogram with taper applied back to disk

end program taper_example
