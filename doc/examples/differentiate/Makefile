
PROGF=diff
PROGC=difc

OBJSF=$(PROGF).o
OBJSC=$(PROGC).o

SACHOME ?= /usr/local/sac   # Set if not specified
LDLIBS=`$(SACHOME)/bin/sac-config -l -c sacio sac`
FFLAGS=`$(SACHOME)/bin/sac-config -c `
CFLAGS=`$(SACHOME)/bin/sac-config -c `
SAC=$(SACHOME)/bin/sac
FC=gfortran


# Compile Fortran 90 to object files
%.o: %.f90
	$(FC) $(FFLAGS) -o $@ -c $<


all: xrun

xrun: macro $(PROGF) $(PROGC)
	./$(PROGF)
	./$(PROGC)

$(PROGF): $(OBJSF)
	$(FC) -o $@ $^ $(LDLIBS)

$(PROGC): $(OBJSC)


macro:
	$(SAC) macro.m

clean:
	\rm -f $(PROGC) $(PROGF) $(OBJSC) $(OBJSF) *~ *.sac

