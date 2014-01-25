
do i = 1 , 17
   fg seismo
   ch kevnm (substring 1 $i "12345678901234567890")
   message "$i &1,kevnm"
   write alpha test.alpha.3
   read alpha test.alpha.3
   message "$i &1,kevnm"
   lh kevnm
enddo
