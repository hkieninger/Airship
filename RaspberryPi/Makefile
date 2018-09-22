CC = g++
CFLAGS = -Wall
LIBS = -lpigpio -lpthread -lrt -lm
OWNLIBS = -L./control/ -lcontrolZ -L./hardware/ -lhardwareZ -L./socket/ -lsocketZ -L./thread/ -lthreadZ

zeppelin: main.o librarys
	$(CC) -o $@ main.o $(LIBS) $(OWNLIBS)

librarys:
	$(MAKE) -C control/ CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C hardware/ CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C socket/ CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C thread/ CC=$(CC) CFLAGS=$(CFLAGS)

main.o: main.cpp control/connection.h control/control_thread.h thread/interrupted_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C control/ clean
	$(MAKE) -C hardware/ clean
	$(MAKE) -C socket/ clean
	$(MAKE) -C thread/ clean
	rm -f *.o
	rm -f zeppelin