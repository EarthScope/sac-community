$KEYS BVALUE

fg line 1 0 npts 10 delta 1.0
ch nzyear 2000 nzjday 1 nzhour 0 nzmin 0 nzsec 0 nzmsec 0
lh kzdate kztime
write test.merge1.sac

fg line 1 0 npts 10 delta 1.0 begin $BVALUE
ch nzyear 2000 nzjday 1 nzhour 0 nzmin 0 nzsec 0 nzmsec 0
lh kzdate kztime
write test.merge2.sac

read test.merge1.sac test.merge2.sac
merge gap interp verbose tolerance 0.0
write test.merge.sac

color on inc
symbol 2
read test.merge.sac test.merge1.sac test.merge2.sac

fg line 1 0 npts &1,npts begin 0.0 delta 1.0
write test.merge.expect.sac

subf test.merge.sac
abs
lh depmin depmax
p1 ; pause period 0.25

read test.merge.expect.sac test.merge.sac
p2 ; pause period 0.25



