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
#include <poll.h>
#include <stdexcept>

#include "socket_exception.h"
#include "../thread/interrupted_exception.h"
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
    if(fd < 0) {
        if(errno == EINTR)
            throw InterruptedException("signal occured: " + std::string(strerror(errno)));
        else
            throw SocketException("couldn't accept client: " + std::string(strerror(errno)));
    }
    return Socket(fd, ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
}

Socket ServerSocket::acceptConnection(int timeout) {
    //wait until a connection comes in or the timeout expires
    struct pollfd poll_struct;
    memset(&poll_struct, 0, sizeof(poll_struct));
    poll_struct.fd = sockfd;
    poll_struct.events = POLLIN;
    int ret = poll(&poll_struct, 1, timeout);
    if(ret < 0)
        throw std::runtime_error("server socket: poll failed: " + std::string(strerror(errno)));
    else if(ret == 0)
        throw TimeoutException("server socket: poll timed out");
    else if((poll_struct.revents & POLLERR) || (poll_struct.revents & POLLNVAL))
        throw std::runtime_error("server socket: poll failed");
    //accept the connection
    return acceptConnection();
}