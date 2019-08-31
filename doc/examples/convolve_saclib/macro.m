fg triangle delta 1e-2 npts 100
write convolvec_in2.sac
write convolvef_in2.sac
fg seismo
write convolvec_in1.sac
write convolvef_in1.sac
read convolvec_in1.sac convolvec_in2.sac
convolve pulse 2
ch b 0.0
div &1,delta
write convolvec_out_sac1.sac
write convolvef_out_sac1.sac
quit
