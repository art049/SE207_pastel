SYSTEMC   ?= /comelec/softs/opt/systemc/current
ARCH      ?= linux64

CC        = gcc
CXX       = g++

CFLAGS   +=  -g -Wall
CPPFLAGS  =  -isystem $(SYSTEMC)/include
CXXFLAGS  =  -Wall -O2 -g
LDFLAGS   =  -g -L$(SYSTEMC)/lib-$(ARCH)
LDLIBS    =  -lsystemc -pthread -lpng

SOURCES_CPP = $(wildcard *.cpp)
SOURCES_C   = $(wildcard *.c)
OBJS        = $(patsubst %.cpp,%.o,$(SOURCES_CPP))
OBJS       += $(patsubst %.c,%.o,$(SOURCES_C))
EXE         = simulation.x

DEPS = $(patsubst %.o,.%.d,$(OBJS))

all: $(EXE)

$(EXE) : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@  $(LDLIBS) 2>&1 | c++filt

clean :
	-rm -f $(EXE)
	-rm -f $(OBJS)
	-rm -f $(DEPS)
	-rm -f *.vcd
	-rm -f *~ *_old
	-rm -f core

.%.d:%.c
	$(CC) $(CPPFLAGS) -MM -MF $@ -MT $(patsubst %.c,%.o,$<) $<
.%.d:%.cpp
	$(CXX) $(CPPFLAGS) -MM -MF $@ -MT $(patsubst %.cpp,%.o,$<) $<

ifneq ($(MAKECMMGOALS),clean)
-include $(DEPS)
endif
