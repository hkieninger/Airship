CC = g++
OBJECTS = mpu6050.o

all: $(OBJECTS)

mpu6050.o: mpu6050.cpp mpu6050.h ../gpio/i2c_dev.h ../gpio/gpio_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o