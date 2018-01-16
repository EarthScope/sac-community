
! Demonstrate performing a rotation on a pair of signals using the SAC library
program rotate_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real :: signal1(nmax), signal2(nmax)
    real :: rotated_signal1(nmax), rotated_signal2(nmax)
    real :: beg, dt
    real*8 :: angle
    real :: baz, cmpaz1
    logical :: lnpi, lnpo

    integer sac_compare

    ! Read in the two signals to be rotated
    call rsac1('signal1.sac', signal1, npts, beg, dt, nmax, nerr)
    call getfhv("cmpaz", cmpaz1, nerr)
    call getfhv("baz", baz, nerr);

    call rsac1('signal2.sac', signal2, npts, beg, dt, nmax, nerr)

    ! Set up parameters for rotation
    lnpi = .true.     ! input signals have "normal" polarity
    lnpo = .true.     ! output signals have "normal polarity

    angle = baz + 180.0 - cmpaz1;

    call rotate(signal1, signal2, npts, angle, lnpi, lnpo, rotated_signal1, rotated_signal2)

    nerr = sac_compare("signalr.sac", rotated_signal1, npts, beg, dt)
    nerr = sac_compare("signalt.sac", rotated_signal2, npts, beg, dt)

    ! write the seismogram with trend removed back to disk
    call wsac0('rotated1.sac', rotated_signal1, rotated_signal1, nerr)
    call wsac0('rotated2.sac', rotated_signal2, rotated_signal2, nerr)

end program rotate_example
