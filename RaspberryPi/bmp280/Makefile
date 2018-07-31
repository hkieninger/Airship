CC = g++
OBJECTS = bmp280.o

all: $(OBJECTS)

bmp280.o: bmp280.cpp bmp280.h ../gpio/i2c_dev.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm *.o