#!/bin/sh

echo "Creating input for timeshift ..."
sac --copyright-off <<EOF
fg seismo
write seismo.sac
rtr
lp co 5.0 np 4
write seismo_lpco5np4.sac
quit
EOF

INPUT=seismo_lpco5np4.sac
OUTPUT=seismo_lpco5np4_ts.sac
EXP=seismo_lpco5np4_ts_org.sac

echo "Compiling timeshift ..."
gfortran -Wall -Wextra -fbounds-check -o time_shift time_shift.f time_shift_subs.f $(sac-config --libs sacio)

echo "Running timeshift ..."
./time_shift $INPUT $OUTPUT -0.05

echo "Comparing timeshift results ..."
diff $OUTPUT $EXP


