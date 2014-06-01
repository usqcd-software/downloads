CHROMA=/home/bj/Devel/QCD/install/chroma_scalar_single
CONFIG=$(CHROMA)/bin/chroma-config
CXX=$(shell $(CONFIG) --cxx)
CXXFLAGS=$(shell $(CONFIG) --cxxflags) -I.
LDFLAGS=$(shell $(CONFIG) --ldflags)
LIBS=$(shell $(CONFIG) --libs)

HDRS=

OBJS= tut3.o

tut3: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.cc $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf tut3 $(OBJS) *~
