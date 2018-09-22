OBJECTS = servo.o

all: $(OBJECTS)

servo.o: servo.cpp servo.h ../gpio/pin_dev.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o