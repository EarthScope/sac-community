#!/bin/sh

\rm -f *.sac brune convolvef convolvec
if [ ! -e synthetic.sac ]; then
    cp sample_runs/synthetic.sac .
fi

echo "Creating input for convolutions ..."
sac --copyright-off <<EOF
fg triangle npts 8 delta 0.02 begin -0.08
write triangle_n8_d0.02.sac
fg impulse npts 12 delta 0.02 begin 0
write impulse_n12_d0.02.sac
quit
EOF

echo "Create a Brune pulse"
gfortran -o brune brune.f `sac-config --libs sacio`
./brune 0.1 brune_pulse.sac

P_NAME_T=triangle_n8_d0.02.sac
P_NAME_B=brune_pulse.sac
WF_NAME_I=impulse_n12_d0.02.sac
WF_NAME_S=synthetic.sac
OUTPUT_N=conv_f_n.sac
OUTPUT_Y=conv_f_y.sac

echo "Compiling convolve in C ..."
cc -o convolvec convolvec.c `sac-config --cflags --libs sacio`

echo "Running convolvec: Time series then Discrete"
./convolvec $P_NAME_T $WF_NAME_I $OUTPUT_N n
./convolvec $P_NAME_T $WF_NAME_I $OUTPUT_Y y

echo "Runs with synthetic"
./convolvec $P_NAME_T $WF_NAME_S triangle_synth-discrete.sac y
./convolvec $P_NAME_T $WF_NAME_S triangle_synth-ts.sac n
./convolvec $P_NAME_B $WF_NAME_S brune_synth-ts.sac y

echo "Compare results"
for z in *.sac ; do
    sacdiff -f 1e-4 $z sample_runs/$z
done
