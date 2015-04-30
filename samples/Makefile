C = g++ -std=c++11
CXX = g++ -std=c++11

INCLUDES = -I../src

CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS  =

executables = broadcast_server echo_client_udp echo_server echo_server_udp ip_addr ip_lookup
objects = broadcast_server.o echo_client_udp.o echo_server.o echo_server_udp.o ip_addr.o ip_lookup.o

.PHONY: default
default: $(executables)

$(executables):

$(objects):

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default

