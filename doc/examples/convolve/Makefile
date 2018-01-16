
PROGF=convolvef
PROGC=convolvec

OBJSF=$(PROGF).o
OBJSC=$(PROGC).o

SACHOME ?= /usr/local/sac   # Set if not specified
LDLIBS=`$(SACHOME)/bin/sac-config -l -c sacio`
FFLAGS=`$(SACHOME)/bin/sac-config -c `
CFLAGS=`$(SACHOME)/bin/sac-config -c `
SAC=$(SACHOME)/bin/sac
FC=gfortran


# Compile Fortran 90 to object files
%.o: %.f90
	$(FC) $(FFLAGS) -o $@ -c $<


all: xrun

xrun: $(PROGF) $(PROGC)
	./$(PROGF) brune_pulse.sac synthetic.sac convolvef_out.sac n
	./$(PROGC) brune_pulse.sac synthetic.sac convolvec_out.sac n
	diff convolvec_out.sac convolvec_out-ok.sac
	diff convolvef_out.sac convolvef_out-ok.sac

$(PROGF): $(OBJSF)
	$(FC) -o $@ $^ $(LDLIBS)

$(PROGC): $(OBJSC)


clean:
	\rm -f $(PROGC) $(PROGF) $(OBJSC) $(OBJSF) *~ convolvef_out.sac convolvec_out.sac 

