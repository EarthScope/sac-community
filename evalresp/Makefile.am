
noinst_LIBRARIES = libevalresp.a libevalresp_log.a libmxmlev.a libspline.a

libevalresp_a_CFLAGS = -I$(srcdir)/libsrc -I$(srcdir)/libsrc/mxml
libevalresp_a_SOURCES = libsrc/evalresp/input.c \
	libsrc/evalresp/evaluation.c \
	libsrc/evalresp/output.c \
  libsrc/evalresp/highlevel.c\
	libsrc/evalresp/regexp.c libsrc/evalresp/regerror.c\
	libsrc/evalresp/regsub.c libsrc/evalresp/calc_fctns.c\
  libsrc/evalresp/resp_fctns.c libsrc/evalresp/file_ops.c\
  libsrc/evalresp/alloc_fctns.c\
  libsrc/evalresp/spline.c libsrc/evalresp/legacy_interface.c\
  libsrc/evalresp/stationxml2resp/dom_to_seed.c\
  libsrc/evalresp/stationxml2resp/xml_to_dom.c\
  libsrc/evalresp/stationxml2resp/wrappers.c\
  libsrc/evalresp/examples/lowlevel.c libsrc/evalresp/examples/highlevel.c

libevalresp_log_a_CFLAGS = -I$(srcdir)/libsrc
libevalresp_log_a_SOURCES = libsrc/evalresp_log/log.c \
   libsrc/evalresp_log/helpers.c \
   libsrc/evalresp_log/examples/to_syslog.c \
	 libsrc/evalresp_log/examples/to_file.c

libmxmlev_a_SOURCES = libsrc/mxml/mxml-attr.c \
  libsrc/mxml/mxml-file.c \
  libsrc/mxml/mxml-node.c \
  libsrc/mxml/mxml-set.c \
  libsrc/mxml/mxml-get.c \
  libsrc/mxml/mxml-private.c \
  libsrc/mxml/mxml-string.c \
  libsrc/mxml/mxml-entity.c \
  libsrc/mxml/mxml-index.c \
  libsrc/mxml/mxml-search.c

libspline_a_CFLAGS = -I$(srcdir)/libsrc
libspline_a_SOURCES = libsrc/spline/spline.c

# CFLAGS (not AM_CFLAGS/per-target _CFLAGS) is appended last on every compile
# line, after the project-wide -Wall -Wextra from the top-level configure.ac,
# so only flags placed here can actually suppress warnings in this vendored
# tree without being re-enabled by -Wextra.
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-deprecated-non-prototype
