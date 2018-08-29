#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>

#include "socket_exception.h"
#include "socket.h"

Socket::Socket(uint32_t remoteIP, uint16_t remotePort) {
    this->remoteIP = remoteIP;
    this->remotePort = remotePort;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        throw SocketException("creating socket: " + std::string(strerror(errno)));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(remoteIP);
    addr.sin_port = htons(remotePort);
    if(connect(sockfd, (struct sockaddr *) &addr, sizeof (addr)) < 0)
        throw SocketException("connecting socket: " + std::string(strerror(errno)));
}

Socket::Socket(int sockfd, uint32_t remoteIP, uint16_t remotePort) {
    this->sockfd = sockfd;
    this->remoteIP = remoteIP;
    this->remotePort = remotePort;
}

Socket::~Socket() {
    close(sockfd);
}

void Socket::sendAll(const void *buf, int len) {
    int sent = 0;
    while(sent < len) {
        int ret = send(sockfd, (uint8_t *) buf + sent, len - sent, 0);
        if(ret < 0)
            throw SocketException("send on socket: " + std::string(strerror(errno)));
        sent += ret;
    }
}

void *Socket::recvAll(void *buf, int len) {
    int received = 0;
    while(received < len) {
        int ret = recv(sockfd, (uint8_t *) buf + received, len - received, 0);
        if(ret < 0)
            throw SocketException("receive on socket: " + std::string(strerror(errno)));
        if(ret == 0)
            throw SocketClosedException("socket has been closed by peer");
        received += ret;
    }
    return buf;
}

std::string Socket::getRemoteIPString() {
    struct in_addr in;
    in.s_addr = htonl(remoteIP);
    return std::string(inet_ntoa(in));
}

uint32_t Socket::stringToInt(const std::string &addr) {
    struct in_addr in;
    if(inet_aton(addr.c_str(), &in) == 0)
        throw std::invalid_argument("converting string IP address to int");
    return ntohl(in.s_addr);
}