package controller.data;

import controller.data.parameter.ConfActuator;
import controller.data.parameter.ConfAutopilot;
import controller.data.parameter.ConfRPI;
import controller.data.parameter.ConfSensor;
import controller.data.parameter.ConfSteering;
import controller.data.parameter.Parameter;

/**
 * represents a device
 */
public enum ConfDevice implements Device {
	RPI, ACTUATOR, STEERING, AUTOPILOT, SENSOR;
	
	public Enum<? extends Parameter>[] getParameters() {
		switch(this) {
			case RPI: return ConfRPI.values();
			case ACTUATOR: return ConfActuator.values();
			case STEERING: return ConfSteering.values();
			case AUTOPILOT: return ConfAutopilot.values();
			case SENSOR: return ConfSensor.values();
		}
		throw new IllegalArgumentException();
	}
}