#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H 1

#include <stdint.h>
#include "socket.h"

/*
 * class for a TCP ServerSocket
 */
class ServerSocket {
    //stored in little endian
    uint16_t port;

    int sockfd;
public:
    /*
     * creates the socket file descriptor
     * binds it to the specified port
     * listen on the specified port
     * @port: the tcp port
     * @backlog: the maximum length to which the queue of pending connections for sockfd may grow
     */
    ServerSocket(uint16_t port, int backlog = 1);

    /*
     * closes the socket file descriptor
     */
    ~ServerSocket();

    /*
     * accepts the first connection in the backlog and returns a Socket to it
     */
    Socket acceptConnection();

    uint16_t getPort() {
        return port;
    }
};

#endif /* SERVER_SOCKET_H */