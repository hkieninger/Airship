all: camera_thread.o

camera_thread.o: camera_thread.cpp camera_thread.h ../../hardware/camera/camera.h ../../thread/thread.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o