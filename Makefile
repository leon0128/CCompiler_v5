CXX      = g++-8
CXXFLAGS = -std=c++11 $(DEBFLAGS)
DEBFLAGS = -g3 -Wall

OBJS     = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)*.cpp))

SHROBJS  = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)*$(SHRDIR)*.cpp))
PPOBJS   = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)$(PPDIR)*.cpp))

COMPILER = compiler

APPDIR = app/
SRCDIR = src/
SHRDIR = share/
PPDIR  = pp_v2/

all: $(COMPILER)

$(COMPILER): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(DEBFRAGS) -o $(APPDIR)$(COMPILER)

shr: $(SHROBJS)

$(PP): $(PPOBJS) $(SHROBJS)
	$(CXX) $(PPOBJS) $(CXXFLAGS) $(SHROBJS) $(DEBFLAGS) -o $(APPDIR)$(PP)

clean:; rm -f $(APPDIR)$(COMPILER) $(SRCDIR)*.o
clean_pp:; rm -f $(APPDIR)$(PP) $(SRCDIR)$(PPDIR)*.o
clean_shr:; rm -f $(SRCDIR)$(SHRDIR)*.o
