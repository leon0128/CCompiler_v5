CXX      = g++-8
CXXFLAGS = -std=c++11 $(DEBFLAGS)
DEBFLAGS = -g3 -Wall

SHROBJS  = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)*$(SHRDIR)*.cpp))
PPOBJS   = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)$(PPDIR)*.cpp))

PP = pp

APPDIR = app/
SRCDIR = src/
SHRDIR = share/
PPDIR  = pp_v2/

all: $(PP)

shr: $(SHROBJS)

$(PP): $(PPOBJS) $(SHROBJS)
	$(CXX) $(PPOBJS) $(CXXFLAGS) $(SHROBJS) $(DEBFLAGS) -o $(APPDIR)$(PP)

clean: clean_pp clean_shr
clean_pp:; rm -f $(APPDIR)$(PP) $(SRCDIR)$(PPDIR)*.o
clean_shr:; rm -f $(SRCDIR)$(SHRDIR)*.o
