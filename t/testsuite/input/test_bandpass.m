
do c list 0.001 0.002 0.003 0.004 0.005 0.006 0.007 0.008 0.009 0.010
   r test_2010334033600.00.ABSI.HHZ.m
   rmean
   rtrend
   taper w 0.1
   interpolate delta $c$
   bp co 0.01 0.5 n 2 p 2 
   message "delta $c$"
   write test.bp.$c$.sac
enddo

read test.bp.*
p1 
pause period 0.25

