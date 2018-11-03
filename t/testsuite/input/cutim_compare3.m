
read %FILE
cutim %CUTLINE1 %CUTLINE2  %CUTLINE3
write test.cut.1.sac test.cut.2.sac test.cut.3.sac

cut %CUTLINE1
read %FILE
cut off
read more test.cut.1.sac
macro cutim_comps.m

cut %CUTLINE2
read %FILE
cut off
read more test.cut.2.sac
macro cutim_comps.m

cut %CUTLINE3
read %FILE
cut off
read more test.cut.3.sac
macro cutim_comps.m

