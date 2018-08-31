package controller.view;

import controller.SendThread;

/**
 * 
 * @author hans
 *
 * this interface allows the controller to directly show information to the user
 * the methods shouldn't block or need too much processing time since they called from the send and receive thread
 *
 */
public interface StatusView {
	
	public static final int GREEN_PING = 50;
	public static final long RED_PING = SendThread.CONNECTION_LOST_TIME;
	
	public static final int WHITE = 0xFFFFFF;
	
	/**
	 * should display the battery percentage
	 * @param percentage
	 */
	void setBatteryPercentage(int percentage);
	
	/**
	 * should display the roundtrip time
	 * @param ping
	 */
	void setNetworkPing(long ping);
	
	/**
	 * should display an information string in the specified color
	 * @param info
	 * @param color
	 */
	void information(String info, int color);

}
