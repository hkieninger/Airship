#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H 1

#include <string>
#include <stdexcept>

/*
 * represents an exception related to sockets
 */
class SocketException: public std::runtime_error {
    public:
        explicit SocketException(const std::string& what_arg) : std::runtime_error(what_arg) {};
        explicit SocketException(const char* what_arg) : std::runtime_error(what_arg) {};
};

class SocketClosedException: public SocketException {
    public:
        explicit SocketClosedException(const std::string& what_arg) : SocketException(what_arg) {};
        explicit SocketClosedException(const char* what_arg) : SocketException(what_arg) {};
};

#endif /* SOCKET_EXCEPTION_H */