SYSTEMC ?= /comelec/softs/opt/systemc/current
ARCH = linux64

CPPFLAGS = -isystem $(SYSTEMC)/include
CXXFLAGS = -Wall -g
LDFLAGS = -L$(SYSTEMC)/lib-$(ARCH)
LDLIBS = -lsystemc

default : seance_1/hello_world seance_1/type_logic seance_1/type_vector
