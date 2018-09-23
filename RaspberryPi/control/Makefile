HARDWARE = \
../hardware/servo/servo.h \
../hardware/motor/motor.h
ALGORITHM = \
algorithm/steering.h
SOCKET = \
../socket/socket.h \
../socket/server_socket.h \
../socket/socket_exception.h
export

libcontrolZ.a: control_thread.o connection.o algorithms
	ar crs $@ control_thread.o connection.o algorithm/*.o
	ranlib $@

algorithms:
	$(MAKE) -C algorithm/

control_thread.o: control_thread.cpp control_thread.h pin.h ../thread/thread.h connection.h makros.h $(ALGORITHM) $(HARDWARE)
	$(CC) -c $(CFLAGS) $<

connection.o: connection.cpp connection.h control_thread.h $(SOCKET)
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C algorithm/ clean
	rm -f *.o
	rm -f *.a