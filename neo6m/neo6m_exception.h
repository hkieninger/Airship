#include <string>

#include "../gpio/gpio_exception.h"

/*
 * represents an exception related to the neo6m
 */
class Neo6MException: public UARTException {
    public:
        explicit Neo6MException(const std::string& what_arg) : UARTException(what_arg) {};
        explicit Neo6MException(const char* what_arg) : UARTException(what_arg) {};
};