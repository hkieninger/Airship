#a not that professional make file, but it works on my rpi ;)
CC = g++
ELFS = \
mpu6050_test01.elf mpu6050_test02.elf mpu6050_test03.elf \
ads1115_test01.elf ads1115_test02.elf \
bmp280_test01.elf bmp280_test02.elf
LIBS = -lwiringPi
CFLAGS = -O3

all: $(ELFS)

mpu6050_test%.elf: mpu6050_test%.o
	#compile the libs
	$(MAKE) -C ../gpio/ i2c_dev.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../mpu6050/ mpu6050.o CFLAGS=$(CFLAGS)
	#link the elf
	$(CC) -o $@ $(CFLAGS) $< ../mpu6050/mpu6050.o ../gpio/i2c_dev.o $(LIBS)

mpu6050_test%.o: mpu6050_test%.cpp ../mpu6050/mpu6050.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

ads1115_test%.elf: ads1115_test%.o
	#compile the libs
	$(MAKE) -C ../gpio/ i2c_dev.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../ads1115/ ads1115.o CFLAGS=$(CFLAGS)
	#link the elf
	$(CC) -o $@ $(CFLAGS) $< ../ads1115/ads1115.o ../gpio/i2c_dev.o $(LIBS)

ads1115_test%.o: ads1115_test%.cpp ../ads1115/ads1115.h ../gpio/i2c_dev.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

bmp280_test%.elf: bmp280_test%.o
	#compile the libs
	$(MAKE) -C ../gpio/ i2c_dev.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../bmp280/ bmp280.o CFLAGS=$(CFLAGS)
	#link the elf
	$(CC) -o $@ $(CFLAGS) $< ../bmp280/bmp280.o ../gpio/i2c_dev.o $(LIBS)

bmp280_test%.o: bmp280_test%.cpp ../bmp280/bmp280.h ../gpio/i2c_dev.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C ../gpio/ clean
	$(MAKE) -C ../mpu6050/ clean
	$(MAKE) -C ../ads1115/ clean
	$(MAKE) -C ../bmp280/ clean
	rm *.elf