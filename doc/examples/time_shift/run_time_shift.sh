#!/bin/sh

\rm -f *.sac time_shift

echo "Creating input for timeshift ..."
sac --copyright-off <<EOF
fg seismo
rtr ; taper
chnhdr allt (0 - &1,o&) IZTYPE IO
lp co 5.0 np 4
ch kevnm "lp_co5_np4"
write lpco5np4.sac
read lpco5np4.sac
rtr ; taper
m ${SACHOME}/macros/sac-ts.m lpco5np4.sac lpco5np4_sacts.sac -0.05
quit
EOF

INPUT=lpco5np4.sac
OUTPUT=lpco5np4_ts.sac

echo "Compiling timeshift ..."
gfortran -Wall -Wextra -fbounds-check -o time_shift time_shift.f  `sac-config --libs libsac libsacio`

echo "Running timeshift ..."
./time_shift $INPUT $OUTPUT -0.05

echo "Comparing results this run and stored"
for z in *.sac ; do
    sacdiff -qf 1e-4 $z sample-run/$z
done

echo "Comparing program and SAC time-shifts"
sacdiff -qf 1e-4 lpco5np4_sacts.sac lpco5np4_ts.sac


