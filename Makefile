CXX      = g++-8
CXXFLAGS = -std=c++11 $(DEBFLAGS)
DEBFLAGS = -g3 -Wall
OBJS     = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)*/*.cpp))
PPOBJS   = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)$(PPDIR)*.cpp))

PP = pp

APPDIR = app/
SRCDIR = src/
PPDIR  = pp/

all: $(PP)

$(PP): $(PPOBJS)
	$(CXX) $(PPOBJS) $(CXXFLAGS) $(DEBFLAGS) -o $(APPDIR)$(PP)

clean: clean_pp
clean_pp:; rm -f $(APPDIR)$(PP) $(SRCDIR)$(PPDIR)*.o