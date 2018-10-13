all: camera_thread.o

camera_thread.o: camera_thread.cpp camera_thread.h ../../hardware/camera/camera.h ../../thread/thread.h ../../socket/socket.h ../../socket/socket_exception.h ../../socket/server_socket.h ../../thread/interrupted_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o