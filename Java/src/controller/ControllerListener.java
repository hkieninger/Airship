package controller;

/**
 * 
 * @author hans
 *
 * this interface allows the Controller to inform for example the gui
 * the methods shouldn't block or use to much processing time, since they are called from the receive and send threads
 */
public interface ControllerListener {
	
	/**
	 * called when an error has occured, thread shouldn't blcok
	 * @param e a exception containing the error
	 */
	public void onError(Exception e);
	
	/**
	 * called when the connection has been closed by the remote host
	 * you may want to retry to connect to the host
	 */
	public void onConnectionClosedByHost();
	
	/**
	 * this event is triggered when the connection is too weak
	 */
	public void onConnectionLost();
	
	/**
	 * this event is triggered when the connection was weak before and is good again
	 */
	public void onConnectionRestored();
	
}
