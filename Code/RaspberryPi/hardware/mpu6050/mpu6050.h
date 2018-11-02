#ifndef MPU6050_H
#define MPU6050_H 1

#include <stdint.h>
#include <stddef.h>

#include "../gpio/i2c_dev.h"

/*
 * possible improvements:
 * -callibration
 * -use fifo buffer
 * -controll other devices via auxiliary i2c
 * -make use of interrupts
 */

/* 
 * i2c address of the mpu6050
 * addr0 is when ad0 is not connected or pulled to GND
 * addr1 is when ad0 is connected to VCC
 */
#define MPU6050_I2C_ADDR0 0x68
#define MPU6050_I2C_ADDR1 0x69

/*
 * registers containing the measured data
 * the register are 16bit and the data is stored bigendian
 */
#define MPU6050_REG_GY_X 0x43
#define MPU6050_REG_GY_Y 0x45
#define MPU6050_REG_GY_Z 0x47
#define MPU6050_REG_AC_X 0x3B
#define MPU6050_REG_AC_Y 0x3D
#define MPU6050_REG_AC_Z 0x3F
#define MPU6050_REG_TEMP 0x41

/*
 * registers related to the interrupts
 */
 #define MPU6050_REG_INT_ENABLE 0x38
 #define MPU6050_REG_INT_STATUS 0x3A
 #define MPU6050_REG_INT_PIN_CFG 0x37

/*
 * registers for basic configuration
 */
#define MPU6050_REG_PWR_MGMT1 0x6B
#define MPU6050_REG_PWR_MGMT2 0x6C
#define MPU6050_REG_SMPRT_DIV 0x19
#define MPU6050_REG_CONFIG 0x1A

/* 
 * constants to be passed to setCycleFreq()
 */
#define MPU6050_1_25_HZ 0 /* 1.25 Hz */
#define MPU6050_5_HZ 1
#define MPU6050_20_HZ 2
#define MPU6050_40_HZ 3

/*
 * constants to be passed to setLowPassFilter()
 * the first number describes the bandwidth for the accelerometer and the second one the bandwidth for the gyroscope
 */
#define MPU6050_BANDWIDTH_260_256_HZ 0
#define MPU6050_BANDWIDTH_184_188_HZ 1
#define MPU6050_BANDWIDTH_94_98_HZ 2
#define MPU6050_BANDWIDTH_44_42_HZ 3
#define MPU6050_BANDWIDTH_21_20_HZ 4
#define MPU6050_BANDWIDTH_10_10_HZ 5
#define MPU6050_BANDWIDTH_5_5_HZ 6


/*
 * represents a mpu6050
 * an ic controlled with i2c containing an accelerometer, a gyroscope, a temperature sensor (all 16bit resolution)
 * furthermore it has a fifo buffer, is able to generate interrupts and can controll other i2c devices
 * for datasheets look at https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/
 * keep in mind that the mpuu6050 works in big endian
 */
class Mpu6050 : public I2CDev {
	/*
	 * returns the value of the accelerometer axis in m/s^2
	 * reg is the register where the measured data of the accelerometer axis is stored
	 */
	float getAccel(int reg);
	
	/*
	 * returns the value of the gyroscope axis in °/s
	 * reg is the register where the measured data of the gyroscope axis is stored
	 */
	float getGyro(int reg);
public:
	/*
	 * intialises the mpu6050
	 * opens the file descriptor
	 * resets the device
	 * sets sleeps mode to false, sample rate divider is set to 0 and digital low pass filter for accel and gyro to 5 Hz
	 */
	Mpu6050(/*int interruptPin, void (*interruptRoutine)(void), */int addr = MPU6050_I2C_ADDR0);
	
	/*
	 * sets device to sleep to save power
	 * closes the file descriptor
	 * releases the resources associated with the mpu6050
	 */
	~Mpu6050();
	
	/*
	 * resets the device
	 * sets all the registers of the device to their default value
	 * sleep mode will be set after it to true and interrupts will be disabled
	 */
	void reset();

	/*
	 * sets the low pass filter
	 * @bandwith: pass one of the constants defined above
	 */
	void setLowPassFilter(int bandwith);

	/*
	 * enables and disables interrupts
	void setInterrupts(bool on); */
	
	/*
	 * sets the sleep mode (low power mode)
	 * that the mpu6050 measures the data, sleep mode must be set to false 
	 * configuration must be transmitted when mpu6050 isn't in sleep mode (else it doesn't get stored depending on register)
	 */
	void setSleepMode(bool on);
	
	/*
	 * sets the cycle mode
	 * when sleep mode is off,
	 * cycle mode will wake up from sleep mode in a certain frequency - see setCycleFreq(),
	 * measure the data and go back to sleep mode
	 * only the data of the accelerometer will be measured
	 * power consumption goes down from 3.9 mA to 0.5mA compared to normal mode
	 * else data will be measured with a higher sample rate (some kHz, see setSampleRateDivider())
	 */
	void setCycleMode(bool on);
	
	/*
	 * sets the frequency for the cycle mode
	 * @freq: pass one of the constants defined above
	 */
	void setCycleFreq(int freq);

	/*
	 * sets the sample rate divider
	 * the sample rate is calculated according to following formula:
	 * Sample Rate = Gyroscope Output Rate / (1 + divider)
	 * where Gyroscope Output Rate = 8kHz when the Digital Low Pass Filter is disabled (DLPF_CFG = 0 or 7), and 1kHz
	 * when the DLPF is enabled (see Register 26).
	 * @divider: value between 0 and 255
	 */
	void setSampleRateDivider(uint8_t divider);
	
	/*
	 * returns the temperature in °C (degree celsius)
	 * range from -40°C to 85°C
	 */
	float getTemperature();

	/*
	 * writes the values into the passed array
	 * @array: must contain at least 3 elements (0 = x, 1 = y, 2 = z)
	 * @return: array
	 */
	float *getGyro(float *array);
	
	/*
	 * returns the angular velocity in °/s (degree per second) around the axis
	 * min/max angular velocity +-250°/s (higher ranges possible -> see datasheet)
	 */
	float getGyroX();
	float getGyroY();
	float getGyroZ();

	/*
	 * writes the values into the passed array
	 * @array: must contain at least 3 elements (0 = x, 1 = y, 2 = z)
	 * @return: array
	 */
	float *getAccel(float *array);
	
	/*
	 * returns the acceleration in m/s^2 in direction of the axis
	 * min/max acceleration +-2g (higher ranges possible -> see datasheet)
	 */
	float getAccelX();
	float getAccelY();
	float getAccelZ();
};

#endif /* MPU6050_H */
