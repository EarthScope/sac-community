
do i = 1 , 10
   message $i
enddo

do i from 1 to 10
   message $i
enddo

do i = 1 , 10 , 2
   message $i
enddo

do i from 1 to 10 by 2
   message $i
enddo

do i = 1 , 10 , 3
   message $i
enddo

do i from 1 to 10 by 3
   message $i
enddo

do i list 1 2 3 5 6 8 2 3 10
   message $i
enddo

do i = 3 , 1
   message $i
enddo

do i from 3 to 1
   message $i
enddo

do i = 5 , 1 , -1
   message $i
enddo

do i from 5 to 1 by -1
   message $i
enddo

setbb i 1
while %i LE 5
   message %i
   setbb i ( %i + 1 )
enddo


