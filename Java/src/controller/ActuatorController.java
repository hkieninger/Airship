package controller;

public interface ActuatorController {
	
	public static final int THRUST_MAX = 100;
	public static final int ANGLE_MAX = 90;
	
	public void setListener(Listener l);
	
	public void setLeftMotor(int thrust);
	public void setRightMotor(int thrust);
	
	public void setLeftRudder(int angle);
	public void setRightRudder(int angle);
	public void setTopRudder(int angle);
	
	public int getLeftMotor();
	public int getRightMotor();
	
	public int getLeftRudder();
	public int getRightRudder();
	public int getTopRudder();
	
	public interface Listener {
		
		public void onLeftMotorChanged(int thrust);
		public void onRightMotorChanged(int thrust);
		
		public void onLeftRudderChanged(int angle);
		public void onRightRudderChanged(int angle);
		public void onTopRudderChanged(int angle);

	}
	
}
