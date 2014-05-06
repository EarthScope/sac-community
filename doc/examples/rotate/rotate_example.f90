
! Demonstrate performing a rotation on a pair of signals using the SAC library
program rotate_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, newlen, nerr
    real :: signal1(nmax), signal2(nmax)
    real :: rotated_signal1(nmax), rotated_signal2(nmax)
    real :: beg, dt, angle
    real :: eps = 0.01
    logical :: lnpi, lnpo

    ! Read in the two signals to be rotated
    call rsac1('signal1.sac', signal1, npts, beg, dt, nmax, nerr)
    call rsac1('signal2.sac', signal2, npts, beg, dt, nmax, nerr)

    ! Set up parameters for rotation
    angle = 45.0      ! rotate components 45 degrees clockwise
    lnpi = .true.     ! input signals have "normal" polarity
    lnpo = .true.     ! output signals have "normal polarity

!   Call rotate ( Interpolates the seismogram to a new sample rate )
!   Assumes "normal" polariy is such that the second component leads the
!   first component by 90 degrees in a clockwise rotation
!    - signal1 - First input signal
!    - signal2 - Second input signal
!    - npts    - Number of points in input signal
!    - angle   - Angle of rotation, clockwise from direction of signal1
!    - lnpi    - True if input signals have "normal" polarity
!    - lnpo    - True if output signals have "normal" polarity
!    - rotated_signal1 - First input signal rotated
!    - rotated_signal2 - Second input signal rotated
    call rotate(signal1, signal2, npts, angle, lnpi, lnpo, rotated_signal1, rotated_signal2)

    ! write the seismogram with trend removed back to disk
    call wsac0('rotated1.sac', rotated_signal1, rotated_signal1, nerr)
    call wsac0('rotated2.sac', rotated_signal2, rotated_signal2, nerr)

end program rotate_example
