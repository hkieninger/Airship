CC = g++
OBJECTS = ads1115.o

all: $(OBJECTS)

ads1115.o: ads1115.cpp ads1115.h ../gpio/i2c_dev.h ../gpio/gpio_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o