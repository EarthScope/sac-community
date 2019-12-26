** ${SACHOME}/macros/cut_runs.m
echo on

* no cutting
cut off
fg seismo
lh b e a kztime

* begin to end---same as no cutting.

lh b e a kztime

* First 3 secs of the file
cut B 0 3
fg seismo
lh b e a kztime

* From 0.5 secs before to 3 secs after first arrival
cut A -0.5 3
fg seismo
lh b e a kztime

* From 10 to 15 secs relative to zero
cut 10 15
fg seismo
lh b e a kztime

* From 0.5 to 5 secs relative to disk file start.
cut b 0.5 5
fg seismo
lh b e a kztime
fg seismo

* First 3 secs of the file and next 3 sec
cut b 0 3
fg seismo
write tmp.1

cut b 3 6
fg seismo
write tmp.2

cut off
read tmp.?
lh b e a kztime
title "top: cut b 0 3; bottom: cut b 3 6" 
p1
save cut-test.pdf
\rm tmp.*

* Examples using CUTERR_

cut off
fg seismo
lh b e a npts kztime

cut a -0.5 15
fg seismo
lh b e a npts kztime

cuterr usebe ; cut a -0.5 15
fg seismo
lh b e a npts kztime

cuterr fillz ; cut a -0.5 15
fg seismo
lh b e a npts kztime

