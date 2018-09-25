all: steering.o

steering.o: steering.cpp steering.h ../../hardware/motor/motor.h ../../hardware/servo/servo.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o