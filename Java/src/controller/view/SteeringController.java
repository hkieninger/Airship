package controller.view;

public interface SteeringController {
	
	public static final int MAX_VELOCITY = 100;
	public static final int MAX_YAW = 100;
	public static final int MAX_PITCH = 100;
	
	//Setters
	public void setListener(Listener l);
	
	//Getters
	public int getVelocity();
	public int getYaw();
	public int getPitch();
	
	//Callbacks
	public interface Listener {
		public void onVelocityChanged(int velocity);
		public void onYawChanged(int yaw);
		public void onPitchChanged(int pitch);
		public void onCallibrated(int yaw, int pitch);
	}
	
}
