


setbb max 40

do i = 1 , %max
  plabel " Label  $i " position 0.5 ( 0.9 - ( $i / %max ) ) 0.0 size small
  message $i
enddo

