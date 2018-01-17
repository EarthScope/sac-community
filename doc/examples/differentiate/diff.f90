

program dif_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer :: npts, nerr
    real*4 :: data(nmax), out(nmax)
    real*4 :: beg, dt

    integer sac_compare

    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    call dif2(data, npts, dble(dt), out)

    ! write the seismogram with trend removed back to disk
    call wsac0('dif.sac', data, data, nerr)

    if(sac_compare('dif_sac.sac', out, npts-1, beg + 0.5 * dt, dt) .ne. 1) then
       write(*,*)'data does not match file'
    endif

end program dif_example
