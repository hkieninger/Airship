OBJECTS = hcsr04.o

all: $(OBJECTS)

hcsr04.o: hcsr04.cpp hcsr04.h ../gpio/pin_dev.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o