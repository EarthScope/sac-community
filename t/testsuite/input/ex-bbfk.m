* BBFK example macro. Usage: m ex-bbfk.m [zero | n-s | e-w | diag ]
* The Seismic Analysis Code: A Primer and User's Guide
*   By George Helffrich, James Wookey, Ian Bastow
*   Cambridge University Press, Sep 19, 2013
*  pp. 102-103

* No cuts by default
setbb cutew *
setbb cutns *

if $1 eq diag
* Time shift N-S and E-W dirs
  setbb cutew cutim
  setbb cutns cutim
endif

if $1 eq n-s
* Time shift on N-S dir
  setbb cutns cutim
endif

if $1 eq e-w
* Time shift on E-W dir
  setbb cutew cutim
endif

* Zero Exterior trace areas
cuterr fillz

* xo and yo are x-y offsets
setbb xo -1
setbb yo  0

do f list a b c d e f g h
  message "%xo %yo"
  fg seismogram
  rmean
  %cutew (&1,b - %xo) (&1,e - %xo)
  ch kuser1 &1,kstnm user7 %xo user8 %yo
  write arr_r$f$.sac
* Increment xo
  setbb xo ( %xo + 1 )
enddo

setbb xo 0
setbb yo -1
do f list a b c e d f g h
  message "%xo %yo"
  fg seismogram
  rmean
  %cutns (&1,b - %yo) (&1,e - %yo)
  ch kuser1 &1,kstnm user7 %xo user8 %yo
  write arr_b$f$.sac
* Increment yo
  setbb yo ( %yo + 1 )
enddo

sc rm arr_rb.sac
read arr_[r,b]*sac

bbfk filter off wave 1 pds size 180 100 norm




