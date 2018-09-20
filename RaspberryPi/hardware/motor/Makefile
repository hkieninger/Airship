CC = g++
OBJECTS = motor.o

all: $(OBJECTS)

motor.o: motor.cpp motor.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o