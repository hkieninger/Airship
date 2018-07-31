#ifndef I2C_DEV_H
#define I2C_DEV_H 1

#include <stdint.h>
#include <stddef.h>

/*
 * represents a i2c device
 * functions throw I2CException on errors
 */
class I2CDev {
protected:
	/*
	 * file descriptor to the device file 
	 */
	int fd;
public:
	/*
	 * initialises an i2c device
	 * opens the file descriptor
	 * @addr: the i2c address of the device
	 */
	I2CDev(int addr);
	
	/*
	 * closes the file descriptor
	 */
	~I2CDev();
	
	/*
	 * sets a bit in a 8bit register
	 * @reg: the address of the register
	 * @bit: the number of the bit 0-7 (lsb = 0, msb = 7)
	 * @value: the value to set (false = 0, true = 1)
	 */
	void writeBitReg8(int reg, int bit, bool value);
	
	/*
	 * gets a bit from a 8bit register
	 * @reg: the address of the register
	 * @bit: the number of the bit 0-7 (lsb = 0, msb = 7)
	 * @return: the value of the bit (false = 0, true = 1)
	 */
	bool readBitReg8(int reg, int bit);

	/*
	 * sets a bit in a 16bit register
	 * @reg: the address of the register
	 * @bit: the number of the bit 0-15 (lsb = 0, msb = 15), the two bytes are handled big endian
	 * @value: the value to set (false = 0, true = 1)
	 */
	void writeBitReg16(int reg, int bit, bool value);
	
	/*
	 * gets a bit from a 16 bit register
	 * @reg: the address of the register
	 * @bit: the number of the bit 0-15 (lsb = 0, msb = 15), the two bytes are handled big endian
	 * @return: the value of the bit (false = 0, true = 1)
	 */
	bool readBitReg16(int reg, int bit);
	
	
	/*
	 * sets the value of a 8bit register
	 * @reg: the address of the register
	 * @value: the value to set (8 bit)
	 */
	void writeReg8(int reg, uint8_t value);
	
	/*
	 * gets the value of a 8bit register
	 * @reg: the address of the register
	 * @return: the value of the register (8 bit)
	 */
	uint8_t readReg8(int reg);
	
	/*
	 * sets the value of a 16bit register
	 * @reg: the address of the register
	 * @value: the value to set (16 bit)
	 *         keep in mind that the i2c device might work in big endian, while the rpi works with little endian
	 *		   therefore you might want to use the functions from <byteswap.h>
	 */
	void writeReg16(int reg, uint16_t value);
	
	/*
	 * gets the value of a 16bit register
	 * @reg: the address of the register
	 * @return: the value of the register (16 bit)
	 * 			keep in mind that the i2c device might work in big endian, while the rpi works with little endian
	 *		    therefore you might want to use the functions from <byteswap.h>
	 */
	uint16_t readReg16(int reg);

	/*
     * wrapper for read from <unistd.h>
     */
    size_t readI2C(void *buf, size_t count);

    /*
     * wrapper for write from <unistd.h>
     */
    size_t writeI2C(const void *buf, size_t count);
};

#endif /* I2C_DEV_H */
