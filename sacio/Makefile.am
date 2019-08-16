
# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

EXTRA_DIST = License README.md t config docs

AM_CFLAGS = -fPIC -Wall -Wextra -Iinclude/

# Notes for including library directly in sac source
# - Remove saciolibdir, sacioincdir
# - Remove tests
# - Rename saciolib_LIBRARIES to noinst_LIBRARIES
# - Move sacioinc_HEADERS to libsacio_bsd_a_SOURCES
# - Run ./bootstrap in base directory
#

noinst_LIBRARIES  = libsacio_bsd.a

libsacio_bsd_a_SOURCES = sacio.c geodesic.c timespec.c \
												 time64.c strip.c compat.c \
												 header_map.c defs.h \
												 time64.h geodesic.h strip.h \
												 time64_limits.h \
												 sacio.h timespec.h \
                         header_map.txt \
                         sacio.h timespec.h

CLEANFILES = t/test*.tmp


doc:
	doxygen docs/Doxyfile

distclean-local:
	-rm -rf autom4te.cache

header_map.c: header_map.txt
	gperf -t -T --output-file=$@ -N sac_keyword_to_header $<

test: check

disttest:
	make clean
	make
	make test
	make dist
	tar -zxf $(PACKAGE)-$(VERSION).tar.gz
	cd $(PACKAGE)-$(VERSION) && ./configure && make && make test
