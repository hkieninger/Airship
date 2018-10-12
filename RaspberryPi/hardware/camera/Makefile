OBJECTS = camera.o

all: $(OBJECTS)

camera.o: camera.cpp camera.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o