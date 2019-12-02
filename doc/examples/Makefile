

FC=gfortran

FFLAGS=-Wall `sac-config --cflags`
CFLAGS=-Wall `sac-config --cflags`
SACLIB=`sac-config --libs sac sacio`
SACIO=`sac-config --libs sacio`
LDLIBS+=$(SACIO)

PROGS_IO = rsac1c rsac1f \
	   rsac2c rsac2f \
	   gethvc gethvf \
	   wsac1c wsac1f \
	   wsac2c wsac2f \
	   wsac3c wsac3f \
	   wsac4c wsac4f \
	   wsac5c wsac5f 

all: iotest dirs

io: $(PROGS_IO)
lib: dirs

time_shift_prog: time_shift/time_shift.f
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)

iotest: io
	./rsac1c
	./rsac1f
	./rsac2c
	./rsac2f
	./gethvc
	./gethvf
	./wsac1c
	./wsac1f
	./wsac2c
	./wsac2f
	./wsac4c
	./wsac4f
	./wsac5c
	./wsac5f

DIRS= cut differentiate envelope fft filter integrate rmean rtrend taper 

dirs:
	(cd cut && make)
	(cd differentiate && make)
	(cd envelope && make)
	(cd fft && make)
	(cd filter && make)
	(cd integrate && make)
	(cd rmean && make)
	(cd rtrend && make)
	(cd taper && make)

clean: dirs-clean
	\rm -f $(PROGS_IO)
	\rm -f STAZ STBZ STCZ STDZ STEZ STFZ STGZ STHE STHN STHZ xyzdata expdata

dirs-clean:
	for d in $(DIRS); do \
	  (cd $$d && make clean ); \
  done
