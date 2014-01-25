
setbb maxerror 1e-14

fg seismo
fg impulse npts 1000 delta 0.01
lh depmin depmax depmen b e
transfer from fap s test_transfer_fap_input.m to none
lh depmin depmax depmen

message "Transfer using FAP file from Polezero"
fg impulse npts 1000 delta 0.01
transfer from fap s test_transfer_fap_from_pz.m to none 
lh depmin depmax depmen
write test.transfer.fappz

message "Transfer using FAP file from Evalresp"
fg impulse npts 1000 delta 0.01
transfer from fap s test_transfer_fap_from_evresp.m to none 
lh depmin depmax depmen
write test.transfer.fapev

message "Transfer using Polezero"
fg impulse npts 1000 delta 0.01
transfer from polezero s test_transfer_pz.m to none 
lh depmin depmax depmen
write test.transfer.pz

read test.transfer.fappz 
subf test.transfer.pz
sqr
sqrt
int
if &1,depmax gt %maxerror%
   message "transfer error too large: &1,depmax& "
else
   message "transfer error: &1,depmax&"
endif

read test.transfer.fapev
subf test.transfer.pz
sqr
sqrt
int
if &1,depmax gt %maxerror%
   message "transfer error too large: &1,depmax& "
else
   message "transfer error: &1,depmax&"
endif

message "Transfer using FAP file from Polezero"
fg seismo npts 1000 delta 0.01
transfer from fap s test_transfer_fap_from_pz.m to none 
lh depmin depmax depmen
write test.transfer.fappz

message "Transfer using FAP file from Evalresp"
fg seismo npts 1000 delta 0.01
transfer from fap s test_transfer_fap_from_evresp.m to none 
lh depmin depmax depmen
write test.transfer.fapev

message "Transfer using Polezero"
fg seismo npts 1000 delta 0.01
transfer from polezero s test_transfer_pz.m to none 
lh depmin depmax depmen
write test.transfer.pz

read test.transfer.fappz 
subf test.transfer.pz
sqr
sqrt
int
if &1,depmax gt %maxerror%
   message "polezero -> fap transfer error too large: &1,depmax& "
else
   message "polezero -> fap transfer error: &1,depmax&"
endif

read test.transfer.fapev
subf test.transfer.pz
sqr
sqrt
int
if &1,depmax gt %maxerror%
   message 'evalresp -> fap transfer error too large: &1,depmax& "
else
   message "evalresp -> fap transfer error: &1,depmax&"
endif




