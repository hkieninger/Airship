libhardwareZ.a: components
	ar crs $@ gpio/gpio_dev.o gpio/pin_dev.o motor/motor.o servo/servo.o
	ranlib $@

components:
	$(MAKE) -C gpio/ CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C motor/ CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C servo/ CC=$(CC) CFLAGS=$(CFLAGS)

clean:
	$(MAKE) -C gpio/ clean
	$(MAKE) -C motor/ clean
	$(MAKE) -C servo/ clean
	rm -f *.a