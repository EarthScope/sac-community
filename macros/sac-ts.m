* ${SACHOME}/macros/sac-ts.m
* Macro to do a time-sift in SAC
* sample: macro sac-ts.m filein fileout ts
*   where ts is time-shift (new-old) in seconds
* The time-shift is stored in user9
* must havve IZTYPE - IO
setbb ts $3
r $1

if &1,IZTYPE ne io
  chnhdr allt (0 - &1,o&) IZTYPE IO
endif

ch b (%ts + &1,b&)
ch user9 %ts
write $2
