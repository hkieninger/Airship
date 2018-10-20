all: camera_thread.o jpg_camera_thread.o

camera_thread.o: camera_thread.cpp camera_thread.h ../../hardware/camera/camera.h ../../thread/thread.h ../../socket/socket.h ../../socket/socket_exception.h ../../socket/server_socket.h ../../thread/interrupted_exception.h ../connection.h
	$(CC) -c $(CFLAGS) $<

jpg_camera_thread.o: jpg_camera_thread.cpp jpg_camera_thread.h camera_thread.o
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o