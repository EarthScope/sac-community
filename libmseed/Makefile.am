
# Build environment can be configured the following
# environment variables:
#   CC : Specify the C compiler to use
#   CFLAGS : Specify compiler options to use
#   LDFLAGS : Specify linker options to use
#   CPPFLAGS : Specify c-preprocessor options to use

# Extract version from libmseed.h, expected line should include LIBMSEED_VERSION "#.#.#"
MAJOR_VER = $(shell grep LIBMSEED_VERSION libmseed.h | grep -Eo '[0-9]+.[0-9]+.[0-9]+' | cut -d . -f 1)
FULL_VER = $(shell grep LIBMSEED_VERSION libmseed.h | grep -Eo '[0-9]+.[0-9]+.[0-9]+')
COMPAT_VER = $(MAJOR_VER).0.0

# Default settings for install target
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
LIBDIR ?= $(EXEC_PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
DATAROOTDIR ?= $(PREFIX)/share
DOCDIR ?= $(DATAROOTDIR)/doc/libmseed
MANDIR ?= $(DATAROOTDIR)/man
MAN3DIR ?= $(MANDIR)/man3

LIB_SRCS = fileutils.c genutils.c gswap.c lmplatform.c lookup.c parson.c \
           msrutils.c extraheaders.c pack.c packdata.c tracelist.c gmtime64.c \
           crc32c.c parseutils.c unpack.c unpackdata.c selection.c logging.c

EXTRA_DIST = ChangeLog INSTALL.md LICENSE.txt Makefile.am Makefile.in \
             Makefile.win README.byteorder README.md TODO.txt doc example \
             libmseed.def libmseed.h libmseed.map mseed.pc.in test \
             $(LIB_SRCS) gmtime64.h mseedformat.h parson.h unpackdata.h \
             libmseed.h packdata.h unpack.h

LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_LOBJS = $(LIB_SRCS:.c=.lo)

LIB_NAME = libmseed
LIB_A = $(LIB_NAME).a

OS := $(shell uname -s)

all: static

static: $(LIB_A)

# Build static library
$(LIB_A): $(LIB_OBJS)
	@echo "Building static library $(LIB_A)"
	$(RM) -f $(LIB_A)
	$(AR) -crs $(LIB_A) $(LIB_OBJS)

#test check: static FORCE
#	@$(MAKE) -C test test

clean:
	@$(RM) $(LIB_OBJS) $(LIB_LOBJS) $(LIB_A) $(LIB_SO) $(LIB_SO_MAJOR) $(LIB_SO_BASE)

# .SUFFIXES: .c .o .lo

# Standard object building
.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Standard object building for shared library using -fPIC
.c.lo:
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $< -o $@

FORCE:
