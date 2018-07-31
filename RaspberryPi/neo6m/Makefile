CC = g++
OBJECTS = neo6m.o

all: $(OBJECTS)

neo6m.o: neo6m.cpp neo6m.h neo6m_exception.h ../gpio/uart_dev.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm *.o