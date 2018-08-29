package controller;

/**
 * 
 * @author hans
 *
 * the callbacks shouldn't block or use too much processing time
 */
public interface ControllerListener {
	
	enum Level {
		CHATTERBOX, NORMAL, ERROR
	}
	
	public void onInformation(String information, Level level);
	public void onError();
	public void onConnectionLost(boolean closedByHost);
	public void onStarted();
	public void onStopped();
	
}
