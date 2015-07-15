CXX = /usr/bin/gcc
CXXFLAGS = -fPIC -Wall -Wno-unknown-pragmas -Iinc -O6
DBG = -g
DEFINES ?=
LDFLAGS = -shared

BINDIR = ./bin
LIBDIR = ./lib
INCDIR = ./inc
SRCDIR = ./src
ROOTS = hb-pow-example
BINS = $(ROOTS:%=$(BINDIR)/%)
OBJS = $(ROOTS:%=$(BINDIR)/%.o)

all: $(BINDIR) $(LIBDIR) $(LIBDIR)/libhbs-pow.so $(BINS)

$(BINDIR):
	-mkdir -p $(BINDIR)

$(LIBDIR):
	-mkdir -p $(LIBDIR)

$(BINDIR)/%.o : $(SRCDIR)/%.c
	$(CXX) -c $(CXXFLAGS) $(DEFINES) $(DBG) -o $@ $<

$(BINS) : $(OBJS)

$(BINS) : % : %.o
	$(CXX) $(CXXFLAGS) -o $@ $< -Llib -lhbs-pow

$(LIBDIR)/libhbs-pow.so: $(SRCDIR)/hb-pow.c $(SRCDIR)/hb-util.c
	$(CXX) $(CXXFLAGS) -DHEARTBEAT_MODE_POW $(LDFLAGS) -Wl,-soname,$(@F) -o $@ $^

# Installation
install: all
	install -m 0644 $(LIBDIR)/*.so /usr/local/lib/
	mkdir -p /usr/local/include/heartbeats-simple
	install -m 0644 $(INCDIR)/* /usr/local/include/heartbeats-simple/

uninstall:
	rm -f /usr/local/lib/libhbs-*.so
	rm -rf /usr/local/include/heartbeats-simple/

## cleaning
clean:
	-rm -rf $(LIBDIR) $(BINDIR) *.log *~ $(SRCDIR)/*~
