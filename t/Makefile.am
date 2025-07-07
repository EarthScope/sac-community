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
SAC_INCLUDES     = -I$(top_srcdir)/inc  -I$(top_srcdir) -I../include
CFLAGS          += $(DEBUG) $(COVERAGE) $(OPTIM) -Wall \
			 $(SAC_INCLUDES) \
			 -DTOP_SRCDIR=\"$(top_srcdir)\"

LDADD            = -L../src/ -lsac_all -lturtle -lsac_all \
									 -lSMnonOracleFuncs \
									 -lSacOracleStubFuncs \
									 -licm \
                   -L../evalresp -levalresp -lspline -lmxmlev -levalresp_log

if READLINE_EDITLINE
LDADD            += ../libedit/lib/libedit.a
CFLAGS           += -I../libedit/include
endif

FORMAT_LIBS      = $(xpm_LIBS) $(png_LIBS) $(zlib_LIBS) $(xft_LIBS) $(XML_LIBS)
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

LDADD            += -L. -lunit ../src/libsacio.a ../libmseed/libmseed.a $(LIBCURL)



EXTRA_DIST = testsuite \
						 testsuite/input testsuite/output \
						 test_io_big.sac test_io_small.sac \
						 test_spec_big.sac test_spec_small.sac \
						 test_uneven_big.sac test_uneven_small.sac  \
						 pytest.py sacio_fortran.sh copy_files.sh \
	acc.txt     dwwssn.txt   lll.txt        ptbllp.txt     sro_bb.txt \
	bbdisp.txt  ekalp6.txt   llsn.txt       redkir.txt     sro_rstn.txt \
	bbvel.txt   ekasp2.txt   lrsmlp.txt     reftek.txt     vel.txt \
	benbog.txt  elmag.txt    lrsmsp.txt     rs7.txt        wa.txt \
	clh.txt     eyeomg.txt   noress_ip.txt  rs7_inv.txt    wabn.txt \
	clz.txt     gbalp.txt    noress_lp.txt  rsk.txt        wiech.txt \
	cmh.txt     gbasp.txt    noress_sp.txt  rsk_inv.txt    wwlpbn.txt \
	cmz.txt     general.txt  noresshf.txt   rsl.txt        wwsp.txt \
	csh.txt     gsref.txt    oldbb.txt      rsm.txt        wwspbn.txt \
	csz.txt     hfslpwb.txt  oldkir.txt     sandia_ol.txt  ykalp.txt \
	dss.txt     hs3.txt      portable.txt   snla3.txt      ykasp.txt

# Unit Testing Suite: Hardly a suite
noinst_LIBRARIES    = libunit.a
libunit_a_SOURCES = unit.c unit.h

# Test programs written in C need to be added to
#  both check_PROGRAMS and TESTS

ucf_SOURCES     = ucf.c ucf_inctim.c ucf_cnvati.c ucf_cnvita.c \
									ucf_cnvatf.c ucf_cnvfta.c
ucf_LDADD = -L. -lunit -L../src -lsacio -lsac_check -lmsg ../src/ucf/cnvita.o ../fern/libpile.a

vars_SOURCES   = vars.c
vars_LDADD = -L. -lunit -L../src -lsacio -lmsg -lvars ../fern/libpile.a

sacio_SOURCES    = sacio.c sacio_rsac1.c sacio_rsac2.c sacio_wsac0.c  \
									 sacio_wsac1.c sacio_wsac2.c sacio_wsac3.c  \
									 sacio_getset.c sacio_test.h

if HAVE_FC
# Check sacio library from fortran
saciof_SOURCES = sacio_f.f
saciof_CFLAGS = $(COVERAGE)
saciof_LDADD = -L../src/ -lsacio

# Check sac library from fortran
saclibf_SOURCES = saclib_f.f
saclibf_LDADD = -L../src/ -lsacio -lsac -L. -lunit
saclibf_CFLAGS =
endif

# Check sacio library
sacio_CFLAGS = $(COVERAGE) -I$(top_srcdir)/include
sacio_LDADD =  -L. -lunit -L../src -lsacio
sacio_LDFLAGS =

# Check data used within sac
use_sac_CFLAGS= $(COVERAGE) $(SAC_INCLUDES)
use_sac_LDADD = -L../src  -L. -lunit -lsacio -lmsg ../fern/libpile.a
use_sac_LDFLAGS =

# Check all functions in sacio
sacio_all_CFLAGS =
sacio_all_LDADD =  -L../src -lsacio
sacio_all_LDFLAGS =

# Check all functions in sac library
sac_all_CFLAGS =
sac_all_LDADD = -L../src -lsac -lsacio -lmsg ../fern/libpile.a
sac_all_LDFLAGS =

lckey_test_CFLAGS =
lckey_test_LDADD = $(LDADD) ../fern/libpile.a ../fern/libfern.a $(LIBCURL) $(XML_LIBS) ../libmseed/libmseed.a
lckey_test_LDFLAGS =

#saclibf${EXEEXT}: saclib_f.o
#	$(F77) $(COVERAGE) -o $@ saclib_f.o -L../src -lsac

icm_SOURCES = icm.c
icm_CFLAGS = $(COVERAGE)
icm_LDADD = -L../src -licm -lsac -lsacio -lmsg ../fern/libpile.a 

# Compiled programs as tests
check_PROGRAMS   = use_sac ucf vars sacio icm sacio_all sac_all lckey_test

if HAVE_FC
check_PROGRAMS += saciof saclibf
endif

TEST_EXTENSIONS = .sm
# Compiled programs to test
TESTS            =  use_sac ucf vars sacio copy_files.sh icm sacio_all sac_all lckey_test

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
testsuite/input/zzzzcd.sm \
testsuite/input/afl_000000.sm \
testsuite/input/afl_000001.sm \
testsuite/input/afl_000002.sm \
testsuite/input/afl_000003.sm \
testsuite/input/afl_000004.sm \
testsuite/input/afl_000005.sm \
testsuite/input/afl_000006.sm \
testsuite/input/afl_000007.sm \
testsuite/input/afl_000008.sm \
testsuite/input/afl_000009.sm \
testsuite/input/afl_000010.sm \
testsuite/input/afl_000011.sm \
testsuite/input/afl_000012.sm \
testsuite/input/julday2.sm \
testsuite/input/long_and_high.sm \
testsuite/input/bbfk.sm \
testsuite/input/v6v7.sm \
testsuite/input/v6v7io.sm \
testsuite/input/v6v7iosp.sm \
testsuite/input/ibody.sm

# testsuite/input/event.sm 
# testsuite/input/traveltime.sm 

# This Test breaks routinely due to Station Database Updates
# testsuite/input/station.sm


# These tests have specific calls to the X11 device
if X11_APP
TESTS += testsuite/input/begindevices.sm \
testsuite/input/report.sm \
testsuite/input/enddevices.sm
endif

SM_LOG_COMPILER = $(top_srcdir)/t/pytest.py
AM_SM_LOG_FLAGS = -s "$(abs_builddir)/$(SACBIN)" -o "${abs_top_srcdir}/t/testsuite/output" --test
