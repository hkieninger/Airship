OBJECTS = motor.o

all: $(OBJECTS)

motor.o: motor.cpp motor.h ../gpio/pin_dev.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o