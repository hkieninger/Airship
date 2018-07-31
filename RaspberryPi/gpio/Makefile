CC = g++
OBJECTS = i2c_dev.o uart_dev.o

all: $(OBJECTS)

i2c_dev.o: i2c_dev.cpp i2c_dev.h gpio_exception.h
	$(CC) -c $(CFLAGS) $<

uart_dev.o: uart_dev.cpp uart_dev.h gpio_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm *.o