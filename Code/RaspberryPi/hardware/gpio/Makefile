CC = g++
OBJECTS = gpio_dev.o pin_dev.o i2c_dev.o uart_dev.o

all: $(OBJECTS)

gpio_dev.o: gpio_dev.cpp gpio_dev.h gpio_exception.h
	$(CC) -c $(CFLAGS) $<

pin_dev.o: pin_dev.cpp pin_dev.h gpio_dev.h gpio_exception.h
	$(CC) -c $(CFLAGS) $<

i2c_dev.o: i2c_dev.cpp i2c_dev.h gpio_dev.h gpio_exception.h
	$(CC) -c $(CFLAGS) $<

uart_dev.o: uart_dev.cpp uart_dev.h gpio_dev.h gpio_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o