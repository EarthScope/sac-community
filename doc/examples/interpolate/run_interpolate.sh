#!/bin/sh

if [ ! -e IP07.dHHZ ]; then
    cp sample-run/IP07.dHHZ .
fi

gfortran -o interpolate interpolate.f interpolate_subs.f `sac-config --cflags --libs sac sacio`

./interpolate IP07.dHHZ 0.0025

sac <<EOF
m interpolate.m
quit
EOF

FILES="IP07.dHHiZ IP07_sac-int0.0025.dHHZ"
for file in $FILES; do
    diff $file sample-run/$file
done


