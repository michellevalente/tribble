#include "socket.hpp"

string Socket::receive(int buffer_size, error_code error) {
    char* buffer = new char[buffer_size+1];
    int sz = recv(c_socket, buffer, sizeof(buffer), 0);

    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    return str;
}

void Socket::send_all(string to_send) {
    int length = to_send.length();
    char *ptr = &to_send[0];
    while (length > 0)
    {
        int i = send(c_socket, ptr, length, 0);
        if (i < 1) 
            throw std::invalid_argument("TODO: change this exception type and description");
        ptr += i;
        length -= i;
    }
}

StreamSocket StreamSocket::accept() {
    StreamSocket sock;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    sock.c_socket = ::accept(c_socket, (struct sockaddr *)&addr, &addrLen);

    return sock;
}

StreamSocket::StreamSocket() {
    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");
}   

StreamSocket::StreamSocket(int port_number) {
    this->port_number = port_number;

    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));       /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(port_number);              /* Local port */

    /* Bind to the local address */
    if (bind(c_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(c_socket, MAXPENDING) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");
}

DatagramSocket::DatagramSocket() {
    if ((c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_TCP)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");
}   