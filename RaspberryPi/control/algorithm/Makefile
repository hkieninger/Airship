all: steering.o

steering.o: steering.cpp steering.h ../../hardware/motor/motor.h ../../hardware/servo/servo.h ../connection.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o