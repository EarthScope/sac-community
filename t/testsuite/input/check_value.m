

setbb maxerror 1e-5
setbb checkvalue (abs ($1 - $2) )
if %checkvalue > %maxerror
    message "Check Failed $1 != $2    [ $3 ] Error: %checkvalue"
endif