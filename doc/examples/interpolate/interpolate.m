r IP07.dHHZ
interpolate delta 0.0025
write IP07_sac-int0.0025.dHHZ
r IP07.dHHZ IP07.dHHiZ IP07_sac-int0.0025.dHHZ
cut 0.93 1.04
r
fileid location ll
line increment list 1 2 3
xlabel "Time (seconds)" ; ydiv power off
title "Top to bottom: raw, Forsythe interp., SAC interp."
p2
save Time-Series_Interpolation.pdf
lh depmin depmax
