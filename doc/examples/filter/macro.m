fg seismo
write raw.sac
bp co 0.10 1.00 p 2 n 4
write bandpass_sac.sac
read raw.sac
lp co 2.0 p 2 n 4
write lowpass_sac.sac
read raw.sac
hp co 10.0 p 2 n 4
write highpass_sac.sac
read raw.sac
br bessel co 2.0 10.0 p 2 n 4
write bandreject_sac.sac
quit
