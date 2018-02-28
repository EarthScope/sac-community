#!/bin/sh

echo "Creating input for timeshift ..."
sac --copyright-off <<EOF
fg seismo
rtr ; taper
chnhdr allt (0 - &1,o&) IZTYPE IO
write seismo.sac
lp co 5.0 np 4
ch kevnm "lp_co5_np4"
write seismo_lpco5np4.sac
m ${SACHOME}/macros/sac-ts.m seismo_lpco5np4.sac seismo_lpco5np4_sacts.sac -0.05
quit
EOF

INPUT=seismo_lpco5np4.sac
OUTPUT=seismo_lpco5np4_ts.sac

echo "Compiling timeshift ..."
gfortran -Wall -Wextra -fbounds-check -o time_shift time_shift.f  `sac-config --libs libsac libsacio`

echo "Running timeshift ..."
./time_shift $INPUT $OUTPUT -0.05

echo "Comparing results"
for z in *.sac ; do
    diff $z sample-run/$z
done


