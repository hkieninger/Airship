CC = g++
OBJECTS = neo6m.o neo6m_thread.o

all: $(OBJECTS)

neo6m.o: neo6m.cpp neo6m.h neo6m_exception.h ../gpio/uart_dev.h
	$(CC) -c $(CFLAGS) $<

neo6m_thread.o: neo6m_thread.cpp neo6m_thread.h neo6m.h ../../thread/thread.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o