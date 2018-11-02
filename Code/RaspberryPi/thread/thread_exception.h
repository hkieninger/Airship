#ifndef THREAD_EXCEPTION_H
#define THREAD_EXCEPTION_H 1

#include <stdexcept>
#include <string>

class ThreadException: public std::runtime_error {
    public:
        explicit ThreadException(const std::string& what_arg) : std::runtime_error(what_arg) {};
        explicit ThreadException(const char* what_arg) : std::runtime_error(what_arg) {};
};

#endif /* THREAD_EXCEPTION_H */