
do type list acc bbdisp bbvel benbog dss dwwssn ekalp6 ekasp2  gbalp gbasp gsref hfslpwb iw llsn lrsmlp lrsmsp oldbb oldkir ptbllp redkir s750 wa wabn wiech wwlpbn wwsp wwspbn ykalp ykasp 
  fg seismo
  transfer from $type to none
  lh depmin depmax depmen 
enddo

fg seismo
transfer from elmag freeperiod 15 magnification 375 to none 
lh depmin depmax depmen 

do type list LV LR LT MV MR MT EV ER ET KV KR KT
  fg seismo
  transfer from LLL subtype $type to none 
  lh depmin depmax depmen 
enddo

do type list BB HF
  fg seismo
  transfer from LNN subtype $type to none 
  lh depmin depmax depmen 
enddo

do type list LP IP SP
  fg seismo
  transfer from NORESS subtype $type to none 
  lh depmin depmax depmen 
enddo

do type list LP IP SP
  fg seismo
  transfer from NORESSHF subtype $type to none 
  lh depmin depmax depmen 
enddo

do type list BB SP LPDE
  fg seismo
  transfer from SRO subtype $type to none 
  lh depmin depmax depmen 
enddo

do a list N O
do b list T L B D N E
do c list V R T
  fg seismo
  if "$a$$b$" != "NE" then
     transfer from SANDIA subtype "$a$$b$$c$" to none 
  else 
     message "Not supported $a$$b$$c$"
  endif
  lh depmin depmax depmen 
enddo
enddo
enddo

do a list N O
do b list T L B D N E
do c list V R T
  fg seismo
  if "$a$$b$" != "NE" then
    transfer from SANDIA3 subtype "$a$$b$$c$" to none 
  else 
     message "Not supported $a$$b$$c$"    
  endif
  lh depmin depmax depmen 
enddo
enddo
enddo

do a list CP ON NT NY SD
do b list KL KM KS 7S
do c list .Z .N .E
  fg seismo
  if "$a$$b$" != "CP7S" then
     transfer from RSTN subtype "$a$$b$$c$" to none 
  else 
     message "Not supported $a$$b$$c$"
  endif
  lh depmin depmax depmen 
enddo
enddo
enddo

fg seismo
transfer from portable freeperiod 1.0 dampingfactor 1.0 cornerfrequency 1.0 to none
lh depmin depmax depmen 

fg seismo
transfer from general nzeros 2 freeperiod 1 magnification 1 dampingfactor 1 to none
lh depmin depmax depmen

fg seismo
transfer from reftek freeperiod 1 damping 1 corner 1 magnification 1 highpass 1  to none
lh depmin depmax depmen

fg seismo
lh depmin depmax
transfer from none to acc
lh depmin depmax