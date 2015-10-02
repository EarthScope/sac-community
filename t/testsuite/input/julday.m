

echo off
fg seismo
do i list 1900 1999 2000 2004 2005
  ch nzyear $i
  do j = -1, 367
    ch nzjday $j
    message "$i $j &1,kzdate"
  enddo
enddo
quit

