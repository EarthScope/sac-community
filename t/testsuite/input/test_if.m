
if 1 EQ 1
   message "True 1 EQ 1"
endif

if 0 EQ 1
   message "False 0 EQ 1"
endif

if 1 EQ 1
   message "True 1 EQ 1"
else
   message "False 1 EQ 1"
endif

if 0 GE 1
   message "True 0 GE 1"
else
   message "False 0 GE 1"
endif

if 0 LE 1
   message "True 0 LE 1"
else
   message "False 0 LE 1"
endif

if 0 LT 1
   message "True 0 LT 1"
else
   message "False 0 LT 1"
endif

if 0 GT 1
   message "True 0 GT 1"
else
   message "False 0 GT 1"
endif

if 1 GE 1
   message "True 1 GE 1"
else
   message "False 1 GE 1"
endif

if 1 LE 1
   message "True 1 LE 1"
else
   message "False 1 LE 1"
endif

if 1 NE 1
   message "True 1 NE 1"
else
   message "False 1 NE 1"
endif

if 0 NE 1
   message "True 0 NE 1"
else
   message "False 0 NE 1"
endif


if 0 LT 1
   message "True 0 LT 1"
elseif 0 EQ 1
   message "False 0 EQ 1"
elseif 0 GT 1
   message "False 0 GT 1"
endif

if 2 LT 1
   message "True 2 LT 1"
elseif 2 EQ 1
   message "False 2 EQ 1"
elseif 2 GT 1
   message "False 2 GT 1"
endif

if 1 LT 1
   message "True 1 LT 1"
elseif 1 EQ 1
   message "False 1 EQ 1"
elseif 1 GT 1
   message "False 1 GT 1"
endif

fg seismo

if &1,npts LT 1000
   message "False &1,npts LT 1000"
elseif &1,npts GT 1000
   message "False &1,npts GT 1000"
elseif &1,npts EQ 1000
   message "True &1,npts EQ 1000"
else 
   message "False"
endif

if 1 EQ 1 Really long line after a comparison, that really shouldnt be here
   message "True 1 EQ 1"
endif

if 1 EQ 0 Really long line after a comparison, that really shouldnt be here
   message "False 1 EQ 0"
endif

if 1.2 GT 1
   message "True 1.2 GT 1"
endif
if 1.2 LT 1
   message "True 1.2 GT 1"
endif

if 1.0 EQ 1
   message "True 1.0 EQ 1"
endif
if 1.0 NE 1
   message "True 1.0 NE 1"
endif
if 0.9 EQ 1
   message "False 0.9 EQ 1"
endif
if 0.9 NE 1
   message "True 0.9 NE 1"
endif

if 1.0 GE 1
   message "True 1.0 GE 1"
endif
if 1.2 GE 1
   message "True 1.2 GE 1"
endif
if 0.9 GE 1
   message "False 0.9 GE 1"
endif

if 1.0 LE 1
   message "True 1.0 LE 1"
endif
if 1.2 LE 1
   message "False 1.2 LE 1"
endif
if 0.9 LE 1
   message "True 0.9 LE 1"
endif



