export

libhardwareZ.a: components
	ar crs $@ gpio/gpio_dev.o gpio/pin_dev.o motor/motor.o servo/servo.o
	ranlib $@

components:
	$(MAKE) -C gpio/
	$(MAKE) -C motor/
	$(MAKE) -C servo/

clean:
	$(MAKE) -C gpio/ clean
	$(MAKE) -C motor/ clean
	$(MAKE) -C servo/ clean
	rm -f *.a