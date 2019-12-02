echo on
fg seismo
write original.sac
r original.sac
rtrend
write raw.sac
r raw.sac
cut 10.45 10.6
r
write raw.sac
taper
write taper_sac.sac
quit
