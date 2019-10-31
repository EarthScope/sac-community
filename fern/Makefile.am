
# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

EXTRA_DIST = License README.md t config

AM_CFLAGS = -fPIC -Wall -Wextra -Iinclude/ $(XML_CPPFLAGS) $(LIBCURL_CPPFLAGS) -I$(top_srcdir)

# Notes for including library directly in sac source
# - Remove fernlibdir, fernincdir
# - Remove tests
# - Rename fernlib_LIBRARIES to noinst_LIBRARIES
# - Move ferninc_HEADERS to libfern_a_SOURCES
# - Run ./bootstrap in base directory
#

noinst_LIBRARIES = libfern.a libpile.a

libfern_a_SOURCES = cJSON.c cJSON.h \
                    datareq.c datareq.h \
										event.c event.h \
										json.c json.h \
										meta.c meta.h \
										miniseed_sac.c miniseed_sac.h \
										quake_xml.c \
										request.c request.h \
                    response.c response.h \
										slurp.c slurp.h \
										station.c station.h \
										stationreq.c stationreq.h \
										strip.c strip.h \
										xml.c xml.h \
                    fern.h urls.h \
                    array.h request.h event.h station.h \
                    stationreq.h datareq.h meta.h \
                    miniseed_sac.h cprint.h fern.h

libpile_a_SOURCES = chash.c chash.h array.c array.h cprint.c cprint.h

CLEANFILES = t/*.test t/test_miniseed*mseed

doc:
	doxygen docs/Doxyfile

distclean-local:
	-rm -rf autom4te.cache

test: check

disttest:
	make clean
	make
	make test
	make dist
	tar -zxf $(PACKAGE)-$(VERSION).tar.gz
	cd $(PACKAGE)-$(VERSION) && ./configure && make && make test
