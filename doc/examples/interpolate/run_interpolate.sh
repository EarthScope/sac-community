#!/bin/sh

\rm -f IP07.dHHiZ IP07_sac-int0.0025.dHHZ Time-Series_Interpolation.pdf

cp -f sample-run/IP07.dHHZ .

gfortran -o interpolate interpolate.f interpolate_subs.f `sac-config --cflags --libs sac sacio`

./interpolate IP07.dHHZ 0.0025

sac  --copyright-off <<EOF
m interpolate.m
quit
EOF

echo  "Compare this run with stored results"
FILES="IP07.dHHiZ IP07_sac-int0.0025.dHHZ"
for file in $FILES; do
    sacdiff -f 1e-4 -q $file sample-run/$file
done


