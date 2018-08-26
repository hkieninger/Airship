package controller;

public interface SteeringController {
	
	public static final double MAX_VELOCITY = 1.0;
	public static final double MAX_ANGLE = 2 * Math.PI;
	public static final double MAX_RADIUS = 1.0;
	
	//Setters
	public void setListener(Listener l);
	
	public void setVelocity(double velocity);
	public void setAngle(double angle);
	public void setRadius(double radius);
	
	//Getters
	public double getVelocity();
	public double getAngle();
	public double getRadius();
	
	//Callbacks
	public interface Listener {
		public void onVelocityChanged(double velocity);
		public void onAngleChanged(double angle);
		public void onRadiusChanged(double radius);
	}
	
}
