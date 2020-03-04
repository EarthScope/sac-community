
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
      character*7 str7
      character*8 str8
      character*9 str9
      integer fails 
!     Declare Variables used in the rsac1() subroutine
      real beg, del, f
      real*8 d,beg8
      integer nlen, i,j, n
      character*64 KNAME,kname2
      integer nerr
      integer nval(15),lval(5)
      character *10 fhdr(70),nhdr(15),ihdr(20),lhdr(5),khdr(23)
      character *9 ival(20)
      character *16 kval(23)
      real fval(70)
      data kval/'sta     ','FUNCGEN: IMPULSE','-12345  ','-12345  ',
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
      data ival/'ITIME    ','UNDEFINED','UNDEFINED','UNDEFINED',
     +     'UNDEFINED','UNDEFINED','UNDEFINED','UNDEFINED',
     +     'UNDEFINED','UNDEFINED','UNDEFINED','UNDEFINED',
     +     'UNDEFINED','UNDEFINED','UNDEFINED','UNDEFINED',
     +     'UNDEFINED','UNDEFINED','UNDEFINED','UNDEFINED'/
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
            !if(i .eq. 1) then
            !   call sac_warning_stdout()
            !endif
            !if(i .eq. 2) then
            !   call sac_warning_off()
            !endif
            !if(i .eq. 3) then
            !   call sac_warning_stderr()
            !endif
            call getfhv(fhdr(4), f, nerr)
            call check_error(nerr, fhdr(4))
            call getdhv(fhdr(4), d, nerr)
            call check_error(nerr, fhdr(4))
            call check_error(nerr, fhdr(4))
       enddo

         !call sac_warning_off()
         do i = 1,70
            call getfhv(fhdr(i), f, nerr)
            call check_error(nerr, fhdr(i))
            if(f .ne. fval(i)) then
               write(*,*)'Fail:',f,fval(i),fhdr(i)
               fails = fails + 1
            endif
            call getdhv(fhdr(i), d, nerr)
            call check_error(nerr, fhdr(i))
            if(d .ne. fval(i)) then
               write(*,*)'Fail:',d,fval(i),fhdr(i)
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
            if(str .ne. ival(i)) then
               write(*,*)'Fail20:',str,ival(i),ihdr(i),i
               fails = fails + 1
            endif
            call getihv(ihdr(i), str7, nerr)
            call check_error(nerr, ihdr(i))
            if(str7 .ne. ival(i)(1:7)) then
               write(*,*)'Fail7:',str7,ival(i),ihdr(i),i
               fails = fails + 1
            endif
            call getihv(ihdr(i), str8, nerr)
            call check_error(nerr, ihdr(i))
            if(str8 .ne. ival(i)(1:8)) then
               write(*,*)'Fail8:',str8,ival(i),ihdr(i),i
               fails = fails + 1
            endif
            call getihv(ihdr(i), str9, nerr)
            call check_error(nerr, ihdr(i))
            if(str9 .ne. ival(i)(1:9)) then
               write(*,*)'Fail9:',str9,ival(i),ihdr(i),str
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
               write(*,*)'Fail20:',str,kval(i),khdr(i),20
               fails = fails + 1
            endif
            if (i .ne. 2) then
               ! Character*7
               str7(:) = ' '
               call getkhv(khdr(i), str7, nerr)
               call check_error(nerr, khdr(i))
               if(str7 .ne. kval(i)(1:7)) then
                  write(*,*)'Fail7:"',str7,'" "',kval(i),'" ',khdr(i),7
                  write(*,*)'Fail7:"',kval(i)(1:7),'"'
                  if( str7(7:7) == char(0) ) then
                     write(*,*)'Last character is \0'
                  endif
                  fails = fails + 1
               endif
               ! Character*8
               call getkhv(khdr(i), str8, nerr)
               call check_error(nerr, khdr(i))
               if(str8 .ne. kval(i)) then
                  write(*,*)'Fail8:"',str8,'" "',kval(i),'" ',khdr(i),8
                  if( str8(8:8) == char(0) ) then
                     write(*,*)'Last character is \0'
                  endif
                  fails = fails + 1
               endif
               ! Character*9
               call getkhv(khdr(i), str9, nerr)
               call check_error(nerr, khdr(i))
               if(str9 .ne. kval(i)) then
                  write(*,*)'Fail9:"',str9,'" "',kval(i),'" ',khdr(i),9
                  fails = fails + 1
               endif
            endif
         enddo
         ! Short Identifies
         i = 1
         call getkhv('kstnm', str, nerr)
         call check_error(nerr, khdr(i))
         if(str .ne. kval(i)) then
            write(*,*)'Failshort:',str,kval(i),khdr(i),20
            fails = fails + 1
         endif

         call getlhv('leven', n, nerr)
         call check_error(nerr, lhdr(i))
         if(n .ne. lval(i)) then
            write(*,*)'Failshort:',n,lval(i),lhdr(i)
            fails = fails + 1
         endif

         call getihv('iftype', str, nerr)
         call check_error(nerr, ihdr(i))
         if(str .ne. ival(i)) then
            write(*,*)'Fail20:',str,ival(i),ihdr(i),i
            fails = fails + 1
         endif

         call getnhv('nzyear', n, nerr)
         call check_error(nerr, nhdr(i))
         if(n .ne. nval(i)) then
            write(*,*)'Fail:',n,nval(i),nhdr(i)
            fails = fails + 1
         endif

         call getfhv('delta', f, nerr)
         call check_error(nerr, fhdr(i))
         if(f .ne. fval(i)) then
            write(*,*)'Fail:',f,fval(i),fhdr(i)
            fails = fails + 1
         endif

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
 
         beg = 3.1415
         call setfhv('b', beg, nerr)
         call check_error(nerr, fhdr(1))
         if(nerr .ne. 0) then
            write(*,*)"Fail:",beg,nerr
            fails = fails + 1
         endif

         f = 0.0
         call getfhv('b', f, nerr)
         call check_error(nerr, fhdr(1))
         if(nerr .ne. 0) then
            write(*,*)"Fail:",beg,f,nerr
            fails = fails + 1
         endif
         if(beg .ne. f) then
            write(*,*)"Fail:",beg,f
            fails = fails + 1
         endif

         beg8 = 1.0d0/3.0d0
         call setdhv('b', beg8, nerr)
         call check_error(nerr, fhdr(1))
         if(nerr .ne. 0) then
            write(*,*)"Fail:",beg8,d,nerr,6
            fails = fails + 1
         endif

         d = 0.0
         call getdhv('b', d, nerr)
         call check_error(nerr, fhdr(1))
         if(real(beg8,4) .ne. real(d,4)) then
            write(*,*)"Fail:",real(beg8,4),real(d,4),6
            fails = fails + 1
         endif

         call setnhv("nvhdr", 7, nerr)
         if(nerr .ne. 0) then
            write(*,*)"Fail: nvhdr",7,nerr
            fails = fails + 1
         endif

         beg8 = 1.0d0/3.0d0
         call setdhv('b', beg8, nerr)
         call check_error(nerr, fhdr(1))
         if(nerr .ne. 0) then
            write(*,*)"Fail:",beg8,d,nerr,7
            fails = fails + 1
         endif

         d = 0.0
         call getdhv('b', d, nerr)
         call check_error(nerr, fhdr(1))
         if(beg8 .ne. d) then
            write(*,*)"Fail:",beg8,d,7
            fails = fails + 1
         endif

      enddo

      call multiple_files(fails)

      if (fails .gt. 0) then
         call exit(-1)
      endif
      call exit(0)
      end


      subroutine multiple_files(fails)
      implicit none
      integer fails
      integer MAX
      parameter (MAX=1000)

      real yarray
      dimension yarray(MAX)
      real beg,del,dt,b
      integer nlen, nerr
      character *8 iftype
      character *8 kstnm
      character *128 kname

      kname  = 'test_io_small.sac'

      call rsac1(kname, yarray, nlen, beg, del, MAX, nerr)

      call getkhv('kstnm',kstnm,nerr)
      call getfhv('delta',dt,nerr)
      call getfhv('b',b,nerr)
      call getihv('iftype',iftype,nerr)
      if (beg .ne. b) then
         write(*,*)'Begin times differ',beg,b
         fails = fails + 1
      endif
      if (kstnm .ne. "sta") then
         write(*,*)'Station names differ',kstnm, "sta"
         fails = fails + 1
      endif
      if (del .ne. dt) then
         write(*,*)'Time sampling differs',del,dt
         fails = fails + 1
      endif
      if (iftype .ne. "ITIME  ") then
         write(*,*)'File types differ',iftype, "ITIME  "
         fails = fails + 1
      endif


      call newhdr()
      del = -12345.0
      beg = -12345.0
      call getkhv('kstnm',kstnm,nerr)
      call getfhv('delta',dt,nerr)
      call getfhv('b',b,nerr)
      call getihv('iftype',iftype,nerr)
      if (b .ne. beg) then
         write(*,*)'Begin times differ',b,beg
         fails = fails + 1
      endif
      if (kstnm .ne. "-12345") then
         write(*,*)'Station names differ',kstnm, "sta"
         fails = fails + 1
      endif
      if (del .ne. dt) then
         write(*,*)'Time sampling differs',del,dt
         fails = fails + 1
      endif
      if (iftype .ne. "ITIME  ") then
         write(*,*)'File types differ',iftype, "ITIME  "
         fails = fails + 1
      endif


      kname  = 'test_io_big.sac'
      call rsac1(kname, yarray, nlen, beg, del, MAX, nerr)

      call getkhv('kstnm',kstnm,nerr)
      call getfhv('delta',dt,nerr)
      call getfhv('b',b,nerr)
      call getihv('iftype',iftype,nerr)
      if (beg .ne. b) then
         write(*,*)'Begin times differ',beg,b
         fails = fails + 1
      endif
      if (del .ne. dt) then
         write(*,*)'Time sampling differs',del,dt
         fails = fails + 1
      endif
      if (iftype .ne. "ITIME  ") then
         write(*,*)'File types differ',iftype, "ITIME  "
         fails = fails + 1
      endif
      if (kstnm .ne. "sta") then
         write(*,*)'Station names differ',kstnm, "sta"
         fails = fails + 1
      endif

      end
