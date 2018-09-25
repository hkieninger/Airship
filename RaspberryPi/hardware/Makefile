export

libhardwareZ.a: components
	ar crs $@ gpio/gpio_dev.o gpio/pin_dev.o gpio/i2c_dev.o motor/motor.o servo/servo.o ads1115/ads1115.o
	ranlib $@

components:
	$(MAKE) -C gpio/
	$(MAKE) -C motor/
	$(MAKE) -C servo/
	$(MAKE) -C ads1115/

clean:
	$(MAKE) -C gpio/ clean
	$(MAKE) -C motor/ clean
	$(MAKE) -C servo/ clean
	$(MAKE) -C ads1115/ clean
	rm -f *.a