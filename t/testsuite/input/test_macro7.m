$keys file comps
* Keys:
* file:  Inputfile without extension
* comps: filename extensions of Inputfile (e.g.: E N Z)
$DEFAULT comps e n z

**echo on
cut off
qdp off
ylim all
setbb set_xlim no

setbb i 0
do sfx list $comps
   setbb i ( %i + 1 )
   if %i eq 1 
     read $file$.$sfx$
   else
     read more $file$.$sfx$
   endif
enddo

sync
rmean
rtrend
clear

#  mks changing the two lowest period filters
#setbb lo1  0.1 hi1  1
#setbb lo2  0.2 hi2  2
setbb lo1  0.4 hi1  4
setbb lo2  0.5 hi2  5
setbb lo3  0.2 hi3  3
setbb lo4  0.3 hi4  3
setbb lo5  0.5 hi5  4
setbb lo6  0.6 hi6  3
setbb lo7  0.8 hi7  6
setbb lo8  1   hi8  3
setbb lo9  1   hi9  5
setbb lo10 1   hi10 8
setbb lo11 2   hi11 3
setbb lo12 2   hi12 6
setbb lo13 3   hi13 8
setbb lo14 4   hi14 10

*echo on
* sc echo event, snr, sne, snn > tmpfprod.sn

DO j FROM 1 TO 14
setbb nyq_fr (1 / 2 / &1,DELTA& )

if $j eq 1
setbb lo %lo1% hi %hi1%
endif
if $j eq 2
setbb lo %lo2% hi %hi2%
endif
if $j eq 3
setbb lo %lo3% hi %hi3%
endif
if $j eq 4
setbb lo %lo4% hi %hi4%
endif
if $j eq 5
setbb lo %lo5% hi %hi5%
endif
if $j eq 6
setbb lo %lo6 hi %hi6
endif
if $j eq 7
setbb lo %lo7 hi %hi7
endif
if $j eq 8
setbb lo %lo8 hi %hi8
endif
if $j eq 9
setbb lo %lo9 hi %hi9
endif
if $j eq 10
setbb lo %lo10 hi %hi10
endif
if $j eq 11
setbb lo %lo11 hi %hi11
endif
if $j eq 12
setbb lo %lo12 hi %hi12
endif
if $j eq 13
setbb lo %lo13 hi %hi13
endif
if $j eq 14
setbb lo %lo14 hi %hi14
endif
*
* beautiful...
*
* we are still in that DO-LOOP:
*

*  message hi %hi% nyq %nyq_fr%
  while %hi% ge %nyq_fr%
  evaluate to hi %hi% - 2
  evaluate to hi %hi% - 2
  enddo
  rmean
  rtrend
*****************************
  bp c %lo% %hi%
*****************************
*rmean
*rtrend
  ch kuser0 %lo%
  ch kuser1 %hi%
  write $file$.%lo%-%hi%.fb.e $file$.%lo%-%hi%.fb.n $file$.%lo%-%hi%.fb.z
message "LETS SEE"
m magicpathtothisincrediblemacro.m file $file$.%lo%-%hi%.fb basef $file$ lo %lo% hi %hi%
message "OK"
  setbb i 0
*
* re-read files:
*
  do sfx list $comps
     setbb i ( %i + 1 )
     if %i eq 1 
       read $file$.$sfx$
     else
       read more $file$.$sfx$
     endif
  enddo
  unsetbb hi lo
enddo
unsetbb i nyq_fr xl xh set_xlim
