C = g++ -std=c++11
CXX = g++ -std=c++11

INCLUDES = -I../src

CXXFLAGS = -g -Wall $(INCLUDES)

executables = broadcast_server echo_client_udp echo_server echo_server_udp ip_addr ip_lookup

.PHONY: default
default: $(sources)
	for file in $(executables) ; do \
		echo "Compiling $$file.cpp..."; \
		$(CXX) $(CXXFLAGS) "$$file.cpp" ; \
    done

.PHONY: clean
clean:
	rm -f *~ a.out core $(executables)

.PHONY: all
all: clean default

