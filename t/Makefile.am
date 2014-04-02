

# Do not create dependencides while compiling
AUTOMAKE_OPTIONS = no-dependencies #dejagnu

TESTS_ENVIRONMENT = SACAUX=$(abs_top_srcdir)/aux \
	SACBIN=../src/sac \
	ABS_TOP_SRCDIR=$(abs_top_srcdir) \
	ABS_BUILDDIR=$(abs_builddir)

#
# Options for testing the compiled code directly 
# 
CFLAGS          += $(DEBUG) $(COVERAGE) $(OPTIM) -Wall \
		   -I$(top_srcdir)/inc \
		   -DTOP_SRCDIR=\"$(top_srcdir)\"

LDADD            = -L../src/ -lsac_all \
                   -levalresp \
                   -lSMnonOracleFuncs \
                   -lSacOracleStubFuncs 
if EDITLINE
LDADD            += ../libedit/lib/libedit.a
CFLAGS           += -I../libedit/include
endif
FORMAT_LIBS      = $(xpm_LIBS) $(png_LIBS) $(zlib_LIBS) $(xft_LIBS)
SAC_GRAPHICS     = -L.. -lsac_x11 -lsgf -lturtle -lsac_all
LDADD            +=  $(X_PRE_LIBS) $(X_EXTRA_LIBS) $(X_LIBS) \
	             $(SAC_GRAPHICS) $(FORMAT_LIBS) -lX11 -L. -lunit

EXTRA_DIST = testsuite \
             testsuite/input testsuite/output \
             test_io_big.sac test_io_small.sac \
             test_spec_big.sac test_spec_small.sac \
             test_uneven_big.sac test_uneven_small.sac  \
             pytest.sh pytest.py sacio_fortran.sh

# Unit Testing Suite: Hardly a suite
noinst_LIBRARIES    = libunit.a
libunit_a_SOURCES = unit.c unit.h

# Test programs written in C need to be added to 
#  both check_PROGRAMS and TESTS

ucf_SOURCES     = ucf.c \
                  ucf_inctim.c \
                  ucf_cnvati.c \
                  ucf_cnvita.c \
                  ucf_cnvatf.c \
                  ucf_cnvfta.c

vars_SOURCES   = vars.c 

sacio_SOURCES    = sacio.c \
	           sacio_rsac1.c  \
	           sacio_rsac2.c  \
                   sacio_wsac0.c  \
                   sacio_wsac1.c  \
                   sacio_wsac2.c  \
                   sacio_wsac3.c  \
                   sacio_getset.c \
                   sacio_test.h
#saciof${EXEEXT}: sacio_f.o
#	$(F77) $(COVERAGE) -o $@ sacio_f.o -L../src -lsacio

saciof_SOURCES = sacio_f.f

sacio_CFLAGS = -I$(top_srcdir)/include
sacio_LDADD = -L../src/ -lsacio -L. -lunit
sacio_LDFLAGS =
saciof_LDADD = -L../src/ -lsacio -L. -lunit
saciof_CFLAGS =
saciof_LDFLAGS =

#saclibf${EXEEXT}: saclib_f.o
#	$(F77) $(COVERAGE) -o $@ saclib_f.o -L../src -lsac

saclibf_SOURCES = saclib_f.f
saclibf_LDADD = -L../src/ -lsacio -lsac -L. -lunit
saclibf_CFLAGS =

# Compiled programs as tests
check_PROGRAMS   = use_sac ucf vars sacio 
if HAVE_F77
check_PROGRAMS += saciof saclibf
endif


# Compiled programs to test
TESTS            =  use_sac ucf vars sacio 
if HAVE_F77
TESTS += sacio_fortran.sh saciof saclibf
endif
TESTS += pytest.sh
#
# Options for dejagnu for testing the sac binary input and output
#
RUNTESTDEFAULTFLAGS = --tool sac \
		     SAC=../src/sac \
		     SACAUX=$(top_srcdir)/aux \
		     --srcdir $(top_srcdir)/t/testsuite \
		     --objdir ../src

