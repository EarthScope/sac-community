
# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

EXTRA_DIST = License README.md t config docs

AM_CFLAGS = -Wall -Wextra -Iinclude/

saciolibdir = $(libdir)/
sacioincdir = $(includedir)/sacio

saciolib_LIBRARIES = libsacio_bsd.a
sacioinc_HEADERS   = sacio.h timespec.h

libsacio_bsd_a_SOURCES = sacio.c geodesic.c timespec.c \
												 time64.c strip.c compat.c \
												 header_map.c defs.h \
												 time64.h geodesic.h strip.h \
												 time64_limits.h \
												 sacio.h timespec.h \
                         header_map.txt

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
