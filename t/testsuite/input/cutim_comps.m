

lh b e npts depmin depmax
setbb d ( &1,npts - &2,npts )
if %d != 0 
  message "npts different: &1,npts &2,npts ERROR"
else
  message "npts equal: &1,npts &2,npts OK"
endif
setbb d (abs( &1,b - &2,b ))
if %d > 1e-14
  message "b different: &1,b &2,b ERROR"
else
  message "b equal: &1,b &2,b OK"
endif
setbb d (abs( &1,e - &2,e ))
if %d > 1e-14
  message "e different: &1,e &2,e ERROR"
else
  message "e equal: &1,e &2,e OK"
endif
setbb d (abs( &1,depmin - &2,depmin ))
if %d > 1e-14
  message "depmin different: &1,depmin &2,depmin ERROR"
else
  message "depmin equal: &1,depmin &2,depmin OK"
endif
setbb d (abs ( &1,depmax - &2,depmax ) )
if %d > 1e-14
  message "depmax different: &1,depmax &2,depmax ERROR"
else
  message "depmax equal: &1,depmax &2,depmax OK"
endif
