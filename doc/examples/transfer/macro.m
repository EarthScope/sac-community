** message "Create Fake Data"
fg seismo
ch kstnm AFI knetwk IU khole 10 kcmpnm BHZ
ch nzyear 2011 nzjday 1
write raw.sac

** message "Evalresp"
transfer from evalresp to none freq 0.002 0.005 12 20
* Convert from nm to m
div 1e9
write deconvolved_from_evr.sac

** message "Polezero"
read raw.sac
transfer from polezero s SAC_PZs_IU_AFI_BHZ_10_ to none freq 0.002 0.005 12 20
write deconvolved_from_pz.sac
quit
