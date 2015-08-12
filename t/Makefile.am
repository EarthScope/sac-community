

# Do not create dependencides while compiling
AUTOMAKE_OPTIONS = no-dependencies #dejagnu

if X11_APP
TESTS_ENVIRONMENT = SACAUX=$(abs_top_srcdir)/aux \
	SACBIN=../src/sac \
	ABS_TOP_SRCDIR=$(abs_top_srcdir) \
	ABS_BUILDDIR=$(abs_builddir)
SACBIN=../src/sac
endif
if OSX_APP
TESTS_ENVIRONMENT = SACAUX=$(abs_top_srcdir)/aux \
	SACBIN=../Sac.app/Contents/MacOS/sac-osx-gui \
	ABS_TOP_SRCDIR=$(abs_top_srcdir) \
	ABS_BUILDDIR=$(abs_builddir)
SACBIN=../Sac.app/Contents/MacOS/sac-osx-gui
endif

#
# Options for testing the compiled code directly 
# 
CFLAGS          += $(DEBUG) $(COVERAGE) $(OPTIM) -Wall \
		   -I$(top_srcdir)/inc \
		   -DTOP_SRCDIR=\"$(top_srcdir)\"

LDADD            = -L../src/ -lsac_all -lturtle -lsac_all \
                   -levalresp \
                   -lSMnonOracleFuncs \
                   -lSacOracleStubFuncs \
									 -licm
if EDITLINE
LDADD            += ../libedit/lib/libedit.a
CFLAGS           += -I../libedit/include
endif

FORMAT_LIBS      = $(xpm_LIBS) $(png_LIBS) $(zlib_LIBS) $(xft_LIBS)
SAC_GRAPHICS     = -L.. 

if X11_APP
SAC_GRAPHICS     += -lsac_x11
endif
if OSX_APP
#SAC_GRAPHICS     += -lsac_osx
endif
SAC_GRAPHICS     +=  -lsgf -lturtle -lsac_all

if X11_APP
LDADD            +=  $(X_PRE_LIBS) $(X_EXTRA_LIBS) $(X_LIBS) 
endif

LDADD            += $(SAC_GRAPHICS) $(FORMAT_LIBS)

if X11_APP
LDADD            += -lX11 
endif 

LDADD            += -L. -lunit



EXTRA_DIST = testsuite \
             testsuite/input testsuite/output \
             test_io_big.sac test_io_small.sac \
             test_spec_big.sac test_spec_small.sac \
             test_uneven_big.sac test_uneven_small.sac  \
             pytest.py sacio_fortran.sh

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

if HAVE_FC
saciof_SOURCES = sacio_f.f
saciof_CFLAGS = $(COVERAGE)
saciof_LDADD = -L../src/ -lsacio -lsac

saclibf_SOURCES = saclib_f.f
saclibf_LDADD = -L../src/ -lsacio -lsac -L. -lunit
saclibf_CFLAGS =
endif

sacio_CFLAGS = $(COVERAGE) -I$(top_srcdir)/include
sacio_LDADD = -L../src/ -lsacio -L. -lunit
sacio_LDFLAGS =

use_sac_CFLAGS= $(COVERAGE) 
use_sac_LDADD = -L../src -lsacio -L. -lunit
use_sac_LDFLAGS = 

#saclibf${EXEEXT}: saclib_f.o
#	$(F77) $(COVERAGE) -o $@ saclib_f.o -L../src -lsac

icm_SOURCES = icm.c 
icm_CFLAGS = $(COVERAGE)
icm_LDADD = -L../src -licm

# Compiled programs as tests
check_PROGRAMS   = use_sac ucf vars sacio icm

if HAVE_FC
check_PROGRAMS += saciof saclibf
endif

TEST_EXTENSIONS = .sm
# Compiled programs to test
TESTS            =  use_sac ucf vars sacio copy_files.sh icm

if HAVE_FC
TESTS += sacio_fortran.sh saciof saclibf
endif
TESTS += testsuite/input/abs.sm \
testsuite/input/add.sm \
testsuite/input/addf.sm \
testsuite/input/alpha.sm \
testsuite/input/apk.sm \
testsuite/input/arraymap.sm \
testsuite/input/axes.sm \
testsuite/input/bandpass.sm \
testsuite/input/bandrej.sm \
testsuite/input/beginwindow.sm \
testsuite/input/benioff.sm \
testsuite/input/binoperr.sm \
testsuite/input/border.sm \
testsuite/input/ch.sm \
testsuite/input/color.sm \
testsuite/input/comcor.sm \
testsuite/input/comment.sm \
testsuite/input/contour.sm \
testsuite/input/convolve.sm \
testsuite/input/copyhdr.sm \
testsuite/input/correlate.sm \
testsuite/input/css.sm \
testsuite/input/cut.sm \
testsuite/input/cuterr.sm \
testsuite/input/cutim.sm \
testsuite/input/decimate.sm \
testsuite/input/deletechannel.sm \
testsuite/input/dif.sm \
testsuite/input/div.sm \
testsuite/input/divf.sm \
testsuite/input/divomega.sm \
testsuite/input/echo.sm \
testsuite/input/empty.sm \
testsuite/input/envelope.sm \
testsuite/input/erase.sm \
testsuite/input/escape.sm \
testsuite/input/evaluate.sm \
testsuite/input/evaluate2.sm \
testsuite/input/exp.sm \
testsuite/input/exp10.sm \
testsuite/input/fg.sm \
testsuite/input/fg_random.sm \
testsuite/input/filterdesign.sm \
testsuite/input/floor.sm \
testsuite/input/full.sm \
testsuite/input/getbb.sm \
testsuite/input/grid.sm \
testsuite/input/hanning.sm \
testsuite/input/help.sm \
testsuite/input/highpass.sm \
testsuite/input/hilbert.sm \
testsuite/input/inicm.sm \
testsuite/input/int.sm \
testsuite/input/interpolate.sm \
testsuite/input/keepam.sm \
testsuite/input/khronhite.sm \
testsuite/input/lh.sm \
testsuite/input/line.sm \
testsuite/input/linefit.sm \
testsuite/input/linlog.sm \
testsuite/input/log.sm \
testsuite/input/log10.sm \
testsuite/input/loglab.sm \
testsuite/input/lowpass.sm \
testsuite/input/makrptp.sm \
testsuite/input/marktimes.sm \
testsuite/input/markvalue.sm \
testsuite/input/mathop.sm \
testsuite/input/merge.sm \
testsuite/input/message.sm \
testsuite/input/mtw.sm \
testsuite/input/mul.sm \
testsuite/input/mulf.sm \
testsuite/input/mulomega.sm \
testsuite/input/oapf.sm \
testsuite/input/ohpf.sm \
testsuite/input/output.sm \
testsuite/input/pause.sm \
testsuite/input/plabel.sm \
testsuite/input/plot.sm \
testsuite/input/plotalpha.sm \
testsuite/input/prewhiten.sm \
testsuite/input/qdp.sm \
testsuite/input/quantize.sm \
testsuite/input/read.sm \
testsuite/input/readalpha.sm \
testsuite/input/readbbf.sm \
testsuite/input/readcomp2.sm \
testsuite/input/readerr.sm \
testsuite/input/readgse.sm \
testsuite/input/readhdr.sm \
testsuite/input/readsdd.sm \
testsuite/input/readsp.sm \
testsuite/input/readtable.sm \
testsuite/input/replace.sm \
testsuite/input/reverse.sm \
testsuite/input/rmean.sm \
testsuite/input/rms.sm \
testsuite/input/rotate.sm \
testsuite/input/rotinc.sm \
testsuite/input/rq.sm \
testsuite/input/rtrend.sm \
testsuite/input/sam_error.sm \
testsuite/input/scallop.sm \
testsuite/input/segy.sm \
testsuite/input/setbb.sm \
testsuite/input/sgf.sm \
testsuite/input/smooth.sm \
testsuite/input/sort.sm \
testsuite/input/spe.sm \
testsuite/input/spectrogram.sm \
testsuite/input/sqr.sm \
testsuite/input/sqrt.sm \
testsuite/input/sss.sm \
testsuite/input/stretch.sm \
testsuite/input/sub.sm \
testsuite/input/subf.sm \
testsuite/input/symbol.sm \
testsuite/input/synchronize.sm \
testsuite/input/systemcommand.sm \
testsuite/input/taper.sm \
testsuite/input/test_break.sm \
testsuite/input/test_do.sm \
testsuite/input/test_if.sm \
testsuite/input/test_macro.sm \
testsuite/input/test_macro_comments.sm \
testsuite/input/test_run.sm \
testsuite/input/ticks.sm \
testsuite/input/title.sm \
testsuite/input/transcript.sm \
testsuite/input/transfer.sm \
testsuite/input/transfer_evresp.sm \
testsuite/input/transfer_fap.sm \
testsuite/input/traveltime.sm \
testsuite/input/unsetbb.sm \
testsuite/input/unwrap.sm \
testsuite/input/vspace.sm \
testsuite/input/weiner.sm \
testsuite/input/wild.sm \
testsuite/input/write.sm \
testsuite/input/writebbf.sm \
testsuite/input/writehdr.sm \
testsuite/input/writesdd.sm \
testsuite/input/writesp.sm \
testsuite/input/writetable.sm \
testsuite/input/xdiv.sm \
testsuite/input/xfir.sm \
testsuite/input/xfudge.sm \
testsuite/input/xgrid.sm \
testsuite/input/xlabel.sm \
testsuite/input/xlim.sm \
testsuite/input/xlin.sm \
testsuite/input/xvport.sm \
testsuite/input/ydiv.sm \
testsuite/input/yfudge.sm \
testsuite/input/ygrid.sm \
testsuite/input/ylabel.sm \
testsuite/input/ylin.sm \
testsuite/input/yvport.sm \
testsuite/input/zaddf.sm \
testsuite/input/zam.sm \
testsuite/input/zfft.sm \
testsuite/input/zifft.sm \
testsuite/input/zprocessline.sm \
testsuite/input/zzzzcd.sm

# These tests have specific calls to the X11 device
if X11_APP
TESTS += testsuite/input/begindevices.sm \
testsuite/input/report.sm \
testsuite/input/enddevices.sm
endif

SM_LOG_COMPILER = $(top_srcdir)/t/pytest.py
AM_SM_LOG_FLAGS = -s "$(abs_builddir)/$(SACBIN)" -o "${abs_top_srcdir}/t/testsuite/output" --test


