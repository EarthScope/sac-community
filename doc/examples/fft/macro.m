echo on
fg impulse npts 16 delta 1.0
write imp.sac
fft
write imp_fft.sac

fg sine (1/8) 0 npts 16 delta 1.0
write sin.sac
fft
write sin_fft.sac
quit

