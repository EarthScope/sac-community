
! Demonstrate removing the instrument response from a seismogram using evresp and the SAC library
program transfer_example
    implicit none

    integer,parameter :: nmax = 1000000
    integer,parameter :: MIN_PERIOD = 60
    integer,parameter :: MAX_PERIOD = 120
    integer,parameter :: FFT_NPTS = 262144
    integer :: npts, newlen, nerr, nfreq, iflag, i, j
    integer :: start_stage, stop_stage, stdio_flag, evresp
    real :: data(nmax)
    real :: f1, f2, f3, f4
    real :: beg, eval, dt, tstart, dtnew
    real :: eps = 0.01
    double precision :: freq(FFT_NPTS), resp(FFT_NPTS*2)
    double precision :: delfrq, x_re(FFT_NPTS), x_im(FFT_NPTS)
    double precision :: resp_re(FFT_NPTS), resp_im(FFT_NPTS)
    real(kind=8), dimension(4) :: F
    character(len=5)  :: unts, sta, cha, net, locid, rtyp
    character(len=18) :: resp_file
    character*200        :: datime
    character*10         :: vbs


    ! Read in the data file
    call rsac1('raw.sac', data, npts, beg, dt, nmax, nerr)

    ! Set up parameters for evresp
    start_stage=-1
    stop_stage = 0
    stdio_flag=0
    unts='DIS'
    vbs='-v'
    rtyp = 'CS'
    do i = 1, FFT_NPTS
      freq(i) = float(i-1)/FFT_NPTS/dt
    enddo
    resp_file = 'RESP.AFI.II.00.BHZ'

    ! Use evresp to generate the instrument respone from RESP file
    iflag = evresp(sta,cha,net,locid,datime,unts,resp_file,freq,FFT_NPTS,resp, &
                    rtyp,vbs,start_stage,stop_stage,stdio_flag)

    ! Set up transfer parameters for deconvolution
    nfreq = 4
    f3 = 1.0/MIN_PERIOD
    f2 = 1.0/MAX_PERIOD
    f1 = f2 * 0.8
    f4 = f3 * 1.2
    F = (/ f1, f2, f3, f4 /)
    delfrq = 1.0/(FFT_NPTS*dt)
    j = 1
    do i = 1, FFT_NPTS
      resp_re(i) = resp(j)
      resp_im(i) = resp(j+1)
      j = j + 2
    enddo
    x_re(1:npts) = data(1:npts)

!   Call ztransfer ( Transfers the response from evalresp to seismogram )
!    - data      - Original Data
!    - npts      - Number of points in data
!    - dt        - Sample rate of original data
!    - x_re      - Real part of TO transfer function
!    - x_im      - Imaginary part of TO transfer function
!    - resp_re   - Real part of FROM transfer function
!    - resp_im   - Imaginary part of FROM transfer function
!    - nfreq     - Number of frequncies in F
!    - FFT_NPTS  - Number of points in frequency domain
!    - detfreq   - Sample interval of number of points in frequency domain
!    - F         - Array containing corner frequncies for deconvolution
    call ztransfer(data, npts, dt, x_re, x_im, resp_re, resp_im, nfreq, FFT_NPTS, delfrq, F)

    ! write the deconvolved seismogram back to disk
    call wsac0('deconvolved.sac', x_re(1:npts), x_re(1:npts), nerr)

end program transfer_example
