echo on
fg seismo
write original.sac
rtr ; taper
write raw.sac
envelope
write env_sac.sac
quit
