
      subroutine check_error(nerr, name)
      implicit none
      character *10 name
      integer nerr
      integer SAC_ILLEGAL_HEADER_NAME
      data SAC_ILLEGAL_HEADER_NAME/1337/
      if(nerr .ne. 0) then
         if(nerr .eq. SAC_ILLEGAL_HEADER_NAME) then
            write(*,*)'Error reading variable: ',name,nerr
            call exit(-1)
         endif
      endif
      end subroutine

      program sacio_test

      implicit none

!     Define the Maximum size of the data Array
      integer MAX
      parameter (MAX=1000)
      integer SAC_ILLEGAL_HEADER_NAME
      data SAC_ILLEGAL_HEADER_NAME/1337/

!     Define the Data Array of size MAX
      real yarray
      dimension yarray(MAX)

      character*20 str
      integer fails 
!     Declare Variables used in the rsac1() subroutine
      real beg, del, f
      integer nlen, i,j, n
      character*64 KNAME,kname2
      integer nerr
      integer nval(15),ival(20),lval(5)
      character *10 fhdr(70),nhdr(15),ihdr(20),lhdr(5),khdr(23)
      character *16 kval(23)
      real fval(70)
      data kval/'sta','FUNCGEN: IMPULSE','-12345  ','-12345  ',
     + '-12345  ',
     + '-12345  ','-12345  ','-12345  ','-12345  ','-12345  ',
     + '-12345  ','-12345  ','-12345  ','-12345  ','-12345  ',
     + '-12345  ','-12345  ','-12345  ','-12345  ','Q',
     + '-12345  ','-12345  ','-12345  '/
      data nhdr/'nzyear','nzjday','nzhour','nzmin','nzsec',
     +     'nzmsec','nvhdr','norid','nevid','npts',
     +     'nsnpts','nwfid','nxsize','nysize','nhdr15'/
      data ihdr/'iftype','idep','iztype','ihdr4','iinst',
     +     'istreg','ievreg','ievtyp','iqual','isynth',
     +     'imagtyp','imagsrc','ihdr13','ihdr14','ihdr15',
     +     'ihdr16','ihdr17','ihdr18','ihdr19','ihdr20'/
      data khdr/'kstnm','kevnm','khole','ko','ka',
     +     'kt0','kt1','kt2','kt3','kt4',
     +     'kt5','kt6','kt7','kt8','kt9',
     +     'kf','kuser0','kuser1','kuser2','kcmpnm',
     +     'knetwk','kdatrd','kinst'/
      data lhdr/'leven','lpspol','lovrok','lcalda','lhdr5'/
      data fhdr/'delta','depmin','depmax','scale', 'odelta',
     +     'b','e','o','a','fmt',
     +     't0','t1','t2','t3','t4',
     +     't5','t6','t7','t8','t9',
     +     'f','resp0','resp1','resp2','resp3',
     +     'resp4','resp5','resp6','resp7','resp8',
     +     'resp9','stla','stlo','stel','stdp',
     +     'evla','evlo','evel','evdp','mag',
     +     'user0','user1','user2','user3','user4',
     +     'user5','user6','user7','user8','user9',
     +     'dist','az','baz','gcarc','sb',
     +     'sdelta','depmen','cmpaz','cmpinc','xminimum',
     +     'xmaximum','yminimum','ymaximum','adjtm','fhdr65',
     +     'fhdr66','fhdr67','fhdr68','fhdr69','fhdr70'/
      data nval/-12345,-12345,-12345,-12345,-12345,
     +     -12345,6,-12345,-12345,100,
     +     -12345,-12345,-12345,-12345,-12345/
      data lval/1,0,1,1,0/
      data ival/-12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345/
      data fval/1,0,1,-12345,-12345,
     +     0,99,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345,
     +     -12345,-12345,-12345,-12345,-12345/
      fval(57) = 1./100.

      fails = 0
!     Define the file to be read      g
      kname  = 'test_io_small.sac'
      kname2 = 'test_io_big.sac'

!     Call rsac1 to read filename kname
!        - Data is loaded into yarray
!        - Length of data is stored in nlen
!        - Begining time and time sampling are in beg and del
!        - MAX is the maximum number of points to be read in 
!        - nerr is the Error return flag
      do j = 1,2
         if(j .eq. 1) then
            call rsac1(kname, yarray, nlen, beg, del, MAX, nerr)
         else 
            call rsac1(kname2, yarray, nlen, beg, del, MAX, nerr)
         endif
!     Check the error status, nerr
!        - 0 on Success
!        - Non-Zero on Failure
         if(nerr .NE. 0) then
            write(*,*)'Error reading in file: ',kname
            call exit(-1)
         endif

!     Do some processing ....
         do i = 1,3
            if(i .eq. 1) then
               call sac_warning_stdout()
            endif
            if(i .eq. 2) then
               call sac_warning_off()
            endif
            if(i .eq. 3) then
               call sac_warning_stderr()
            endif
            call getfhv(fhdr(4), f, nerr)
            call check_error(nerr, fhdr(4))
         enddo

         call sac_warning_off()
         do i = 1,70
            call getfhv(fhdr(i), f, nerr)
            call check_error(nerr, fhdr(i))
            if(f .ne. fval(i)) then
               write(*,*)'Fail:',f,fval(i),fhdr(i)
               fails = fails + 1
            endif
         enddo
         do i = 1,15
            call getnhv(nhdr(i), n, nerr)
            call check_error(nerr, nhdr(i))
            if(n .ne. nval(i)) then
               write(*,*)'Fail:',n,nval(i),nhdr(i)
               fails = fails + 1
            endif
         enddo
         do i = 1,20
            call getihv(ihdr(i), str, nerr)
            call check_error(nerr, ihdr(i))
            if(n .ne. ival(i)) then
               write(*,*)'Fail:',n,ival(i),ihdr(i)
               fails = fails + 1
            endif
         enddo
         do i = 1,5
            call getlhv(lhdr(i), n, nerr)
            call check_error(nerr, lhdr(i))
            if(n .ne. lval(i)) then
               write(*,*)'Fail:',n,lval(i),lhdr(i)
               fails = fails + 1
            endif
         enddo
         do i = 1,23
            call getkhv(khdr(i), str, nerr)
            call check_error(nerr, khdr(i))
            if(str .ne. kval(i)) then
               write(*,*)'Fail:',str,kval(i),khdr(i)
               fails = fails + 1
            endif
         enddo
         do i = 1,nlen
            if(i .eq. 50) then
               if (yarray(i) .ne. 1.0) then
                  write(*,*)'Fail:',i,yarray(i),1.0
                  fails = fails + 1
               endif
            else
               if(yarray(i) .ne. 0.0) then
                  write(*,*)'Fail:',i,yarray(i),0.0
                  fails = fails + 1
               endif
            endif
         enddo
      enddo
      if (fails .gt. 0) then
         call exit(-1)
      endif
      call exit(0)
      end
