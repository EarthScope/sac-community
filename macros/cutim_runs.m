** ${SACHOME}/macros/cutim_runs.m

echo on

* no cutting
cut off
fg seismo
lh b e a kztime

fg seismo
* begin to end---same as no cutting.
cutim B E
lh b e a kztime

fg seismo
* First 3 secs of the file.
cutim B 0 3
lh b e a kztime

fg seismo
* From 0.5 secs before to 3 secs after first arrival
cutim A -0.5 3
lh b e a kztime

fg seismo
* From 10 to 15 secs relative to zero
cutim 10 15
lh b e a kztime

fg seismo
* From 0.5 to 5 secs relative to disk file start.
cutim b 0.5 5
lh b e a kztime

fg seismo
* First 3 secs of the file and next 3 sec
cutim b 0 3 b 3 6
lh b e a kztime
title "cutim b 0 3 b 3 6"
p1
save cutim_run.pdf
\rm tmp.*

