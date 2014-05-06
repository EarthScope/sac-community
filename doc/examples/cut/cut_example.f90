
! Demonstrate applying a cut to a seismogram using the SAC library
program cut_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, npts_cut, nstart, nstop
    integer :: nfillb, nfille, nerr, cuterr
    real    :: data(nmax), cut_data(nmax)
    real    :: beg, dt, begin_cut, end_cut

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)
    
    ! Set up cut parameters
    begin_cut = 10.0     ! Begin time of cut
    end_cut = 15.0       ! End time of cut
    cuterr = 3         ! fill with zeros if the window is too large

!   Call cut_define
!    - begin_cut    - Begin time for cut
!    - dt           - Sample rate of data
!    - end_cut      - End time for cut
!    - npts_cut     - Number of points in data after cutting
    call cut_define(begin_cut, dt, end_cut, npts_cut)

!   Call cut_define_check
!    - begin_cut - Begin time for cut
!    - end_cut   - End time for cut
!    - npts      - Number of points in data
!    - cuterr    - How to handle cuts outside the length of the trace. Three possible values:
!                - CUT_FATAL = 1 throws an error if the cut window is too large
!                - CUT_USEBE = 2 use the b and e values of the trace if the cut window is too large
!                - CUT_FILLZ = 3 fills with zeros if the cut windows is too large
!    - nstart    - Number of points corresponding to begin_cut
!    - nstop     - Number of points corresponding to end_cut
!    - nfillb    - Number of points filled before begin_time
!    - nfille    - Number of pionts filled after end_time
!    - nerr      - Error value returned
    call cut_define_check(begin_cut, end_cut, npts, cuterr, nstart, nstop, nfillb, nfille, nerr)

!   Call cut
!   - data       - Original data to cut
!   - nstart     - Number of points corresponding to begin_cut
!   - nstop      - Number of points corresponding to end_cut
!   - nfillb     - Number of points filled with zeros if begin_time is before data
!   - nfille     - Number of points filled with zeros if end_time is after data
!   - cut_data   - Cut data
    call cut(data, nstart, nstop, nfillb, nfille, cut_data)

    ! write the cut seismogram back to disk
    call wsac1('cut.sac', cut_data(nstart:nstop), npts_cut, begin_cut, dt, nerr)

end program cut_example
