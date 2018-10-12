CC = g++
CFLAGS = -Wall -g
LIBS = -lpigpio -lpthread -lm
OWNLIBS = -L./control/ -lcontrolZ -L./hardware/ -lhardwareZ -L./socket/ -lsocketZ -L./thread/ -lthreadZ
export

zeppelin: main.o librarys
	$(CC) -o $@ $(CFLAGS) main.o $(LIBS) $(OWNLIBS)

librarys:
	$(MAKE) -C control/
	$(MAKE) -C hardware/
	$(MAKE) -C socket/
	$(MAKE) -C thread/

main.o: main.cpp control/connection.h control/control_thread.h thread/interrupted_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C control/ clean
	$(MAKE) -C hardware/ clean
	$(MAKE) -C socket/ clean
	$(MAKE) -C thread/ clean
	rm -f *.o
	rm -f zeppelin