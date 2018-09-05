package controller;

import java.util.ArrayList;
import java.util.List;

import controller.objects.GPSData;

/**
 * 
 * @author hans
 *
 * class to handle the data which comes from the airship
 * the data is in SI units
 *
 */
public class DataPool {
	
	//flags telling if the data has changed
	private boolean[] changed;
	
	//vector data: 0 = x, 1 = y, 2 = z
	private float[] accel, gyro, compass;
	//gps data
	private GPSData gpsData;
	//private camera pixel;
	private byte[] cameraFront, cameraBottom;
	//the ping of the airship in ms, the battery status of the airship in %
	private int ping, battery;
	//messages from the airship or the socket
	private String message;
	private int messageColor;
	//other data
	private double pressure, distanceFront, distanceBottom;
	
	private List<Listener> listeners;
	
	public DataPool() {
		listeners = new ArrayList<>();
		changed = new boolean[Data.values().length];
	}
	
	public void addListener(Listener l) {
		listeners.add(l);
	}
	
	public void removeListener(Listener l) {
		listeners.remove(l);
	}
	
	/**
	 * notifies the listener that data has changed
	 * @param data a hint for the listener which data has changed, could be null if several data has changed
	 */
	void notifyListeners(Data data) {
		for(Listener l : listeners) {
			l.onDataChanged(this, data);
		}
		for(int i = 0; i < changed.length; i++) {
			changed[i] = false;
		}
	}
	
	/**
	 * @param message the message to set
	 * @param color the color of the message
	 */
	void setMessage(String message, int color) {
		this.message = message;
		messageColor = color;
		changed[Data.MESSAGE.ordinal()] = true;
	}
	
	/**
	 * @param ping the ping to set
	 */
	void setPing(int ping) {
		this.ping = ping;
		changed[Data.NETWORK.ordinal()] = true;
	}

	/**
	 * @param battery the battery to set
	 */
	void setBattery(int battery) {
		this.battery = battery;
		changed[Data.BATTERY.ordinal()] = true;
	}

	/**
	 * @param accel the accel to set
	 */
	void setAccel(float[] accel) {
		this.accel = accel;
		changed[Data.ACCEL.ordinal()] = true;
	}

	/**
	 * @param gyro the gyro to set
	 */
	void setGyro(float[] gyro) {
		this.gyro = gyro;
		changed[Data.GYRO.ordinal()] = true;
	}

	/**
	 * @param compass the compass to set
	 */
	void setCompass(float[] compass) {
		this.compass = compass;
		changed[Data.COMPASS.ordinal()] = true;
	}

	/**
	 * @param gpsData the gpsData to set
	 */
	void setGpsData(GPSData gpsData) {
		this.gpsData = gpsData;
		changed[Data.GPS.ordinal()] = true;
	}

	/**
	 * @param cameraFront the cameraFront to set
	 */
	void setCameraFront(byte[] cameraFront) {
		this.cameraFront = cameraFront;
		changed[Data.CAM_FRONT.ordinal()] = true;
	}

	/**
	 * @param cameraBottom the cameraBottom to set
	 */
	void setCameraBottom(byte[] cameraBottom) {
		this.cameraBottom = cameraBottom;
		changed[Data.CAM_BOTTOM.ordinal()] = true;
	}

	/**
	 * @param pressure the pressure to set
	 */
	void setPressure(double pressure) {
		this.pressure = pressure;
		changed[Data.BARO.ordinal()] = true;
	}

	/**
	 * @param distanceFront the distanceFront to set
	 */
	void setDistanceFront(double distanceFront) {
		this.distanceFront = distanceFront;
		changed[Data.DIST_FRONT.ordinal()] = true;
	}

	/**
	 * @param distanceBottom the distanceBottom to set
	 */
	void setDistanceBottom(double distanceBottom) {
		this.distanceBottom = distanceBottom;
		changed[Data.DIST_BOTTOM.ordinal()] = true;
	}

	/**
	 * @return the accel
	 */
	public float[] getAccel() {
		return accel;
	}

	/**
	 * @return the gyro
	 */
	public float[] getGyro() {
		return gyro;
	}

	/**
	 * @return the compass
	 */
	public float[] getCompass() {
		return compass;
	}

	/**
	 * @return the gpsData
	 */
	public GPSData getGpsData() {
		return gpsData;
	}

	/**
	 * @return the cameraFront
	 */
	public byte[] getCameraFront() {
		return cameraFront;
	}

	/**
	 * @return the cameraBottom
	 */
	public byte[] getCameraBottom() {
		return cameraBottom;
	}

	/**
	 * @return the pressure
	 */
	public double getPressure() {
		return pressure;
	}

	/**
	 * @return the distanceFront
	 */
	public double getDistanceFront() {
		return distanceFront;
	}

	/**
	 * @return the distanceBottom
	 */
	public double getDistanceBottom() {
		return distanceBottom;
	}
	
	/**
	 * @return the ping
	 */
	public int getPing() {
		return ping;
	}

	/**
	 * @return the battery
	 */
	public int getBattery() {
		return battery;
	}
	
	/**
	 * @return the message
	 */
	public String getMessage() {
		return message;
	}
	
	/**
	 * @return the color of the message
	 */
	public int getMessageColor() {
		return messageColor;
	}
	
	public interface Listener {
		
		void onDataChanged(DataPool pool, Data data);
		
	}
	
	public enum Data {
		ACCEL, GYRO, COMPASS, GPS, BARO, CAM_FRONT, CAM_BOTTOM, DIST_FRONT, DIST_BOTTOM, KALMAN, NETWORK, BATTERY, MESSAGE
	}

}
