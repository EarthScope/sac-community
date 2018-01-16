fg seismo
ch cmpaz 0.0 cmpinc 90.0
write signal1.sac
ch cmpaz 90.0 cmpinc 90.0
write signal2.sac

read signal1.sac signal2.sac
rotate
write signalr.sac signalt.sac

quit
