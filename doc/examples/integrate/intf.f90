

program int_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real*4 :: data(nmax)
    real*4 :: beg, dt

    integer sac_compare

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    call int_trap(data, npts, dble(dt))

    ! write the seismogram with trend removed back to disk
    call wsac0('int.sac', data, data, nerr)

    if(sac_compare('int_sac.sac', data, npts-1, beg + 0.5 * dt, dt) .ne. 1) then
       write(*,*)'data does not match file'
    endif

end program int_example
