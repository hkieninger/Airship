package controller.objects;

/**
 * represents a device
 */
public enum ConfDevice implements Device {
	RPI, ACTUATOR, STEERING, AUTOPILOT, SENSOR;
	
	public Class<? extends Enum<? extends Parameter>> getParameter() {
		switch(this) {
			case RPI: return ConfRPI.class;
			case ACTUATOR: return ConfActuator.class;
			case STEERING: return ConfSteering.class;
			case AUTOPILOT: return ConfAutopilot.class;
			case SENSOR: return ConfSensor.class;
		}
		throw new IllegalArgumentException();
	}
}