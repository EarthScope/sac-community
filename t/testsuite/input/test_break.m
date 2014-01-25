
setbb i 1
while %i LE 10
  message %i
  if %i EQ 5
     break
  endif
  setbb i ( %i + 1 )
enddo

message " "

do i = 1 , 3
   do j = 1 , 3
      message " $i , $j "
   enddo
enddo

message " "

do i = 1 , 3
   message " $i "
   if $i EQ 2 
      break
   endif
enddo
 
message " "

do i = 1 , 3
   do j = 1 , 3
      message " $i , $j "
      if $j EQ 2
         break
      endif
   enddo
enddo

message " "

do i = 1 , 3
   do j = 1 , 3
      do k = 1 , 3
         message " $i , $j , $k "
         if $k EQ 2
            break
         endif
      enddo
   enddo
enddo

message " "
