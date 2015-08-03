BUILDDIR = _build
MAKE_PARAMS = --no-print-directory

all: $(BUILDDIR) cmake

$(BUILDDIR):
	mkdir -p $@

cmake: $(BUILDDIR)
	cd $(BUILDDIR) && cmake .. && make $(MAKE_PARAMS)

install: all
	cd _build && make install $(MAKE_PARAMS)

uninstall: install
	cd $(BUILDDIR) && make uninstall $(MAKE_PARAMS)

clean:
	cd $(BUILDDIR) && make clean $(MAKE_PARAMS)
	-rm -rf $(BUILDDIR)
