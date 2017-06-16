SYSTEMC ?= /comelec/softs/opt/systemc/current
ARCH = linux64

CPPFLAGS = -isystem $(SYSTEMC)/include
CXXFLAGS = -Wall -g
LDFLAGS = -L$(SYSTEMC)/lib-$(ARCH)
LDLIBS = -lsystemc
seance_6 : seance_6/signalbuffer seance_6/fifopixels seance_6/mutex
seance_5 : seance_5/pgcd
seance_4 : seance_4/events
seance_3 : seance_3/modules
seance_2 : seance_2/exemple seance_2/structure seance_2/clocks
seance_1 : seance_1/hello_world seance_1/type_logic seance_1/type_vector seance_1/type_int_uint
