SYSTEMC    = /comelec/softs/opt/systemc-2.3.0
ARCH       = linux64

CC        = gcc
CXX       = g++

CPPFLAGS  =  -isystem $(SYSTEMC)/include
CXXFLAGS  =  -Wall -O2 -g
LDFLAGS   =  -L$(SYSTEMC)/lib-$(ARCH)
LDLIBS    =  -lsystemc -pthread

SOURCES_CPP = $(wildcard *.cpp)
SOURCES_C   = $(wildcard *.c)
OBJS        = $(patsubst %.cpp,%.o,$(SOURCES_CPP))
OBJS       += $(patsubst %.c,%.o,$(SOURCES_C))
EXE         = simulation.x

DEPS = $(patsubst %.o,.%.dep,$(OBJS))

all: $(EXE)

$(EXE) : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@  -lsystemc -lpng 2>&1 | c++filt

%.o:%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-rm -f $(EXE)
	-rm -f $(OBJS)
	-rm -f $(DEPS)
	-rm -f *.vcd
	-rm -f *~ *_old
	-rm -f core

.%.dep:%.c
	$(CC) $(CPPFLAGS) -MMD -MF $@ -c -o $(patsubst %.c,%.o,$<) $<
.%.dep:%.cpp
	$(CXX) $(CPPFLAGS) -MMD -MF $@ -c -o $(patsubst %.cpp,%.o,$<) $<

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
