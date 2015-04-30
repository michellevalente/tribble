C = g++ -std=c++11
CXX = g++ -std=c++11

INCLUDES = -I../src

CXXFLAGS = -g -Wall $(INCLUDES)

executables = date_client echo_server file_exchange_client ip_addr broadcast_server date_server echo_server_buffered file_exchange_server ip_lookup broadcast_server echo_client_buffered echo_server_operators multicast_receiver echo_client_udp echo_server_udp multicast_sender
#not compiling yet: 
#not to be compiled: http_sample

.PHONY: default
default: $(sources)
	for file in $(executables) ; do \
		echo "Compiling $$file.cpp..."; \
		$(CXX) $(CXXFLAGS) $$file.cpp -o $$file; \
    done

.PHONY: clean
clean:
	rm -f *~ a.out core $(executables)
	rm -rf *.dSYM

.PHONY: all
all: clean default

