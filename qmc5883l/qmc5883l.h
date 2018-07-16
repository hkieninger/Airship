#ifndef QMC5883L_H
#define QMC5883L_H 1

#include "../gpio/i2c_dev.h"

/*
 * possible improvements:
 * -take advantage of interrupts
 * -some small features not implemented yet
 */

/*
 * i2c address of the qmc5883l
 */
#define QMC5883L_I2C_ADDR 0x0D

/*
 * registers containing the measured data of the qmc5883l
 * the data is 16 bit and stored little endian
 */
#define QMC5883L_REG_DX 0x00
#define QMC5883L_REG_DY 0x02
#define QMC5883L_REG_DZ 0x04
#define QMC5883L_REG_TEMP 0x07

/*
 * register for configuration and status of the qmc5883l
 * registers are 8 bit
 */
#define QMC5883L_REG_CTRL1 0x09
#define QMC5883L_REG_CTRL2 0x0A
#define QMC5883L_REG_STAT 0x06

/*
 * constants to be passed to setContinuous()
 */
#define QMC5883L_10_HZ 0
#define QMC5883L_50_HZ 1
#define QMC5883L_100_HZ 2
#define QMC5883L_200_HZ 3

/*
 * constants to be passed to setRange()
 */
#define QMC5883L_2_GAUSS 0
#define QMC5883L_8_GAUSS 1

/*
 * represents a qmc5883l
 * an ic controlled with i2c with sensors for magnetic fields (12 bit resolution)
 * the qmc5883l is little endian
 */
class Qmc5883l : public I2CDev {
	/*
	 * returns the value of the magnetic sensor axis in gauss
	 * reg is the register where the measured data of the magnetic sensor axis is stored
	 */
	float getMag(int reg);
public:
	/*
	 * initialises the qmc5883l
	 * reset it
	 * sets it into conitnuous mode with a freq of 10 Hz
	 */
	Qmc5883l();

	/*
	 * sets the qmc5883l into standby mode
	 * releases the other associated resources
	 */
	~Qmc5883l();
	
	/*
	 * resets the qmc5883l, put the default value into all registers
	 * after reset the qmc5883l is in standby mode
	 */
	void reset();

	/*
	 * sets the range of the magnetic field to be measured
	 * @range: use the constants defined above
	 */
	void setRange(int range);

	/*
	 * @return: true if new data has been measured, false if data has already been read
	 */
	bool dataReady();

	/*
	 * puts the qmc5883l in standbymode to save power
	 * no measurments are made
	 */
	void setStandby();

	/*
	 * puts the qmc5883l in continuous mode with the specified freq
	 * @freq: use the constants defined above
	 */
	void setContinuous(int freq);

	/*
	 * writes the values into the passed array
	 * throws std::range_error if the magnetic field exceeds min or max magnitude
	 * @array: must contain at least 3 elements (0 = x, 1 = y, 2 = z)
	 * @return: array
	 */
	float *getMag(float *array);
	
	/*
	 * returns the magnitude of the magnetic field in the direction of the axis in gauss (1 gauss = 100 micro tesla)
	 * throws std::range_error if the magnetic field exceeds min or max magnitude
	 */
	float getMagX();
	float getMagY();
	float getMagZ();
	
};

#endif /* QMC5883L_H */