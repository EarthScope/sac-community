
# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

CFLAGS          += -O -Wall -pipe -g3

# Scripts to install
#bin_SCRIPTS      = sgftoeps.csh

# Compiled libraries to not install
noinst_LIBRARIES  = lib/libedit.a

#LIBEDIT_DIR = libedit-2.6.9
LIBEDIT_DIR = libedit-20090923-3.0-sac
#LIBEDIT_DIR = libedit-20120601-3.0
LIBEDIT_TAR = $(srcdir)/$(LIBEDIT_DIR).tar.gz
LIBEDIT_OPT = --prefix=$(abs_top_builddir)/libedit 
LIBEDIT_ENV = env CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" INSTALL="$(abs_top_srcdir)/config/install-sh"
CONFIG_H_IN = config.h.in
#CONFIG_H_IN = $(srcdir)/config.h.in

EXTRA_DIST  = License.BSD
lib_libedit_a_SOURCES = $(LIBEDIT_DIR).tar.gz $(CONFIG_H_IN)

all: lib/libedit.a

$(LIBEDIT_DIR):
	echo "Creating the Line Editing Library"
	gzip -dc $(LIBEDIT_TAR) | tar xf -
#	cp -f $(srcdir)/$(CONFIG_H_IN) $(LIBEDIT_DIR)

lib/libedit.a: $(LIBEDIT_DIR) 
	(cd $(LIBEDIT_DIR) && $(LIBEDIT_ENV) ./configure $(LIBEDIT_OPT) --disable-dependency-tracking )
#	(cd $(LIBEDIT_DIR) && $(MAKE) MAKEFLAGS="-j 1"  )
#	(cd $(LIBEDIT_DIR) && $(MAKE) MAKEFLAGS="-j 1" install )
	(cd $(LIBEDIT_DIR)/src && $(MAKE) )
	(cd $(LIBEDIT_DIR)/src && $(MAKE) install )
	(cd lib && ranlib libedit.a)

clean-local:
	rm -rf lib man include $(LIBEDIT_DIR)

#       This code is here to Force a 32 Bit Compilation of the libedit shared library
#       Note: We do not use the libedit shared library, only the static version.
#	(cd $(LIBEDIT_DIR) && sed "s/S_LDFLAGS = -shared/S_LDFLAGS = -shared  /" Makefile > Makefile.m32 )
#	(cd $(LIBEDIT_DIR) && mv -f Makefile.m32 Makefile )
#	(cd $(LIBEDIT_DIR) && $(MAKE) )
#	(cd $(LIBEDIT_DIR) && $(MAKE) install )
