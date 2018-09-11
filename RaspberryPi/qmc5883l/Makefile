CC = g++
OBJECTS = qmc5883l.o

all: $(OBJECTS)

qmc5883l.o: qmc5883l.cpp qmc5883l.h ../gpio/i2c_dev.h ../gpio/gpio_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o