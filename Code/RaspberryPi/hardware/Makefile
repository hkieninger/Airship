OBJECTS = \
gpio/gpio_dev.o \
gpio/pin_dev.o \
gpio/i2c_dev.o \
gpio/uart_dev.o \
motor/motor.o \
servo/servo.o \
ads1115/ads1115.o \
mpu6050/mpu6050.o \
qmc5883l/qmc5883l.o \
bmp280/bmp280.o \
hcsr04/hcsr04.o \
camera/camera.o \
neo6m/neo6m.o \
neo6m/neo6m_thread.o

export

libhardwareZ.a: components
	ar crs $@ $(OBJECTS)
	ranlib $@

components:
	$(MAKE) -C gpio/
	$(MAKE) -C motor/
	$(MAKE) -C servo/
	$(MAKE) -C ads1115/
	$(MAKE) -C mpu6050/
	$(MAKE) -C qmc5883l/
	$(MAKE) -C bmp280/
	$(MAKE) -C hcsr04/
	$(MAKE) -C camera/
	$(MAKE) -C neo6m/

clean:
	$(MAKE) -C gpio/ clean
	$(MAKE) -C motor/ clean
	$(MAKE) -C servo/ clean
	$(MAKE) -C ads1115/ clean
	$(MAKE) -C mpu6050/ clean
	$(MAKE) -C qmc5883l/ clean
	$(MAKE) -C bmp280/ clean
	$(MAKE) -C hcsr04/ clean
	$(MAKE) -C camera/ clean
	$(MAKE) -C neo6m/ clean
	rm -f *.a