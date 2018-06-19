#ifndef I2C_DEV_H
#define I2C_DEV_H 1

/*
 * represents a i2c device
 * functions throw GPIOException on errors
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
	 * sets the value of a 8bit register
	 * @reg: the address of the register
	 * @value: the value to set (8 bit)
	 */
	void writeReg8(int reg, int value);
	
	/*
	 * gets the value of a 8bit register
	 * @reg: the address of the register
	 * @return: the value of the register (8 bit)
	 */
	int readReg8(int reg);
	
	/*
	 * sets the value of a 16bit register
	 * @reg: the address of the register
	 * @value: the value to set (16 bit)
	 *         function takes care of endianess (pass value in little endian)
	 */
	void writeReg16(int reg, int value);
	
	/*
	 * gets the value of a 16bit register
	 * @reg: the address of the register
	 * @return: the value of the register (16 bit)
	 * 			function takes care of endianess (value is little endian)
	 */
	int readReg16(int reg);
};

#endif /* I2C_DEV_H */
