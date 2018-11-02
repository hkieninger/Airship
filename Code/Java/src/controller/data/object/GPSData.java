package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/*
 * make thread safe!
 */
public class GPSData implements ConnectionData {
	
	//gps data: 0 = latitude [°], 1 = longitude [°], 2 = altitude [m over sea]
	private double[] gpsPosition;
	//gps position precision: 0 = vertical position [m], 1 = horizontal precision[m]
	private double[] gpsPosPrecision;
	//gps velocity: 0 = north [m/s], 1 = east [m/s], 2 = down [m/s]
	private double[] gpsVelocity;
	private double gpsHeading;
	//gps velocity precision: 0 = velocity, 1 = heading;
	//private double[] gpsVelPrecision;
	//number of satelites
	private int sattelites;
	
	private static GPSData defautInstance = new GPSData();
	
	public static GPSData getDefaultInstance() {
		return defautInstance;
	}
	
	public GPSData() {
		this.gpsPosition = new double[3];
		this.gpsPosPrecision = new double[2];
		this.gpsVelocity = new double[3];
		//this.gpsVelPrecision = new double[2];
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
	
	public double getVelocityOverGround() {
		return Math.sqrt(gpsVelocity[0] * gpsVelocity[0] + gpsVelocity[1] * gpsVelocity[1]);
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
	/*public double[] getGpsVelPrecision() {
		return gpsVelPrecision;
	}*/

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
	public void receive(DataInputStream in) throws IOException {
		in.readUnsignedShort();
		gpsPosition[0] = in.readInt() * 1e-7;
		gpsPosition[1] = in.readInt() * 1e-7;
		gpsPosition[2] = in.readInt() * 1e-3;
		gpsPosPrecision[0] = in.readInt() * 1e-3;
		gpsPosPrecision[1] = in.readInt() * 1e-3;
		gpsVelocity[0] = in.readInt() * 1e-2;
		gpsVelocity[1] = in.readInt() * 1e-2;
		gpsVelocity[2] = in.readInt() * 1e-2;
		gpsHeading = in.readInt() * 1e-5;
		sattelites = in.readUnsignedByte();
	}

}
