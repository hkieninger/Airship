package controller.view;

public interface SensorView {
	
	/**
	 * vector of magnetic field in gauss
	 * @param x
	 * @param y
	 * @param z
	 */
	void setCompass(float x, float y, float z);
	
	/**
	 * vector of acceleration in m/s 2
	 * @param x
	 * @param y
	 * @param z
	 */
	void setAccelerometer(float x, float y, float z);
	
	/**
	 * vector of angular velocity in °/s
	 * @param x
	 * @param y
	 * @param z
	 */
	void setGyroscope(float x, float y, float z);
	
	/**
	 * GPS measurements
	 * @param latitude ° scaled with 10e-7
	 * @param longitude ° scaled with 10e-7
	 * @param altitude in mm
	 * @param precision in mm
	 * @param velocity cm/s
	 * @param azimuth ° scaled with 10e-5
	 */
	void setGPS(int latitude, int longitude, int altitude, int precision, int velocity, int azimuth);
	
	/**
	 * pressue in Pa
	 * @param pressure
	 */
	void setBarometer(double pressure);
	
	/**
	 * distance in cm measured by the front ultrasound sensor
	 * @param cm
	 */
	void setDistanceFront(float cm);
	
	/**
	 * distance in cm measured by the bottom ultrasound sensor
	 * @param cm
	 */
	void setDistanceBottom(float cm);
	
}
