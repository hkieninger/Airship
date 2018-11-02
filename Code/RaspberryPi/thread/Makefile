libthreadZ.a: thread.o
	ar crs $@ $^
	ranlib $@

thread.o: thread.cpp thread.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o
	rm -f *.a