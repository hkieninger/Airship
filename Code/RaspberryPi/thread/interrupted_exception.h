#ifndef INTERRUPTED_EXCEPTION_H
#define INTERRUPTED_EXCEPTION_H 1

#include <stdexcept>
#include <string>

class InterruptedException: public std::runtime_error {
    public:
        explicit InterruptedException(const std::string& what_arg) : std::runtime_error(what_arg) {};
        explicit InterruptedException(const char* what_arg) : std::runtime_error(what_arg) {};
};

#endif /* INTERRUPTED_EXCEPTION_H */