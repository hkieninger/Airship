CC = g++
ELFS = mpu6050_test01.elf mpu6050_test02.elf mpu6050_test03.elf
LIBS = -lwiringPi
CFLAGS = -O3

all: $(ELFS)

mpu6050_test%.elf: mpu6050_test%.o
	#compile the libs
	$(MAKE) -C ../gpio/ i2c_dev.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../mpu6050/ mpu6050.o CFLAGS=$(CFLAGS)
	#link the elfs
	$(CC) -o $@ $(FLAGS) $< ../mpu6050/mpu6050.o ../gpio/i2c_dev.o $(LIBS)

mpu6050_test%.o: mpu6050_test%.cpp ../mpu6050/mpu6050.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C ../gpio/ clean
	$(MAKE) -C ../mpu6050/ clean
	rm *.elf