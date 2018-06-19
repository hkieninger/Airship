#ifndef QMC5883L_H
#define QMC5883L_H 1				/* remove bswap from i2cdev qmc works in little endian */

#include "../i2c_dev.h"

/*
 * possible improvements:
 * -make use of continous mode
 * -take advantage of interrupts
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
 * represents a qmc5883l
 * an ic controlled with i2c with sensors for magnetic fields (12 bit resolution)
 */
class Qmc5883l : public I2CDev {
	/*
	 * returns the value of the magnetic sensor axis in gauss
	 * reg is the register where the measured data of the magnetic sensor axis is stored
	 */
	float getMag(int reg);
public:
	Qmc5883l();
	~Qmc5883l();
	
	void reset();
	
	/*
	 * returns the magnitude of the magnetic field in the direction of the axis in gauss (1 gauss = 100 micro tesla)
	 * min/max magnitude +-1.3 gauss (higher and lower ranges possible -> see datasheet)
	 * throws std::range_error if the magnetic field exceeds min or max magnitude
	 */
	float getMagX();
	float getMagY();
	float getMagZ();
	
};

#endif /* QMC5883L_H */
