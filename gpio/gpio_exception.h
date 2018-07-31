#ifndef GPIO_EXCEPTION_H
#define GPIO_EXCEPTION_H 1

#include <stdexcept>
#include <string>

/*
 * represents an exception related to the gpio pins
 */
class GPIOException: public std::runtime_error {
    public:
        explicit GPIOException(const std::string& what_arg) : std::runtime_error(what_arg) {};
        explicit GPIOException(const char* what_arg) : std::runtime_error(what_arg) {};
};

/*
 * represents an exception related to the i2c bus
 */
class I2CException: public GPIOException {
    public:
        explicit I2CException(const std::string& what_arg) : GPIOException(what_arg) {};
        explicit I2CException(const char* what_arg) : GPIOException(what_arg) {};
};

/*
 * represents an exception related to the uart
 */
class UARTException: public GPIOException {
    public:
        explicit UARTException(const std::string& what_arg) : GPIOException(what_arg) {};
        explicit UARTException(const char* what_arg) : GPIOException(what_arg) {};
};

#endif /* GPIO_EXCEPTION_H */