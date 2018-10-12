HARDWARE = \
../hardware/servo/servo.h \
../hardware/motor/motor.h \
../hardware/ads1115/ads1115.h \
../hardware/mpu6050/mpu6050.h \
../hardware/bmp280/bmp280.h \
../hardware/qmc5883l/qmc5883l.h \
../hardware/hcsr04/hcsr04.h
ALGORITHM = \
algorithm/steering.h
THREADS = \
threads/camera_thread.h
SOCKET = \
../socket/socket.h \
../socket/server_socket.h \
../socket/socket_exception.h
export

libcontrolZ.a: control_thread_conf.o control_thread_meas.o connection.o algorithms threads
	ar crs $@ control_thread_conf.o control_thread_meas.o connection.o algorithm/*.o threads/*.o
	ranlib $@

threads:
	$(MAKE) -C threads/

algorithms:
	$(MAKE) -C algorithm/

control_thread_conf.o: control_thread_conf.cpp control_thread.h pin.h ../thread/thread.h connection.h makros.h $(THREADS) $(ALGORITHM) $(HARDWARE)
	$(CC) -c $(CFLAGS) $<

control_thread_meas.o: control_thread_meas.cpp control_thread.h pin.h ../thread/thread.h connection.h makros.h $(THREADS) $(ALGORITHM) $(HARDWARE)
	$(CC) -c $(CFLAGS) $<

connection.o: connection.cpp connection.h control_thread.h $(SOCKET)
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C algorithm/ clean
	rm -f *.o
	rm -f *.a