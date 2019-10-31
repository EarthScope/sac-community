
all: libevalresp.a libspline.a libxmxlev.a libevalresp_log.a

EVRESP=libsrc/evalresp/alloc_fctns.c \
       libsrc/evalresp/calc_fctns.c \
       libsrc/evalresp/file_ops.c \
       libsrc/evalresp/regexp.c \
       libsrc/evalresp/regsub.c \
       libsrc/evalresp/resp_fctns.c \
       libsrc/evalresp/spline.c \
       libsrc/evalresp/input.c \
       libsrc/evalresp/output.c \
       libsrc/evalresp/stationxml2resp/wrappers.c \
       libsrc/evalresp/highlevel.c \
       libsrc/evalresp/evaluation.c \
       libsrc/evalresp/legacy_interface.c \
       libsrc/evalresp/stationxml2resp/dom_to_seed.c \
       libsrc/evalresp/stationxml2resp/xml_to_dom.c \
       libsrc/evalresp/public_api.h \
       libsrc/evalresp/public_channels.h \
       libsrc/evalresp/public_responses.h \
       libsrc/evalresp/public_compat.h \
       libsrc/evalresp/stationxml2resp.h \
       libsrc/evalresp/evresp.h

libevalresp.a: $(EVRESP)
