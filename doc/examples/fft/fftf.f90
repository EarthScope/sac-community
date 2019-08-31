
subroutine check_result_pair(re, im, n)
  implicit none
  integer :: i, n
  real*8 :: re(n), im(n)
  real*8 :: re_true(16), im_true(16)
  n = 16
  re_true(:)  = 0.0d0
  im_true(:)  = 0.0d0
  im_true(3)  = -8.0
  im_true(15) = +8.0

  do i = 1,n
     !write(*,'(i3,1x,f15.8,1x,f15.8)')i,re(i),im(i)
     if(abs(re_true(i) - re(i)) >= 1e-10) then
        write(*,*)'Error in fft value (re)',i,re(i),re_true(i)
        call exit(-2)
     endif
     if(abs(im_true(i) - im(i)) >= 1e-10) then
        write(*,*)'Error in fft value (im)',i,im(i),im_true(i)
        call exit(-2)
     endif
  enddo
end subroutine check_result_pair

subroutine check_result_pairf(re, im, n)
  implicit none
  integer :: i, n
  real*4 :: re(n), im(n), tol
  real*4 :: re_true(16), im_true(16)
  tol = 1e-7
  n = 16
  re_true(:)  = 0.0d0
  im_true(:)  = 0.0d0
  im_true(3)  = -8.0
  im_true(15) = +8.0

  do i = 1,n
     !write(*,'(i3,2(1x,f15.8),2(1x,e15.8))')i,re(i),im(i),abs(re(i)-re_true(i)),abs(im(i)-im_true(i))
     if(abs(re_true(i) - re(i)) >= tol) then
        write(*,*)'Error in fft value (re)',i,re(i),re_true(i)
        call exit(-2)
     endif
     if(abs(im_true(i) - im(i)) >= tol) then
        write(*,*)'Error in fft value (im)',i,im(i),im_true(i)
        call exit(-2)
     endif
  enddo
end subroutine check_result_pairf

subroutine check_result_complex(z, n)
  implicit none
  integer :: i, n
  complex*16 :: z(n)
  real*8 :: re(n), im(n)
  do i = 1,n
     re(i) = real(z(i))
     im(i) = imag(z(i))
  enddo
  call check_result_pair(re, im, n)
end subroutine check_result_complex

subroutine check_result_complexf(z, n)
  implicit none
  integer :: i, n
  complex*8 :: z(n)
  real*4 :: re(n), im(n)
  do i = 1,n
     re(i) = real(z(i))
     im(i) = imag(z(i))
  enddo
  call check_result_pairf(re, im, n)
end subroutine check_result_complexf

subroutine check_dataf(a, b, n)
  integer n
  real*4 :: a(n),b(n),tol
  tol = 1e-7
  do i = 1,n
     if(abs(a(i) - b(i)) >= tol) then
        write(*,*)'Error in data value',i,a(i),b(i)
     endif
  end do
end subroutine check_dataf

subroutine check_data(a, b, n)
  integer n
  real*8 :: a(n),b(n),tol
  tol = 1e-10
  do i = 1,n
     if(abs(a(i) - b(i)) >= tol) then
        write(*,*)'Error in data value',i,a(i),b(i)
     endif
  end do
end subroutine check_data

subroutine fft_double()

  implicit none

  integer i, n, nf
  real*8 :: data(16),data2(16)
  real*8 :: pi
  real*8 :: re(16), im(16)
  complex*16 :: z(16)
  real*8, allocatable :: rea(:), ima(:)
  complex*16, allocatable :: za(:)
  n = 16
  nf = 16
  pi = acos(-1.0d0);
  ! Sine wave - Input
  do i = 1,n
     data(i) = sin( (i-1) * 2.0 * pi / (n / 2.0))
  enddo

  ! FFT
  call dfft(data, n, re, im, nf)
  call check_result_pair(re, im, nf)
  call idfft(data2, n, re, im, nf)
  call check_data(data, data2, n);

  ! FFT with complex number
  call dfftz(data, n, z, nf)
  call check_result_complex(z, nf)
  call idfftz(data2, n, z, nf)
  call check_data(data, data2,n );

  ! FFT with allocated complex number
  ! Get next/bounding power of 2
  nf = 2
  do while (nf < n)
     nf = nf * 2
  enddo
  allocate(za(n))
  call dfftz(data, n, za, nf)
  call check_result_complex(za, nf)
  deallocate(za)

  ! FFT
  nf = 2
  do while (nf < n)
     nf = nf * 2
  enddo
  allocate(rea(n))
  allocate(ima(n))
  call dfft(data, n, rea, ima, nf)
  call check_result_pair(rea, ima, nf)
  deallocate(rea)
  deallocate(ima)

end subroutine fft_double

subroutine fft_single()

  implicit none

  integer i, n, nf
  real*4 :: data(16),data2(16)
  real*8 :: pi
  real*4 :: re(16), im(16)
  complex*8 :: z(16)
  real*4, allocatable :: rea(:), ima(:)
  complex*8, allocatable :: za(:)
  n = 16
  nf = 16
  pi = acos(-1.0d0);
  ! Sine wave - Input
  do i = 1,n
     data(i) = sin( (i-1) * 2.0 * pi / (n / 2.0))
  enddo

  ! FFT
  call fft(data, n, re, im, nf)
  call check_result_pairf(re, im, nf)
  call ifft(data2, n, re, im, nf)
  call check_dataf(data, data2, n)

  ! FFT with complex number
  call fftz(data, n, z, nf)
  call check_result_complexf(z, nf)
  call ifftz(data2, n, z, nf)
  call check_dataf(data, data2, n)

  ! FFT with allocated complex number
  ! Get next/bounding power of 2
  nf = 2
  do while (nf < n)
     nf = nf * 2
  enddo
  allocate(za(n))
  call fftz(data, n, za, nf)
  call check_result_complexf(za, nf)
  call ifftz(data2, n, z, nf)
  call check_dataf(data, data2, n)
  deallocate(za)

  ! FFT
  nf = 2
  do while (nf < n)
     nf = nf * 2
  enddo
  allocate(rea(n))
  allocate(ima(n))
  call fft(data, n, rea, ima, nf)
  call check_result_pairf(rea, ima, nf)
  call ifft(data2, n, re, im, nf)
  call check_dataf(data, data2, n)
  deallocate(rea)
  deallocate(ima)

end subroutine fft_single

program fftf
  implicit none
  call fft_double()
  call fft_single()
end program fftf
