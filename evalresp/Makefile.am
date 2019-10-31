
noinst_LIBRARIES = libevalresp.a libspline.a libmxmlev.a libevalresp_log.a

libevalresp_a_CFLAGS=-I$(top_srcdir)/evalresp/libsrc/ -g \
                     -I$(top_srcdir)/evalresp/libsrc/mxml
libspline_a_CFLAGS=-I$(top_srcdir)/evalresp/libsrc/ \
                   -I$(top_srcdir)/evalresp/libsrc/evalresp_log
libevalresp_log_a_CFLAGS=-I$(top_srcdir)/evalresp/libsrc/ \
                         -I$(top_srcdir)/evalresp/libsrc/evalresp_log

libevalresp_a_SOURCES=libsrc/evalresp/alloc_fctns.c \
libsrc/evalresp/calc_fctns.c \
libsrc/evalresp/constants.h \
libsrc/evalresp/evaluation.c \
libsrc/evalresp/evresp.h \
libsrc/evalresp/file_ops.c\
libsrc/evalresp/highlevel.c\
libsrc/evalresp/input.c\
libsrc/evalresp/input.h\
libsrc/evalresp/legacy_interface.c\
libsrc/evalresp/output.c\
libsrc/evalresp/private.h\
libsrc/evalresp/public.h\
libsrc/evalresp/public_api.h\
libsrc/evalresp/public_channels.h\
libsrc/evalresp/public_compat.h\
libsrc/evalresp/public_responses.h\
libsrc/evalresp/regerror.c\
libsrc/evalresp/regexp.c\
libsrc/evalresp/regexp.h\
libsrc/evalresp/regmagic.h\
libsrc/evalresp/regsub.c\
libsrc/evalresp/resp_fctns.c\
libsrc/evalresp/spline.c\
libsrc/evalresp/spline.h\
libsrc/evalresp/stationxml2resp/dom_to_seed.c\
libsrc/evalresp/stationxml2resp/dom_to_seed.h\
libsrc/evalresp/stationxml2resp/wrappers.c\
libsrc/evalresp/stationxml2resp/wrappers.h\
libsrc/evalresp/stationxml2resp/xml_to_dom.c\
libsrc/evalresp/stationxml2resp/xml_to_dom.h\
libsrc/evalresp/stationxml2resp.h

libspline_a_SOURCES=libsrc/spline/spline.c libsrc/spline/spline.h
libevalresp_log_a_SOURCES=libsrc/evalresp_log/helpers.c \
           libsrc/evalresp_log/log.c \
           libsrc/evalresp_log/log.h \
           libsrc/evalresp_log/examples/to_file.c \
           libsrc/evalresp_log/examples/to_file.h \
           libsrc/evalresp_log/examples/to_syslog.c \
           libsrc/evalresp_log/examples/to_syslog.h
libmxmlev_a_SOURCES=libsrc/mxml/config.h \
	   libsrc/mxml/mmd.c \
		 libsrc/mxml/mmd.h \
     libsrc/mxml/mxml-attr.c \
     libsrc/mxml/mxml-entity.c \
     libsrc/mxml/mxml-file.c \
     libsrc/mxml/mxml-get.c \
     libsrc/mxml/mxml-index.c \
     libsrc/mxml/mxml-node.c \
     libsrc/mxml/mxml-private.c \
     libsrc/mxml/mxml-private.h \
     libsrc/mxml/mxml-search.c \
     libsrc/mxml/mxml-set.c \
     libsrc/mxml/mxml-string.c \
     libsrc/mxml/mxml.h \
     libsrc/mxml/mxmldoc.c \
     libsrc/mxml/testmxml.c \
     libsrc/mxml/zipc.c \
     libsrc/mxml/zipc.h

