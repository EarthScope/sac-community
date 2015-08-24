

FC=gfortran

FFLAGS=-Wall `sac-config --cflags`
CFLAGS=-Wall `sac-config --cflags`
SACLIB=`sac-config --libs sac`
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

PROGS_LIB = convolve_input filterf filterc \
	    envelopef envelopec \
	    correlatec correlatef \
	    convolvef convolvec \
			convolvef2


all: 
	( LDLIBS=''          $(MAKE) io   )
	( LDLIBS='$(SACLIB)' $(MAKE) lib  )
	( LDLIBS=''          $(MAKE) test )

io: $(PROGS_IO)
lib: $(PROGS_LIB)

test:
	./create_input.sh
	./convolve_input
	./filterf
	./filterc
	diff filterf_out_sac.sac filterf_out.sac 
	diff filterc_out_sac.sac filterc_out.sac 
	./envelopef
	./envelopec
	diff envelopef_out_sac.sac envelopef_out.sac
	diff envelopec_out_sac.sac envelopec_out.sac
	./convolvef
	./convolvec
	diff convolvef_out_sac1.sac convolvef_out1.sac
	diff convolvec_out_sac1.sac convolvec_out1.sac
	./correlatef
	./correlatec
	diff correlatef_out_sac1.sac correlatef_out1.sac
	diff correlatec_out_sac1.sac correlatec_out1.sac
	./convolvef2

clean:
	rm -f $(PROGS_IO) $(PROGS_LIB) *.o *~ core
	rm -f *.sac
	rm -f `find . -name '*.o'`
	rm -f `find . -name '*~'`

filterf: filter/filterf.o
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)
envelopef: envelope/envelopef.o
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)
correlatef: correlate/correlatef.o
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)
convolvef: convolve/convolvef.o
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)
convolvef2: convolve/convolve.o convolve/convolvef2.o
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)

filterc: filter/filterc.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
envelopec: envelope/envelopec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
correlatec: correlate/correlatec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
convolvec: convolve/convolvec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
