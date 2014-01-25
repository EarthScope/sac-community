
setbb maxerror 1e-14

read test_transfer_evresp_data_in.m
rtr
lh depmin depmax depmen b e
transfer from evalresp fname test_transfer_evresp_resp.m to none freq 0.002 0.005 0.1 0.2
lh depmin depmax depmen

subf test_transfer_evresp_data_out.m
sqr
sqrt
int

if &1,depmax gt %maxerror%
   message "transfer error too large: &1,depmax&"
else
   message "transfer error: &1,depmax&"
endif




