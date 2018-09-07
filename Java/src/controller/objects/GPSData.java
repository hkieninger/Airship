package controller.objects;

import java.io.DataInputStream;
import java.io.DataOutputStream;

public class GPSData implements ConnectionData {
	
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
	
	public GPSData() {
		this.gpsPosition = new double[3];
		this.gpsPosPrecision = new double[2];
		this.gpsVelocity = new double[3];
		this.gpsVelPrecision = new double[2];
	}

	/**
	 * @return the gpsPosition
	 */
	public double[] getGpsPosition() {
		return gpsPosition;
	}

	/**
	 * @return the gpsPosPrecision
	 */
	public double[] getGpsPosPrecision() {
		return gpsPosPrecision;
	}

	/**
	 * @return the gpsVelocity
	 */
	public double[] getGpsVelocity() {
		return gpsVelocity;
	}

	/**
	 * @return the gpsHeading
	 */
	public double getGpsHeading() {
		return gpsHeading;
	}

	/**
	 * @return the gpsVelPrecision
	 */
	public double[] getGpsVelPrecision() {
		return gpsVelPrecision;
	}

	/**
	 * @return the sattelites
	 */
	public int getSattelites() {
		return sattelites;
	}

	@Override
	public void send(DataOutputStream out) {
		//not needed yet
		throw new UnsupportedOperationException("Not implemented yet.");
	}
	
	@Override
	public void receive(DataInputStream in) {
		
	}

}
