#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdint.h>

#include "socket_exception.h"
#include "socket.h"
#include "server_socket.h"

ServerSocket::ServerSocket(uint16_t port, int backlog) {
    this->port = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        throw SocketException("creating socket: " + std::string(strerror(errno)));
    int one = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) < 0) //avoids waiting 2 min before restarting the programm
        throw SocketException("setting socket option: " + std::string(strerror(errno)));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        throw SocketException("bind on socket: " + std::string(strerror(errno)));
    if(listen(sockfd, backlog) < 0)
        throw SocketException("listen on socket: " + std::string(strerror(errno)));
}

ServerSocket::~ServerSocket() {
    close(sockfd);
}

Socket ServerSocket::acceptConnection() {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int fd = accept(sockfd, (struct sockaddr *) &addr, &addrlen);
    if(fd < 0)
        throw SocketException("couldn't accept client: " + std::string(strerror(errno)));
    return Socket(fd, ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
}