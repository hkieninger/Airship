package controller.objects;

public class GPSData {
	
	//gps data: 0 = latitude [°], 1 = longitude [°], 2 = altitude [m over sea]
	private double[] gpsPosition;
	//gps position precision: 0 = vertical position [m], 1 = horizontal precision[m]
	private double[] gpsPosPrecision;
	//gps velocity: 0 = north [m/s], 1 = east [m/s], 2 = down [m/s]
	private double[] gpsVelocity;
	private double gpsHeading;
	//gps velocity precision: 0 = velocity, 1 = heading;
	private double[] gpsVelPrecision;
	//number of satelites
	private int sattelites;
	
	public GPSData() {}
	
	public GPSData(double[] gpsPosition, double[] gpsPosPrecision, 
			double[] gpsVelocity, double gpsHeading,
			double[] gpsVelPrecision, int sattelites) {
		this.gpsPosition = gpsPosition;
		this.gpsPosPrecision = gpsPosPrecision;
		this.gpsVelocity = gpsVelocity;
		this.gpsHeading = gpsHeading;
		this.gpsVelPrecision = gpsVelPrecision;
		this.sattelites = sattelites;
	}

	/**
	 * @return the gpsPosition
	 */
	public double[] getGpsPosition() {
		return gpsPosition;
	}

	/**
	 * @param gpsPosition the gpsPosition to set
	 */
	public void setGpsPosition(double[] gpsPosition) {
		this.gpsPosition = gpsPosition;
	}

	/**
	 * @return the gpsPosPrecision
	 */
	public double[] getGpsPosPrecision() {
		return gpsPosPrecision;
	}

	/**
	 * @param gpsPosPrecision the gpsPosPrecision to set
	 */
	public void setGpsPosPrecision(double[] gpsPosPrecision) {
		this.gpsPosPrecision = gpsPosPrecision;
	}

	/**
	 * @return the gpsVelocity
	 */
	public double[] getGpsVelocity() {
		return gpsVelocity;
	}

	/**
	 * @param gpsVelocity the gpsVelocity to set
	 */
	public void setGpsVelocity(double[] gpsVelocity) {
		this.gpsVelocity = gpsVelocity;
	}

	/**
	 * @return the gpsHeading
	 */
	public double getGpsHeading() {
		return gpsHeading;
	}

	/**
	 * @param gpsHeading the gpsHeading to set
	 */
	public void setGpsHeading(double gpsHeading) {
		this.gpsHeading = gpsHeading;
	}

	/**
	 * @return the gpsVelPrecision
	 */
	public double[] getGpsVelPrecision() {
		return gpsVelPrecision;
	}

	/**
	 * @param gpsVelPrecision the gpsVelPrecision to set
	 */
	public void setGpsVelPrecision(double[] gpsVelPrecision) {
		this.gpsVelPrecision = gpsVelPrecision;
	}

	/**
	 * @return the sattelites
	 */
	public int getSattelites() {
		return sattelites;
	}

	/**
	 * @param sattelites the sattelites to set
	 */
	public void setSattelites(int sattelites) {
		this.sattelites = sattelites;
	}

}
