


setbb max 40

do i = 1 , %max
  plabel " Label  $i " position 0.5 ( 0.9 - ( $i / %max ) ) 0.0 size small
  message $i
enddo

fg seismo

p1

pause period 0.5

do i = 1, %max
  plabel $i off
enddo
