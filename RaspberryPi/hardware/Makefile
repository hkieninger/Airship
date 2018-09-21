libhardwareZ.a: components
	ar crs $@ motor/motor.o servo/servo.o
	ranlib $@

components:
	$(MAKE) -C motor/ CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C servo/ CC=$(CC) CFLAGS=$(CFLAGS)

clean:
	$(MAKE) -C motor/ clean
	$(MAKE) -C servo/ clean
	rm -f *.a