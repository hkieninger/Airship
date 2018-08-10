CC = g++
OBJECTS = socket.o server_socket.o

all: $(OBJECTS)

socket.o: socket.cpp socket.h socket_exception.h
	$(CC) -c $(CFLAGS) $<

server_socket.o: server_socket.cpp server_socket.h socket.h socket_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o