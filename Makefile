CXX = /usr/bin/gcc
CXXFLAGS = -fPIC -Wall -Wno-unknown-pragmas -Iinc -O6
DBG = -g
LDFLAGS = -shared
BINCXXFLAGS = -Wall -Wno-unknown-pragmas -Iinc -g -O0
BINLDFLAGS = -Llib -lhbs -lhbs-acc -lhbs-pow -lhbs-acc-pow

LIBDIR = ./lib
INCDIR = ./inc
SRCDIR = ./src
BINDIR = ./bin
PCDIR = ./pkgconfig

EXADIR = ./example
EXABINDIR = $(BINDIR)/example
EXA_SOURCES = $(wildcard $(EXADIR)/*.c)
EXA_OBJECTS = $(patsubst $(EXADIR)/%.c,$(EXABINDIR)/%.o,$(EXA_SOURCES))
EXAMPLES = $(patsubst $(EXADIR)/%.c,$(EXABINDIR)/%,$(EXA_SOURCES))

TESTDIR = ./test
TESTBINDIR = $(BINDIR)/test
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.c,$(TESTBINDIR)/%.o,$(TEST_SOURCES))
TESTS = $(patsubst $(TESTDIR)/%.c,$(TESTBINDIR)/%,$(TEST_SOURCES))

all: $(LIBDIR) $(LIBDIR)/libhbs.so $(LIBDIR)/libhbs-acc.so $(LIBDIR)/libhbs-pow.so $(LIBDIR)/libhbs-acc-pow.so $(EXAMPLES) $(TESTS)

$(LIBDIR) $(BINDIR) $(EXABINDIR) $(TESTBINDIR):
	mkdir -p $@

# Libraries

$(LIBDIR)/libhbs.so: $(SRCDIR)/hb.c $(SRCDIR)/hb-util.c
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -Wl,-soname,$(@F) -o $@ $^

$(LIBDIR)/libhbs-acc.so: $(SRCDIR)/hb.c $(SRCDIR)/hb-util.c $(SRCDIR)/hb-acc-util.c
	$(CXX) $(CXXFLAGS) -DHEARTBEAT_MODE_ACC -DHEARTBEAT_USE_ACC $(LDFLAGS) -Wl,-soname,$(@F) -o $@ $^

$(LIBDIR)/libhbs-pow.so: $(SRCDIR)/hb.c $(SRCDIR)/hb-util.c $(SRCDIR)/hb-pow-util.c
	$(CXX) $(CXXFLAGS) -DHEARTBEAT_MODE_POW -DHEARTBEAT_USE_POW $(LDFLAGS) -Wl,-soname,$(@F) -o $@ $^

$(LIBDIR)/libhbs-acc-pow.so: $(SRCDIR)/hb.c $(SRCDIR)/hb-util.c $(SRCDIR)/hb-acc-util.c $(SRCDIR)/hb-pow-util.c
	$(CXX) $(CXXFLAGS) -DHEARTBEAT_MODE_ACC_POW -DHEARTBEAT_USE_ACC -DHEARTBEAT_USE_POW $(LDFLAGS) -Wl,-soname,$(@F) -o $@ $^

# Build example object files
$(EXABINDIR)/%.o: $(EXADIR)/%.c | $(EXABINDIR)
	$(CXX) -c $(BINCXXFLAGS) $< -o $@

# Build example binaries
$(EXABINDIR)/%: $(EXABINDIR)/%.o
	$(CXX) $< $(BINLDFLAGS) -o $@

# Build test object files
$(TESTBINDIR)/%.o: $(TESTDIR)/%.c | $(TESTBINDIR)
	$(CXX) -c $(BINCXXFLAGS) $< -o $@

# Build test binaries
$(TESTBINDIR)/%: $(TESTBINDIR)/%.o
	$(CXX) $< $(BINLDFLAGS) -o $@


# Installation
install: all
	install -m 0644 $(LIBDIR)/*.so /usr/local/lib/
	mkdir -p /usr/local/include/heartbeats-simple
	install -m 0644 $(INCDIR)/* /usr/local/include/heartbeats-simple/
	mkdir -p /usr/local/lib/pkgconfig
	install -m 0644 $(PCDIR)/*.pc /usr/local/lib/pkgconfig

uninstall:
	rm -f /usr/local/lib/libhbs-*.so
	rm -rf /usr/local/include/heartbeats-simple/
	rm -f /usr/local/lib/pkgconfig/hbs*.pc

## cleaning
clean:
	-rm -rf $(LIBDIR) $(BINDIR) *.log *~
