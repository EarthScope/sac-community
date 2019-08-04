
## Makefile.am -- Process this file with automake to produce Makefile.in

# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

# Subdirectories to MAKE recursively
SUBDIRS          = 
if READLINE_EDITLINE
SUBDIRS         += libedit
endif
SUBDIRS         += libmseed sacio fern src utils t

SACAUX_WIN       = winaux
SACAUX_UNIX      = aux

if WINAUX_NOT_AUX
SACAUX_DIR       = $(SACAUX_WIN)
else
SACAUX_DIR       = $(SACAUX_UNIX)
endif

EXTRA_ALWAYS = CHANGES Editline.license HISTORY License README 

# Extra Subdirectories/Files to Distribute
#   File included here will be included in a Distribution:
#   % make dist
EXTRA_DIST        = $(EXTRA_ALWAYS) \
		    Readme.buildsac \
		    bin inc lib macros include \
                    doc/README doc/examples \
                    $(SACAUX_DIR)

# Extra Subdirectories/Files to install 
#   Files included here will be included in a Binary Distribution:
#   % make install
#   % make dist-binary
EXTRA_INSTALL    = macros $(EXTRA_ALWAYS) \
	           utils/saclst.c utils/sacswap.c utils/sgftops.c utils/sac.h \
		   utils/bbfswap.c utils/sgfswap.c utils/Makefile.utils \
		   utils/readsac.m utils/writesac.m \
		   utils/padcat.m utils/getsacdata.m \
		   utils/unvis.c utils/README_utils \
		   lib/README_lib \
		   include doc/examples doc/README $(SACAUX_DIR)

include_HEADERS = include/sacio.h include/evalresp.h \
                  include/sac.h include/sacf.h

bin_SCRIPTS = bin/sacinit.sh \
	      bin/sacinit.csh \
	      bin/sgftoeps.csh \
	      bin/sgftox.csh \
	      bin/sac-config

include/sacio.h: $(top_srcdir)/sacio/sacio.h
	[ ! -e include ] && mkdir include
	cp $(top_srcdir)/sacio/sacio.h include/sacio.h


CLEANFILES =  bin/sacinit.sh bin/sacinit.csh bin/sac-config

# Rules to install Extra Subdirectories and Files
install-data-hook:
	$(INSTALL) -d $(prefix)/utils
	if [ -e $(prefix)/lib64 ]; then \
     cd $(prefix); ln -s lib64 lib; \
  fi
	cd $(srcdir); \
	for d in $(EXTRA_INSTALL); do \
	   for f in `(find $$d -type d)`; do \
	      $(INSTALL) -d $(prefix)/$$f; \
	   done; \
	   for f in `(find $$d -type f)`; do \
        $(INSTALL_DATA) $$f $(prefix)/$$f; \
	   done; \
	done; \
	cd -
	( cd $(prefix)/lib && rm -f sacio.a )
	( cd $(prefix)/lib && $(LN_S) libsacio.a sacio.a )
	$(MAKE) dist-hook

dist-save-time: distdir
	tardir=$(distdir) && ${AMTAR} --atime-preserve -chof - "$$tardir" | GZIP=$(GZIP_ENV) gzip -c > $(distdir).tar.gz
	$(am__remove_distdir)

dist-binary: dist-binary-gzip

dist-binary-dir:
	$(am__remove_distdir)
	mkdir $(distdir)

dist-binary-install: dist-binary-dir 
	$(MAKE) prefix=$(PWD)/$(distdir) install 
	mkdir -p bin
	$(SED) "s,__SAC_PREFIX__,/usr/local/sac," $(abs_top_srcdir)/bin/sacinit.csh.in > $(PWD)/$(distdir)/bin/sacinit.csh
	$(SED) "s,__SAC_PREFIX__,/usr/local/sac," $(abs_top_srcdir)/bin/sacinit.sh.in > $(PWD)/$(distdir)/bin/sacinit.sh
	$(SED) "s,__SAC_PREFIX__,/usr/local/sac," $(abs_top_srcdir)/bin/sac-config.in > $(PWD)/$(distdir)/bin/sac-config
	mv $(PWD)/$(distdir) $(PWD)/$(PACKAGE)

dist-binary-gzip: dist-binary-install
	tardir=$(PACKAGE) && $(am__tar) |  GZIP=$(GZIP_ENV) gzip -c > $(distdir)-$(BINARY_DIST).tar.gz
	mv $(PWD)/$(PACKAGE) $(PWD)/$(distdir)
	$(am__remove_distdir)

bin/sacinit.sh: $(abs_top_srcdir)/bin/sacinit.sh.in
	@mkdir -p bin
	$(SED) "s,__SAC_PREFIX__,$(prefix)," $(abs_top_srcdir)/bin/sacinit.sh.in > bin/sacinit.sh

bin/sacinit.csh: $(abs_top_srcdir)/bin/sacinit.csh.in
	@mkdir -p bin
	$(SED) "s,__SAC_PREFIX__,$(prefix)," $(abs_top_srcdir)/bin/sacinit.csh.in > bin/sacinit.csh

bin/sac-config: $(abs_top_srcdir)/bin/sac-config.in
	@mkdir -p bin
	$(SED) -e "s,__SAC_PREFIX__,$(prefix)," -e "s,__SAC_VERSION__,$(VERSION)," $(abs_top_srcdir)/bin/sac-config.in > bin/sac-config

# Run when creating a distribtion
#   - Remove the CVS files
dist-hook:
	test ! -d $(distdir) || rm -rf `find $(distdir) -name CVS -type d `
	test ! -d $(distdir)/inc || rm -rf `find $(distdir)/inc -name config.h  `

dist-cygwin: distdir
	test -d $(distdir) 
	test -d $(distdir)/$(SACAUX_UNIX)
	mv $(distdir)/$(SACAUX_UNIX) $(distdir)/$(SACAUX_WIN)
	$(SED) 's/SACAUX_UNIX = aux/SACAUX_UNIX = winaux/' $(distdir)/Makefile.in > $(distdir)/Makefile.in.new
	mv $(distdir)/Makefile.in.new $(distdir)/Makefile.in
	$(SED) 's/aux/winaux/' $(distdir)/bin/sacinit.sh.in  > $(distdir)/bin/sacinit.sh.in.new
	mv $(distdir)/bin/sacinit.sh.in.new $(distdir)/bin/sacinit.sh.in
	$(SED) 's/aux/winaux/' $(distdir)/bin/sacinit.csh.in > $(distdir)/bin/sacinit.csh.in.new
	mv $(distdir)/bin/sacinit.csh.in.new $(distdir)/bin/sacinit.csh.in
	tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir)-cygwin.tar.gz
	$(am__remove_distdir)

check-clean:
	cd t && $(MAKE) clean

check-libsac:
	cd t && $(MAKE) check-TESTS

coverage:
	rm -f sac_coverage.info
	lcov --base-directory src --directory src -c -o sac_coverage.info
	lcov --remove sac_coverage.info "/usr*" -o sac_coverage.info
	lcov --remove sac_coverage.info "*/expr_*" -o sac_coverage.info
	lcov --remove sac_coverage.info "*<stdout>*" -o sac_coverage.info
	genhtml -o sac_coverage sac_coverage.info

if OSX_APP

all-local: Sacapp

Sacapp: 
	@echo " CREATE " Sac.app
	rm -rf Sac.app
	cp -fpR $(abs_top_srcdir)/src/osx/Sac.app .
	cp -fpR $(abs_top_srcdir)/aux Sac.app/Contents/Resources
	mkdir $(SACAPP_RSC)/doc
	cp -fpR $(abs_top_srcdir)/doc/README $(SACAPP_RSC)/doc
	cp -fpR $(abs_top_srcdir)/doc/examples $(SACAPP_RSC)/doc
	cd utils && $(MAKE) sacapp
	cd src && $(MAKE) sacapp

clean-local:
	rm -rf Sac.app

dmg: Sac.app
	mkdir dmgdir
	cp -fpR Sac.app dmgdir
	cd dmgdir && ln -s /Applications .
	hdiutil create -format UDZO -volname "SAC v$(PACKAGE_VERSION)" -srcfolder dmgdir ./SAC-$(PACKAGE_VERSION).dmg
	rm -fr dmgdir
endif
