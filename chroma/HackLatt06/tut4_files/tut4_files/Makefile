CHROMA=/home/bj/Devel/SciDAC/install/chroma_scalar_single
CONFIG=$(CHROMA)/bin/chroma-config
CXX=$(shell $(CONFIG) --cxx)
CXXFLAGS=$(shell $(CONFIG) --cxxflags) -I.
LDFLAGS=$(shell $(CONFIG) --ldflags)
LIBS=$(shell $(CONFIG) --libs)

HDRS=inline_my_meas.h

OBJS=chroma.o \
     inline_my_meas.o

chroma: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.cc $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -rf chroma $(OBJS) *~
