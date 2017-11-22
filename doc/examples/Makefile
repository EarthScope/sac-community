

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
	    time_shift_prog \
      interpolate_prog \
      convolve_saclib_c \
      convolve_saclib_f

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
	./convolvef convolve/brune_pulse.sac convolve/synthetic.sac convolve/convolvef_out_sac1.sac n
	./convolvec convolve/brune_pulse.sac convolve/synthetic.sac convolve/convolvec_out_sac1.sac n
	diff convolve/convolvef_out_sac1.sac convolve/convolvef_out-ok.sac
	diff convolve/convolvec_out_sac1.sac convolve/convolvec_out-ok.sac
	./correlatef
	./correlatec
	diff correlatef_out_sac1.sac correlatef_out1.sac
	diff correlatec_out_sac1.sac correlatec_out1.sac
	./convolve_saclib_c
	./convolve_saclib_f
	diff convolvec_out1.sac convolvec_out_sac1.sac
	diff convolvef_out1.sac convolvef_out_sac1.sac

clean:
	rm -f $(PROGS_IO) $(PROGS_LIB) *.o *~ core
	rm -f *.sac
	rm -f convolve/convolvef_out_sac1.sac
	rm -f convolve/convolvec_out_sac1.sac
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
convolve_saclib_f: convolve_saclib/convolve_saclib_f.o
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)

filterc: filter/filterc.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
envelopec: envelope/envelopec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
correlatec: correlate/correlatec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
convolvec: convolve/convolvec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
convolve_saclib_c: convolve_saclib/convolve_saclib_c.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

time_shift_prog: time_shift/time_shift.f time_shift/time_shift_subs.f
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)

interpolate_prog: interpolate/interpolate.f interpolate/interpolate_subs.f
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)

convolve_input: convolve/convolve_input.f
	$(FC) $(CFLAGS) -o $@ $^ $(LDLIBS)

