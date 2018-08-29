#ifndef SOCKET_H
#define SOCKET_H 1

#include <stddef.h>
#include <string>
#include <stdint.h>

/*
 * class for a TCP Socket
 */
class Socket {
    /*
     * are stored in little endian,
     * to apply inet_aton(), etc to them you must convert them to big endian e.g with htons() or htonl()
     */
    uint32_t remoteIP;
    uint16_t remotePort;
    int sockfd;
public:
    /*
     * creates a socket and connect it to the specified server
     * @remoteIP: ip of the server
     * @remotePort: port of the server
     */
    Socket(uint32_t remoteIP, uint16_t remotePort);

    /*
     * constructor for an already connected socket file descriptor
     */
    Socket(int sockfd, uint32_t remoteIP, uint16_t remotePort);

    /*
     * closes the file descriptor
     */
    ~Socket();

    /*
     * sends len bytes from the buffer
     */
    void sendAll(const void *buf, int len);

    /*
     * reads len bytes into the buffer
     * if the socket has been closed by the peer a SocketClosedException is thrown
     * @return: buf
     */
    void *recvAll(void *buf, int len);

    /*
     * returns the remote IP address as string
     */
    std::string getRemoteIPString();

    uint32_t getRemoteIP() {
        return remoteIP;
    }

    uint16_t getRemotePort() {
        return remotePort;
    }

    /*
     * converts an ip address in string form (with punctuation e.g. 127.0.0.1) to an uint32_t
     */
    static uint32_t stringToInt(const std::string &addr);
};

#endif /* SOCKET_H */