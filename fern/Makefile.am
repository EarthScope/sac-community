
# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

EXTRA_DIST = License README.md t config

AM_CFLAGS = -Wall -Wextra -Iinclude/ $(XML_CPPFLAGS) $(LIBCURL_CPPFLAGS) -I$(top_srcdir)

fernlibdir = $(libdir)/
fernincdir = $(includedir)/fern

fernlib_LIBRARIES = libfern.a libpile.a
ferninc_HEADERS   = array.h request.h event.h station.h \
                    stationreq.h datareq.h meta.h \
                    miniseed_sac.h cprint.h fern.h

#bin_PROGRAMS = fern
#fern_SOURCES = fern.c
#fern_LDADD = libfern.a libpile.a $(XML_LIBS) $(LIBCURL)

libfern_a_SOURCES = cJSON.c cJSON.h \
                    datareq.c datareq.h \
										event.c event.h \
										json.c json.h \
										meta.c meta.h \
										miniseed_sac.c miniseed_sac.h \
										quake_xml.c \
										request.c request.h \
										slurp.c slurp.h \
										station.c station.h \
										stationreq.c stationreq.h \
										strip.c strip.h \
										xml.c xml.h \
                    fern.h

libpile_a_SOURCES = chash.c chash.h array.c array.h cprint.c cprint.h


# TESTS

# TEST_EXTENSIONS = .sh
# TESTS = t/test_event.sh t/test_station.sh t/test_station_event.sh \
#         t/test_avail.sh t/test_miniseed.sh t/test_sac.sh \
#         t/eventsearch t/stationsearch t/datadownload

# check_PROGRAMS = t/eventsearch t/stationsearch t/datadownload
# t_eventsearch_SOURCES = t/event_search.c
# t_eventsearch_LDADD = libfern.a $(XML_LIBS) $(LIBCURL)
# t_stationsearch_SOURCES = t/station_search.c
# t_stationsearch_LDADD = libfern.a $(XML_LIBS) $(LIBCURL)
# t_datadownload_SOURCES = t/data_download.c
# t_datadownload_LDADD = libfern.a $(XML_LIBS) $(LIBCURL)

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
