#ifndef ADS1115_H
#define ADS1115_H 1

#include "../gpio/i2c_dev.h"

/*
 * possible improvements:
 * -make use of alert pin and interrupts for dataready or internal comparator, would need pullup resistor (1-10kOhm)
 * -setting range
 * -differential input (read voltage difference between two pins)
 * -implement reset (general i2c reset call)
 */

/*
 * i2c addresses of the ads1115
 */
#define ADS1115_I2C_ADDR0 0x48 //ADDR connected to GND
#define ADS1115_I2C_ADDR1 0x49 //ADDR connected to VCC
#define ADS1115_I2C_ADDR2 0x4A //ADDR connected to SDA
#define ADS1115_I2C_ADDR3 0x4B //ADDR connected to SCL

/*
 * the four registers of the ads1115
 */
#define ADS1115_REG_CONV 0x00 //contains the data of the last conversion
#define ADS1115_REG_CONF 0x01 //contains the configuration
#define ADS1115_REG_LOTH 0x02 //contains lower threshold for comparator
#define ADS1115_REG_HITH 0x03 //contains higher threshold for comparator

/*
 * constants to be passed to setSampleRate()
 */
#define ADS1115_8_SPS 0
#define ADS1115_16_SPS 1
#define ADS1115_32_SPS 2
#define ADS1115_64_SPS 3
#define ADS1115_128_SPS 4
#define ADS1115_250_SPS 5
#define ADS1115_475_SPS 6
#define ADS1115_860_SPS 7

/*
 * represents an ads1115
 * an analog digital converter with a resolution of 16 bit, which can be accessed via i2c
 * the ads1115 works big endian
 */
class Ads1115: public I2CDev {
public:
    /*
     * intialises the ads1115
	 * opens the file descriptor
     * sets the input pin to 0 (differential to GND)
     * sets range to 2.048 V
     * sets single shot mode (in continous conversion mode data rate would be 128 SPS by default)
     * disables dataready/alert interrupts
     */
    Ads1115(int addr = ADS1115_I2C_ADDR0);

    /*
	 * sets device to single shot mode to save power
	 * closes the file descriptor
	 * releases the resources associated with the mpu6050
	 */
    ~Ads1115();

    /*
     * triggers the ads1115 to take one measurement on the actual input pin (see setInputPin())
     * waits that the ads1115 terminates the measurement which corresponds to ~25 micro seconds
     * @return: the measurement
     */
    float getSingleShot();

    /*
     * same as getSingleShot() but you can specify the pin
     * @pin: the acutal input pin is set to pin before the measurement, value between 0 and 3
     * @return: the measurement
     */
    float getSingleShot(int pin);

    /*
     * @return: the value of the last conversion, which lays in the conversion register
     *          value is transformed into a voltage up to 2.048 V
     */
    float getVoltage();

    /*
     * sets the actual input pin, the voltage of the pin is compared to GND
     * @pin: value between 0 and 3
     */
    void setInputPin(int pin);

    /*
     * in continuous mode device takes measurement periodically in a freq set by setSampleRate() (default is 128 SPS/Hz)
     * else device is in single shot mode, in single shot mode measurements must be triggered by the master (you ;)
     */
    void setContinuousMode(bool on);

    /*
     * sets the sample rate for the continuous mode
     * @rate: pass one of the constants defined above
     */
    void setSampleRate(int rate);
};

#endif /* ADS1115_H */