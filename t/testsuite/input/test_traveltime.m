fg seismo
ch evla 0.0 evlo 0.0
ch stla 0.0 stlo 30.0
lh dist gcarc lcalda
do dist = 1 , 91 , 10
   ch stlo $dist
   lh gcarc
   do depth = 15 , 615 , 100
      ch evdp $depth
      traveltime       
   enddo
enddo

