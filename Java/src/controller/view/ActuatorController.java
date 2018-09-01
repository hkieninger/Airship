package controller.view;

public interface ActuatorController {
	
	public static final int THRUST_MAX = 100;
	public static final int ANGLE_MAX = 90;
	
	//Setters
	public void setListener(Listener l);
	
	//Getters
	public int getLeftMotor();
	public int getRightMotor();
	
	public int getLeftRudder();
	public int getRightRudder();
	public int getTopRudder();
	
	//Callbacks
	public static interface Listener {
		
		public void onLeftMotorChanged(int thrust);
		public void onRightMotorChanged(int thrust);
		
		public void onLeftRudderChanged(int angle);
		public void onRightRudderChanged(int angle);
		public void onTopRudderChanged(int angle);

	}
	
}
