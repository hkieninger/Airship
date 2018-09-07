package controller.objects;

public enum ConfSteering implements Parameter {
	VELOCITY, YAW, PITCH, CALLIBRATION_YAW, CALLIBRATION_PITCH;
	
	public static final int MAX = 100;

	@Override
	public Class<?> getDataType() {
		return Byte.class;
	}
}
