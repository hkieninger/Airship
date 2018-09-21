OBJECTS = servo.o

all: $(OBJECTS)

servo.o: servo.cpp servo.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o