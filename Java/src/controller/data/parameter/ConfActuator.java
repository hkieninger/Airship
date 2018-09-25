package controller.data.parameter;

import controller.data.object.ConnectionData;

public enum ConfActuator implements Parameter {
	LEFT_MOTOR, RIGHT_MOTOR, LEFT_RUDDER, RIGHT_RUDDER, TOP_RUDDER;
	
	public static final int MOTOR_THRUST_MAX = 100;
	public static final int RUDDER_ANGLE_MAX = 90;

	@Override
	public ConnectionData getDataInstance() {
		if(this == LEFT_MOTOR || this == RIGHT_MOTOR) 
			return new ConnectionData.UByte();
		else
			return new ConnectionData.Byte();
	}
}