#a not that professional make file, but it works on my rpi ;)
CC = g++
ELFS = \
mpu6050_test01.elf mpu6050_test02.elf \
ads1115_test01.elf ads1115_test02.elf \
bmp280_test01.elf bmp280_test02.elf \
qmc5883l_test01.elf qmc5883l_test02.elf \
neo6m_test01.elf neo6m_test02.elf neo6m_test03.elf \
socket_test01.elf server_socket_test01.elf
LIBS = -lpigpio -lpthread
CFLAGS = -O3 -Wall

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

qmc5883l_test%.elf: qmc5883l_test%.o
	#compile the libs
	$(MAKE) -C ../gpio/ i2c_dev.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../qmc5883l/ qmc5883l.o CFLAGS=$(CFLAGS)
	#link the elf
	$(CC) -o $@ $(CFLAGS) $< ../qmc5883l/qmc5883l.o ../gpio/i2c_dev.o $(LIBS)

qmc5883l_test%.o: qmc5883l_test%.cpp ../qmc5883l/qmc5883l.h ../gpio/i2c_dev.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

motor_test%.elf: motor_test%.o
	#compile the libs
	$(MAKE) -C ../motor/ motor.o CFLAGS=$(CFLAGS)
	#link the elf
	$(CC) -o $@ $(CFLAGS) $< ../motor/motor.o $(LIBS)

motor_test%.o: motor_test%.cpp ../motor/motor.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

neo6m_test%.elf: neo6m_test%.o
	#compile the libs
	$(MAKE) -C ../gpio/ uart_dev.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../neo6m/ neo6m.o CFLAGS=$(CFLAGS)
	#link the elf
	$(CC) -o $@ $(CFLAGS) $< ../neo6m/neo6m.o ../gpio/uart_dev.o $(LIBS)

neo6m_test%.o: neo6m_test%.cpp ../neo6m/neo6m.h ../gpio/uart_dev.h
	#compile the test programms
	$(CC) -c $(CFLAGS) $<

socket_test01.elf: socket_test01.o
	$(MAKE) -C ../socket/ socket.o CFLAGS=$(CFLAGS)
	#the socket library is independent from wiringPi
	$(CC) -o $@ $(CFLAGS) $< ../socket/socket.o

socket_test01.o: socket_test01.cpp ../socket/socket.h
	$(CC) -c $(CFLAGS) $<

server_socket_test01.elf: server_socket_test01.o
	$(MAKE) -C ../socket/ socket.o CFLAGS=$(CFLAGS)
	$(MAKE) -C ../socket/ server_socket.o CFLAGS=$(CFLAGS)
	#the socket library is independent from wiringPi
	$(CC) -o $@ $(CFLAGS) $< ../socket/socket.o ../socket/server_socket.o

server_socket_test01.o: server_socket_test01.cpp ../socket/socket.h ../socket/server_socket.h ../socket/socket_exception.h
	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C ../gpio/ clean
	$(MAKE) -C ../mpu6050/ clean
	$(MAKE) -C ../ads1115/ clean
	$(MAKE) -C ../bmp280/ clean
	$(MAKE) -C ../qmc5883l/ clean
	$(MAKE) -C ../neo6m/ clean
	$(MAKE) -C ../socket/ clean
	rm *.elf
