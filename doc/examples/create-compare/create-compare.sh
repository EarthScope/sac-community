#!/bin/sh

\rm -f seismo.sac envelope_sac.sac convolve_sac.sac envelope.pdf convolve.pdf original.sac taper_sac.sac taper.pdf

# taper
sac --copyright-off  <<EOF
echo on
fg seismo
ch kevnm "WAVEFORM"
write original.sac
r original.sac
rtrend
write raw.sac
r raw.sac
cut 10.45 10.6
r
taper
write taper_sac.sac
r original.sac taper_sac.sac taper_program.sac
fileid location ll
line increment list 1 2 4
p2
save taper.pdf
quit
EOF

# envelope
sac --copyright-off  <<EOF
echo on
fg seismo
ch kevnm "WAVEFORM"
write seismo.sac
rtrend ; taper
envelope
ch kevnm "ENVELOPE SAC"
write envelope_sac.sac
read seismo.sac envelope_sac.sac envelope_program.sac
qdp off
p1
save envelope.pdf
quit
EOF

# Convolution
sac --copyright-off  <<EOF
echo on
read brune.sac synthetic.sac
convolve
ch kevnm "CONVOLVE SAC"
write convolve_sac.sac
read brune.sac synthetic.sac convolve_program.sac convolve_sac.sac
xlim -1 14
qdp off
mul 1.0 0.024 1.0 1.0
title "mul 1.0 0.024 1.0 1.0"
fileid location ur
line increment list 3 1 2 6
p2
save convolve.pdf
quit
EOF
