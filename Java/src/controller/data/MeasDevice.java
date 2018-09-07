package controller.data;

import controller.data.parameter.MeasRPI;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;

public enum MeasDevice implements Device {
	RPI, SENSOR;

	@Override
	public Enum<? extends Parameter>[] getParameters() {
		switch(this) {
			case RPI: return MeasRPI.values();
			case SENSOR: return MeasSensor.values();
		}
		return null;
	}

}
